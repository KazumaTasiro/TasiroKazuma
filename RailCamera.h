#pragma once
#include "Object3d.h"
#include"Input/input.h"

///<summary>
/// ������
///</summary>
class RailCamera {
public:
	///<summary>
	/// ������
	///</summary>
	void Initalize();

	///<summary>
	/// �X�V
	///</summary>
	void Update();

	XMFLOAT3 MatVector(XMMATRIX matrix4, XMFLOAT3 vector3);


	void Reset();

private:
	//���[���h�ϊ��f�[�^
	Object3d* worldTransform_=nullptr;

	//�L�[����
	Input* input_ = nullptr;

};
