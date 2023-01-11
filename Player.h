#pragma once
#include"Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input/Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include <WinApp.h>
#include "Sprite.h"

///<summary>
///���L����
///</summary>
class Player {
public:
	Player();
	~Player();
	///<summary>
	///������
	///</summary>
	void Initialize(Model* model, uint32_t textureHandle);
	///<summary>
	///�X�V
	///</summary>
	void Update();
	void Move();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	XMFLOAT3 ConvertToVector3();

	XMFLOAT3 GetWorldPosition();


	///<summary>
	///�`��
	///</summary>
	void Draw();
	///<summary>
	///�U��
	///</summary>
	void Attack();

	//�e���X�g���擾
	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() { return bullets_; }

	//�x�N�g���𐳋K������
	int Vec3Normalize(XMFLOAT3* pOut, XMFLOAT3* pV);

	///<summary>
	///UI�`��
	///</summary>
	void DrawUI();

	XMFLOAT3 clossV3V4(const XMFLOAT3& vec, const XMMATRIX& mat);

	int MatrixInverse(XMMATRIX& pOut, XMMATRIX& pM);
	XMFLOAT3 AddVector(const XMFLOAT3 v1, const XMFLOAT3 v2);

	void Reset();

private:
	//���[���h�ϊ��f�[�^
	Object3d worldTransform_;

	//3D���e�B�N���p���[���h�g�����X�t�H�[��
	Object3d worldTransform3DReticle_;

	//���f��
	Model* model_ = nullptr;
	//�e���f��
	Model* bulletModel_ = nullptr;
	//�e�N�X�`���n���h��
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
	//�e
	std::list<std::unique_ptr<PlayerBullet>> bullets_;
	//2D���e�B�N���p�X�v���C�g
	std::unique_ptr<Sprite> sprite2DReticle_;

	//�f�X�t���O
	bool isDead_ = false;

};