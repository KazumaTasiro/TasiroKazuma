#include <Windows.h>
//#include <d3d12.h>
//#include <dxgi1_6.h>
//#include <cassert>
//#include <vector>
#include <string>
#include <DirectXMath.h>
#include <d3dcompiler.h>
#include <dinput.h>
#include <math.h>
#include <DirectXTex.h>
//#include <wrl.h>
#include "Input/Input.h"
#include "WinApp.h"
#include "DirectXCommon.h"

#include <DXGIDebug.h>
#define DIRECTINPUT_VERSION  0x0800 //DirectInput�̃o�[�W�����w��


using namespace DirectX;
//#pragma comment(lib,"d3d12.lib")
//#pragma comment(lib,"dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

const float PI = 3.141592f;
//using namespace Microsoft::WRL;

struct ConstBufferDataTransform {
	XMMATRIX mat;	//3D�ϊ��s��
	/*XMMATRIX unk;*/
};

//3D�I�u�W�F�N�g
struct Object3d
{
	//�萔�o�b�t�@(�s��p)
	ComPtr<ID3D12Resource> constBuffTransform;
	//�萔�o�b�t�@�}�b�v(�s��p)
	ConstBufferDataTransform* constMapTransform = nullptr;
	//�A�t�B���ϊ����
	XMFLOAT3 scale = { 1,1,1 };
	XMFLOAT3 rotation = { 0,0,0 };
	XMFLOAT3 position = { 0,0,0 };
	//���[���h�ϊ��s��
	XMMATRIX matWorld = { 0,0,0,0,
	0,0,0,0,
	0,0,0,0,
	0,0,0,0 };
	//�e�I�u�W�F�N�g�ւ̃|�C���^
	Object3d* parent = nullptr;
};

void InitializeObject3d(Object3d* object, ID3D12Device* device)
{
	HRESULT result;

	//�萔�o�b�t�@�̃q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES heapProp{};
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD;
	//�萔�o�b�t�@�̃��\�[�X�ݒ�
	D3D12_RESOURCE_DESC resdesc{};
	resdesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resdesc.Width = (sizeof(ConstBufferDataTransform) * 0xff) & ~0xff;
	resdesc.Height = 1;
	resdesc.DepthOrArraySize = 1;
	resdesc.MipLevels = 1;
	resdesc.SampleDesc.Count = 1;
	resdesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	//�萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&heapProp,
		D3D12_HEAP_FLAG_NONE,
		&resdesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&object->constBuffTransform));
	assert(SUCCEEDED(result));
	//�萔�o�b�t�@�̃}�b�s���O
	result = object->constBuffTransform->Map(0, nullptr, (void**)&object->constMapTransform);
	assert(SUCCEEDED(result));

}

void UpdateObject3d(Object3d* object, XMMATRIX& matView, XMMATRIX& matProjection)
{
	XMMATRIX matScale, matRot, matTrans;

	//�X�P�[���A��]�A���s�ړ��s��̌v�Z
	matScale = XMMatrixScaling(object->scale.x, object->scale.y, object->scale.z);
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(object->rotation.z);
	matRot *= XMMatrixRotationX(object->rotation.x);
	matRot *= XMMatrixRotationY(object->rotation.y);
	matTrans = XMMatrixTranslation(
		object->position.x, object->position.y, object->position.z);
	//���[���h�s��̍���
	object->matWorld = XMMatrixIdentity();
	object->matWorld *= matScale;
	object->matWorld *= matRot;
	object->matWorld *= matTrans;

	//�e�I�u�W�F�N�g�������
	if (object->parent != nullptr) {
		//�e�I�u�W�F�N�g�̃��[���h�s���������
		object->matWorld *= object->parent->matWorld;
	}
	//�萔�o�b�t�@�ւ̃f�[�^�]��
	object->constMapTransform->mat = object->matWorld * matView * matProjection;
}
void DrawObject3d(Object3d* object, ID3D12GraphicsCommandList* commandList, D3D12_VERTEX_BUFFER_VIEW& vbView,
	D3D12_INDEX_BUFFER_VIEW& ibView, UINT numIndices) {
	//���_�o�b�t�@�̐ݒ�
	commandList->IASetVertexBuffers(0, 1, &vbView);
	//�C���f�b�N�X�o�b�t�@�̐ݒ�
	commandList->IASetIndexBuffer(&ibView);
	//�萔�o�b�t�@�r���[�iCBV�j�̐ݒ�R�}���h
	commandList->SetGraphicsRootConstantBufferView(2, object->constBuffTransform->GetGPUVirtualAddress());

	//�`��R�}���h
	commandList->DrawIndexedInstanced(numIndices, 1, 0, 0, 0);
}

//�E�B���h�E�v���V�[�W��
LRESULT WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam) {
	//���b�Z�[�W�ɉ����ăQ�[���ŗL�̏������s��
	switch (msg) {
		//�E�B���h�E���j�󂳂ꂽ
	case WM_DESTROY:
		//OS�ɑ΂��āA�A�v���̏I����`����
		PostQuitMessage(0);
		return 0;
	}

	//�W���̃��b�Z�[�W�������s��
	return DefWindowProc(hwnd, msg, wparam, lparam);
}


//Windows�A�v���ł̃G���g���[�|�C���g(main�֐�)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//�R���\�[���̕����o��
	OutputDebugStringA("Hello,DirectX!!\n");

	WinApp* winApp = nullptr;

	//WindowsAPI�̏�����
	winApp = new WinApp();
	winApp->Initialize();

	//�|�C���^
	DirectXCommon* dxCommon = nullptr;
	dxCommon = new DirectXCommon();
	dxCommon->Initialize(winApp);

	MSG msg{};//���b�Z�[�W

	//DIrectX���������������܂���
