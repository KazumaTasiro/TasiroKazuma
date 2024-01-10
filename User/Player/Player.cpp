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

//いろいろな初期化
void Player::Initialize(SpriteCommon* spriteCommon,ParticleManager* particle)
{
	assert(spriteCommon);
	assert(particle);



	input_ = Input::GetInstance();
	winApp = WinApp::GetInstance();
	//引数として受け取ったデータをメンバ変数に記録する
	//spriteCommon_ = spriteCommon;
	//ワールド変換の初期化
	worldTransform_ = Object3d::Create();
	worldTransform3DReticle_ = Object3d::Create();

	//model_ = FbxLoader::GetInstance()->LoadModelFromFile("trakku");
	model_ = Model::LoadFormOBJ("trakku");

	worldTransform_->Initialize();
	worldTransform_->SetModel(model_);
	worldTransform_->wtf.position = playerPos;
	worldTransform_->wtf.scale = playerSc;

	bulletModel_ = Model::LoadFormOBJ("playerBullet");

	worldTransform3DReticle_->SetModel(bulletModel_);
	//スプライト生成
	sprite2DReticle_ = new Sprite();
	sprite2DReticle_->Initialize(spriteCommon,zero);

	sprite2DReticleLock_ = new Sprite();
	sprite2DReticleLock_->Initialize(spriteCommon,one);

	worldTransform_->wtf.position = playerResetPos;

	playerHPNone = new Sprite();
	playerHPMax = new Sprite();
	playerHPNone->Initialize(spriteCommon,17);
	playerHPMax->Initialize(spriteCommon,18);
	playerHPNone->SetAnchorPoint({ 0,0 });
	playerHPNone->SetPozition({ 0,WinApp::window_height - hpSpriteHight });
	playerHPMax->SetAnchorPoint({ 0,0 });
	playerHPMax->SetPozition({ spritePos.x,WinApp::window_height - hpSpriteHight });

	damageEffect = new Sprite();
	damageEffect->Initialize(spriteCommon,23);
	damageEffect->SetSize({ WinApp::window_width,WinApp::window_height });
	damageEffect->SetAnchorPoint({ 0.5f,0.5f });
	damageEffect->SetPozition({ WinApp::window_width / 2,WinApp::window_height / 2 });

	spriteHpSize = playerHPMax->GetSize();
	////パーティクル生成
	playerDeadParticle = particle;

	playerHp = playerMaxHp;
	//worldTransform_->ShadowUse();
	shadowNmb = LightData::GetInstance()->AddCircleShadow(worldTransform_->wtf.position,circleShadowDir,circleShadowAtten,circleShadowFactorAngle);

}

//いろいろな更新
void Player::Update()
{
	playerHPMax->SetAnchorPoint({ 0,0 });
	//デスフラグの立った弾を削除
	bullets_.remove_if([ ] (std::unique_ptr<PlayerBullet>& bullet)
 {
	 return bullet->IsDead();
		});
	if ( playerHp <= 0 )
	{
		if ( DeadParticle == false )
		{
			lightActive = false;
			PlayerDeadParticle();
		}
		DeadParticle = true;
		//isDead_ = true;
	}
	//キャラクター移動処理
	Move();
	//キャラクター攻撃更新
	PlayerHpUpdate();
	LightData::GetInstance()->UpdateCircleShadow(shadowNmb,worldTransform_->wtf.position,circleShadowDir,circleShadowAtten,circleShadowFactorAngle,lightActive);
	//弾更新
	for ( std::unique_ptr<PlayerBullet>& bullet : bullets_ )
	{
		bullet->Update();
	}

	MouseReticle();
	ReticleLimit();
	sprite2DReticleLock_->SetPozition(sprite2DReticle_->GetPosition());
	playerDeadParticle->Update();
	if ( DeadParticle )
	{
		EffectWaiteTime--;

	}
	if ( EffectWaiteTime <= zero )
	{
		isDead_ = true;
	}
}

//攻撃を制限するための分離
void Player::AttackUpdate()
{
	if ( playerAttckTime > 0 )
	{
		playerAttckTime--;
	}
	if ( isDead_ == false )
	{
		Attack();
	}
}
//画面が揺れないようにするための分離
void Player::ReticleUpdate()
{
	MouseReticle();
	ReticleLimit();
}

