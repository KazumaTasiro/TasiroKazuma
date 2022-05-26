#pragma once
#include <d3d12.h>
#include <math.h>
#include <DirectXMath.h>

class DrawingMesh
{
public:
	struct Vertex
	{
		DirectX::XMFLOAT3 pos; // xyz座標
		DirectX::XMFLOAT2 uv;  // uv座標
	};

	DrawingMesh(ID3D12Device* device, Vertex vertex[], int verticesCount);
	~DrawingMesh();
	void Update();
	void Draw(ID3D12GraphicsCommandList* commandList);

	// インデックスデータ
	unsigned short indices[6] = {
	0, 1, 2, // 三角形1つ目
	1, 2, 3, // 三角形2つ目
	};
	HRESULT result;
	D3D12_HEAP_PROPERTIES heapProp{};
	ID3D12PipelineState* pipelineState = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	ID3D12Resource* constBuffMaterial = nullptr;
	ID3D12DescriptorHeap* srvHeap = nullptr;
	D3D12_INDEX_BUFFER_VIEW ibView{};
};