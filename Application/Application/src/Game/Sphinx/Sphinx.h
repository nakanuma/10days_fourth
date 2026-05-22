#pragma once

#include <Object3D.h>
#include <Collider/CollisionManager.h>
#include "RandomWalk.h"
#include "State/StateMachine.h"
#include "State/State.h"

/// <summary>
/// ステートの種類
/// </summary>
enum class SphinxState
{
	Wander,
	Charge,
	Attack,
	Faint,
	CoolDown
};

/// <summary>
/// スフィンクスの制御クラス
/// </summary>
class Sphinx : public Cygnus::ICollisionCallback
{
public:
	/// <summary>
	/// 初期化処理を行います。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理を行います。
	/// </summary>
	/// <param name="deltaTime">経過時間</param>
	/// <param name="targetPos">追跡対象の座標</param>
	void Update(float deltaTime, const Cygnus::Float3& targetPos);

	void UpdateAttackSign(float t);

	/// <summary>
	/// 描画処理を行います。
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ用表示処理を行います。
	/// </summary>
	void Debug();

	/// <summary>
	/// 衝突時のコールバック処理を行います。
	/// </summary>
	/// <param name="other">衝突相手のコライダー</param>
	void OnCollision(Cygnus::Collider* other) override;

	// --- Getter / Setter ---

	/// <summary>
	/// 3Dオブジェクトを取得します。
	/// </summary>
	Cygnus::Object3D* GetObject() { return object_.get(); }

	/// <summary>
	/// ランダムウォーク制御を取得します。
	/// </summary>
	RandomWalk* GetRandomWalk() { return randomWalk_.get(); }

	/// <summary>
	/// ターゲット座標を取得します。
	/// </summary>
	const Cygnus::Float3& GetTargetPos() const { return targetPos_; }

	/// <summary>
	/// ターゲット座標を設定します。
	/// </summary>
	void SetTargetPos(const Cygnus::Float3& pos) { targetPos_ = pos; }

	/// <summary>
	/// 攻撃方向を取得します。
	/// </summary>
	const Cygnus::Float3& GetAttackDir() const { return attackDir_; }

	/// <summary>
	/// 攻撃方向を設定します。
	/// </summary>
	void SetAttackDir(const Cygnus::Float3& dir) { attackDir_ = dir; }

	/// <summary>
	/// 採掘中フラグを取得します。
	/// </summary>
	bool GetIsMining() const { return isMining_; }

	/// <summary>
	/// 採掘中フラグを設定します。
	/// </summary>
	void SetIsMining(bool is) { isMining_ = is; }

	// 定数へのアクセス
	float GetSearchRange() const { return kSearchRange_; }
	float GetChargeTime() const { return kChargeTime_; }
	float GetHomingLimitTime() const { return kHomingLimitTime_; }
	float GetChargeTurnSpeed() const { return kChargeTurnSpeed_; }
	float GetWanderTurnSpeed() const { return kWanderTurnSpeed_; }
	float GetBounceHeight() const { return kBounceHeight_; }
	float GetBounceSpeed() const { return kBounceSpeed_; }
	float GetBaseY() const { return kBaseY_; }
	float GetAttackTime() const { return kAttackTime_; }
	float GetAttackMoveSpeed() const { return kAttackMoveSpeed_; }
	float GetFaintTime() const { return kFaintTime_; }
	float GetAttackCoolTime() const { return kAttackCoolTime_; }

	/// <summary>
	/// 移動中かどうかを取得します。
	/// </summary>
	bool GetIsMoving() { return isMoving_; }

	// --- 共通アクション (ステートから呼ばれる) ---

	/// <summary>
	/// 前方に移動します。
	/// </summary>
	void MoveForward(float speed, float deltaTime);

	/// <summary>
	/// 鉱石の採掘処理を行います。
	/// </summary>
	void OreMining();

private:
	/// <summary>
	/// 移動範囲のクランプ処理を行います。
	/// </summary>
	void MoveClamp();

private:
	// =========================================================
	// Constants
	// =========================================================

	const float kMoveSpeed = 1.0f;											// 通常移動速度
	const float kMoveChangeTime_ = 2.0f;									// 移動方向変化の間隔
	const Cygnus::Float3 kColliderSize = { 2.0f, 2.0f, 3.0f };				// コライダーのサイズ
	const float kAttackMoveSpeed_ = 60.0f;									// 突進攻撃の速度
	const float kAttackTime_ = 0.334f;										// 突進持続時間
	const float kFaintTime_ = 2.0f;											// 気絶時間
	const float kSearchRange_ = kAttackMoveSpeed_ * kAttackTime_;			// プレイヤー索敵範囲
	const float kAttackCoolTime_ = 3.0f;									// 攻撃後のクールタイム
	const float kChargeTime_ = 1.5f;										// 溜め時間
	const float kWanderTurnSpeed_ = 6.0f;									// 徘徊時の旋回スピード
	const float kChargeTurnSpeed_ = 10.0f;									// 溜め時の旋回スピード
	const float kBounceHeight_ = 2.0f;										// 跳ねる高さ
	const float kHomingLimitTime_ = 0.5f;									// ホーミングを制限する時間
	const float kBounceSpeed_ = kHomingLimitTime_ * 25.0f;					// 跳ねる速度
	const float kBaseY_ = 2.0f;												// 基本の高さ(Y座標)
	const float kMiningOffset = 3.5f;										// 採掘判定のオフセット
	const float kMiningRange = 5.5f;										// 採掘判定の範囲

	const Cygnus::Float3 kMoveMin = { -30.0f, 0.0f, -30.0f };				// 移動可能範囲の最小値
	const Cygnus::Float3 kMoveMax = { 30.0f, 0.0f, 15.0f };					// 移動可能範囲の最大値

	const Cygnus::Float4 kPlaneColor = { 1.0f, 0.3f, 0.3f, 0.75f };			// 移動可能範囲の最小値
	const Cygnus::Float4 kFrameColor = { 1.0f, 0.6f, 0.6f, 0.9f };			// 移動可能範囲の最大値

	const float kRageMultiplier = 1.1f;

	// =========================================================
	// Member Variables
	// =========================================================

	std::unique_ptr<Cygnus::Object3D> object_;			// 3Dモデルオブジェクト
	std::unique_ptr<Cygnus::Collider> collider_;		// コライダー
	std::unique_ptr<RandomWalk> randomWalk_;			// ランダムウォーク制御

	StateMachine<SphinxState, Sphinx> stateMachine_;	// ステートマシン

	Cygnus::Float3 targetPos_;							// 共有変数：プレイヤー座標
	Cygnus::Float3 attackDir_;							// 共有変数：突進方向
	bool isMining_ = false;								// 共有変数：採掘フラグ
	bool isMoving_ = true;								// 移動中フラグ


	int rageCount_ = 0;
	float rageMultiPlier_ = 1.0f;


	std::unique_ptr<Cygnus::Object3D> attackPlane_;		// 攻撃予兆オブジェクト
	std::unique_ptr<Cygnus::Object3D> attackFrame_;		// 攻撃予兆オブジェクト
};