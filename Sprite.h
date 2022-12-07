#pragma once
#include"SpriteCommon.h"
#include <DirectXMath.h>

using namespace DirectX;

//�X�v���C�g
class Sprite {
public:
	//������
	void Initialize(SpriteCommon* spritecommon_);

	void Draw();
private:
	SpriteCommon* spritecomon;
	HRESULT result;
	// ���_�f�[�^
	XMFLOAT3 vertices[3] = {
		{ -0.5f, -0.5f, 0.0f },	// ����
		{-0.5f, +0.5f, 0.0f },	// ����
		{+0.5f, -0.5f, 0.0f },	// �E��
	};
	//XMFLOAT3 vertices[] = {
	//{ -0.5f, -0.5f, 0.0f }, // ����
	//{ -0.5f, +0.5f, 0.0f }, // ����
	//{ +0.5f, -0.5f, 0.0f }, // �E��
	//};
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

};