void Player::cameraUpdate()
{
	//ダメージを受けた時の画面シェイク
	if ( playerHp > 1 )
	{
		camera_->SetEye({ camera_->GetEye() + damageShakeBefor });
		DamageShakeUpdate();
		camera_->SetEye({ camera_->GetEye().x + damageShakeX,camera_->GetEye().y + damageShakeY,camera_->GetEye().z + damageShakeZ });
	}
	camera_->SetTarget({ ( GetReticlePos().x / 100 ),( GetReticlePos().y / 100 ),camera_->GetTarget().z });
}

void Player::Move()
{
	//キャラクターの移動ベクトル
	Vector3 move = { };
	Vector3 camMove = { };
	Vector3 pos = worldTransform_->wtf.position;

	//const float RotSpeed = 0.05f;
	if ( input_->PushKey(DIK_A) )
	{
		move.x -= speed;
	}
	else if ( input_->PushKey(DIK_D) )
	{
		move.x += speed;

	}
	if ( input_->PushKey(DIK_LEFT) )
	{
		camMove.x -= speed;
	}
	else if ( input_->PushKey(DIK_RIGHT) )
	{
		camMove.x += speed;

	}
	worldTransform_->wtf.position = { pos.x + move.x,pos.y + move.y,pos.z + move.z };
	worldTransform_->camera->SetTarget(worldTransform_->camera->GetTarget() + camMove);
	//worldTransform_->camera->SetEye({ worldTransform_->camera->GetEye().x + camMove.x, worldTransform_->camera->GetEye().y + camMove.y, worldTransform_->wtf.position.z - 10 });

	//worldTransform_->SetEye({ pos.x + move.x,pos.y + move.y,pos.z + move.z });
	//worldTransform_->SetTarget({ pos.x + move.x,pos.y + move.y,10 });
	//移動限界座標

	//範囲を超えない処理
	worldTransform_->wtf.position.x = max(worldTransform_->wtf.position.x,-kMoveLimitX);
	worldTransform_->wtf.position.x = min(worldTransform_->wtf.position.x,+kMoveLimitX);
	worldTransform_->wtf.position.y = max(worldTransform_->wtf.position.y,-kMoveLimitY);
	worldTransform_->wtf.position.y = min(worldTransform_->wtf.position.y,+kMoveLimitY);

	sprite2DReticle_->SetPozition({ max(worldTransform_->wtf.position.x, WinApp::window_width),max(worldTransform_->wtf.position.y, WinApp::window_height) });
	sprite2DReticle_->SetPozition({ min(worldTransform_->wtf.position.x, zeroNmb),min(worldTransform_->wtf.position.y, zeroNmb) });

	//worldTransform_->wtf.position = { 0,0,-30 };
	PlayerLimit();
	worldTransform_->Update();
}

void Player::OnCollision()
{
	shakeFlag = true;
	playerHp--;
}

//Matorix4をVector3に直す処理
Vector3 Player::ConvertToVector3(const Matrix4& mat,Vector3 vec)
{
	Vector3 retVec = {};


	retVec.x = vec.x * mat.m[ zero ][ zero ] + vec.y * mat.m[ one ][ zero ] + vec.z * mat.m[ two ][ zero ];

	retVec.y = vec.x * mat.m[ zero ][ one ] + vec.y * mat.m[ one ][ one ] + vec.z * mat.m[ two ][ one ];

	retVec.z = vec.x * mat.m[ zero ][ two ] + vec.y * mat.m[ one ][ two ] + vec.z * mat.m[ two ][ two ];

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
	if ( DeadParticle == false )
	{
		if ( worldTransform_->wtf.position.z < 100 )
		{
			worldTransform_->Draw();
		}
	}

	/*model_->Draw(worldTransform_, viewProjection_, textureHandle_);*/
//弾描画
	for ( std::unique_ptr<PlayerBullet>& bullet : bullets_ )
	{
		bullet->Draw();
	}
	////3Dレティクルを描画
	//model_->Draw(worldTransform3DReticle_, viewProjection_, textureHandle_);

}

void Player::DrawFbx()
{

}

void Player::Attack()
{
	if ( input_->TriggerMouse(zero) )
	{
		if ( playerAttckTime <= 0 )
		{
		//弾の速度

			Vector3 velocity(zero,zero,kBulletSpeed);

			//速度ベクトルを自機の向きに合わせて回転させる
			velocity = ConvertToVector3(worldTransform_->wtf.matWorld,velocity);

			//自機から標準オブジェクトへのベクトル
			velocity = worldTransform3DReticle_->wtf.position - worldTransform_->wtf.position;
			velocity.nomalize();
			//弾を生成し、初期化
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(bulletModel_,GetWorldPosition(),velocity);
			//弾を発射する
			bullets_.push_back(std::move(newBullet));

			playerAttckTime = playerAttckTimeRe;
		}
	}
}


