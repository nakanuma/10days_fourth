#pragma once

// Engine
#include <Object3D.h>
#include <Collider/Collider.h>

// =========================================================
// 宇宙船
// =========================================================
class Spaceship : public Cygnus::ICollisionCallback
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
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Debug();

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// 現在位置の取得
	/// </summary>
	/// <returns></returns>
	const Cygnus::Float3& GetTranslate() { return object_->transform_.translate_; }

private:
	// =========================================================
	// Constants
	// =========================================================

	// コライダーの大きさ
	static constexpr Cygnus::Float3 kColliderSize = { 6.0f, 3.0f, 3.0f };

	// =========================================================
	// Member Variables
	// =========================================================

	// オブジェクト
	std::unique_ptr<Cygnus::Object3D> object_;

	// コライダー
	std::unique_ptr<Cygnus::Collider> collider_;
};

