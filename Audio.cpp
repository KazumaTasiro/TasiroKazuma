#include "Audio.h"
#pragma comment(lib,"xaudio2.lib")
#pragma comment(lib, "Mf.lib")
#pragma comment(lib, "mfplat.lib")
#pragma comment(lib, "Mfreadwrite.lib")
#pragma comment(lib, "mfuuid.lib")

#pragma comment(lib, "Propsys.lib")

void Audio::Initialize(const std::string directoryPath)
{
	directoryPath_ = directoryPath;

	HRESULT result;

	IXAudio2MasteringVoice* masterVoice;
	result = XAudio2Create(&xAudio2_, 0, XAUDIO2_DEFAULT_PROCESSOR);
	assert(SUCCEEDED(result));

	result = xAudio2_->CreateMasteringVoice(&masterVoice);
	assert(SUCCEEDED(result));

	//CoInitializeEx(nullptr, COINIT_MULTITHREADED);

	MFStartup(MF_VERSION, MFSTARTUP_NOSOCKET);


}

void Audio::Finalize()
{
	//xAudio解放
	xAudio2_.Reset();
	//サウンドデータ解放
	std::map<std::string, SoundData>::iterator it = soundDatas_.begin();
	for (; it != soundDatas_.end(); it++) {
		Unload(&it->second);
	}
	soundDatas_.clear();
}

void Audio::LoadWave(const std::string filename)
{
	if (soundDatas_.find(filename) != soundDatas_.end()) {
		//重複
		return;
	}

	//デレクトリパスとファイル名を連結してフルパスを得る
	std::string fullPath = directoryPath_ + filename;

	// ユニコード文字列に変換
	wchar_t wfilePath[256];
	MultiByteToWideChar(CP_ACP, 0, fullPath.c_str(), -1, wfilePath, _countof(wfilePath));

	IMFSourceReader* pMFSourceReader{ nullptr };
	MFCreateSourceReaderFromURL(wfilePath, NULL, &pMFSourceReader);

	IMFMediaType* pMFMediaType{ nullptr };
	MFCreateMediaType(&pMFMediaType);
	pMFMediaType->SetGUID(MF_MT_MAJOR_TYPE, MFMediaType_Audio);
	pMFMediaType->SetGUID(MF_MT_SUBTYPE, MFAudioFormat_PCM);
	pMFSourceReader->SetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, nullptr, pMFMediaType);

	pMFMediaType->Release();
	pMFMediaType = nullptr;
	pMFSourceReader->GetCurrentMediaType(MF_SOURCE_READER_FIRST_AUDIO_STREAM, &pMFMediaType);

	WAVEFORMATEX* waveFormat{ nullptr };
	MFCreateWaveFormatExFromMFMediaType(pMFMediaType, &waveFormat, nullptr);

	// 長さを取得
	float duration = 0.0f;
	PROPVARIANT var;
	HRESULT hr =
		pMFSourceReader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &var);
	if (SUCCEEDED(hr)) {
		// 100ナノ秒単位なので秒に長さを直す
		LONGLONG durationInt = 0;
		PropVariantToInt64(var, &durationInt);
		duration = float(double(durationInt) / 10000000.0);
		PropVariantClear(&var);
	}

	std::vector<BYTE> mediaData;
	mediaData.reserve(size_t(duration * waveFormat->nAvgBytesPerSec));
	while (true)
	{
		IMFSample* pMFSample{ nullptr };
		DWORD dwStreamFlags{ 0 };
		pMFSourceReader->ReadSample(MF_SOURCE_READER_FIRST_AUDIO_STREAM, 0, nullptr, &dwStreamFlags, nullptr, &pMFSample);

		if (dwStreamFlags & MF_SOURCE_READERF_ENDOFSTREAM || pMFSample == nullptr)
		{
			break;
		}

		IMFMediaBuffer* pMFMediaBuffer{ nullptr };
		pMFSample->ConvertToContiguousBuffer(&pMFMediaBuffer);

		BYTE* pBuffer{ nullptr };
		DWORD cbCurrentLength{ 0 };
		pMFMediaBuffer->Lock(&pBuffer, nullptr, &cbCurrentLength);

		mediaData.resize(mediaData.size() + cbCurrentLength);
		memcpy(mediaData.data() + mediaData.size() - cbCurrentLength, pBuffer, cbCurrentLength);

		pMFMediaBuffer->Unlock();

		pMFMediaBuffer->Release();
		pMFSample->Release();
	}
	pMFSourceReader->Release();

	////ファイル入力ストリームのインスタンス
	//std::ifstream file;
	////.wavファイルをバイナリモードで開く
	//file.open(fullPath, std::ios_base::binary);
	////ファイルオープン失敗をチェック
	//assert(file.is_open());

	////RIFFヘッダーの読み込み
	//RiffHeader riff;
	//file.read((char*)&riff, sizeof(riff));
	////ファイルがRIFFかチェック
	//if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
	//	assert(0);
	//}
	////タイプがWAVEかチェック
	//if (strncmp(riff.type, "WAVE", 4) != 0) {
	//	assert(0);
	//}
	////Formatチャンクの読み込み
	//FormatChunk format = {};
	////チャンクヘッダーの確認
	//file.read((char*)&format, sizeof(ChunkHeader));
	//if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
	//	assert(0);
	//}

	////チャンク本体の読み込み
	//assert(format.chunk.size <= sizeof(format.fmt));
	//file.read((char*)&format.fmt, format.chunk.size);

	////Dataチャンクの読み込み
	//ChunkHeader data;
	//file.read((char*)&data, sizeof(data));
	////JUNKチャンクを検出した場合
	//if (strncmp(data.id, "JUNK", 4) == 0) {
	//	//読み取り位置をJUNKチャンクの終わりまで進める
	//	file.seekg(data.size, std::ios_base::cur);
	//	//再読み込み
	//	file.read((char*)&data, sizeof(data));
	//}
	//if (strncmp(data.id, "data", 4) != 0) {
	//	assert(0);
	//}

	////Dataチャンクのデータ部(波形データ）の読み込み
	//char* pBuffer = new char[data.size];
	//file.read(pBuffer, data.size);

	////Waveファイルを閉じる
	//file.close();

	//returnするための音声データ
	SoundData soundData = {};

	soundData.wfex = *waveFormat;
	soundData.pBuffer = mediaData.data();
	soundData.bufferSize = sizeof(BYTE) * static_cast<UINT32>(mediaData.size());

	//サウンドデータを連想配列に格納
	soundDatas_.insert(std::make_pair(filename, soundData));
}


void Audio::Unload(SoundData* soundData)
{
	//バッファのメモリを解放
	delete[] soundData->pBuffer;

	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void Audio::PlayWave(const std::string filename)
{
	HRESULT result;

	std::map<std::string, SoundData>::iterator it = soundDatas_.find(filename);
	assert(it != soundDatas_.end());

	//サウンドデータの参照を取得
	SoundData& soundData = it->second;

	//波形フォーマットをもとにSourceVoiceの生成
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//再生する波形データの設定
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//波形データの再生
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}
