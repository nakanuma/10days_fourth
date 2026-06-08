#pragma once

// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include <Collider/CollisionManager.h>
#include <Object3D.h>
#include "src/Game//Sandstrom/FlyAway.h"

// =========================================================
// 落ちている鉱石（ドロップアイテム）オブジェクトクラス
// =========================================================
class DroppedOre : public Cygnus::ICollisionCallback {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(const Cygnus::Float3 translate);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other"></param>
	void OnCollision(Cygnus::Collider* other) override;

	/// <summary>
	/// コライダーの登録解除
	/// </summary>
	void UnregisterCollider() { Cygnus::CollisionManager::GetInstance()->Unregister(collider_.get()); }

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// プレイヤーに回収されたかどうかを取得
	/// </summary>
	/// <returns></returns>
	bool IsPickedUp() const { return isPickedUp_; }

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// 上下揺れ + 回転を行うアニメーション
	/// </summary>
	void BobbingAnimation();

private:
	// =========================================================
	// Constants
	// =========================================================

	const Cygnus::Float3 kColliderSize = {0.5f, 0.5f, 0.5f}; // コライダーサイズ

	const float kRotateSpeed = 2.0f;	// 回転速度
	const float kBobbingSpeed = 3.0f;	// 上下揺れの速さ
	const float kBobbingAmplitude = 0.2f;	// 上下揺れの幅

	// =========================================================
	// Member Variables
	// =========================================================

	std::unique_ptr<Cygnus::Object3D> object_;   // オブジェクト
	std::unique_ptr<Cygnus::Collider> collider_; // コライダー

	Cygnus::Float3 basePosition_;	// 生成時の初期座標
	float timer_ = 0.0f;	// アニメーション用タイマー

	bool isPickedUp_ = false;	// プレイヤーに拾われたか

	FlyAway flyAway_;
	Cygnus::Float3 velocity_;
};
