#pragma once

// C++
#include <string>

// Engine
#include <MyMath.h>
#include <Object3D.h>
#include <Collider/Collider.h>

/// <summary>
/// 飛翔物の大分類
/// </summary>
enum class ObjectCategory {
	Meteor,     // 隕石
	RepairPart, // 修理パーツ
};

// =========================================================
// 飛翔物の基底クラス
// =========================================================
class FlyingObject : public Cygnus::ICollisionCallback {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	virtual ~FlyingObject() = default;

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="position">位置</param>
	/// <param name="isRightToLeft">true: 右から左へ, false: 左から右へ</param>
	virtual void Initialize(const Cygnus::Float3& position, bool isRightToLeft = true);

	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw();

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// 位置の取得
	/// </summary>
	/// <returns></returns>
	const Cygnus::Float3& GetTranslate() const { return object_->transform_.translate_; } 

	/// <summary>
	/// 当たり判定半径の取得
	/// </summary>
	/// <returns></returns>
	float GetHitRadius() const { return hitRadius_; }

	/// <summary>
	/// 大分類の取得
	/// </summary>
	/// <returns></returns>
	ObjectCategory GetCategory() const { return category_; }

	/// <summary>
	/// 死亡フラグの取得
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// 消滅させる
	/// </summary>
	void Destroy();

protected:
	// =========================================================
	// Member Variables
	// =========================================================

	// オブジェクト
	std::unique_ptr<Cygnus::Object3D> object_;
	// コライダー
	std::unique_ptr<Cygnus::Collider> collider_;

	// 飛翔物の大分類
	ObjectCategory category_ = ObjectCategory::Meteor;
	// 移動速度
	float speed_ = 0.0f;
	// 回転速度
	float rotationSpeed_ = 0.0f;
	// 当たり判定半径
	float hitRadius_ = 0.0f;
	// 死亡フラグ
	bool isDead_ = false;

	// 左右の移動方向管理（-1.0f : 左方向, 1.0 : 右方向）
	float directionX_ = -1.0f;

	// 消滅境界線（仮）
	static constexpr float kDespawnX = 50.0f;
};
