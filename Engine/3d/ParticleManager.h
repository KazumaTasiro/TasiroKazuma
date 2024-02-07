#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <forward_list>
#include "Affin.h"
#include "Transform.h"
#include "Vector4.h"
#include <DirectXMath.h>

#include "Camera.h"
#include <array>
#include <sstream>
#include <fstream>
#include <string>
#include <filesystem>

/// <summary>
/// 3Dオブジェクト
/// </summary>
class ParticleManager
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;


public: // サブクラス
	// 定数バッファ用データ構造体（マテリアル）
	struct ConstBufferDataMaterial {
		Vector4 color; // 色 (RGBA)
	};

	// 頂点データ構造体
	struct VertexPos
	{
		Vector3 pos; // xyz座標
		float scale;
	};

	// 定数バッファ用データ構造体
	struct ConstBufferData
	{
		Matrix4 mat;
		Matrix4 matBillboard;
		Vector4 color;// ビルボード行列
	};

	//パーティクル一粒
	struct Particle {

		//座標
		Vector3 position = {};
		//速度
		Vector3 velocity = {};
		//加速度
		Vector3 accel = {};
		//現在フレーム
		int frame = 0;
		//過去フレーム
		int num_frame = 0;

		//スケール
		float scale = 1.0f;
		//初期値
		float s_scale = 1.0f;
		//最終値
		float e_scale = 0.0f;
		//初期カラー
		Vector4 s_color = { 1,1,1,1 };
		//最終カラー
		Vector4 e_color = { 1,1,1,1 };

		//パーティクルの種類
		int particleNmb = 0;

		Vector4 color; // 色 (RGBA)

	};

	//CSVの中身を保存する
	struct ParticleData
	{
			//位置
	Vector3 particlePos = { 0,0,0 };
	Vector3 randomParticlePos{ 0,0,0 };
	//終点
	Vector3 particleEndPos = { 0,0,0 };
	Vector3 endPointPos = { 0,0,0 };
	//大きさ
	//最初
	float particleStertScale = 1;
	float particleRandomStertScale = 1;
	//最後
	float particleEndScale = 1;
	float particleRandomEndScale = 1;

	//速度
	Vector3 particleSpeed = { 0,0,0 };
	float particleEndPointSpeed = 1.0f;
	Vector3 particleRandomSpeed = { 0,0,0 };
	float particleRandomSpeedX = 0;
	float particleRandomSpeedY = 0;
	float particleRandomSpeedZ = 0;

	//色
	Vector4 particleStertColor = { 1,1,1,1 };
	Vector4 particleEndColor = { 1,1,1,1 };

	//数
	int particleNmber = 0;
	//パーティクルのライフ
	int particleLife = 10;
	//イージングのナンバー
	int easingNmb = 0;

	char texFileName[ 30 ] = { "LockOnParticle" };

	//パーティクルのスイッチ
	bool randomParticleStertColor = false;
	bool randomParticleEndColor = false;
	bool randomParticleSize = false;
	bool endPoint = false;
	//ランダムpos

	bool randomParticlePosX;
	bool randomParticlePosY;
	bool randomParticlePosZ;
	//ランダムspeed
	bool randomParticleSpeedX;
	bool randomParticleSpeedY;
	bool randomParticleSpeedZ;
	//ランダムscale
	bool randomParticleStertScale = false;
	bool randomParticleEndScale = false;
	};


private: // 定数
	//const int division = 50;					// 分割数
	//const float radius;				// 底面の半径
	//const float prizmHeight;			// 柱の高さ
	//const int planeCount = division * 2 + division * 2;		// 面の数
	//static const int vertexCount = 30;//頂点数
	const int vertexCount = 4048;

public: // 静的メンバ関数
	/// <summary>
	/// 静的初期化
	/// </summary>
	/// <param name="device">デバイス</param>
	/// <param name="window_width">画面幅</param>
	/// <param name="window_height">画面高さ</param>
	static void StaticInitialize(ID3D12Device* device, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// グラフィックパイプライン生成
	/// </summary>
	/// <returns>成否</returns>
	static void InitializeGraphicsPipeline();

private: // 静的メンバ変数

	// デバイス
	static Microsoft::WRL::ComPtr<ID3D12Device> device;
	// コマンドリスト
	static Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmdList;
	// ルートシグネチャ
	static ComPtr<ID3D12RootSignature> rootsignature;
	// パイプラインステートオブジェクト
	static ComPtr<ID3D12PipelineState> pipelinestate;

	// デスクリプタサイズ
	UINT descriptorHandleIncrementSize;
	// デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeap;
	// 頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	// テクスチャバッファ
	ComPtr<ID3D12Resource>texbuff;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	// シェーダリソースビューのハンドル(CPU)
	CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;

	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView;
	// 頂点データ配列
	VertexPos vertices[2000];
	// 定数バッファ
	ComPtr<ID3D12Resource> constBuff;

	//パーティクル配列
	std::forward_list<Particle>particles;

private:// メンバ関数

	/// <summary>
	/// 3Dオブジェクト生成
	/// </summary>
	/// <returns></returns>
	ParticleManager* Create();

	/// <summary>
	/// デスクリプタヒープの初期化
	/// </summary>
	void InitializeDescriptorHeap();

	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture();

	/// <summary>
	/// モデル作成
	/// </summary>
	void CreateModel();

public: // メンバ関数

	ParticleManager();
	~ParticleManager();

	void LoadTexture(const std::string& fileName);
	bool Initialize();
	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// マネージャーの座標をもとにランダムに放出する
	/// </summary>
	void RandParticle();

	/// <summary>
	/// パーティクルの追加
	/// </summary>
	///	<param name="life">生存時間</param>
	///	<param name="position">初期座標</param>
	///	<param name="velocity">向き</param>
	///	<param name="speed">加速度</param>
	///	<param name="particleNmb">パーティクルの種類</param>
	/// 0.通常,1.徐々に速く,2.徐々に遅く
	void Add(int life, Vector3 position, Vector3 velociy, Vector3 speed, float start_scale, float end_scale,int particleNmb);
	void Add(int life,Vector3 position,Vector3 velociy,Vector3 speed,float start_scale,float end_scale,int particleNmb,Vector4 start_color,Vector4 end_color);

	static void SetCamera(Camera* camera_) { ParticleManager::camera = camera_; }

	void SetTransform(Transform wtf) { wtf_ = wtf; };

	void Setposition(Vector3 position) { wtf_.position = position; };

	void Setrotation(Vector3 rotation) { wtf_.rotation = rotation; };

	double lerp(double a, double b, double t);

	void SetColor(Vector4 color_);

	void LoadCSVfile(const std::string& fileNames);

	void UpdateCSVfile();

	void UpdateParticleAdd();

	float RandNmber(float randNmb);

private: // メンバ変数
	static Camera* camera;
	// ローカルスケール
	Transform wtf_;

	Vector4 color = { 1,1,1,1 };

	ConstBufferDataMaterial* constMapMaterial = nullptr;

	//パーティクル情報取得コマンド
	std::stringstream particleCSV;

	//パーティクルの追加の情報
	ParticleData pData;

};