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

