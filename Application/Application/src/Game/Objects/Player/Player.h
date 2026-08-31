#pragma once

// Engine
#include <Object3D.h>
#include <Collider/Collider.h>

// =========================================================
// プレイヤー
// =========================================================
class Player : public Cygnus::ICollisionCallback
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
	// Internal Methods
	// =========================================================

	/// <summary>
	/// キー入力による移動処理
	/// </summary>
	void Move();

private:
	// =========================================================
	// Constants
	// =========================================================

	// コライダーの大きさ
	static constexpr Cygnus::Float3 kColliderSize = { 1.0f, 2.0f, 1.0f };

	// 移動速度
	static constexpr float kMoveSpeed = 0.2f;

	// =========================================================
	// Member Variables
	// =========================================================

	// オブジェクト
	std::unique_ptr<Cygnus::Object3D> object_;

	// コライダー
	std::unique_ptr<Cygnus::Collider> collider_;
};