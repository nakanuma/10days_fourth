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
	/// 攻撃開始処理
	/// </summary>
	/// <param name="targetDir">ターゲットへの正規化向きベクトル</param>
	void StartAttack(const Cygnus::Float3& targetDir);
	/// <summary>
	/// 攻撃処理
	/// </summary>
	void Attack(float deltaTime);
	/// <summary>
	/// 攻撃停止処理
	/// </summary>
	void StopAttack();

	/// <summary>
	/// 気絶処理
	/// </summary>
	bool Faint(float deltaTime);
	/// <summary>
	/// 気絶開始処理
	/// </summary>
	void StartFaint();

	/// <summary>
	/// 鉱石破壊処理
	/// </summary>
	void OreMining();

	void MoveClamp();

private:
	// =========================================================
	// Constants
	// =========================================================

	const float kMoveSpeed = 1.0f;	// 移動速度
	const float kMoveChangeTime_ = 2.0f;
	const Cygnus::Float3 kColliderSize = { 2.0f, 2.0f, 3.0f };	// コライダーサイズ

	const float kAttackMoveSpeed_ = 30.0f;	// 突進時速度
	const float kAttackTime_ = 0.5f;		// 突進時全体時間

	const float kFaintTime_ = 2.0f;

	const Cygnus::Float3 kMoveMin = { -30.0f, 0.0f, -30.0f };
	const Cygnus::Float3 kMoveMax = { 30.0f, 0.0f, 15.0f };

	// =========================================================
	// Member Variables
	// =========================================================

	std::unique_ptr<Cygnus::Object3D> object_;	// オブジェクト
	std::unique_ptr<Cygnus::Collider> collider_;	// コライダー

	std::unique_ptr<RandomWalk> randomWalk_;	// ランダム移動用

	Cygnus::Float3 velocity_ = { 0.0f, 0.0f, 0.0f };	// 速度ベクトル

	Cygnus::Float3 moveDir_ = { 0.0f, 0.0f, 1.0f };		// 移動時の向きベクトル

	bool isAttack_ = false;								// 突進状態判定
	Cygnus::Float3 attackDir_ = { 0.0f, 0.0f, 1.0f };	// 突進時向きベクトル
	float attackTimer_ = 0.0f;							// 突進用タイマー

	float faintTimer_ = 0.0f;


	const float kMiningOffset = 3.5f;	// 採掘時の前方オフセット
	const float kMiningRange = 2.5f;	// 採掘時のブレ許容値
};

