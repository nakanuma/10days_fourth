#pragma once

// Engine
#include <Object3D.h>
#include <Collider/CollisionManager.h>

// =========================================================
// 経路に沿って移動するオブジェクトクラス
// =========================================================
class Carrier : public Cygnus::ICollisionCallback
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
	/// デバッグ描画
	/// </summary>
	void Debug();

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="collider"></param>
	void OnCollision(Cygnus::Collider* other) override;
	
	/// <summary>
	/// setter_座標位置
	/// </summary>
	/// <param name="translate">座標を設定</param>
	void SetTranslate(const Cygnus::Float3& translate) { object_->transform_.translate_ = translate; }

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// 経路に沿って移動
	/// </summary>
	/// <param name="deltaTime"></param>
	void MoveAlongPath(float deltaTime);

	/// <summary>
	/// 歯車を使用された際の処理
	/// </summary>
	void SupplyGear();

private:
	// =========================================================
	// Constants
	// =========================================================

	const Cygnus::Float3 kColliderSize = {1.0f, 1.0f, 1.0f}; // コライダーサイズ
	const Cygnus::Float3 kSensorSize = {2.5f, 1.0f, 2.5f};	// 歯車注入範囲コライダーサイズ

	const uint32_t kRequiredGearCount = 1;	// 1回の注入で消費する歯車の数

	const float kMoveSpeed = 2.0f;	// 移動速度
	const float kMaxEnergy = 10.0f;	// 歯車注入時の動作時間（秒）
	const float kAccelerationTime = 1.0f;	// 動き出しの滑らかさ（秒）
	const float kDecelerationTime = 1.5f;	// 停止時の滑らかさ（秒）

	// =========================================================
	// Member Variables
	// =========================================================

	std::unique_ptr<Cygnus::Object3D> object_;	// オブジェクト
	std::unique_ptr<Cygnus::Collider> collider_;	// コライダー（本体）
	std::unique_ptr<Cygnus::Collider> colliderSensor_;	// コライダー（歯車注入範囲用）

	size_t targetIndex_ = 1; // 移動経路に使用するインデックス

	bool isGoal_ = false;	// ゴール済フラグ
	bool isActive_ = false; // 有効化フラグ

	float energyTimer_ = 0.0f;	// 残り動作時間
	float currentVelocityRate_ = 0.0f;	// 現在の速度倍率（0.0f ~ 1.0f）（動き出し・停止時の滑らかな移動に使用）
};