#pragma once
#include"Object3d.h"
#include "Model.h"
#include <cassert>
#include "Input.h"
#include "PlayerBullet.h"
#include <memory>
#include <list>
#include <WinApp.h>
#include "Sprite.h"
#include "Vector3.h"
#include "ConvertXM.h"
#include "Affin.h"

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
	void Initialize(SpriteCommon* spriteCommon, Input* input, WinApp* winApp_);
	///�X�V
	///</summary>
	void Update();
	void Move();

	//�Փ˂����o������Ăяo�����R�[���o�b�N�֐�
	void OnCollision();

	Vector3 ConvertToVector3(const Matrix4& mat, Vector3 vec);

	Vector3 GetWorldPosition();


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
	int Vec3Normalize(Vector3* pOut, Vector3* pV);

	///<summary>
	///UI�`��
	///</summary>
	void DrawUI();

	Vector3 clossV3V4(const Vector3& vec, const XMMATRIX& mat);

	/*int MatrixInverse(XMMATRIX& pOut, XMMATRIX& pM);*/
	Vector3 AddVector(const Vector3 v1, const Vector3 v2);

	void MouseReticle();

	void Reset();

	Vector2 GetReticlePos();

	Vector3 GetFarNear();
	
	void ReticleLimit();

	void PlayerLimit();

private:
	//���[���h�ϊ��f�[�^
	Object3d* worldTransform_;

	//3D���e�B�N���p���[���h�g�����X�t�H�[��
	Object3d* worldTransform3DReticle_;

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
	Sprite* sprite2DReticle_;
	Sprite* sprite2DReticleLock_;
	SpriteCommon* spriteCommon_ = nullptr;
	//�f�X�t���O
	bool isDead_ = false;
	WinApp* winApp = nullptr;

	Vector3 farCre;
	Vector3 nearCre;

	Vector3 moveLimit = { 20,20,20 };
	bool limitIn = false;

	Vector3 playerResetPos = { 0,-2.5f,0 };

	const float speed = 0.5f;
};