#ifdef _DEBUG
////�f�o�b�O���C���[���I����
//	ID3D12Debug* debugController;
//	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController)))) {
//		debugController->EnableDebugLayer();
//	}
#endif
	HRESULT result;
	ComPtr<ID3D12Device> device;
	ComPtr<IDXGIFactory7> dxgiFactory;
	ComPtr<IDXGISwapChain4> swapChain;
	ComPtr<ID3D12CommandAllocator> cmdAllocator;
	ComPtr<ID3D12GraphicsCommandList> commandList;
	ComPtr<ID3D12CommandQueue> commandQueue;
	ComPtr<ID3D12DescriptorHeap> rtvHeap;

	////DXG�̃t�@�N�g���[�̐���
	//result = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	//assert(SUCCEEDED(result));

	////�A�_�v�^�̗񋓗p
	//std::vector<ComPtr<IDXGIAdapter4>>adapters;
	////�����œ���̖��O�����A�_�v�^�[�I�u�W�F�N�g������
	//ComPtr<IDXGIAdapter4> tmpAdapter;
	////�p�t�H�[�}���X���������̂��珇�ɁA���ׂẴA�_�v�^�[��񋓂���
	//for (UINT i = 0;
	//	dxgiFactory->EnumAdapterByGpuPreference(i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE, IID_PPV_ARGS(&tmpAdapter)) != DXGI_ERROR_NOT_FOUND; i++) {
	//	//���I�z��ɒǉ�����
	//	adapters.push_back(tmpAdapter);
	//}
	////���ƒǂ��ȃA�_�v�^��I�ʂ���
	//for (size_t i = 0; i < adapters.size(); i++) {
	//	DXGI_ADAPTER_DESC3 adapterDesc;
	//	//�A�_�v�^�[�̏����擾����
	//	adapters[i]->GetDesc3(&adapterDesc);

	//	//�\�t�g�E�F�A�f�o�C�X�����
	//	if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE)) {
	//		//�f�o�C�X�𗘗p���ă��[�v�𔲂���
	//		tmpAdapter = adapters[i].Get();
	//		break;
	//	}
	//}
	////�Ή����x���̔z��
	//D3D_FEATURE_LEVEL levels[] = {
	//	D3D_FEATURE_LEVEL_12_1,
	//	D3D_FEATURE_LEVEL_12_0,
	//	D3D_FEATURE_LEVEL_11_1,
	//	D3D_FEATURE_LEVEL_11_0,
	//};
	//D3D_FEATURE_LEVEL featureLevel;

	//for (size_t i = 0; i < _countof(levels); i++) {
	//	//�̗p�����A�_�v�^�[�Ńf�o�C�X�𐶐�
	//	result = D3D12CreateDevice(tmpAdapter.Get(), levels[i], IID_PPV_ARGS(&device));
	//	if (result == S_OK) {
	//		//�f�o�C�X�𐶐��ł������_�Ń��[�v�𔲂���
	//		featureLevel = levels[i];
	//		break;
	//	}
	//}


	//result = device->CreateCommandAllocator(
	//	D3D12_COMMAND_LIST_TYPE_DIRECT,
	//	IID_PPV_ARGS(&cmdAllocator));
	//assert(SUCCEEDED(result));
	//// �R�}���h���X�g�𐶐�
	//result = device->CreateCommandList(0,
	//	D3D12_COMMAND_LIST_TYPE_DIRECT,
	//	cmdAllocator.Get(), nullptr,
	//	IID_PPV_ARGS(&commandList));
	//assert(SUCCEEDED(result));
	////�R�}���h�L���[�̐ݒ�
	//D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	////�R�}���h�L���[�𐶐�
	//result = device->CreateCommandQueue(&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	//assert(SUCCEEDED(result));
	//// �X���b�v�`�F�[���̐ݒ�
	//DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	//swapChainDesc.Width = 1280;
	//swapChainDesc.Height = 720;
	//swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM; // �F���̏���
	//swapChainDesc.SampleDesc.Count = 1; // �}���`�T���v�����Ȃ�
	//swapChainDesc.BufferUsage = DXGI_USAGE_BACK_BUFFER; // �o�b�N�o�b�t�@�p
	//swapChainDesc.BufferCount = 2; // �o�b�t�@����2�ɐݒ�
	//swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD; // �t���b�v��͔j��
	//swapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	////IDXGISwapChain1��ComPtr��p��
	//ComPtr<IDXGISwapChain1> swapchain1;

	//// �X���b�v�`�F�[���̐���
	//result = dxgiFactory->CreateSwapChainForHwnd(
	//	commandQueue.Get(),
	//	winApp->GetHwnd(),
	//	&swapChainDesc,
	//	nullptr,
	//	nullptr,
	//	&swapchain1);

	////��������IDXGISwapChain1�̃I�u�W�F�N�g��IDXGISwapChain4�ɕϊ�����
	//swapchain1.As(&swapChain);

	//assert(SUCCEEDED(result));
	
	//// �f�X�N���v�^�q�[�v�̐ݒ�
	//D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc{};
	//rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV; // �����_�[�^�[�Q�b�g�r���[
	//rtvHeapDesc.NumDescriptors = swapChainDesc.BufferCount; // ���\��2��
	//// �f�X�N���v�^�q�[�v�̐���
	//device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&rtvHeap));
	//// �o�b�N�o�b�t�@
	//std::vector<ComPtr<ID3D12Resource>> backBuffers;
	//backBuffers.resize(swapChainDesc.BufferCount);
	//// �X���b�v�`�F�[���̑S�Ẵo�b�t�@�ɂ��ď�������
	//for (size_t i = 0; i < backBuffers.size(); i++) {
	//	// �X���b�v�`�F�[������o�b�t�@���擾
	//	swapChain->GetBuffer((UINT)i, IID_PPV_ARGS(&backBuffers[i]));
	//	// �f�X�N���v�^�q�[�v�̃n���h�����擾
	//	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
	//	// �����\���ŃA�h���X�������
	//	rtvHandle.ptr += i * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
	//	// �����_�[�^�[�Q�b�g�r���[�̐ݒ�
	//	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	//	// �V�F�[�_�[�̌v�Z���ʂ�SRGB�ɕϊ����ď�������
	//	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	//	// �����_�[�^�[�Q�b�g�r���[�̐���
	//	device->CreateRenderTargetView(backBuffers[i].Get(), &rtvDesc, rtvHandle);
	//}
	// �t�F���X�̐���
	//ComPtr<ID3D12Fence> fence;
	//UINT64 fenceVal = 0;
	//result = device->CreateFence(fenceVal, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	////DirectInput�̏�����
	//IDirectInput8* directInput = nullptr;
	//result = DirectInput8Create(
	//	w.hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8,
	//	(void**)&directInput, nullptr);
	//assert(SUCCEEDED(result));
	////�L�[�{�[�h�f�o�C�X�̐���
	//IDirectInputDevice8* keyboard = nullptr;
	//result = directInput->CreateDevice(GUID_SysKeyboard, &keyboard, NULL);
	//assert(SUCCEEDED(result));
	////���̓f�[�^�`���̃Z�b�g
	//result = keyboard->SetDataFormat(&c_dfDIKeyboard);
	//assert(SUCCEEDED(result));
	////�r���I���䃌�x���̃Z�b�g
	//result = keyboard->SetCooperativeLevel(
	//	hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE | DISCL_NOWINKEY);
	//assert(SUCCEEDED(result));

	//�|�C���^
	Input* input = nullptr;

	//���͂̏�����
	input = new Input();
	input->Initalize(winApp);
	//DIrectX���������������܂�
	// �`�揉��������
	// ���_�f�[�^�\����
	struct Vertex
	{
		XMFLOAT3 pos; // xyz���W
		XMFLOAT3 normal; //�@���x�N�g��
		XMFLOAT2 uv;  // uv���W
	};
	//// ���_�f�[�^
	//Vertex vertices[] = {
	//	// x      y     z       u     v
	//	{{-0.4f, -0.7f, 0.0f}, {0.0f, 1.0f}}, // ����
	//	{{-0.4f, +0.7f, 0.0f}, {0.0f, 0.0f}}, // ����
	//	{{+0.4f, -0.7f, 0.0f}, {1.0f, 1.0f}}, // �E��
	//	{{+0.4f, +0.7f, 0.0f}, {1.0f, 0.0f}}, // �E��
	//};
	Vertex vertices[] = {
		//x		//y		//z		//u		//v
		//�O
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}},	//����
		{{-5.0f, 5.0f,-5.0f},{},{0.0f,0.0f}},	//����								
		{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}},	//�E��
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,0.0f}},	//�E��
		//��
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,1.0f}},	//����
		{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}},	//����								
		{{ 5.0f,-5.0f, 5.0f},{},{1.0f,1.0f}},	//�E��
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}},	//�E��
		//��
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}},	//����
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}},	//����								
		{{-5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}},	//�E��
		{{-5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}},	//�E��
		//�E
		{{ 5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}},	//����
		{{ 5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}},	//����								
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}},	//�E��
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}},	//�E��
		//��
		{{-5.0f, 5.0f,-5.0f},{},{0.0f,1.0f}},	//����
		{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}},	//����								
		{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}},	//�E��
		{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}},	//�E��
		//{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}},	//����
		//{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}},	//����								
		//{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}},	//�E��
		//{{ 5.0f,-5.0f, 5.0f},{},{1.0f,0.0f}},	//�E��
		//��
		{{-5.0f,-5.0f,-5.0f},{},{0.0f,1.0f}},	//����
		{{-5.0f,-5.0f, 5.0f},{},{0.0f,0.0f}},	//����								
		{{ 5.0f,-5.0f,-5.0f},{},{1.0f,1.0f}},	//�E��
		{{ 5.0f,-5.0f, 5.0f},{},{1.0f,0.0f}},	//�E��

		//{{-5.0f, 5.0f,-5.0f},{},{0.0f,1.0f}},	//����
		//{{-5.0f, 5.0f, 5.0f},{},{0.0f,0.0f}},	//����								
		//{{ 5.0f, 5.0f,-5.0f},{},{1.0f,1.0f}},	//�E��
		//{{ 5.0f, 5.0f, 5.0f},{},{1.0f,0.0f}},	//�E��
	};
	// �C���f�b�N�X�f�[�^
	unsigned short indices[] = {
		//�O
		0, 1, 2, // �O�p�`1��
		2, 1, 3, // �O�p�`2��
		//��
		4,6,5,// �O�p�`3��
		5,6,7,// �O�p�`4��
		//��
		8,9,10,	 //�O�p�`5
		10,9,11,//�O�p�`6
		//�E
		12,14,13, //�O�p�`7
		13,14,15, //�O�p�`8
		//��
		16,17,18, //�O�p�`9
		18,17,19, //�O�p�`10
		//��
		20,22,21, //�O�p�`11
		21,22,23, //�O�p�`12

	};

	for (int i = 0; i < _countof(indices) / 3; i++)
	{//�O�p�`1���ƂɌv�Z���Ă���
		//�O�p�`�ɃC���f�b�N�X�����o���āA�ꎞ�I�ȕϐ��ɓ����
		unsigned short index0 = indices[i * 3 + 0];
		unsigned short index1 = indices[i * 3 + 1];
		unsigned short index2 = indices[i * 3 + 2];
		//�O�p�`���\������\�����钸�_���W�x�N�g���ɑ��
		XMVECTOR p0 = XMLoadFloat3(&vertices[index0].pos);
		XMVECTOR p1 = XMLoadFloat3(&vertices[index1].pos);
		XMVECTOR p2 = XMLoadFloat3(&vertices[index2].pos);
		//p0->p1�x�N�g���A	p0->p2�x�N�g�����v�Z(�x�N�g���̌��Z)
		XMVECTOR v1 = XMVectorSubtract(p1, p0);
		XMVECTOR v2 = XMVectorSubtract(p2, p0);

		//�O�ς͗������琂���ȃx�N�g��
		XMVECTOR normal = XMVector3Cross(v1, v2);
		//���K���i�������P�ɂ���j
		normal = XMVector3Normalize(normal);
		//���߂��@���𒸓_�f�[�^�ɑ��
		XMStoreFloat3(&vertices[index0].normal, normal);
		XMStoreFloat3(&vertices[index1].normal, normal);
		XMStoreFloat3(&vertices[index2].normal, normal);


	};

	// ���_�f�[�^�S�̂̃T�C�Y = ���_�f�[�^����̃T�C�Y * ���_�f�[�^�̗v�f��
	UINT sizeVB = static_cast<UINT>(sizeof(vertices[0]) * _countof(vertices));
	// ���_�o�b�t�@�̐ݒ�
	D3D12_HEAP_PROPERTIES heapProp{}; // �q�[�v�ݒ�
	heapProp.Type = D3D12_HEAP_TYPE_UPLOAD; // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC resDesc{};
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeVB; // ���_�f�[�^�S�̂̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
	// ���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff;
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertBuff));
	assert(SUCCEEDED(result));
	// GPU��̃o�b�t�@�ɑΉ��������z������(���C����������)���擾
	Vertex* vertMap = nullptr;
	result = vertBuff->Map(0, nullptr, (void**)&vertMap);
	assert(SUCCEEDED(result));
	// �S���_�ɑ΂���
	for (int i = 0; i < _countof(vertices); i++) {
		vertMap[i] = vertices[i]; // ���W���R�s�[
	}
	// �q���������
	vertBuff->Unmap(0, nullptr);
	// ���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	// GPU���z�A�h���X
	vbView.BufferLocation = vertBuff->GetGPUVirtualAddress();
	// ���_�o�b�t�@�̃T�C�Y
	vbView.SizeInBytes = sizeVB;
	// ���_1���̃f�[�^�T�C�Y
	vbView.StrideInBytes = sizeof(vertices[0]);

	ID3DBlob* vsBlob = nullptr; // ���_�V�F�[�_�I�u�W�F�N�g
	ID3DBlob* psBlob = nullptr; // �s�N�Z���V�F�[�_�I�u�W�F�N�g
	ID3DBlob* errorBlob = nullptr; // �G���[�I�u�W�F�N�g
	// ���_�V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicVS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "vs_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&vsBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	// �s�N�Z���V�F�[�_�̓ǂݍ��݂ƃR���p�C��
	result = D3DCompileFromFile(
		L"Resources/shaders/BasicPS.hlsl", // �V�F�[�_�t�@�C����
		nullptr,
		D3D_COMPILE_STANDARD_FILE_INCLUDE, // �C���N���[�h�\�ɂ���
		"main", "ps_5_0", // �G���g���[�|�C���g���A�V�F�[�_�[���f���w��
		D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION, // �f�o�b�O�p�ݒ�
		0,
		&psBlob, &errorBlob);
	// �G���[�Ȃ�
	if (FAILED(result)) {
		// errorBlob����G���[���e��string�^�ɃR�s�[
		std::string error;
		error.resize(errorBlob->GetBufferSize());
		std::copy_n((char*)errorBlob->GetBufferPointer(),
			errorBlob->GetBufferSize(),
			error.begin());
		error += "\n";
		// �G���[���e���o�̓E�B���h�E�ɕ\��
		OutputDebugStringA(error.c_str());
		assert(0);
	}
	// ���_���C�A�E�g
	D3D12_INPUT_ELEMENT_DESC inputLayout[] = {
	{ // xyz���W(1�s�ŏ������ق������₷��)
		"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	{//�@���x�N�g��
		"NORMAL",0,DXGI_FORMAT_R32G32B32_FLOAT,0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,0
	},
	{ // uv���W(1�s�ŏ������ق������₷��)
		"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0,
		D3D12_APPEND_ALIGNED_ELEMENT,
		D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0
	},
	};



	// �O���t�B�b�N�X�p�C�v���C���ݒ�
	D3D12_GRAPHICS_PIPELINE_STATE_DESC pipelineDesc{};

	// �V�F�[�_�[�̐ݒ�
	pipelineDesc.VS.pShaderBytecode = vsBlob->GetBufferPointer();
	pipelineDesc.VS.BytecodeLength = vsBlob->GetBufferSize();
	pipelineDesc.PS.pShaderBytecode = psBlob->GetBufferPointer();
	pipelineDesc.PS.BytecodeLength = psBlob->GetBufferSize();
	// �T���v���}�X�N�̐ݒ�
	pipelineDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK; // �W���ݒ�

	// ���X�^���C�U�̐ݒ�
	pipelineDesc.RasterizerState.CullMode = D3D12_CULL_MODE_BACK; // �J�����O���Ȃ�
	pipelineDesc.RasterizerState.FillMode = D3D12_FILL_MODE_SOLID; // �|���S�����h��Ԃ�
	pipelineDesc.RasterizerState.DepthClipEnable = true; // �[�x�N���b�s���O��L����

	// �u�����h�X�e�[�g
	/*pipelineDesc.BlendState.RenderTarget[0].RenderTargetWriteMask
		= D3D12_COLOR_WRITE_ENABLE_ALL;*/

	D3D12_RENDER_TARGET_BLEND_DESC& blenddesc = pipelineDesc.BlendState.RenderTarget[0];
	blenddesc.RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;// RBGA�S�Ẵ`�����l����`��

	blenddesc.BlendEnable = true;                   // �u�����h��L���ɂ���
	blenddesc.BlendOpAlpha = D3D12_BLEND_OP_ADD;    // ���Z
	blenddesc.SrcBlendAlpha = D3D12_BLEND_ONE;      // �\�[�X�̒l��100% �g��
	blenddesc.DestBlendAlpha = D3D12_BLEND_ZERO;    // �f�X�g�̒l��  0% �g��

	//// ���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD; // ���Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;   // �\�[�X�̒l��100% �g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;  // �f�X�g�̒l��100% �g��

	//// ���Z����
	//blenddesc.BlendOp = D3D12_BLEND_OP_REV_SUBTRACT;    // �f�X�g����\�[�X�����Z
	//blenddesc.SrcBlend = D3D12_BLEND_ONE;               // �\�[�X�̒l��100% �g��
	//blenddesc.DestBlend = D3D12_BLEND_ONE;

	//// �F���]
	//blenddesc.BlendOp = D3D12_BLEND_OP_ADD;             // ���Z
	//blenddesc.SrcBlend = D3D12_BLEND_INV_DEST_COLOR;    // 1.0f-�f�X�g�J���[�̒l
	//blenddesc.DestBlend = D3D12_BLEND_ZERO;

	// ����������
	blenddesc.BlendOp = D3D12_BLEND_OP_ADD;             // ���Z
	blenddesc.SrcBlend = D3D12_BLEND_SRC_ALPHA;         // �\�[�X�̃A���t�@�l
	blenddesc.DestBlend = D3D12_BLEND_INV_SRC_ALPHA;

	// ���_���C�A�E�g�̐ݒ�
	pipelineDesc.InputLayout.pInputElementDescs = inputLayout;
	pipelineDesc.InputLayout.NumElements = _countof(inputLayout);

	// �}�`�̌`��ݒ�
	pipelineDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	// ���̑��̐ݒ�
	pipelineDesc.NumRenderTargets = 1; // �`��Ώۂ�1��
	pipelineDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB; // 0~255�w���RGBA
	pipelineDesc.SampleDesc.Count = 1; // 1�s�N�Z���ɂ�1��T���v�����O

	// �f�X�N���v�^�����W�̐ݒ�
	D3D12_DESCRIPTOR_RANGE descriptorRange{};
	descriptorRange.NumDescriptors = 1;         //��x�̕`��Ɏg���e�N�X�`����1���Ȃ̂�1
	descriptorRange.RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange.BaseShaderRegister = 0;     //�e�N�X�`�����W�X�^0��
	descriptorRange.OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;


	// ���[�g�p�����[�^�̐ݒ�
	D3D12_ROOT_PARAMETER rootParams[3] = {};
	// �萔�o�b�t�@0��
	rootParams[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // ���
	rootParams[0].Descriptor.ShaderRegister = 0;                   // �萔�o�b�t�@�ԍ�
	rootParams[0].Descriptor.RegisterSpace = 0;                    // �f�t�H���g�l
	rootParams[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // �S�ẴV�F�[�_���猩����
	// �e�N�X�`�����W�X�^0��
	rootParams[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;   //���
	rootParams[1].DescriptorTable.pDescriptorRanges = &descriptorRange;		  //�f�X�N���v�^�����W
	rootParams[1].DescriptorTable.NumDescriptorRanges = 1;              		  //�f�X�N���v�^�����W��
	rootParams[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;
	//�萔�o�b�t�@1��
	rootParams[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;   // ���
	rootParams[2].Descriptor.ShaderRegister = 1;                   // �萔�o�b�t�@�ԍ�
	rootParams[2].Descriptor.RegisterSpace = 0;                    // �f�t�H���g�l
	rootParams[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_ALL;  // �S�ẴV�F�[�_���猩����

	// �e�N�X�`���T���v���[�̐ݒ�
	D3D12_STATIC_SAMPLER_DESC samplerDesc{};
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //���J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //�c�J��Ԃ��i�^�C�����O�j
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;                 //���s�J��Ԃ��i�^�C�����O�j
	samplerDesc.BorderColor = D3D12_STATIC_BORDER_COLOR_TRANSPARENT_BLACK;  //�{�[�_�[�̎��͍�
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;                   //�S�ă��j�A���
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;                                 //�~�b�v�}�b�v�ő�l
	samplerDesc.MinLOD = 0.0f;                                              //�~�b�v�}�b�v�ŏ��l
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	samplerDesc.ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;           //�s�N�Z���V�F�[�_����̂ݎg�p�\

	// ���[�g�V�O�l�`��
	ComPtr<ID3D12RootSignature> rootSignature;

	// ���[�g�V�O�l�`���̐ݒ�
	D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc{};
	rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	rootSignatureDesc.pParameters = rootParams; //���[�g�p�����[�^�̐擪�A�h���X
	rootSignatureDesc.NumParameters = _countof(rootParams);        //���[�g�p�����[�^��
	rootSignatureDesc.pStaticSamplers = &samplerDesc;
	rootSignatureDesc.NumStaticSamplers = 1;



	// ���[�g�V�O�l�`���̃V���A���C�Y
	ComPtr<ID3DBlob> rootSigBlob = nullptr;
	result = D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0,
		&rootSigBlob, &errorBlob);
	assert(SUCCEEDED(result));
	result = device->CreateRootSignature(0, rootSigBlob->GetBufferPointer(), rootSigBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));
	assert(SUCCEEDED(result));

	// �p�C�v���C���Ƀ��[�g�V�O�l�`�����Z�b�g
	pipelineDesc.pRootSignature = rootSignature.Get();

	//// �p�C�v�����X�e�[�g�̐���
	//ID3D12PipelineState* pipelineState = nullptr;
	//result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	//assert(SUCCEEDED(result));

	// �萔�o�b�t�@�p�f�[�^�\���́i�}�e���A���j
	struct ConstBufferDataMaterial {
		XMFLOAT4 color; // �F (RGBA)
	};

	//�萔�o�b�t�@�p�\���́i�RD�ϊ��s��j
	struct ConstBufferDataTransform {
		XMMATRIX mat;	//3D�ϊ��s��
		/*XMMATRIX unk;*/
	};

	//ID3D12Resource* constBuffTransform0 = nullptr;
	//ConstBufferDataTransform* constMapTransform0 = nullptr;

	//ID3D12Resource* constBuffTransform1 = nullptr;
	//ConstBufferDataTransform* constMapTransform1 = nullptr;

	//3D�I�u�W�F�N�g�̐�
	const size_t kObjectCount = 50;
	//3D�I�u�W�F�N�g�̔z��
	Object3d object3ds[kObjectCount];

	for (int i = 0; i < _countof(object3ds); i++) {
		//������
		InitializeObject3d(&object3ds[i], device.Get());
		//��������͐e�q�\���̃T���v��
		//�擪�ȊO�Ȃ�
		if (i > 0) {
			//��O�̃I�u�W�F�N�g��e�I�u�W�F�N�g�Ƃ���
			object3ds[i].parent = &object3ds[i - 1];
			//�e�I�u�W�F�N�g��9���̑傫��
			object3ds[i].scale = { 0.9f,0.9f,0.9f };
			//�e�I�u�W�F�N�g�ɑ΂���Z���܂���30�x��]
			object3ds[i].rotation = { 0.0f,0.0f,XMConvertToRadians(30.0f) };

			//�e�I�u�W�F�N�g�ɑ΂���Z�����]�W.0���炷
			object3ds[i].position = { 0.0f,0.0f,-8.0f };
		}
	}
	//{
	//	 �q�[�v�ݒ�
	//	D3D12_HEAP_PROPERTIES cbHeapProp{};
	//	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;                   // GPU�ւ̓]���p
	//	 ���\�[�X�ݒ�
	//	D3D12_RESOURCE_DESC cbResourceDesc{};
	//	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	//	cbResourceDesc.Width = (sizeof(ConstBufferDataTransform) + 0xff) & ~0xff;   // 256�o�C�g�A���C�������g
	//	cbResourceDesc.Height = 1;
	//	cbResourceDesc.DepthOrArraySize = 1;
	//	cbResourceDesc.MipLevels = 1;
	//	cbResourceDesc.SampleDesc.Count = 1;
	//	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	//	// �萔�o�b�t�@�̐���
	//	result = device->CreateCommittedResource(
	//		&cbHeapProp, // �q�[�v�ݒ�
	//		D3D12_HEAP_FLAG_NONE,
	//		&cbResourceDesc, // ���\�[�X�ݒ�
	//		D3D12_RESOURCE_STATE_GENERIC_READ,
	//		nullptr,
	//		IID_PPV_ARGS(&constBuffTransform0));
	//	assert(SUCCEEDED(result));

	//	// �萔�o�b�t�@�̃}�b�s���O
	//	result = constBuffTransform0->Map(0, nullptr, (void**)&constMapTransform0); // �}�b�s���O
	//	assert(SUCCEEDED(result));

	//	// �萔�o�b�t�@�̐���
	//	result = device->CreateCommittedResource(
	//		&cbHeapProp, // �q�[�v�ݒ�
	//		D3D12_HEAP_FLAG_NONE,
	//		&cbResourceDesc, // ���\�[�X�ݒ�
	//		D3D12_RESOURCE_STATE_GENERIC_READ,
	//		nullptr,
	//		IID_PPV_ARGS(&constBuffTransform1));
	//	assert(SUCCEEDED(result));

	//	// �萔�o�b�t�@�̃}�b�s���O
	//	result = constBuffTransform1->Map(0, nullptr, (void**)&constMapTransform1); // �}�b�s���O
	//	assert(SUCCEEDED(result));
	//}
	//constMapTransform0->unk = XMMatrixIdentity();
	////���s���e�s��̌v�Z
	//constMapTransform0->mat = XMMatrixOrthographicOffCenterLH(
	//	-1.0f, 1.0f,
	//	-1.0f, 1.0f,
	//	0.0f, 1.0f
	//);

	//XMMATRIX matWorld;
	//matWorld = XMMatrixIdentity();

	//XMMATRIX matScale;//�X�P�[�����O�s��
	//matScale = XMMatrixScaling(1.0f, 0.5f, 1.0f);
	//matWorld *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f

	//XMMATRIX matRot;//��]�s��
	//matRot = XMMatrixIdentity();
	//matRot *= XMMatrixRotationZ(XMConvertToRadians(0.0f));//Z�������0�x��]���Ă���
	//matRot *= XMMatrixRotationX(XMConvertToRadians(15.0f));//X�������15�x��]���Ă���
	//matRot *= XMMatrixRotationY(XMConvertToRadians(30.0f));//Y���܂���30�x��]
	//
	//matWorld *= matRot;//���[���h�s��ɉ�]�𔽉f

	//XMMATRIX  matTrans;//���s�ړ��s��
	//matTrans = XMMatrixTranslation(-50, 0, 0);//(-50,0,0)���s�ړ�
	//matWorld *= matTrans;

	//XMMATRIX matWorld;
	//XMMATRIX matScale;//�X�P�[�����O�s��
	//XMMATRIX matRot;//��]�s��
	//XMMATRIX  matTrans;//���s�ړ��s��

	////���W
	//XMFLOAT3 scale(1.0f, 1.0f, 1.0f);
	//XMFLOAT3 rotation(0.0f, 0.0f, 0.0f);
	//XMFLOAT3 position(0.0f, 0.0f, 0.0f);

	//�ˉe�ϊ��s��i�������e�j
	XMMATRIX matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(45.0f),			//�㉺��p45�x
		(float)WinApp::window_width / WinApp::window_height,//�A�X�y�N�g��i��ʉ���/��ʏc���j
		0.1f, 1000.0f						//�O�[�A���[
	);

	////�r���[�ϊ��s��
	XMMATRIX matView;
	XMFLOAT3 eye(0, 0, -100);	//���_���W
	XMFLOAT3 target(0, 0, 0);	//�����_���W
	XMFLOAT3 up(0, 1, 0);		//������x�N�g��

	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

	float angle = 0.0f;//�J�����̉�]�p

	////�萔�o�b�t�@�ɓ]��
	//constMapTransform0->mat = matView * matProjection;

	// �q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES cbHeapProp{};
	cbHeapProp.Type = D3D12_HEAP_TYPE_UPLOAD;                   // GPU�ւ̓]���p
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc{};
	cbResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;   // 256�o�C�g�A���C�������g
	cbResourceDesc.Height = 1;
	cbResourceDesc.DepthOrArraySize = 1;
	cbResourceDesc.MipLevels = 1;
	cbResourceDesc.SampleDesc.Count = 1;
	cbResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC cbResourceDesc2{};
	cbResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	cbResourceDesc2.Width = (sizeof(ConstBufferDataMaterial) + 0xff) & ~0xff;   // 256�o�C�g�A���C�������g
	cbResourceDesc2.Height = 1;
	cbResourceDesc2.DepthOrArraySize = 1;
	cbResourceDesc2.MipLevels = 1;
	cbResourceDesc2.SampleDesc.Count = 1;
	cbResourceDesc2.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> constBuffMaterial;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial));

	ComPtr<ID3D12Resource> constBuffMaterial2;

	// �萔�o�b�t�@�̐���
	result = device->CreateCommittedResource(
		&cbHeapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&cbResourceDesc2, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&constBuffMaterial2));

	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial = nullptr;
	result = constBuffMaterial->Map(0, nullptr, (void**)&constMapMaterial); // �}�b�s���O
	assert(SUCCEEDED(result));

	// �萔�o�b�t�@�̃}�b�s���O
	ConstBufferDataMaterial* constMapMaterial2 = nullptr;
	result = constBuffMaterial2->Map(0, nullptr, (void**)&constMapMaterial2); // �}�b�s���O
	assert(SUCCEEDED(result));

	// �l���������ނƎ����I�ɓ]�������
	constMapMaterial2->color = XMFLOAT4(0, 1, 1, 1);              // RGBA�Ŕ������̐�

