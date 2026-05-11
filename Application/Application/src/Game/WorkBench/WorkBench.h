#pragma once

// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include <Object3D.h>
#include <Collider/CollisionManager.h>

// =========================================================
// 工作台オブジェクトクラス
// =========================================================
class WorkBench : public Cygnus::ICollisionCallback {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="translate"></param>
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

	// =========================================================
	// Accessor
	// =========================================================

private:
	// =========================================================
	// Constants
	// =========================================================

	const Cygnus::Float3 kColliderSize = {1.0f, 1.0f, 1.0f};	// コライダーサイズ
	const Cygnus::Float3 kSensorSize = {2.5f, 1.0f, 2.5f};	// クラフト範囲判定用コライダーサイズ（本体よりやや大きめ）

	const uint32_t kRequiredOreCount = 2;	// 歯車の作成に使用する鉱石数

	const float kDropOffset = -2.0f;	// ドロップの前方オフセット

	// =========================================================
	// Member Variables
	// =========================================================

	std::unique_ptr<Cygnus::Object3D> object_;	// オブジェクト
	std::unique_ptr<Cygnus::Collider> collider_;	// コライダー（作業台本体）
	std::unique_ptr<Cygnus::Collider> colliderSensor_;	// コライダー（クラフト範囲判定用）
};