void Player::DrawUI()
{
	playerHPNone->Draw();
	if ( playerHp > 0 )
	{
		playerHPMax->Draw();
	}

	if ( input_->PushMouse(one) )
	{
		sprite2DReticleLock_->Draw();
	}
	else
	{
		sprite2DReticle_->Draw();
	}

	if ( shakeFlag || isDead_ )
	{
		damageEffect->Draw();
	}

}

//Vector3とXMMATRIXの外積
Vector3 Player::clossV3V4(const Vector3& vec,const XMMATRIX& mat)
{
	Vector4 divVec = {};

	divVec.x = vec.x * mat.r[ zeroNmbI ].m128_f32[ zeroNmbI ] + vec.y * mat.r[ oneNmbI ].m128_f32[ zeroNmbI ] + vec.z * mat.r[ twoNmbI ].m128_f32[ zeroNmbI ] + oneNmb * mat.r[ threeNmbI ].m128_f32[ zeroNmbI ];

	divVec.y = vec.x * mat.r[ zeroNmbI ].m128_f32[ oneNmbI ] + vec.y * mat.r[ oneNmbI ].m128_f32[ oneNmbI ] + vec.z * mat.r[ twoNmbI ].m128_f32[ oneNmbI ] + oneNmb * mat.r[ threeNmbI ].m128_f32[ oneNmbI ];

	divVec.z = vec.x * mat.r[ zeroNmbI ].m128_f32[ twoNmbI ] + vec.y * mat.r[ oneNmbI ].m128_f32[ twoNmbI ] + vec.z * mat.r[ twoNmbI ].m128_f32[ twoNmbI ] + oneNmb * mat.r[ threeNmbI ].m128_f32[ twoNmbI ];

	divVec.w = vec.x * mat.r[ zeroNmbI ].m128_f32[ threeNmbI ] + vec.y * mat.r[ 1 ].m128_f32[ threeNmbI ] + vec.z * mat.r[ twoNmbI ].m128_f32[ threeNmbI ] + oneNmb * mat.r[ threeNmbI ].m128_f32[ threeNmbI ];

	divVec.x = divVec.x / divVec.w;

	divVec.y = divVec.y / divVec.w;

	divVec.z = divVec.z / divVec.w;

	return { divVec.x, divVec.y, divVec.z };
}

