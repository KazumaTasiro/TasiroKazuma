#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
	//delete input_;
	//delete sprite2DReticle_;
	//delete spriteCommon_;
	//delete model_;
	//delete worldTransform_;
	//delete worldTransform3DReticle_;
}

void Player::Initialize(SpriteCommon* spriteCommon, Input* input, WinApp* winApp_)
{
	assert(spriteCommon);
	assert(input);
	assert(winApp_);

	input_ = input;
	winApp = winApp_;
	//�����Ƃ��Ď󂯎�����f�[�^�������o�ϐ��ɋL�^����
	//spriteCommon_ = spriteCommon;
	//���[���h�ϊ��̏�����
	worldTransform_ = Object3d::Create();
	worldTransform3DReticle_ = Object3d::Create();

	model_ = Model::LoadFormOBJ("trakku");

	worldTransform_->SetModel(model_);

	worldTransform_->wtf.position = { 0, 0, 0 };

	bulletModel_ = Model::LoadFormOBJ("bullet");

	worldTransform3DReticle_->SetModel(bulletModel_);
	//�X�v���C�g����
	sprite2DReticle_ = new Sprite();
	sprite2DReticle_->Initialize(spriteCommon, 0);

	sprite2DReticleLock_ = new Sprite();
	sprite2DReticleLock_->Initialize(spriteCommon, 1);

	worldTransform_->wtf.position = playerResetPos;

}

void Player::Update()
{

	//�f�X�t���O�̗������e���폜
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
	//�L�����N�^�[�ړ�����
	Move();
	//�L�����N�^�[�U���X�V
	Attack();
	//�e�X�V
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	MouseReticle();
	sprite2DReticleLock_->SetPozition(sprite2DReticle_->GetPosition());
}

void Player::Move()
{
	//�L�����N�^�[�̈ړ��x�N�g��
	Vector3 move = { 0,0,0 };
	Vector3 camMove = { 0,0,0 };
	Vector3 pos = worldTransform_->wtf.position;
	const float speed = 1.0f;
	const float RotSpeed = 0.05f;
	if (input_->PushKey(DIK_A)) {
		move.x -= speed;
	}
	else if (input_->PushKey(DIK_D)) {
		move.x += speed;

	}
	if (input_->PushKey(DIK_LEFT)) {
		camMove.x -= speed;
	}
	else if (input_->PushKey(DIK_RIGHT)) {
		camMove.x += speed;

	}
	worldTransform_->wtf.position = { pos.x + move.x,pos.y + move.y,pos.z + move.z };
	worldTransform_->camera->SetTarget(worldTransform_->camera->GetTarget() + camMove);
	worldTransform_->camera->SetEye({ worldTransform_->camera->GetEye().x + camMove.x, worldTransform_->camera->GetEye().y + camMove.y, worldTransform_->wtf.position.z - 10 });

	//worldTransform_->SetEye({ pos.x + move.x,pos.y + move.y,pos.z + move.z });
	//worldTransform_->SetTarget({ pos.x + move.x,pos.y + move.y,10 });
	//�ړ����E���W
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;
	//�͈͂𒴂��Ȃ�����
	worldTransform_->wtf.position.x = max(worldTransform_->wtf.position.x, -kMoveLimitX);
	worldTransform_->wtf.position.x = min(worldTransform_->wtf.position.x, +kMoveLimitX);
	worldTransform_->wtf.position.y = max(worldTransform_->wtf.position.y, -kMoveLimitY);
	worldTransform_->wtf.position.y = min(worldTransform_->wtf.position.y, +kMoveLimitY);

	sprite2DReticle_->SetPozition({ max(worldTransform_->wtf.position.x, 1280),max(worldTransform_->wtf.position.y, 720) });
	sprite2DReticle_->SetPozition({ min(worldTransform_->wtf.position.x, 0),min(worldTransform_->wtf.position.y, 0) });

	//worldTransform_->wtf.position = { 0,0,-30 };

	worldTransform_->Update();
}

void Player::OnCollision()
{
	//�f�X�t���O
	bool isDead_ = false;
}

Vector3 Player::ConvertToVector3(const Matrix4& mat, Vector3 vec)
{
	Vector3 retVec = {};


	retVec.x = vec.x * mat.m[0][0] + vec.y * mat.m[1][0] + vec.z * mat.m[2][0];

	retVec.y = vec.x * mat.m[0][1] + vec.y * mat.m[1][1] + vec.z * mat.m[2][1];

	retVec.z = vec.x * mat.m[0][2] + vec.y * mat.m[1][2] + vec.z * mat.m[2][2];

	return retVec;
}

