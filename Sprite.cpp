#include"Sprite.h"

void Sprite::Initialize(SpriteCommon* spritecommon_)
{
	
	spritecomon = spritecommon_;
	
	// 頂点データ全体のサイズ = 頂点データ一つ分のサイズ * 頂点データの要素数
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	// 頂点バッファの設定
	D3D12_HEAP_PROPERTIES heapProp{}; // ヒープ設定
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPUへの転送用
	// リソース設定
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // 頂点データ全体のサイズ
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// 頂点バッファの生成
	ID3D12Resource* vertBuff = nullptr;
	result = spritecomon->GetDxCommon()->GetDevice()->CreateCommittedResource(
		&heapProp, // ヒープ設定
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // リソース設定
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));
	// GPU上のバッファに対応した仮想メモリ(メインメモリ上)を取得
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// 全頂点に対して
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // 座標をコピー
	}
	// 繋がりを解除
	vertBuff->Unmap(0, nullptr);
	
	// GPU仮想アドレス
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// 頂点バッファのサイズ
	vbView.SizeInBytes = sizeVB;
	// 頂点1つ分のデータサイズ
	vbView.StrideInBytes = sizeof(vertices[0]);

	// シェーダリソースビュー設定
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;

	// ハンドルの指す位置にシェーダーリソースビュー作成
	spritecomon->GetDxCommon()->GetDevice()->CreateShaderResourceView(spritecomon->GetTexBuff(), &srvDesc, spritecomon->GetSrvHandle());
}

void Sprite::Draw()
{
	// パイプラインステートとルートシグネチャの設定コマンド
	spritecomon->GetDxCommon()->GetCommandList()->SetPipelineState(spritecomon->GetPipelineState());
	spritecomon->GetDxCommon()->GetCommandList()->SetGraphicsRootSignature(spritecomon->GetRootSignature());
	//プリミティブ形状の設定コマンド
	spritecomon->GetDxCommon()->GetCommandList()->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);//三角リスト
	//頂点バッファビューの設定コマンド
	spritecomon->GetDxCommon()->GetCommandList()->IASetVertexBuffers(0, 1, &vbView);
	// 定数バッファビュー(CBV)の設定コマンド
	spritecomon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(0, spritecomon->GetConstBuffMaterial()->GetGPUVirtualAddress());

	// SRVヒープの設定コマンド
	spritecomon->GetDxCommon()->GetCommandList()->SetDescriptorHeaps(1, &spritecomon->srvHeap);
	// SRVヒープの先頭ハンドルを取得（SRVを指しているはず）
	D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = spritecomon->GetSrvHeap()->GetGPUDescriptorHandleForHeapStart();
	// SRVヒープの先頭にあるSRVをルートパラメータ1番に設定
	spritecomon->GetDxCommon()->GetCommandList()->SetGraphicsRootDescriptorTable(1, srvGpuHandle);

	// 定数バッファビュー(CBV)の設定コマンド
	spritecomon->GetDxCommon()->GetCommandList()->SetGraphicsRootConstantBufferView(2, spritecomon->GetConstBuffTransform()->GetGPUVirtualAddress());

	// 描画コマンド
	spritecomon->GetDxCommon()->GetCommandList()->DrawInstanced(_countof(vertices), 1, 0, 0); // 全ての頂点を使って描画
}
