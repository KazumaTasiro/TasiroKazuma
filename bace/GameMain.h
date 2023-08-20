#pragma once

#include "GameScene.h"
#include "PostEffect.h"
#include "Framework.h"

//�Q�[���S��
class GameMain:public Framework
{
public:
	GameMain();
	~GameMain();

	//����������
	void Initialize() override;

	//�I��
	void Finalize() override;

	//���t���[���X�V()
	void Update() override;

	//�`��
	void Draw() override;


private:
	
	//�|�X�g�G�t�F�N�g
	PostEffect* posteffect = nullptr;
	
	//GameScene
	GameScene* gameScene = nullptr;

};