Vector3 Player::AddVector(const Vector3 v1,const Vector3 v2)
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
	offset = ConvertToVector3(worldTransform_->wtf.matWorld,offset);
	//ベクトルの長さを整える
	//Vec3Normalize(&offset, &offset);
	offset.nomalize();
	offset.x *= kDistancePlayerTo3DReticle;
	offset.y *= kDistancePlayerTo3DReticle;
	offset.z *= kDistancePlayerTo3DReticle;
	//3Dレティクル座標設定
	worldTransform3DReticle_->wtf.position = {
		offset.x + worldTransform_->wtf.matWorld.m[ three ][ zero ],
		offset.y + worldTransform_->wtf.matWorld.m[ three ][ one ],
		offset.z + worldTransform_->wtf.matWorld.m[ three ][ two ] };
	worldTransform_->Update();
	//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
	Vector3 positionReticle = { worldTransform3DReticle_->wtf.matWorld.m[ three ][ zero ],worldTransform3DReticle_->wtf.matWorld.m[ three ][ one ],worldTransform3DReticle_->wtf.matWorld.m[ three ][ two ] };

	Vector2 windowWH =
		Vector2(WinApp::window_width,WinApp::window_height);

	//ビューポート行列
	Matrix4 Viewport = {
	   windowWH.x / 2,			  zeroNmb,  zeroNmb,  zeroNmb,
					zeroNmb,	-windowWH.y / twoNmb,  zeroNmb,  zeroNmb,
					zeroNmb,				  zeroNmb,  oneNmb,  zeroNmb,
	   windowWH.x / twoNmb,	 windowWH.y / twoNmb,  zeroNmb,  oneNmb
	};

	//ビュー行列とプロジェクション行列、ビューポート行列を合成する
	Matrix4 matViewProjectionViewport = worldTransform_->camera->GetViewMatrix() * worldTransform_->camera->GetProjectionMatrix() * Viewport;

	//ワールド→スクリーン座標変換(ここから3Dから2Dになる)
	positionReticle = clossV3V4(positionReticle,ConvertXM::ConvertMat4toXMMAT(matViewProjectionViewport));

	//スプライトのレティクルに座標設定
	sprite2DReticle_->SetPozition(Vector2(positionReticle.x,positionReticle.y));

	//マウスカーソルのスクリーン座標からワールド座標を取得して3Dレティクル配置
	{
		POINT mousePosition;
		//マウス座標（スクリーン座標）を取得する
		GetCursorPos(&mousePosition);

		//クライアントエリア座標に変換する
		assert(winApp);
		HWND hwnd = winApp->GetHwnd();
		assert(hwnd);
		ScreenToClient(hwnd,&mousePosition);

		//マウス座標を2Dレティクルのスプライトに代入する
		sprite2DReticle_->SetPozition(Vector2(static_cast< float > ( mousePosition.x ),static_cast< float > ( mousePosition.y )));
		ReticleLimit();
		mousePosition = { static_cast< long >( sprite2DReticle_->GetPosition().x ),static_cast< long >( sprite2DReticle_->GetPosition().y ) };

		//ビュー行列、射影変換、ビューポート行列の合成行列を計算する
		Matrix4 matVPV = worldTransform_->camera->GetViewMatrix() * worldTransform_->camera->GetProjectionMatrix() * Viewport;

		//合成行列の逆行列を計算する
		Matrix4 matInverseVPV;
		Matrix4::MatrixInverse(matInverseVPV,matVPV);
		//ニアクリップ面上のワールド座標得る（スクリーン→ワールド変換）
		Vector3 posNear = Vector3(static_cast< float > ( mousePosition.x ),static_cast< float > ( mousePosition.y ),zero);
		//ファークリップ面上のワールド座標を得る（スクリーン→ワールド変換）
		Vector3 posFar = Vector3(static_cast< float > ( mousePosition.x ),static_cast< float > ( mousePosition.y ),one);

		//スクリーン座標系からワールド座標系へ
		posNear = clossV3V4(posNear,ConvertXM::ConvertMat4toXMMAT(matInverseVPV));
		posFar = clossV3V4(posFar,ConvertXM::ConvertMat4toXMMAT(matInverseVPV));

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

		worldTransform3DReticle_->wtf.position = ( AddVector(posNear,{ mouseDirection.x * kDistanceTestObject,mouseDirection.y * kDistanceTestObject,mouseDirection.z * kDistanceTestObject }) );

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
	shakeFlag = false;
	lightActive = true;
	shakeTime = shakeTimeRe;
	damageShakeX = 0.0f;
	damageShakeY = 0.0f;
	damageShakeZ = 0.0f;
	shadowNmb = LightData::GetInstance()->AddCircleShadow(worldTransform_->wtf.position,circleShadowDir,circleShadowAtten,circleShadowFactorAngle);
	worldTransform_->wtf.position = playerResetPos;
	playerAttckTime = playerAttckTimeRe;
	bullets_.clear();
	playerHp = playerMaxHp;
	isDead_ = false;
	DeadParticle = false;
	EffectWaiteTime = five * ten;
	worldTransform_->Update();
}

Vector2 Player::GetReticlePos()
{
	Vector2 pos = { worldTransform3DReticle_->GetWorldPosition().x,worldTransform3DReticle_->GetWorldPosition().y };
	return pos;
}

//カメラのfarを取る関数
Vector3 Player::GetFarNear()
{
	Vector3 vec = farCre - nearCre;
	vec.nomalize();
	return vec;
}

//レティクルの制限
void Player::ReticleLimit()
{
	if ( sprite2DReticle_->GetPosition().x <= RetiRim )
	{
		sprite2DReticle_->SetPozition({ RetiRim,sprite2DReticle_->GetPosition().y });
	}
	if ( sprite2DReticle_->GetPosition().y <= RetiRim )
	{
		sprite2DReticle_->SetPozition({ sprite2DReticle_->GetPosition().x,RetiRim });
	}
	if ( sprite2DReticle_->GetPosition().x >= winApp->window_width )
	{
		sprite2DReticle_->SetPozition({ winApp->window_width,sprite2DReticle_->GetPosition().y });
	}
	if ( sprite2DReticle_->GetPosition().y >= winApp->window_height )
	{
		sprite2DReticle_->SetPozition({ sprite2DReticle_->GetPosition().x,winApp->window_height });
	}
}

