#include "Player.h"

Player::Player()
{
}

Player::~Player()
{
}

void Player::Initialize(SpriteCommon* spriteCommon, Input* input)
{
	assert(spriteCommon);
	assert(input);

	input_ = input;
	//引数として受け取ったデータをメンバ変数に記録する
	//spriteCommon_ = spriteCommon;
	//ワールド変換の初期化
	worldTransform_ = Object3d::Create();
	worldTransform3DReticle_ = Object3d::Create();

	worldTransform_->SetPosition({ 0, 0, 5 });

	bulletModel_ = Model::LoadFormOBJ("bullet");
	//スプライト生成
	sprite2DReticle_ = new Sprite();
	sprite2DReticle_->Initialize(spriteCommon, 7);
}

void Player::Update()
{

	//デスフラグの立った弾を削除
	bullets_.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->IsDead();
		});
	//キャラクター移動処理
	Move();
	//キャラクター攻撃更新
	Attack();
	//弾更新
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Update();
	}
	//自機から3Dレティクルへの距離
	const float kDistancePlayerTo3DReticle = 60.0f;
	//自機から3Dレティクルへのオフセット(Z+向き)
	XMFLOAT3 offset = { 0, 0, 1.0f };
	//自機のワールド座標の回転を反映
	offset = ConvertToXMFLOAT3(worldTransform_->GetmMatWorld(), offset);
	//ベクトルの長さを整える
	Vec3Normalize(&offset, &offset);
	offset.x *= kDistancePlayerTo3DReticle;
	offset.y *= kDistancePlayerTo3DReticle;
	offset.z *= kDistancePlayerTo3DReticle;
	//3Dレティクル座標設定
	worldTransform3DReticle_->SetPosition(
		{ offset.x + worldTransform_->GetmMatWorld().r[3].m128_f32[0],
		offset.y + worldTransform_->GetmMatWorld().r[3].m128_f32[1],
		offset.z + worldTransform_->GetmMatWorld().r[3].m128_f32[2] }
	);

	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	XMFLOAT3 positionReticle = { worldTransform3DReticle_->GetmMatWorld().r[3].m128_f32[0],worldTransform3DReticle_->GetmMatWorld().r[3].m128_f32[1],worldTransform3DReticle_->GetmMatWorld().r[3].m128_f32[2] };

	XMFLOAT2 windowWH =
		XMFLOAT2(WinApp::window_width, WinApp::window_height);

	//ビューポート行列
	XMMATRIX Viewport = {
	   windowWH.x / 2,			  0,  0,  0,
					0,	-windowWH.y / 2,  0,  0,
					0,				  0,  1,  0,
	   windowWH.x / 2,	 windowWH.y / 2,  0,  1
	};

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	XMMATRIX matViewProjectionViewport = worldTransform_->GetMatView() * worldTransform_->GetMatProjection() * Viewport;

	//ワールド→スクリーン座標変換(ここから3Dから2Dになる)
	positionReticle = clossV3V4(positionReticle, matViewProjectionViewport);

	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPozition(XMFLOAT2(positionReticle.x, positionReticle.y));

	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	{
		POINT mousePosition;
		//マウス座標（スクリーン座標）を取得する
		GetCursorPos(&mousePosition);

		//クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		//マウス座標を2Dレティクルのスプライトに代入する
		sprite2DReticle_->SetPozition(XMFLOAT2(mousePosition.x, mousePosition.y));

		//ビュー行列、射影変換、ビューポート行列の合成行列を計算する
		XMMATRIX matVPV = worldTransform_->GetMatView() * worldTransform_->GetMatProjection() * Viewport;

		//合成行列の逆行列を計算する
		//合成行列の逆行列を計算する
		XMMATRIX matInverseVPV;
		XMMatrixInverse(matInverseVPV.r, matVPV);
		//ニアクリップ面上のワールド座標得る（スクリーン→ワールド変換）
		XMFLOAT3 posNear = XMFLOAT3(mousePosition.x, mousePosition.y, 0);
		//ファークリップ面上のワールド座標を得る（スクリーン→ワールド変換）
		XMFLOAT3 posFar = XMFLOAT3(mousePosition.x, mousePosition.y, 1);

		//スクリーン座標系からワールド座標系へ
		posNear = clossV3V4(posNear, matInverseVPV);
		posFar = clossV3V4(posFar, matInverseVPV);

		//マウスの前方ベクトルを計算する
		//マウスレイの方向
		XMFLOAT3 mouseDirection;
		mouseDirection.x = posFar.x - posNear.x;
		mouseDirection.y = posFar.y - posNear.y;
		mouseDirection.z = posFar.z - posNear.z;
		mouseDirection = XMFLOAT3Normalize(mouseDirection);
		//ニアクリップ面上のワールド座標から一定距離前進したところに3Dレティクルを配置
		//カメラから照準オブジェクトの距離
		const float kDistanceTestObject = 222.0f;
		worldTransform3DReticle_->SetPosition(AddVector(posNear, { mouseDirection.x * kDistanceTestObject,mouseDirection.y * kDistanceTestObject,mouseDirection.z * kDistanceTestObject }));

		/*debugText_->SetPos(50, 150);
		debugText_->Printf(
			"translation : %f,%f,%f", worldTransform_.translation_.x,
			worldTransform_.translation_.y,
			worldTransform_.translation_.z);*/
		worldTransform3DReticle_->Update();
	}
}

