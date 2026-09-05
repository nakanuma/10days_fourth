#pragma once

// C++
#include <vector>

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// =========================================================
// ゲームの制限時間表示UI
// =========================================================
class GameTimerUI
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 更新処理
	/// </summary>
	void Initialize(Cygnus::SpriteCommon* spriteCommon);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(float remainingTime);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// 数字スプライトのUV切り出し設定
	/// </summary>
	/// <param name="sprite"></param>
	/// <param name="digit"></param>
	void SetDigitValue(Cygnus::Sprite* sprite, int32_t digit);

private:
	// =========================================================
	// Constants
	// =========================================================

	// アンカーポイント
	static constexpr Cygnus::Float2 kAnchorPointCenter = { 0.5f, 0.5f };

	// 表示位置とレイアウト設定
	static constexpr float kYPos = 40.0f;             // 数字群のY軸座標
	static constexpr Cygnus::Float2 kClockIconPos = { 565.0f, kYPos }; // 時計アイコン
	static constexpr float kOffsetHundredsX = 600.0f; // 百の位
	static constexpr float kOffsetTensX = 623.0f; // 十の位
	static constexpr float kOffsetOnesX = 646.0f; // 一の位
	static constexpr float kOffsetDotX = 663.0f; // ドット
	static constexpr float kOffsetDecimalX = 680.0f; // 小数第一位

	// 各パーツの表示サイズ
	static constexpr Cygnus::Float2 kClockIconSize = { 40.0f, 40.0f };
	static constexpr Cygnus::Float2 kDigitDisplaySize = { 32.0f, 32.0f }; // 数字の描画サイズ
	static constexpr Cygnus::Float2 kDotDisplaySize = { 32.0f, 32.0f }; // ドットの描画サイズ

	// テクスチャ設定
	static constexpr Cygnus::Float2 kDigitTextureSize = { 64.0f, 64.0f }; // テクスチャ内1文字のピクセルサイズ

private:
	// =========================================================
	// Member Variables
	// =========================================================

	// テクスチャハンドル
	uint32_t texNumbers_ = 0;

	// スプライト
	std::unique_ptr<Cygnus::Sprite> spriteClockIcon_;
	std::unique_ptr<Cygnus::Sprite> spriteDigitHundreds_; // 百の位
	std::unique_ptr<Cygnus::Sprite> spriteDigitTens_;     // 十の位
	std::unique_ptr<Cygnus::Sprite> spriteDigitOnes_;     // 一の位
	std::unique_ptr<Cygnus::Sprite> spriteDot_;           // ドット「.」
	std::unique_ptr<Cygnus::Sprite> spriteDigitDecimal_;  // 小数第一位

	// キャッシュ用（直前の数値と変わった場合のみ更新）
	int32_t currentDisplayValue_ = -1;
};

