#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// 前方宣言
class Player;

// =========================================================
// プレイヤーの残り酸素UI
// =========================================================
class PlayerOxygenUI
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 更新処理
	/// </summary>
	void Initialize(Cygnus::SpriteCommon* spriteCommon, const Player* player);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// 点滅処理
	/// </summary>
	void UpdateBlink();

private:
	// =========================================================
	// Constants
	// =========================================================

	// バーのサイズ（横10, 縦40）
	static constexpr Cygnus::Float2 kBarSize = { 10.0f, 50.0f };

	// 上中央基準のアンカーポイント（上から下へ縮小させる軸）
	static constexpr Cygnus::Float2 kAnchorPointBottomCenter = { 0.5f, 1.0f };

	// プレイヤー右側のワールド座標オフセット
	static constexpr Cygnus::Float3 kWorldOffset = { 2.0f, 0.0f, 0.0f };

	// バーの色設定
	static constexpr Cygnus::Float4 kColorBackground = { 0.2f, 0.2f, 0.2f, 0.8f }; // 暗い灰色（背景）
	static constexpr Cygnus::Float4 kColorForeground = { 0.0f, 0.8f, 1.0f, 1.0f }; // 水色（酸素）

private:
	// =========================================================
	// Member Variables
	// =========================================================

	const Player* player_ = nullptr;

	std::unique_ptr<Cygnus::Sprite> spriteBackground_; // 背景（灰色）
	std::unique_ptr<Cygnus::Sprite> spriteForeground_; // 前景（水色）

	//点滅用タイマー
	static constexpr float kBlinkInterval_ = 0.5f; // 点滅間隔（秒）
	float blinkTimer_ = 0.0f;
};

