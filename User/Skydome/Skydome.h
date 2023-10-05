#pragma once
#include "Object3d.h"
#include "Model.h"
#include <cassert>


///<summary>
///�V��
///</summary>
class Skydome {
public:
	///<summary>
	///������
	///</summary>
	void Initalize();

	///<summary>
	///�X�V
	///</summary>
	void Update();

	///<summary>
	///�`��
	///</summary>
	void Draw();


private:
	//���[���h�ϊ��f�[�^
	Object3d* worldTransform_ = nullptr;
	//���f��
	Model* model_ = nullptr;

};