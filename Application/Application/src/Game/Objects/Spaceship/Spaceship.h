#pragma once

// Engine
#include <Object3D.h>
#include <Collider/Collider.h>

// =========================================================
// 宇宙船クラス
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
	// Internal Methods
	// =========================================================

	/// <summary>
	/// 無入力時の漂うオフセットを取得
	/// </summary>
	void Drift();

private:
	// =========================================================
	// Constants
	// =========================================================

	// コライダーの大きさ
	static constexpr Cygnus::Float3 kColliderSize = { 6.0f, 3.0f, 3.0f };

	// 漂いパラメーター
	static constexpr float kDriftFrequency = 1.0f;  // 漂う周期の速さ
	static constexpr float kDriftAmplitudeX = 0.5f; // X方向の揺れ幅
	static constexpr float kDriftAmplitudeY = 0.3f; // Y軸方向の揺れ幅

	// =========================================================
	// Member Variables
	// =========================================================

	// オブジェクト
	std::unique_ptr<Cygnus::Object3D> object_;

	// コライダー
	std::unique_ptr<Cygnus::Collider> collider_;

	// 基準座標（初期位置）
	Cygnus::Float3 basePosition_ = {0.0f, 0.0f, 0.0f};

	// 漂い計算用タイマー
	float driftTimer_ = 0.0f;
};