void Player::PlayerLimit()
{
	if ( worldTransform_->wtf.position.x < -posLim )
	{
		worldTransform_->wtf.position.x = -posLim;
	}
	if ( worldTransform_->wtf.position.x > posLim )
	{
		worldTransform_->wtf.position.x = posLim;
	}
}

void Player::PlayerDeadParticle()
{
	//パーティクル範囲
	for ( int i = 0; i < 5; i++ )
	{
//X,Y,Z全て[-5.0f,+5.0f]でランダムに分布
		const float rnd_pos = rnd_posS;
		Vector3 pos = worldTransform_->wtf.position;
		pos.x += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.y += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;
		pos.z += ( float ) rand() / RAND_MAX * rnd_pos - rnd_pos / 2.0f;

		//速度
		//X,Y,Z全て[-0.05f,+0.05f]でランダムに分布
		const float rnd_vel = rnd_velS;
		Vector3 vel{};
		vel.x = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.y = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		vel.z = ( float ) rand() / RAND_MAX * rnd_vel - rnd_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f,0]でランダムに分布
		const float rnd_acc = rnd_accS;
		Vector3 acc{};
		acc.x = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;
		acc.y = ( float ) rand() / RAND_MAX * rnd_acc - rnd_acc / 2.0f;

		//追加
		playerDeadParticle->Add(particleLife,pos,vel,acc,particleScaleStert,particleScaleEnd,one);
		playerDeadParticle->Add(particleLife,pos,vel,acc,particleScaleStert,particleScaleEnd,two);
		playerDeadParticle->Update();
	}
}

void Player::ParticleDraw()
{
	playerDeadParticle->Draw();
	for ( std::unique_ptr<PlayerBullet>& bullet : bullets_ )
	{
		bullet->ParticleDraw();
	}
	
}

//クリア時の自機の動き
void Player::ClearMove()
{
	LightData::GetInstance()->UpdateCircleShadow(shadowNmb,worldTransform_->wtf.position,circleShadowDir,circleShadowAtten,circleShadowFactorAngle,lightActive);
	worldTransform_->wtf.position += clearMove;
	worldTransform_->Update();
}

void Player::TitleMove()
{
	worldTransform_->wtf.rotation.y;
}

//プレイヤーのHPの更新
void Player::PlayerHpUpdate()
{
	float HpSize = playerHp / playerMaxHp;
	playerHPMax->SetSize({ spriteHpSize.x * HpSize,spriteHpSize.y , });
}

void Player::DamageShakeUpdate()
{
	//初期化
	damageShakeX = 0.0f;
	damageShakeY = 0.0f;
	damageShakeZ = 0.0f;
	damageShakeBefor = { 0,0,0 };

	//フラグがたったら画面をシェイクする度合いを決める
	if ( shakeFlag )
	{
		shakeTime--;
		damageShakeX = ( float ) rand() / RAND_MAX * shakeLimit - shakeLimit / 2.0f;
		damageShakeY = ( float ) rand() / RAND_MAX * shakeLimit - shakeLimit / 2.0f;
		damageShakeZ = ( float ) rand() / RAND_MAX * shakeLimit - shakeLimit / 2.0f;
		damageShakeBefor = { damageShakeBefor.x - damageShakeX ,damageShakeBefor.y - damageShakeY,damageShakeBefor.z - damageShakeZ };
	}
	//終了処理
	if ( shakeTime < 0 )
	{
		shakeTime = shakeTimeRe;
		shakeFlag = false;
		//camera_->SetEye({ camera_->GetEye() + damageShakeBefor });
	}
	//else
	//{
	//	damageShakeBefor = camera_->GetEye();
	//}
}

void Player::ResetDamageFlag()
{
	shakeFlag = false;
}

void Player::ImguiDraw()
{
	ImGui::SliderFloat4("circleShadowDir",&circleShadowDir.x,-2,1);
	ImGui::SliderFloat3("circleShadowAtten",&circleShadowAtten.x,-2,2);
	ImGui::SliderFloat2("circleShadowFactorAngle",&circleShadowFactorAngle.x,-2,2);
}
