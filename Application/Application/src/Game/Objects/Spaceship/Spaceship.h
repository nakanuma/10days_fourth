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

	/// <summary>
	/// 修理処理（パーツを受け取って耐久力を回復）
	/// </summary>
	/// <param name="Low"></param>
	/// <param name="medium"></param>
	/// <param name="high"></param>
	void Repair(int32_t low, int32_t medium, int32_t high);

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// 現在位置の取得
	/// </summary>
	/// <returns></returns>
	const Cygnus::Float3& GetTranslate() const { return object_->transform_.translate_; }

	/// <summary>
	/// 現在の耐久力取得
	/// </summary>
	/// <returns></returns>
	float GetDurability() const { return durability_; }

	/// <summary>
	/// 最大耐久力取得
	/// </summary>
	/// <returns></returns>
	float GetMaxDurability() const { return kMaxDurability; }

	/// <summary>
	/// 修理完了（クリア）判定
	/// </summary>
	/// <returns></returns>
	bool IsFullyRepaired() const { return durability_ >= kMaxDurability; }

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

	// 耐久力パラメーター
	static constexpr float kMaxDurability = 150.0f; // 最大耐久力

	// 各パーツの基礎回復量
	static constexpr float kRepairPointLow = 2.0f;
	static constexpr float kRepairPointMedium = 4.0f;
	static constexpr float kRepairPointHigh = 6.0f;

	// まとめ持ちボーナス倍率（1個増えるごとの加算倍率）
	static constexpr float kBonusMultiplierPerItem = 0.1f; // 例: 5個持って帰ると +40％（1.4倍）

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

	// 耐久力（最初は0スタート）
	float durability_ = 0.0f;
};

