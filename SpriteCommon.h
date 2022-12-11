#pragma once
#include"DirectXCommon.h"
#include <DirectXTex.h>

using namespace DirectX;


//�X�v���C�g���ʕ���
class SpriteCommon {
public:
	// �萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
	struct ConstBufferDataMaterial {
		XMFLOAT4 color; // �F (RGBA)
	};

public:
	//������
	void Initialize(DirectXCommon* dxcommon);

	DirectXCommon* GetDxCommon() { return dxcommon_; }

	ID3D12RootSignature* GetRootSignature() { return rootSignature; }

	ID3D12PipelineState* GetPipelineState() { return pipelineState; }

	ID3D12Resource* GetConstBuffMaterial() { return constBuffMaterial; }

	ID3D12DescriptorHeap* GetSrvHeap() { return srvHeap; }

	D3D12_CPU_DESCRIPTOR_HANDLE GetSrvHandle() { return srvHandle; }

	ID3D12Resource* GetTexBuff() { return texBuff; }

	//SRV�p�f�X�N���v�^�q�[�v
	ID3D12DescriptorHeap* srvHeap = nullptr;
private:
	DirectXCommon* dxcommon_ = nullptr;
	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
	HRESULT result;


	// ���[�g�V�O�l�`��
	ID3D12RootSignature* rootSignature;


	// �p�C�v�����X�e�[�g�̐���
	ID3D12PipelineState* pipelineState = nullptr;
	ID3D12Resource* constBuffMaterial = nullptr;

	// �e�N�X�`���o�b�t�@
	ID3D12Resource* texBuff = nullptr;

	//�������s�N�Z����
	const size_t textureWidth = 256;
	//�c�����s�N�Z����
	const size_t textureHeight = 256;
	//�z��̗v�f��
	const size_t imageDataCount = textureWidth * textureHeight;

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle;
};