#pragma once

// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include <MyMath.h>

class RandomWalk
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	/// <param name="changeTime">向きを変更するまでの時間</param>
	void Update(float deltaTime, float changeTime);

	/// <summary>
	/// 角度を滑らかに補間する（最短回り）
	/// </summary>
	float SmoothTurn(float currentAngle, float targetAngle, float turnSpeed, float deltaTime);

	/// <summary>
	/// 歩く向きを取得
	/// </summary>
	/// <param name="deltaTime">デルタタイム</param>
	/// <param name="changeTime">向きを変更するまでの時間</param>
	Cygnus::Float3& GetRandomWalkDir() { return randomDir_; }

	/// <summary>
	/// 有効化
	/// </summary>
	void Enable() { enable_ = true; }

	/// <summary>
	/// 無効化
	/// </summary>
	void Disable() { enable_ = false; }

	/// <summary>
	/// タイマーのリセット
	/// </summary>
	void Reset() { walkTimer_ = 0.0f; }


private:
	// =========================================================
	// Member Variables
	// =========================================================

	Cygnus::Float3 randomDir_ = {};

	float walkTimer_ = 0.0f;

	bool enable_ = true;

};