void Player::Move()
{
	//キャラクターの移動ベクトル
	XMFLOAT3 move = { 0,0,0 };
	XMFLOAT3 pos = worldTransform_->GetPosition();
	const float speed = 1.0f;
	const float RotSpeed = 0.05f;
	if (input_->PushKey(DIK_A)) {
		move.x -= speed;
	}
	else if (input_->PushKey(DIK_D)) {
		move.x += speed;
	}
	if (input_->PushKey(DIK_W)) {
		move.y += speed;
	}
	else if (input_->PushKey(DIK_S)) {
		move.y -= speed;
	}
	//if (input_->PushKey(DIK_U)) {
	//	worldTransform_.rotation_.y -= RotSpeed;
	//}
	//else if (input_->PushKey(DIK_I)) {
	//	worldTransform_.rotation_.y += RotSpeed;
	//}
	worldTransform_->SetPosition({ pos.x + move.x,pos.y + move.y,pos.z + move.z });
	worldTransform_->SetEye({ pos.x + move.x,pos.y + move.y,pos.z + move.z });
	worldTransform_->SetTarget({ pos.x + move.x,pos.y + move.y,10 });
	//移動限界座標
	const float kMoveLimitX = 35.0f;
	const float kMoveLimitY = 19.0f;
	//範囲を超えない処理
	worldTransform_->position.x = max(worldTransform_->position.x, -kMoveLimitX);
	worldTransform_->position.x = min(worldTransform_->position.x, +kMoveLimitX);
	worldTransform_->position.y = max(worldTransform_->position.y, -kMoveLimitY);
	worldTransform_->position.y = min(worldTransform_->position.y, +kMoveLimitY);


	worldTransform_->Update();
}

void Player::OnCollision()
{
	//デスフラグ
	bool isDead_ = false;
}

XMFLOAT3 Player::ConvertToXMFLOAT3(const XMMATRIX& mat, XMFLOAT3 vec)
{
	XMFLOAT3 retVec = {};


	retVec.x = vec.x * mat.r[0].m128_f32[0] + vec.y * mat.r[1].m128_f32[0] + vec.z * mat.r[2].m128_f32[0];

	retVec.y = vec.x * mat.r[0].m128_f32[1] + vec.y * mat.r[1].m128_f32[1] + vec.z * mat.r[2].m128_f32[1];

	retVec.z = vec.x * mat.r[0].m128_f32[2] + vec.y * mat.r[1].m128_f32[2] + vec.z * mat.r[2].m128_f32[2];

	return retVec;
}

XMFLOAT3 Player::GetWorldPosition()
{
	//ワールド座標を入れる変数
	XMFLOAT3 worldPos;
	//ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldTransform_->GetmMatWorld().r[3].m128_f32[0];
	worldPos.y = worldTransform_->GetmMatWorld().r[3].m128_f32[1];
	worldPos.z = worldTransform_->GetmMatWorld().r[3].m128_f32[2];

	return worldPos;
}

XMFLOAT3 Player::XMFLOAT3Normalize(XMFLOAT3& v)
{
	return XMFLOAT3();
}

void Player::Draw()
{
	/*model_->Draw(worldTransform_, viewProjection_, textureHandle_);*/
	//弾描画
	for (std::unique_ptr<PlayerBullet>& bullet : bullets_) {
		bullet->Draw();
	}
	////3Dレティクルを描画
	//model_->Draw(worldTransform3DReticle_, viewProjection_, textureHandle_);
}

void Player::Attack()
{
	if (input_->TriggerKey(DIK_SPACE))
	{
		//弾の速度
		const float kBulletSpeed = 0.01f;
		XMFLOAT3 velocity(0, 0, kBulletSpeed);

		//速度ベクトルを自機の向きに合わせて回転させる
		velocity = ConvertToXMFLOAT3(worldTransform_->GetmMatWorld(), velocity);

		//自機から標準オブジェクトへのベクトル
		velocity = {
		worldTransform3DReticle_->GetmMatWorld().r[3].m128_f32[0] - worldTransform_->GetmMatWorld().r[3].m128_f32[0],
		worldTransform3DReticle_->GetmMatWorld().r[3].m128_f32[1] - worldTransform_->GetmMatWorld().r[3].m128_f32[1],
		worldTransform3DReticle_->GetmMatWorld().r[3].m128_f32[2] - worldTransform_->GetmMatWorld().r[3].m128_f32[2] };
		Vec3Normalize(&velocity, &velocity);
		//弾を生成し、初期化
		std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
		newBullet->Initialize(bulletModel_, GetWorldPosition(), velocity);
		//弾を発射する
		bullets_.push_back(std::move(newBullet));
	}
}

