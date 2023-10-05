#pragma once

#include "CollisionPrimitive.h"
#include "Vector3.h"
#include "Vector2.h"

class Collision
{
public:
	/// <summary>
	/// 点と三角形の最近接点を求める
	/// </summary>
	/// <param name="point">点</param>
	/// <param name="triangle">三角形</param>
	/// <param name="closest">最近接点（出力用）</param>
	static void ClosestPtPoint2Triangle(const Vector3& point, const Triangle& triangle, Vector3* closest);

	/// <summary>
	/// 球と法線付き三角形の当たり判定
	/// </summary>
	/// <param name="sphere">球</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">交点（三角形上の最近接点）</param>
	/// <param name="reject">排斥ベクトル（出力用）</param>
	/// <returns>交差しているか否か</returns>
	//static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, DirectX::Vector3* inter = nullptr);
	static bool CheckSphere2Triangle(const Sphere& sphere, const Triangle& triangle, Vector3* inter = nullptr);

	/// <summary>
	/// 球と平面の当たり判定
	/// </summary>
	/// <param name="sphereA">球A</param>
	/// <param name="sphereB">球B</param>
	/// <param name="inter">交点（出力用）</param>
	/// <param name="reject">排斥ベクトル（出力用）</param>
	/// <returns></returns>
	static bool CheckSphere2Plane(const Sphere& sphere, const Plane& plane, Vector3* inter = nullptr);

	/// <summary>
	/// レイと平面の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="plane">平面</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Plane(const Ray& ray, const Plane& plane, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// レイと法線付き三角形の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="triangle">三角形</param>
	/// <param name="inter">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Triangle(const Ray& ray, const Triangle& triangle, float* distance = nullptr, Vector3* inter = nullptr);

	/// <summary>
	/// レイと球の当たり判定
	/// </summary>
	/// <param name="lay">レイ</param>
	/// <param name="sphere">球</param>
	/// <param name="sphere">距離（出力用）</param>
	/// <param name="inter">交点（出力用）</param>
	/// <returns>交差しているか否か</returns>
	static bool CheckRay2Sphere(const Ray& ray, const Sphere& sphere, float* distance = nullptr, Vector3* inter = nullptr);

	static bool BoxCollision(Vector3 player, Vector3 enemy, Vector3 playerWidth, Vector3 enemyWidth);

	static bool BoxCircle(Vector3 boxPos, Vector3 circlePos, Vector3 boxWidth, float circleWidth);

	static bool CircleCollision(Vector3 playerPos, Vector3 enemyPos, float playerWidth, float enemyWidth);

	static bool CircleCollisionXZ(Vector3 playerPos, Vector3 enemyPos, float playerWidth, float enemyWidth);

	static bool Box2DCollision(Vector2 playerPos,Vector2 enemyPos,float playerWidth,float enemyWidth );

	static bool RaySphere(Vector3 rayPos, Vector3 enemyPos,float enemyRedius, Vector3 dir);
};

