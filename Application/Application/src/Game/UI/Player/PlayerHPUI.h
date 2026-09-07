#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// 前方宣言
class Player;

// =========================================================
// プレイヤーのHP表示UI
// =========================================================
class PlayerHPUI
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

private:
	// =========================================================
	// Constants
	// =========================================================

	// バーの全幅・高さ
	static constexpr Cygnus::Float2 kBarSize = {40.0f, 10.0f};

	// 左端基準のアンカーポイント
	static constexpr Cygnus::Float2 kAnchorPointLeftCenter = {0.0f, 0.5f};

	// プレイヤーからの相対オフセット
	static constexpr Cygnus::Float3 kWorldOffset = {0.0f, 4.0f, 0.0f};

	// バーの色設定
	static constexpr Cygnus::Float4 kColorBackground = {0.3f, 0.3f, 0.3f, 1.0f};
	static constexpr Cygnus::Float4 kColorForeground = {1.0f, 0.0f, 0.0f, 1.0f};

private:
	// =========================================================
	// Member Variables
	// =========================================================

	const Player* player_ = nullptr;

	std::unique_ptr<Cygnus::Sprite> spriteBackground_; // 背景（灰色）
	std::unique_ptr<Cygnus::Sprite> spriteForeground_; // 前景（赤色）
};