Vector3 Player::GetWorldPosition()
{
	//���[���h���W������ϐ�
	Vector3 worldPos;
	//���[���h�s��̕��s�ړ��������擾�i���[���h���W�j
	worldPos = worldTransform_->GetWorldPosition();

	return worldPos;
}


void Player::Draw()
{

	worldTransform3DReticle_->Draw();
	worldTransform_->Draw();
	/*model_->Draw(worldTransform_, viewProjection_, textureHandle_);*/
//�e�`��
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}
	////3D���e�B�N����`��
	//model_->Draw(worldTransform3DReticle_, viewProjection_, textureHandle_);
}

void Player::Attack()
{
	if (input_->TriggerMouse(0))
	{
		//�e�̑��x
		const float kBulletSpeed = 0.01f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//���x�x�N�g�������@�̌����ɍ��킹�ĉ�]������
		velocity = ConvertToVector3(worldTransform_->wtf.matWorld, velocity);

		//���@����W���I�u�W�F�N�g�ւ̃x�N�g��
		velocity = worldTransform3DReticle_->wtf.position - worldTransform_->wtf.position;
		Vec3Normalize(&velocity, &velocity);
		//�e�𐶐����A������
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletModel_, GetWorldPosition(), velocity);
		//�e�𔭎˂���
		bullets_.push_back(std::move(newBullet));
	}
}

int Player::Vec3Normalize(Vector3* pOut, Vector3* pV)
{
	double len;
	double x, y, z;

	x = (double)(pV->x);
	y = (double)(pV->y);
	z = (double)(pV->z);
	len = sqrt(x * x + y * y + z * z);

	if (len < (1e-6)) return 0;

	len = 1.0 / len;
	x *= len;
	y *= len;
	z *= len;

	pOut->x = (float)x;
	pOut->y = (float)y;
	pOut->z = (float)z;

	return 1;
}

void Player::DrawUI()
{

	if (input_->PushMouse(1)) {
		sprite2DReticleLock_->Draw();
	}
	else {
		sprite2DReticle_->Draw();
	}

}

Vector3 Player::clossV3V4(const Vector3& vec, const XMMATRIX& mat)
{
	Vector4 divVec = {};

	divVec.x = vec.x * mat.r[0].m128_f32[0] + vec.y * mat.r[1].m128_f32[0] + vec.z * mat.r[2].m128_f32[0] + 1 * mat.r[3].m128_f32[0];

	divVec.y = vec.x * mat.r[0].m128_f32[1] + vec.y * mat.r[1].m128_f32[1] + vec.z * mat.r[2].m128_f32[1] + 1 * mat.r[3].m128_f32[1];

	divVec.z = vec.x * mat.r[0].m128_f32[2] + vec.y * mat.r[1].m128_f32[2] + vec.z * mat.r[2].m128_f32[2] + 1 * mat.r[3].m128_f32[2];

	divVec.w = vec.x * mat.r[0].m128_f32[3] + vec.y * mat.r[1].m128_f32[3] + vec.z * mat.r[2].m128_f32[3] + 1 * mat.r[3].m128_f32[3];

	divVec.x = divVec.x / divVec.w;

	divVec.y = divVec.y / divVec.w;

	divVec.z = divVec.z / divVec.w;

	return { divVec.x, divVec.y, divVec.z };
}

Vector3 Player::AddVector(const Vector3 v1, const Vector3 v2)
{
	Vector3 addVec = {};

	addVec.x = v1.x + v2.x;

	addVec.y = v1.y + v2.y;

	addVec.z = v1.z + v2.z;

	return addVec;
}

