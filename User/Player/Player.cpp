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

void Player::Initialize(SpriteCommon* spriteCommon, Input* input, WinApp* winApp_, DirectXCommon* dxCommon, ParticleManager* particle)
{
	assert(spriteCommon);
	assert(particle);
	assert(input);
	assert(winApp_);
	assert(dxCommon);

	input_ = input;
	winApp = winApp_;
	dxCommon_ = dxCommon;
	//引数として受け取ったデータをメンバ変数に記録する
	//spriteCommon_ = spriteCommon;
	//ワールド変換の初期化
	worldTransform_ = new Object3dFbx;
	/*worldTransform_ = Object3d::Create();*/
	worldTransform3DReticle_ = Object3d::Create();

	model_ = FbxLoader::GetInstance()->LoadModelFromFile("player2");
	/*model_ = Model::LoadFormOBJ("trakku");*/

	worldTransform_->Initialize();
	worldTransform_->SetModel(model_);
	worldTransform_->PlayAnimation();
	worldTransform_->wtf.position = { 0, 0, -10 };
	worldTransform_->wtf.scale = { 0.5f, 0.5f, 0.5f };

	bulletModel_ = Model::LoadFormOBJ("playerBullet");

	worldTransform3DReticle_->SetModel(bulletModel_);
	//スプライト生成
	sprite2DReticle_ = new Sprite();
	sprite2DReticle_->Initialize(spriteCommon, 0);

	sprite2DReticleLock_ = new Sprite();
	sprite2DReticleLock_->Initialize(spriteCommon, 1);

	worldTransform_->wtf.position = playerResetPos;

	////パーティクル生成
	playerDeadParticle = particle;

	playerHp = 10;
}

void Player::Update()
{
	
	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
	if (playerHp <= 0) {
		if (DeadParticle == false) {
			PlayerDeadParticle();
		}
		DeadParticle = true;
		//isDead_ = true;
	}
	//キャラクター移動処理
	Move();
	//キャラクター攻撃更新
	Attack();
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}

	MouseReticle();
	ReticleLimit();
	sprite2DReticleLock_->SetPozition(sprite2DReticle_->GetPosition());
	playerDeadParticle->Update();
	if (DeadParticle) {
		EffectWaiteTime--;

	}
	if (EffectWaiteTime <= 0) {
		isDead_ = true;
	}
}

void Player::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { 0,0,0 };
	Vector3 camMove = { 0,0,0 };
	Vector3 pos = worldTransform_->wtf.position;

	//const float RotSpeed = 0.05f;
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
	//移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;
	//範囲を超えない処理
	worldTransform_->wtf.position.x = max(worldTransform_->wtf.position.x, -kMoveLimitX);
	worldTransform_->wtf.position.x = min(worldTransform_->wtf.position.x, +kMoveLimitX);
	worldTransform_->wtf.position.y = max(worldTransform_->wtf.position.y, -kMoveLimitY);
	worldTransform_->wtf.position.y = min(worldTransform_->wtf.position.y, +kMoveLimitY);

	sprite2DReticle_->SetPozition({ max(worldTransform_->wtf.position.x, 1280),max(worldTransform_->wtf.position.y, 720) });
	sprite2DReticle_->SetPozition({ min(worldTransform_->wtf.position.x, 0),min(worldTransform_->wtf.position.y, 0) });

	//worldTransform_->wtf.position = { 0,0,-30 };
	PlayerLimit();
	worldTransform_->Update();
}

void Player::OnCollision()
{

	playerHp--;
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
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos = worldTransform_->GetWorldPosition();

	return worldPos;
}


void Player::Draw()
{

	/*worldTransform3DReticle_->Draw();*/
	
	/*model_->Draw(worldTransform_, viewProjection_, textureHandle_);*/
//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}
	////3Dレティクルを描画
	//model_->Draw(worldTransform3DReticle_, viewProjection_, textureHandle_);

}

void Player::DrawFbx()
{
	if (DeadParticle == false) {
		if (worldTransform_->wtf.position.z < 100) {
			worldTransform_->Draw(dxCommon_->GetCommandList());
		}
	}
}