//// �C���f�b�N�X�f�[�^
//	unsigned short indices[] = {
//		//�O
//		0, 1, 2, // �O�p�`1��
//		2, 1, 3, // �O�p�`2��
//		//��
//		4,6,5,// �O�p�`3��
//		5,6,7,// �O�p�`4��
//		//��
//		8,9,10,	 //�O�p�`5
//		10,9,11,//�O�p�`6
//		//�E
//		12,14,13, //�O�p�`7
//		13,14,15, //�O�p�`8
//		//��
//		16,17,18, //�O�p�`9
//		17,18,19, //�O�p�`10
//		//��
//		20,21,22, //�O�p�`11
//		21,22,23, //�O�p�`12
//
//	};


	// �C���f�b�N�X�f�[�^�S�̂̃T�C�Y
	UINT sizeIB = static_cast<UINT>(sizeof(uint16_t) * _countof(indices));

	// ���\�[�X�ݒ�
	resDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	resDesc.Width = sizeIB; // �C���f�b�N�X��񂪓��镪�̃T�C�Y
	resDesc.Height = 1;
	resDesc.DepthOrArraySize = 1;
	resDesc.MipLevels = 1;
	resDesc.SampleDesc.Count = 1;
	resDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	// �C���f�b�N�X�o�b�t�@�̐���
	ComPtr<ID3D12Resource> indexBuff;
	result = device->CreateCommittedResource(
		&heapProp, // �q�[�v�ݒ�
		D3D12_HEAP_FLAG_NONE,
		&resDesc, // ���\�[�X�ݒ�
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBuff));

	// �C���f�b�N�X�o�b�t�@���}�b�s���O
	uint16_t* indexMap = nullptr;
	result = indexBuff->Map(0, nullptr, (void**)&indexMap);
	// �S�C���f�b�N�X�ɑ΂���
	for (int i = 0; i < _countof(indices); i++)
	{
		indexMap[i] = indices[i];   // �C���f�b�N�X���R�s�[
	}
	// �}�b�s���O����
	indexBuff->Unmap(0, nullptr);

	// �C���f�b�N�X�o�b�t�@�r���[�̍쐬
	D3D12_INDEX_BUFFER_VIEW ibView{};
	ibView.BufferLocation = indexBuff->GetGPUVirtualAddress();
	ibView.Format = DXGI_FORMAT_R16_UINT;
	ibView.SizeInBytes = sizeIB;

	TexMetadata metadata{};
	ScratchImage scratchImg{};
	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/meemu.jpg",   //�uResources�v�t�H���_�́utexture.png�v
		WIC_FLAGS_NONE,
		&metadata, scratchImg);

	TexMetadata metadata2{};
	ScratchImage scratchImg2{};
	// WIC�e�N�X�`���̃��[�h
	result = LoadFromWICFile(
		L"Resources/wakka.jpg",   //�uResources�v�t�H���_�́utexture.png�v
		WIC_FLAGS_NONE,
		&metadata2, scratchImg2);

	ScratchImage mipChain{};



	// �~�b�v�}�b�v����
	result = GenerateMipMaps(
		scratchImg.GetImages(), scratchImg.GetImageCount(), scratchImg.GetMetadata(),
		TEX_FILTER_DEFAULT, 0, mipChain);
	if (SUCCEEDED(result)) {
		scratchImg = std::move(mipChain);
		metadata = scratchImg.GetMetadata();
	}
	// �ǂݍ��񂾃f�B�t���[�Y�e�N�X�`����SRGB�Ƃ��Ĉ���
	metadata.format = MakeSRGB(metadata.format);


	//�q�[�v�ݒ�
	D3D12_HEAP_PROPERTIES textureHeapProp{};
	textureHeapProp.Type = D3D12_HEAP_TYPE_CUSTOM;
	textureHeapProp.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	textureHeapProp.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	// ���\�[�X�ݒ�
	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc{};
	textureResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc.Format = metadata.format;
	textureResourceDesc.Width = metadata.width;
	textureResourceDesc.Height = (UINT)metadata.height;
	textureResourceDesc.DepthOrArraySize = (UINT16)metadata.arraySize;
	textureResourceDesc.MipLevels = (UINT16)metadata.mipLevels;
	textureResourceDesc.SampleDesc.Count = 1;

	// ���\�[�X�ݒ�
	D3D12_RESOURCE_DESC textureResourceDesc2{};
	textureResourceDesc2.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	textureResourceDesc2.Format = metadata2.format;
	textureResourceDesc2.Width = metadata2.width;
	textureResourceDesc2.Height = (UINT)metadata2.height;
	textureResourceDesc2.DepthOrArraySize = (UINT16)metadata2.arraySize;
	textureResourceDesc2.MipLevels = (UINT16)metadata2.mipLevels;
	textureResourceDesc2.SampleDesc.Count = 1;


	// �e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff));
	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	// �S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata.mipLevels; i++) {
		// �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(
			(UINT)i,
			nullptr,              // �S�̈�փR�s�[
			img->pixels,          // ���f�[�^�A�h���X
			(UINT)img->rowPitch,  // 1���C���T�C�Y
			(UINT)img->slicePitch // 1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	// �e�N�X�`���o�b�t�@�̐���
	ComPtr<ID3D12Resource> texBuff2;
	result = device->CreateCommittedResource(
		&textureHeapProp,
		D3D12_HEAP_FLAG_NONE,
		&textureResourceDesc2,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&texBuff2));
	assert(SUCCEEDED(result));

	//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
	// �S�~�b�v�}�b�v�ɂ���
	for (size_t i = 0; i < metadata2.mipLevels; i++) {
		// �~�b�v�}�b�v���x�����w�肵�ăC���[�W���擾
		const Image* img = scratchImg2.GetImage(i, 0, 0);
		// �e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff2->WriteToSubresource(
			(UINT)i,
			nullptr,              // �S�̈�փR�s�[
			img->pixels,          // ���f�[�^�A�h���X
			(UINT)img->rowPitch,  // 1���C���T�C�Y
			(UINT)img->slicePitch // 1���T�C�Y
		);
		assert(SUCCEEDED(result));
	}

	// SRV�̍ő��
	const size_t kMaxSRVCount = 2056;

	// �f�X�N���v�^�q�[�v�̐ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvHeapDesc = {};
	srvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;//�V�F�[�_���猩����悤��
	srvHeapDesc.NumDescriptors = kMaxSRVCount;

	// �ݒ������SRV�p�f�X�N���v�^�q�[�v�𐶐�
	ComPtr<ID3D12DescriptorHeap> srvHeap = nullptr;
	result = device->CreateDescriptorHeap(&srvHeapDesc, IID_PPV_ARGS(srvHeap.GetAddressOf()));
	assert(SUCCEEDED(result));

	//SRV�q�[�v�̐擪�n���h�����擾
	D3D12_CPU_DESCRIPTOR_HANDLE srvHandle = srvHeap->GetCPUDescriptorHandleForHeapStart();

	// �V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = resDesc.Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = resDesc.MipLevels;


	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(texBuff.Get(), &srvDesc, srvHandle);

	//SRV�q�[�v�̐擪�n���h�����擾
	UINT incrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	// �V�F�[�_���\�[�X�r���[�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = textureResourceDesc2.Format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = textureResourceDesc2.MipLevels;

	srvHandle.ptr += incrementSize;
	// �n���h���̎w���ʒu�ɃV�F�[�_�[���\�[�X�r���[�쐬
	device->CreateShaderResourceView(texBuff2.Get(), &srvDesc2, srvHandle);

	////���\�[�X�ݒ�
	//D3D12_RESOURCE_DESC depthResourceDesc{};
	//depthResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	//depthResourceDesc.Width = WinApp::window_width;//�����_�[�^�[�Q�b�g�ɍ��킹��
	//depthResourceDesc.Height = WinApp::window_height;//�����_�[�^�[�Q�b�g�ɍ��킹��
	//depthResourceDesc.DepthOrArraySize = 1;
	//depthResourceDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	//depthResourceDesc.SampleDesc.Count = 1;
	//depthResourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;//�f�v�X�X�e���V��

	////�[�x�l�p�q�[�v�v���p�e�B
	//D3D12_HEAP_PROPERTIES depthHeapProp{};
	//depthHeapProp.Type = D3D12_HEAP_TYPE_DEFAULT;
	////�[�x�l�̃N���A�ݒ�
	//D3D12_CLEAR_VALUE depthClearValue{};
	//depthClearValue.DepthStencil.Depth = 1.0f;//�[�x�l1.0f(�ő�l)�ŃN���A
	//depthClearValue.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g

	////���\�[�X
	//ComPtr<ID3D12Resource> depthBuff;
	//result = device->CreateCommittedResource(
	//	&depthHeapProp,
	//	D3D12_HEAP_FLAG_NONE,
	//	&depthResourceDesc,
	//	D3D12_RESOURCE_STATE_DEPTH_WRITE,//�[�x�l�������݂Ɏg�p
	//	&depthClearValue,
	//	IID_PPV_ARGS(&depthBuff));

	////�[�x�r���[�p�f�X�N���v�^�q�[�v�쐬
	//D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc{};
	//dsvHeapDesc.NumDescriptors = 1;
	//dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;//�f�v�X�X�e���V���r���[
	//ComPtr<ID3D12DescriptorHeap> dsvHeap;
	//result = device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&dsvHeap));

	////�[�x�r���[�쐬
	//D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
	//dsvDesc.Format = DXGI_FORMAT_D32_FLOAT;//�[�x�l�t�H�[�}�b�g
	//dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	//device->CreateDepthStencilView(
	//	depthBuff.Get(),
	//	&dsvDesc,
	//	dsvHeap->GetCPUDescriptorHandleForHeapStart());
	////�f�v�X�X�e���V���X�e�[�g
	pipelineDesc.DepthStencilState.DepthEnable = true;//�[�x�e�X�g���s��
	pipelineDesc.DepthStencilState.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;//�������݋���
	pipelineDesc.DepthStencilState.DepthFunc = D3D12_COMPARISON_FUNC_LESS;
	pipelineDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;//�[�x�t�H�[�}�b�g


	// �p�C�v�����X�e�[�g�̐���
	ComPtr<ID3D12PipelineState> pipelineState;
	result = device->CreateGraphicsPipelineState(&pipelineDesc, IID_PPV_ARGS(&pipelineState));
	assert(SUCCEEDED(result));

	//�Q�[�����[�v
	while (true) {
		//���b�Z�[�W������H
		//if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE)) {
		//	TranslateMessage(&msg);//�L�[���̓��b�Z�[�W�̏���
		//	DispatchMessage(&msg);//�v���V�[�W���Ƀ��b�Z�[�W�𑗂�
		//}
		//Windows�̃��b�Z�[�W����
		if (winApp->ProcessMessage()) {
			//�Q�[�����[�v�𔲂���
			break;
		}

		// X�{�^���ŏI�����b�Z�[�W��������Q�[�����[�v�𔲂���
		if (msg.message == WM_QUIT) {

#ifdef _DEBUG
			//�f�o�b�O���C���[���I����
			//ComPtr<ID3D12DebugDevice> mDebugDevice;
			//device->QueryInterface(mDebugDevice.GetAddressOf());

			//mDebugDevice->ReportLiveDeviceObjects(D3D12_RLDO_DETAIL);


					// �쐬
			IDXGIDebug* pDxgiDebug;
			typedef HRESULT(__stdcall* fPtr)(const IID&, void**);
			HMODULE hDll = GetModuleHandleW(L"dxgidebug.dll");
			fPtr DXGIGetDebugInterface = (fPtr)GetProcAddress(hDll, "DXGIGetDebugInterface");

			DXGIGetDebugInterface(__uuidof(IDXGIDebug), (void**)&pDxgiDebug);

			// �o��
			pDxgiDebug->ReportLiveObjects(DXGI_DEBUG_D3D12, DXGI_DEBUG_RLO_DETAIL);
#endif
			break;
		}
		//DirectX���t���[�������@��������
		////�L�[�{�[�h���̎擾�J�n
		//keyboard->Acquire();

		////�S�L�[�̓��͏�Ԃ��擾����

		//BYTE key[256] = {};
		//keyboard->GetDeviceState(sizeof(key), key);
		input->Update();
		//�����̂O�L�[��������Ă�����
		//if (input->PushKey(DIK_0)) {
		//	OutputDebugStringA("Hit 0\n");//�o�̓E�B���h�E�ɁuHit 0�v�ƕ\��
		//}


		for (int i = 0; i < _countof(vertices); i++) {
			vertMap[i] = vertices[i]; // ���W���R�s�[
		}
		//DirectX���t���[�������@�����܂�

// �o�b�N�o�b�t�@�̔ԍ����擾(2�Ȃ̂�0�Ԃ�1��)
		UINT bbIndex = swapChain->GetCurrentBackBufferIndex();
		// 1.���\�[�X�o���A�ŏ������݉\�ɕύX
		D3D12_RESOURCE_BARRIER barrierDesc{};
		barrierDesc.Transition.pResource = backBuffers[bbIndex].Get(); // �o�b�N�o�b�t�@���w��
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ�
		commandList->ResourceBarrier(1, &barrierDesc);

		// 2.�`���̕ύX
		// �����_�[�^�[�Q�b�g�r���[�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandle = rtvHeap->GetCPUDescriptorHandleForHeapStart();
		rtvHandle.ptr += bbIndex * device->GetDescriptorHandleIncrementSize(rtvHeapDesc.Type);
		//�[�x�X�e���V���r���[�p�f�X�N���v�^�\�q�[�v�̃n���h�����擾
		D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle = dsvHeap->GetCPUDescriptorHandleForHeapStart();
		commandList->OMSetRenderTargets(1, &rtvHandle, false, &dsvHandle);

		// 3.��ʃN���A R G B A
		FLOAT clearColor[] = { 0.1f,0.25f, 0.5f,0.0f }; // ���ۂ��F

		commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		commandList->ClearDepthStencilView(dsvHandle, D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);


		if (input->PushKey(DIK_SPACE)) {
			FLOAT clearColor[] = { 1.0f,0.2f,0.8f };
			commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
		}
		if (input->PushKey(DIK_D) || input->PushKey(DIK_A))
		{
			if (input->PushKey(DIK_D)) { angle += XMConvertToRadians(1.0f); }
			else if (input->PushKey(DIK_A)) { angle -= XMConvertToRadians(1.0f); }

			//angle���W�A��������������ɉ�]�B���a�́]100
			eye.x = -100 * sinf(angle);
			eye.z = -100 * cosf(angle);

			//�r���[�ϊ��s��
			//XMMATRIX matView;
			//XMFLOAT3 eye(0, 0, -100);	//���_���W
			//XMFLOAT3 target(0, 0, 0);	//�����_���W
			//XMFLOAT3 up(0, 1, 0);		//������x�N�g��

			matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));

		}

		////�����ꂩ�̃L�[�������Ă�����
		//if (key[DIK_UP] | key[DIK_DOWN] | key[DIK_RIGHT] | key[DIK_LEFT])
		//{
		//	//���W���ړ����鏈��(Z���W)
		//	if (key[DIK_UP]) { position.z += 1.0f; }
		//	else if (key[DIK_DOWN]) { position.z -= 1.0f; }
		//	if (key[DIK_RIGHT]) { position.x += 1.0f; }
		//	else if (key[DIK_LEFT]) { position.x -= 1.0f; }
		//}


		//matScale = XMMatrixScaling(scale.x, scale.y, scale.z);

		//matRot = XMMatrixIdentity();
		//matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));//Z�������0�x��]���Ă���
		//matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));//X�������15�x��]���Ă���
		//matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));//Y���܂���30�x��]

		//matTrans = XMMatrixTranslation(position.x, position.y, position.z);//(-50,0,0)���s�ړ�


		//matWorld = XMMatrixIdentity();//�ό`�����Z�b�g
		//matWorld *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
		//matWorld *= matScale;//���[���h�s��ɃX�P�[�����O�𔽉f
		//matWorld *= matTrans;

		////�萔�o�b�t�@�ɓ]��
		//constMapTransform0->mat = matWorld * matView * matProjection;

		//XMMATRIX matWorld1;
		//matWorld1 = XMMatrixIdentity();

		////�e��ό`�s����v�Z
		//XMMATRIX matScale1 = XMMatrixScaling(1.0f, 1.0f, 1.0f);
		//XMMATRIX matRot1 = XMMatrixRotationY(XM_PI / 4.0f);
		//XMMATRIX matTrans1 = XMMatrixTranslation(-20.0f, 0, 0);
		////���[���h�s�������
		//matWorld1 = matScale1 * matRot1 * matTrans1;
		////���[���h�A�r���[�A�ˉe�s����������ăV�F�[�_�[�ɓ]��
		//constMapTransform1->mat = matWorld1 * matView * matProjection;

		//���W����
		if (input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_LEFT))
		{
			//���W���ړ����鏈��(Z���W)
			if (input->PushKey(DIK_UP)) { object3ds[0].position.y += 1.0f; }
			else if (input->PushKey(DIK_DOWN)) { object3ds[0].position.y -= 1.0f; }
			if (input->PushKey(DIK_RIGHT)) { object3ds[0].position.x += 1.0f; }
			else if (input->PushKey(DIK_LEFT)) { object3ds[0].position.x -= 1.0f; }
		}
		if (input->TriggerKey(DIK_0)) {
			OutputDebugStringA("Hit 0\n");
		}
		for (int i = 0; i < _countof(object3ds); i++) {
			UpdateObject3d(&object3ds[i], matView, matProjection);
		}

		// 4.�`��R�}���h��������
		// �r���[�|�[�g�ݒ�R�}���h
		D3D12_VIEWPORT viewport{};
		viewport.Width = WinApp::window_width;//����
		viewport.Height = WinApp::window_height;//�c��
		viewport.TopLeftX = 0;//����X
		viewport.TopLeftY = 0;//����Y
		viewport.MinDepth = 0.0f;//�ŏ��[�x�i�O�ł悢�j
		viewport.MaxDepth = 1.0f;//�ő�[�x�i�P�ł悢�j
		// �r���[�|�[�g�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
		commandList->RSSetViewports(1, &viewport);
		//�V�U�[�Z�`
		D3D12_RECT scissorRect{};
		scissorRect.left = 0;									// �؂蔲�����W��
		scissorRect.right = scissorRect.left + WinApp::window_width;	// �؂蔲�����W�E
		scissorRect.top = 0;									// �؂蔲�����W��
		scissorRect.bottom = scissorRect.top + WinApp::window_height;	// �؂蔲�����W��
		// �V�U�[��`�ݒ�R�}���h���A�R�}���h���X�g�ɐς�
		commandList->RSSetScissorRects(1, &scissorRect);
		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���̐ݒ�R�}���h
		commandList->SetPipelineState(pipelineState.Get());
		commandList->SetGraphicsRootSignature(rootSignature.Get());
		//�v���~�e�B�u�`��̐ݒ�R�}���h
		commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);//�O�p���X�g
		//���_�o�b�t�@�r���[�̐ݒ�R�}���h
		commandList->IASetVertexBuffers(0, 1, &vbView);

		// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		commandList->SetGraphicsRootConstantBufferView(0, constBuffMaterial2->GetGPUVirtualAddress());
		// SRV�q�[�v�̐ݒ�R�}���h
		commandList->SetDescriptorHeaps(1, srvHeap.GetAddressOf());
		// SRV�q�[�v�̐擪�n���h�����擾�iSRV���w���Ă���͂��j
		D3D12_GPU_DESCRIPTOR_HANDLE srvGpuHandle = srvHeap->GetGPUDescriptorHandleForHeapStart();
		// SRV�q�[�v�̐擪�ɂ���SRV�����[�g�p�����[�^1�Ԃɐݒ�
		srvGpuHandle.ptr += incrementSize;
		commandList->SetGraphicsRootDescriptorTable(1, srvGpuHandle);
		// �C���f�b�N�X�o�b�t�@�r���[�̐ݒ�R�}���h
		commandList->IASetIndexBuffer(&ibView);

		//// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		//commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform0->GetGPUVirtualAddress());
		//// �`��R�}���h
		//commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);

		//// �萔�o�b�t�@�r���[(CBV)�̐ݒ�R�}���h
		//commandList->SetGraphicsRootConstantBufferView(2, constBuffTransform1->GetGPUVirtualAddress());
		//// �`��R�}���h
		//commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);

		// �`��R�}���h
		//commandList->DrawInstanced(_countof(vertices), 1, 0, 0); // �S�Ă̒��_���g���ĕ`��
		/*commandList->DrawIndexedInstanced(_countof(indices), 1, 0, 0, 0);*/
		//XMFLOAT3 vertics[] = {
		//	{-0.5f,-0.5f,0.0f},//X���[�ō��@Y���[�ŉ��@����
		//	{-0.5f,+0.5f,0.0f},//X���[�ō��@Y���{�ŏ�@����
		//	{+0.5f,-0.5f,0.0f},//X���{�ŉE�@Y���[�ŉ��@�E��

		//};

		//�S�I�u�W�F�N�g�ɂ��ď���
		for (int i = 0; i < _countof(object3ds); i++) {
			DrawObject3d(&object3ds[i], commandList.Get(), vbView, ibView, _countof(indices));
		}

		D3D12_INPUT_ELEMENT_DESC inputLayout[]{
			{
			"POSITION",//�Z�}���`�b�N
			0,//�����Z�}���`�b�N������������Ƃ��Ɏg���C���f�b�N�X�i�O�ł悢�j
			DXGI_FORMAT_R32G32B32_FLOAT,//�v�f���ƃr�b�g����\���iXYZ�̂R��float�^�Ȃ̂�R32G32B32_FLOAT�j
			0,//���̓X���b�g�C���f�b�N�X
			D3D12_APPEND_ALIGNED_ELEMENT,//�f�[�^�̃I�t�Z�b�g�l�iD3D12_APPEND_ALIGNED_ELEMENT���Ǝ����ݒ�j
			D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA,//���̓f�[�^��ʁi�W����D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA�j
			0//��x�ɕ`�悷��C���X�^���X���i0�ł悢�j
			},
			//���W�ȊO�Ɂ@�F�A�e�N�X�`��UV�Ȃǂ�n���ꍇ�͍X�ɑ�����
		};

		// 4.�`��R�}���h�����܂�

		// 5.���\�[�X�o���A��߂�
		barrierDesc.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET; // �`���Ԃ���
		barrierDesc.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT; // �\����Ԃ�
		commandList->ResourceBarrier(1, &barrierDesc);
		// ���߂̃N���[�Y
		result = commandList->Close();
		assert(SUCCEEDED(result));
		// �R�}���h���X�g�̎��s
		ID3D12CommandList* commandLists[] = { commandList.Get() };
		commandQueue->ExecuteCommandLists(1, commandLists);
		// ��ʂɕ\������o�b�t�@���t���b�v(���\�̓��ւ�)
		result = swapChain->Present(1, 0);
		assert(SUCCEEDED(result));
		// �R�}���h�̎��s������҂�
		commandQueue->Signal(fence.Get(), ++fenceVal);
		if (fence->GetCompletedValue() != fenceVal) {
			HANDLE event = CreateEvent(nullptr, false, false, nullptr);
			fence->SetEventOnCompletion(fenceVal, event);
			if (event != 0) {
				WaitForSingleObject(event, INFINITE);
				CloseHandle(event);
			}
		}
		// �L���[���N���A
		result = cmdAllocator->Reset();
		assert(SUCCEEDED(result));
		// �ĂуR�}���h���X�g�𒙂߂鏀��
		result = commandList->Reset(cmdAllocator.Get(), nullptr);
		assert(SUCCEEDED(result));

	}
	winApp->Finalize();
	delete winApp;
	delete input;
	delete dxCommon;
	

	return 0;
}