void Player::MouseReticle()
{
	//���@����3D���e�B�N���ւ̋���
	const float kDistancePlayerTo3DReticle = 60.0f;
	//���@����3D���e�B�N���ւ̃I�t�Z�b�g(Z+����)
	Vector3 offset = { 0, 0, 1.0f };
	//���@�̃��[���h���W�̉�]�𔽉f
	offset = ConvertToVector3(worldTransform_->wtf.matWorld, offset);
	//�x�N�g���̒����𐮂���
	Vec3Normalize(&offset, &offset);
	offset.x *= kDistancePlayerTo3DReticle;
	offset.y *= kDistancePlayerTo3DReticle;
	offset.z *= kDistancePlayerTo3DReticle;
	//3D���e�B�N�����W�ݒ�
	worldTransform3DReticle_->wtf.position = {
		offset.x + worldTransform_->wtf.matWorld.m[3][0],
		offset.y + worldTransform_->wtf.matWorld.m[3][1],
		offset.z + worldTransform_->wtf.matWorld.m[3][2] };
	worldTransform_->Update();
	//3D���e�B�N���̃��[���h���W����2D���e�B�N���̃X�N���[�����W���v�Z
	Vector3 positionReticle = { worldTransform3DReticle_->wtf.matWorld.m[3][0],worldTransform3DReticle_->wtf.matWorld.m[3][1],worldTransform3DReticle_->wtf.matWorld.m[3][2] };

	Vector2 windowWH =
		Vector2(WinApp::window_width, WinApp::window_height);

	//�r���[�|�[�g�s��
	Matrix4 Viewport = {
	   windowWH.x / 2,			  0,  0,  0,
					0,	-windowWH.y / 2,  0,  0,
					0,				  0,  1,  0,
	   windowWH.x / 2,	 windowWH.y / 2,  0,  1
	};

	//�r���[�s��ƃv���W�F�N�V�����s��A�r���[�|�[�g�s�����������
	Matrix4 matViewProjectionViewport = worldTransform_->camera->GetViewMatrix() * worldTransform_->camera->GetProjectionMatrix() * Viewport;

	//���[���h���X�N���[�����W�ϊ�(��������3D����2D�ɂȂ�)
	positionReticle = clossV3V4(positionReticle, ConvertXM::ConvertMat4toXMMAT(matViewProjectionViewport));

	//�X�v���C�g�̃��e�B�N���ɍ��W�ݒ�
	sprite2DReticle_->SetPozition(Vector2(positionReticle.x, positionReticle.y));

	//�}�E�X�J�[�\���̃X�N���[�����W���烏�[���h���W���擾����3D���e�B�N���z�u
	{
		POINT mousePosition;
		//�}�E�X���W�i�X�N���[�����W�j���擾����
		GetCursorPos(&mousePosition);

		//�N���C�A���g�G���A���W�ɕϊ�����
		assert(winApp);
		HWND hwnd = winApp->GetHwnd();
		assert(hwnd);
		ScreenToClient(hwnd, &mousePosition);

		//�}�E�X���W��2D���e�B�N���̃X�v���C�g�ɑ������
		sprite2DReticle_->SetPozition(Vector2(static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y)));

		//�r���[�s��A�ˉe�ϊ��A�r���[�|�[�g�s��̍����s����v�Z����
		Matrix4 matVPV = worldTransform_->camera->GetViewMatrix() * worldTransform_->camera->GetProjectionMatrix() * Viewport;

		//�����s��̋t�s����v�Z����
		Matrix4 matInverseVPV;
		Matrix4::MatrixInverse(matInverseVPV, matVPV);
		//�j�A�N���b�v�ʏ�̃��[���h���W����i�X�N���[�������[���h�ϊ��j
		Vector3 posNear = Vector3(static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y), 0);
		//�t�@�[�N���b�v�ʏ�̃��[���h���W�𓾂�i�X�N���[�������[���h�ϊ��j
		Vector3 posFar = Vector3(static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y), 1);

		//�X�N���[�����W�n���烏�[���h���W�n��
		posNear = clossV3V4(posNear, ConvertXM::ConvertMat4toXMMAT(matInverseVPV));
		posFar = clossV3V4(posFar, ConvertXM::ConvertMat4toXMMAT(matInverseVPV));

		farCre = posNear;
		farCre = posFar;
		//�}�E�X�̑O���x�N�g�����v�Z����
		//�}�E�X���C�̕���
		Vector3 mouseDirection;
		mouseDirection.x = posFar.x - posNear.x;
		mouseDirection.y = posFar.y - posNear.y;
		mouseDirection.z = posFar.z - posNear.z;
		mouseDirection = mouseDirection.nomalize();
		//�j�A�N���b�v�ʏ�̃��[���h���W�����苗���O�i�����Ƃ����3D���e�B�N����z�u
		//�J��������Ə��I�u�W�F�N�g�̋���
		const float kDistanceTestObject = 60.0f;
		worldTransform3DReticle_->wtf.position = (AddVector(posNear, { mouseDirection.x * kDistanceTestObject,mouseDirection.y * kDistanceTestObject,mouseDirection.z * kDistanceTestObject }));

		/*debugText_->SetPos(50, 150);
		debugText_->Printf(
			"translation : %f,%f,%f", worldTransform_.translation_.x,
			worldTransform_.translation_.y,
			worldTransform_.translation_.z);*/


		worldTransform3DReticle_->Update();
	}
}

void Player::Reset()
{
	worldTransform_->wtf.position = playerResetPos;
	bullets_.clear();
}

Vector2 Player::GetReticlePos()
{
	Vector2 pos = { worldTransform3DReticle_->GetWorldPosition().x,worldTransform3DReticle_->GetWorldPosition().y };
	return pos;
}

Vector3 Player::GetFarNear()
{
	Vector3 vec = farCre - nearCre;
	vec.nomalize();
	return vec;
}