void Player::Attack()
{
	if (input_->TriggerMouse(0))
	{
		//弾の速度
		const float kBulletSpeed = 0.01f;
		Vector3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = ConvertToVector3(worldTransform_->wtf.matWorld, velocity);

		//自機から標準オブジェクトへのベクトル
		velocity = worldTransform3DReticle_->wtf.position - worldTransform_->wtf.position;
		Vec3Normalize(&velocity, &velocity);
		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletModel_, GetWorldPosition(), velocity);
		//弾を発射する
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
	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 60.0f;
	//自機から3Dレティクルへのオフセット(Z+向き)
	Vector3 offset = { 0, 0, 1.0f };
	//自機のワールド座標の回転を反映
	offset = ConvertToVector3(worldTransform_->wtf.matWorld, offset);
	//ベクトルの長さを整える
	Vec3Normalize(&offset, &offset);
	offset.x *= kDistancePlayerTo3DReticle;
	offset.y *= kDistancePlayerTo3DReticle;
	offset.z *= kDistancePlayerTo3DReticle;
	//3Dレティクル座標設定
	worldTransform3DReticle_->wtf.position = {
		offset.x + worldTransform_->wtf.matWorld.m[3][0],
		offset.y + worldTransform_->wtf.matWorld.m[3][1],
		offset.z + worldTransform_->wtf.matWorld.m[3][2] };
	worldTransform_->Update();
	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = { worldTransform3DReticle_->wtf.matWorld.m[3][0],worldTransform3DReticle_->wtf.matWorld.m[3][1],worldTransform3DReticle_->wtf.matWorld.m[3][2] };

	Vector2 windowWH =
		Vector2(WinApp::window_width, WinApp::window_height);

	//ビューポート行列
	Matrix4 Viewport = {
	   windowWH.x / 2,			  0,  0,  0,
					0,	-windowWH.y / 2,  0,  0,
					0,				  0,  1,  0,
	   windowWH.x / 2,	 windowWH.y / 2,  0,  1
	};

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matViewProjectionViewport = worldTransform_->camera->GetViewMatrix() * worldTransform_->camera->GetProjectionMatrix() * Viewport;

	//ワールド→スクリーン座標変換(ここから3Dから2Dになる)
	positionReticle = clossV3V4(positionReticle, ConvertXM::ConvertMat4toXMMAT(matViewProjectionViewport));

	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPozition(Vector2(positionReticle.x, positionReticle.y));

	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	{
		POINT mousePosition;
		//マウス座標（スクリーン座標）を取得する
		GetCursorPos(&mousePosition);

		//クライアントエリア座標に変換する
		assert(winApp);
		HWND hwnd = winApp->GetHwnd();
		assert(hwnd);
		ScreenToClient(hwnd, &mousePosition);

		//マウス座標を2Dレティクルのスプライトに代入する
		sprite2DReticle_->SetPozition(Vector2(static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y)));
		ReticleLimit();
		mousePosition = { static_cast<long>(sprite2DReticle_->GetPosition().x),static_cast<long>(sprite2DReticle_->GetPosition().y) };

		//ビュー行列、射影変換、ビューポート行列の合成行列を計算する
		Matrix4 matVPV = worldTransform_->camera->GetViewMatrix() * worldTransform_->camera->GetProjectionMatrix() * Viewport;

		//合成行列の逆行列を計算する
		Matrix4 matInverseVPV;
		Matrix4::MatrixInverse(matInverseVPV, matVPV);
		//ニアクリップ面上のワールド座標得る（スクリーン→ワールド変換）
		Vector3 posNear = Vector3(static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y), 0);
		//ファークリップ面上のワールド座標を得る（スクリーン→ワールド変換）
		Vector3 posFar = Vector3(static_cast<float> (mousePosition.x), static_cast<float> (mousePosition.y), 1);

		//スクリーン座標系からワールド座標系へ
		posNear = clossV3V4(posNear, ConvertXM::ConvertMat4toXMMAT(matInverseVPV));
		posFar = clossV3V4(posFar, ConvertXM::ConvertMat4toXMMAT(matInverseVPV));

		farCre = posNear;
		farCre = posFar;
		//マウスの前方ベクトルを計算する
		//マウスレイの方向
		Vector3 mouseDirection;
		mouseDirection.x = posFar.x - posNear.x;
		mouseDirection.y = posFar.y - posNear.y;
		mouseDirection.z = posFar.z - posNear.z;
		mouseDirection = mouseDirection.nomalize();
		//ニアクリップ面上のワールド座標から一定距離前進したところに3Dレティクルを配置
		//カメラから照準オブジェクトの距離
		const float kDistanceTestObject = 120.0f;
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
	playerHp = 10;
	isDead_ = false;
	DeadParticle = false;
	EffectWaiteTime = 50;
	worldTransform_->Update();
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

void Player::ReticleLimit()
{
	if (sprite2DReticle_->GetPosition().x <= 0) {
		sprite2DReticle_->SetPozition({ 0,sprite2DReticle_->GetPosition().y });
	}
	if (sprite2DReticle_->GetPosition().y <= 0) {
		sprite2DReticle_->SetPozition({ sprite2DReticle_->GetPosition().x,0 });
	}
	if (sprite2DReticle_->GetPosition().x >= winApp->window_width) {
		sprite2DReticle_->SetPozition({ winApp->window_width,sprite2DReticle_->GetPosition().y });
	}
	if (sprite2DReticle_->GetPosition().y >= winApp->window_height) {
		sprite2DReticle_->SetPozition({ sprite2DReticle_->GetPosition().x,winApp->window_height });
	}
}

void Player::PlayerLimit()
{
	if (worldTransform_->wtf.position.x < -4) {
		worldTransform_->wtf.position.x = -4;
	}
	if (worldTransform_->wtf.position.x > 4) {
		worldTransform_->wtf.position.x = 4;
	}
}

void Player::PlayerDeadParticle()
{
	//パーティクル範囲
	for (int i = 0; i < 5; i++) {
		//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = 5.0f;
		Vector3 pos = worldTransform_->wtf.position;
		pos.x += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = 0.0f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = 0.0000f;
		Vector3 acc{};
		acc.x = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = (float)rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//追加
		playerDeadParticle->Add(30, pos, vel, acc, 0.0f, 25.0f, 1);
		playerDeadParticle->Add(30, pos, vel, acc, 0.0f, 25.0f, 2);
		playerDeadParticle->Update();
	}
}

void Player::ParticleDraw()
{
	playerDeadParticle->Draw();
}

void Player::ClearMove()
{
	Vector3 move = { 0,0,1 };
	worldTransform_->wtf.position += move;
	worldTransform_->Update();
}

void Player::TitleMove()
{
	worldTransform_->wtf.rotation.y;
}

