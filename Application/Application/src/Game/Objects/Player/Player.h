#pragma once

// Engine
#include <Object3D.h>
#include <Collider/Collider.h>

// =========================================================
// プレイヤークラス
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

	/// <summary>
	/// 無入力時の漂うオフセットを取得
	/// </summary>
	Cygnus::Float3 Drift();

private:
	// =========================================================
	// Constants
	// =========================================================

	// コライダーの大きさ
	static constexpr Cygnus::Float3 kColliderSize = { 1.0f, 2.0f, 1.0f };

	// 宇宙空間移動用のパラメーター
	static constexpr float kAcceleration = 0.01f; // 加速度（入力時の増分）
	static constexpr float kMaxSpeed = 0.25f; // 最高速度
	static constexpr float kDamping = 0.985f; // 減衰率（1.0に近いほど止まりにくくなる）

	// 漂いパラメーター
	static constexpr float kDriftFrequency = 3.0f; // 漂う周期の速さ
	static constexpr float kDriftAmplitude = 0.01f; // 漂う力の強さ

	// =========================================================
	// Member Variables
	// =========================================================

	// オブジェクト
	std::unique_ptr<Cygnus::Object3D> object_;

	// コライダー
	std::unique_ptr<Cygnus::Collider> collider_;

	// 速度ベクトル
	Cygnus::Float3 velocity_ = {0.0f, 0.0f, 0.0f};

	// 漂い計算用タイマー
	float driftTimer_ = 0.0f;
};