int Player::Vec3Normalize(XMFLOAT3* pOut, XMFLOAT3* pV)
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
	sprite2DReticle_->SetPozition({ (1220 / 2),(660 / 2) });
	sprite2DReticle_->Draw();
}

XMFLOAT3 Player::clossV3V4(const XMFLOAT3& vec, const XMMATRIX& mat)
{
	XMFLOAT4 divVec = {};

	divVec.x = vec.x * mat.r[0].m128_f32[0] + vec.y * mat.r[1].m128_f32[0] + vec.z * mat.r[2].m128_f32[0] + 1 * mat.r[3].m128_f32[0];

	divVec.y = vec.x * mat.r[0].m128_f32[1] + vec.y * mat.r[1].m128_f32[1] + vec.z * mat.r[2].m128_f32[1] + 1 * mat.r[3].m128_f32[1];

	divVec.z = vec.x * mat.r[0].m128_f32[2] + vec.y * mat.r[1].m128_f32[2] + vec.z * mat.r[2].m128_f32[2] + 1 * mat.r[3].m128_f32[2];

	divVec.w = vec.x * mat.r[0].m128_f32[3] + vec.y * mat.r[1].m128_f32[3] + vec.z * mat.r[2].m128_f32[3] + 1 * mat.r[3].m128_f32[3];

	divVec.x = divVec.x / divVec.w;

	divVec.y = divVec.y / divVec.w;

	divVec.z = divVec.z / divVec.w;

	return { divVec.x, divVec.y, divVec.z };
}

//int Player::MatrixInverse(XMMATRIX& pOut, XMMATRIX& pM)
//{
//	XMMATRIX mat;
//	int i, j, loop;
//	double fDat, fDat2;
//	double mat_8x4[4][8];
//	int flag;
//	float* pF;
//	double* pD;
//
//	//8 x 4行列に値を入れる
//	for (i = 0; i < 4; i++) {
//		pF = pM.r[i];
//		for (j = 0; j < 4; j++, pF++) mat_8x4[i][j] = (double)(*pF);
//		pD = mat_8x4[i] + 4;
//		for (j = 0; j < 4; j++) {
//			if (i == j)   *pD = 1.0;
//			else         *pD = 0.0;
//			pD++;
//		}
//	}
//
//	flag = 1;
//	for (loop = 0; loop < 4; loop++) {
//		fDat = mat_8x4[loop][loop];
//		if (fDat != 1.0) {
//			if (fDat == 0.0) {
//				for (i = loop + 1; i < 4; i++) {
//					fDat = mat_8x4[i][loop];
//					if (fDat != 0.0) break;
//				}
//				if (i >= 4) {
//					flag = 0;
//					break;
//				}
//				//行を入れ替える
//				for (j = 0; j < 8; j++) {
//					fDat = mat_8x4[i][j];
//					mat_8x4[i][j] = mat_8x4[loop][j];
//					mat_8x4[loop][j] = fDat;
//				}
//				fDat = mat_8x4[loop][loop];
//			}
//
//			for (i = 0; i < 8; i++) mat_8x4[loop][i] /= fDat;
//		}
//		for (i = 0; i < 4; i++) {
//			if (i != loop) {
//				fDat = mat_8x4[i][loop];
//				if (fDat != 0.0f) {
//					//mat[i][loop]をmat[loop]の行にかけて
//					//(mat[j] - mat[loop] * fDat)を計算
//					for (j = 0; j < 8; j++) {
//						fDat2 = mat_8x4[loop][j] * fDat;
//						mat_8x4[i][j] -= fDat2;
//					}
//				}
//			}
//		}
//	}
//
//	if (flag) {
//		for (i = 0; i < 4; i++) {
//			pF = mat.m[i];
//			pD = mat_8x4[i] + 4;
//			for (j = 0; j < 4; j++) {
//				*pF = (float)(*pD);
//				pF++;
//				pD++;
//			}
//		}
//	}
//	else {
//		//単位行列を求める
//		mat = {
//		1,0,0,0,
//		0,1,0,0,
//		0,0,1,0,
//		0,0,0,1
//		};
//	}
//
//	pOut = mat;
//
//	if (flag) return 1;
//	return 0;
//}

XMFLOAT3 Player::AddVector(const XMFLOAT3 v1, const XMFLOAT3 v2)
{
	XMFLOAT3 addVec = {};

	addVec.x = v1.x + v2.x;

	addVec.y = v1.y + v2.y;

	addVec.z = v1.z + v2.z;

	return addVec;
}

void Player::Reset()
{
	worldTransform_->position = { 0, 0, 5 };
	bullets_.clear();
}
