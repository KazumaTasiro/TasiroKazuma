#pragma once
#include"SpriteCommon.h"
#include <DirectXMath.h>

using namespace DirectX;


// ���_�f�[�^�\����
struct Vertex
{
	XMFLOAT3 pos; // xyz���W
	XMFLOAT2 uv;  // uv���W
};
//�X�v���C�g
class Sprite {
public:

public:
	//������
	void Initialize(SpriteCommon* spritecommon_);

	void Draw();
private:
	SpriteCommon* spritecomon;
	HRESULT result;
	// ���_�f�[�^
	Vertex vertices[4] = {
		// x      y     z       u     v
		{{-0.4f, -0.7f, 0.0f}, {0.0f, 1.0f}}, // ����
		{{-0.4f, +0.7f, 0.0f}, {0.0f, 0.0f}}, // ����
		{{+0.4f, -0.7f, 0.0f}, {1.0f, 1.0f}}, // �E��
		{{+0.4f, +0.7f, 0.0f}, {1.0f, 0.0f}}, // �E��
	};
	//XMFLOAT3 vertices[] = {
	//{ -0.5f, -0.5f, 0.0f }, // ����
	//{ -0.5f, +0.5f, 0.0f }, // ����
	//{ +0.5f, -0.5f, 0.0f }, // �E��
	//};
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};

};