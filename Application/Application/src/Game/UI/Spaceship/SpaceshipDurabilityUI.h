#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// 前方宣言
#include <src/Game/Objects/Spaceship/Spaceship.h>

// =========================================================
// 宇宙船の耐久度ゲージUI
// =========================================================
class SpaceshipDurabilityUI
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Cygnus::SpriteCommon* spriteCommon, const Spaceship* spaceship);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// 宇宙船の位置を取得
	/// </summary>
	/// <returns></returns>
	const Cygnus::Float3& GetSpaceshipTranslate() const { return spaceship_->GetTranslate(); }

private:
	// =========================================================
	// Internal Methods
	// =========================================================

private:
	// =========================================================
	// Constants
	// =========================================================

	// バーのサイズ（横100, 縦15）
	static constexpr Cygnus::Float2 kBarSize = { 200.0f, 25.0f };

	// 左中央基準のアンカーポイント（左端を固定して右へ伸ばす）
	static constexpr Cygnus::Float2 kAnchorPointLeftCenter = { 0.0f, 0.5f };

	// 宇宙船オブジェクトの頭上（ワールド座標）オフセット
	static constexpr Cygnus::Float3 kWorldOffset = { 0.0f, 6.0f, 0.0f };

	// バーの色設定
	static constexpr Cygnus::Float4 kColorBackground = { 0.2f, 0.2f, 0.2f, 0.8f }; // 暗い灰色（背景）
	static constexpr Cygnus::Float4 kColorPrediction = {0.2f, 0.9f, 0.3f, 0.4f}; // 薄い緑色（回復予測）
	static constexpr Cygnus::Float4 kColorForeground = { 0.2f, 0.9f, 0.3f, 1.0f }; // 黄緑色（修理耐久度）

	static constexpr float kGaugeCatchupSpeed = 0.2f; // アニメーション追従スピード（小さくするほどゆっくり）

private:
	// =========================================================
	// Member Variables
	// =========================================================

	const Spaceship* spaceship_ = nullptr;

	std::unique_ptr<Cygnus::Sprite> spriteBackground_; // 背景
	std::unique_ptr<Cygnus::Sprite> spritePrediction_; // 回復予測
	std::unique_ptr<Cygnus::Sprite> spriteForeground_; // 前景

	float displayDurability_ = 0.0f; // アニメーション表示用の実耐久度数値
};

