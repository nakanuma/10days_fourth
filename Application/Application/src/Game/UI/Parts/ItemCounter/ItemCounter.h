#pragma once

#include <Sprite.h>
#include <SpriteCommon.h>

// =========================================================
// プレイヤーの所持アイテム数表示UI
// =========================================================
class ItemCounter {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="spriteCommon">スプライト基盤機能</param>
	/// <param name="textureHandle">アイテムのテクスチャ</param>
	/// <param name="position">位置</param>
	void Initialize(Cygnus::SpriteCommon* spriteCommon, uint32_t textureHandle, const Cygnus::Float2 position);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="itemCount">アイテム数</param>
	/// /// <param name="itemCount">最大数に達したか</param>
	void Update(uint32_t itemCount, bool isMax);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// =========================================================
	// Constants
	// =========================================================
	const Cygnus::Float4 kColorWhite = {1.0f, 1.0f, 1.0f, 1.0f};
	const Cygnus::Float4 kColorRed = {1.0f, 0.0f, 0.0f, 1.0f};

	const Cygnus::Float2 kDigitSize = {64.0f, 64.0f};	// テクスチャの数字1つのサイズ（固定）
	const Cygnus::Float2 kDisplayDigitSize = {48.0f, 48.0f}; // 描画する数字1つのサイズ

	const Cygnus::Float2 kOffsetXSign = {64.0f, 32.0f};	// xマークのオフセット（アイコン基準）
	const Cygnus::Float2 kOffsetNumber10 = {92.0f, 16.0f}; // 10の位のオフセット（アイコン基準）

	const float kDigitSpacing = 36.0f;	// 数字と数字の横間隔

	// =========================================================
	// Member Variables
	// =========================================================
	Cygnus::Float2 initialPosition = {0.0f, 0.0f};	// 基準となる初期位置（最も左上）

	std::unique_ptr<Cygnus::Sprite> spriteIcon_; // アイテムのアイコン用スプライト
	std::unique_ptr<Cygnus::Sprite> spriteXSign_;	// 「x」マーク用スプライト

	std::vector<std::unique_ptr<Cygnus::Sprite>> digitSprites_;
	std::vector<uint32_t> digitValues_;
};
