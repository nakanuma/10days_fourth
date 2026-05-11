#pragma once

// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include <Object3D.h>
#include <Collider/CollisionManager.h>

// =========================================================
// 鉱石オブジェクトクラス
// =========================================================
class Ore : public Cygnus::ICollisionCallback
{
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
	/// 位置を取得します。
	/// </summary>
	/// <returns></returns>
	const Cygnus::Float3& GetTranslate() const { return object_->transform_.translate_; }

private:
	// =========================================================
	// Constants
	// =========================================================

	const Cygnus::Float3 kColliderSize = {1.0f, 1.0f, 1.0f};	// コライダーサイズ

	// =========================================================
	// Member Variables
	// =========================================================
	
	std::unique_ptr<Cygnus::Object3D> object_;	// オブジェクト
	std::unique_ptr<Cygnus::Collider> collider_;	// コライダー
};

