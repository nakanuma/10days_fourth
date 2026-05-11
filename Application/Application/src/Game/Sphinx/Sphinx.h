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
	void Update(float deltaTime, const Cygnus::Float3& targetPos);

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

	/// <summary>
	/// 動かすかどうか
	/// </summary>
	void IsMoving(bool is) { isMoving_ = is; }

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// 移動処理
	/// </summary>
	void Move(float deltaTime);

	/// <summary>
	/// 攻撃の予備動作開始
	/// </summary>
	void StartCharge(const Cygnus::Float3& targetDir);
	/// <summary>
	/// 攻撃開始処理
	/// </summary>
	/// <param name="targetDir">ターゲットへの正規化向きベクトル</param>
	void StartAttack();
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

	const float kAttackMoveSpeed_ = 40.0f;	// 突進時速度
	const float kAttackTime_ = 0.5f;		// 突進時全体時間

	const float kFaintTime_ = 2.0f;

	const Cygnus::Float3 kMoveMin = { -30.0f, 0.0f, -30.0f };
	const Cygnus::Float3 kMoveMax = { 30.0f, 0.0f, 15.0f };

	const float kSearchRange_ = 20.0f;		// プレイヤーをサーチ(検知)する範囲
	const float kAttackCoolTime_ = 3.0f;	// 攻撃後のクールダウン(待機)時間

	const float kChargeTime_ = 1.5f;		// 突進前の予備動作（チャージ）時間

	// 旋回スピードの定数を追加
	const float kWanderTurnSpeed_ = 6.0f;	// 徘徊時の旋回スピード
	const float kChargeTurnSpeed_ = 10.0f;	// 予兆時の旋回スピード

	const float kBounceHeight_ = 2.0f;		// 飛び跳ねる高さ
	const float kBounceSpeed_ = (kMoveChangeTime_ - kChargeTime_) * 25.0f;		// 飛び跳ねるスピード
	const float kBaseY_ = 2.0f;				// 地面の基本Y座標

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

	float attackCoolTimer_ = 0.0f;			// クールダウン用タイマー

	bool isCharge_ = false;					// チャージ状態判定
	float chargeTimer_ = 0.0f;				// チャージ用タイマー
	const float kHomingLimitTime_ = 0.5f;	// チャージ終了の残り何秒で追従を止めるか

	bool isMining_ = false;

	const float kMiningOffset = 3.5f;	// 採掘時の前方オフセット
	const float kMiningRange = 2.5f;	// 採掘時のブレ許容値

	bool isMoving_ = true;
};

