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
	//xAudio���
	xAudio2_.Reset();
	//�T�E���h�f�[�^���
	std::map<std::string, SoundData>::iterator it = soundDatas_.begin();
	for (; it != soundDatas_.end(); it++) {
		Unload(&it->second);
	}
	soundDatas_.clear();
}

void Audio::LoadWave(const std::string filename)
{
	if (soundDatas_.find(filename) != soundDatas_.end()) {
		//�d��
		return;
	}

	//�f���N�g���p�X�ƃt�@�C������A�����ăt���p�X�𓾂�
	std::string fullPath = directoryPath_ + filename;

	// ���j�R�[�h������ɕϊ�
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

	// �������擾
	float duration = 0.0f;
	PROPVARIANT var;
	HRESULT hr =
		pMFSourceReader->GetPresentationAttribute(MF_SOURCE_READER_MEDIASOURCE, MF_PD_DURATION, &var);
	if (SUCCEEDED(hr)) {
		// 100�i�m�b�P�ʂȂ̂ŕb�ɒ����𒼂�
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

	////�t�@�C�����̓X�g���[���̃C���X�^���X
	//std::ifstream file;
	////.wav�t�@�C�����o�C�i�����[�h�ŊJ��
	//file.open(fullPath, std::ios_base::binary);
	////�t�@�C���I�[�v�����s���`�F�b�N
	//assert(file.is_open());

	////RIFF�w�b�_�[�̓ǂݍ���
	//RiffHeader riff;
	//file.read((char*)&riff, sizeof(riff));
	////�t�@�C����RIFF���`�F�b�N
	//if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
	//	assert(0);
	//}
	////�^�C�v��WAVE���`�F�b�N
	//if (strncmp(riff.type, "WAVE", 4) != 0) {
	//	assert(0);
	//}
	////Format�`�����N�̓ǂݍ���
	//FormatChunk format = {};
	////�`�����N�w�b�_�[�̊m�F
	//file.read((char*)&format, sizeof(ChunkHeader));
	//if (strncmp(format.chunk.id, "fmt ", 4) != 0) {
	//	assert(0);
	//}

	////�`�����N�{�̂̓ǂݍ���
	//assert(format.chunk.size <= sizeof(format.fmt));
	//file.read((char*)&format.fmt, format.chunk.size);

	////Data�`�����N�̓ǂݍ���
	//ChunkHeader data;
	//file.read((char*)&data, sizeof(data));
	////JUNK�`�����N�����o�����ꍇ
	//if (strncmp(data.id, "JUNK", 4) == 0) {
	//	//�ǂݎ��ʒu��JUNK�`�����N�̏I���܂Ői�߂�
	//	file.seekg(data.size, std::ios_base::cur);
	//	//�ēǂݍ���
	//	file.read((char*)&data, sizeof(data));
	//}
	//if (strncmp(data.id, "data", 4) != 0) {
	//	assert(0);
	//}

	////Data�`�����N�̃f�[�^��(�g�`�f�[�^�j�̓ǂݍ���
	//char* pBuffer = new char[data.size];
	//file.read(pBuffer, data.size);

	////Wave�t�@�C�������
	//file.close();

	//return���邽�߂̉����f�[�^
	SoundData soundData = {};

	soundData.wfex = *waveFormat;
	soundData.pBuffer = mediaData.data();
	soundData.bufferSize = sizeof(BYTE) * static_cast<UINT32>(mediaData.size());

	//�T�E���h�f�[�^��A�z�z��Ɋi�[
	soundDatas_.insert(std::make_pair(filename, soundData));
}


void Audio::Unload(SoundData* soundData)
{
	//�o�b�t�@�̃����������
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

	//�T�E���h�f�[�^�̎Q�Ƃ��擾
	SoundData& soundData = it->second;

	//�g�`�t�H�[�}�b�g�����Ƃ�SourceVoice�̐���
	IXAudio2SourceVoice* pSourceVoice = nullptr;
	result = xAudio2_->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
	assert(SUCCEEDED(result));

	//�Đ�����g�`�f�[�^�̐ݒ�
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = soundData.pBuffer;
	buf.AudioBytes = soundData.bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;

	//�g�`�f�[�^�̍Đ�
	result = pSourceVoice->SubmitSourceBuffer(&buf);
	result = pSourceVoice->Start();
}
