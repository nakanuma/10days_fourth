#pragma once

// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include <Object3D.h>
#include <Collider/CollisionManager.h>

// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include "RandomWalk.h"

// =========================================================
// スフィンクスクラス
// =========================================================
class Sphinx : public Cygnus::ICollisionCallback
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(float deltaTime);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Debug();

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other"></param>
	void OnCollision(Cygnus::Collider* other) override;

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move(float deltaTime);

	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack(float deltaTime);

	void Stop();

private:
	// =========================================================
	// Constants
	// =========================================================

	const float kMoveSpeed = 1.0f;	// 移動速度
	const Cygnus::Float3 kColliderSize = { 2.0f, 2.0f, 3.0f };	// コライダーサイズ
	const float kAttackMoveSpeed_ = 30.0f;
	const float kAttackTime_ = 0.5f;

	// =========================================================
	// Member Variables
	// =========================================================

	std::unique_ptr<Cygnus::Object3D> object_;	// オブジェクト
	std::unique_ptr<Cygnus::Collider> collider_;	// コライダー

	std::unique_ptr<RandomWalk> randomWalk_;	// ランダム移動用

	Cygnus::Float3 velocity_ = { 0.0f, 0.0f, 0.0f };	// 速度ベクトル

	Cygnus::Float3 moveDir_ = { 0.0f, 0.0f, 1.0f };

	bool isAttack_ = false;
	Cygnus::Float3 attackDir_ = { 0.0f, 0.0f, 1.0f };
	float attackTimer_ = 0.0f;
};

