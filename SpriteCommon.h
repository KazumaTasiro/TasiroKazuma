#pragma once
#include"DirectXCommon.h"




//�X�v���C�g���ʕ���
class SpriteCommon {
public:
	//������
	void Initialize(DirectXCommon* dxcommon);

	DirectXCommon* GetDxCommon() { return dxcommon_; }

	ID3D12RootSignature* GetRootSignature() { return rootSignature; }

	ID3D12PipelineState* GetPipelineState() { return pipelineState; }
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
};