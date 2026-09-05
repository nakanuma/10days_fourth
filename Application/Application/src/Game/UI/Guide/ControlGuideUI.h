#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// =========================================================
// プレイヤーの入力操作UI
// =========================================================
class ControlGuideUI
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

	// アイコンとテキストのデフォルト表示サイズ
	static constexpr Cygnus::Float2 kButtonSize = { 48.0f, 48.0f }; // ボタンアイコンサイズ
	static constexpr Cygnus::Float2 kTextSize = { 80.0f, 40.0f };   // テキストデフォルトサイズ

	// 左上（ポーズUI）の標準座標と間隔
	static constexpr Cygnus::Float2 kPauseGroupPos = { 40.0f, 40.0f };
	static constexpr float kPauseTextOffset = 70.0f; // メニューボタンからの横オフセット

	// 右下（操作ガイドUI）の標準座標と間隔
	static constexpr Cygnus::Float2 kRightBottomGroupPos = { 950.0f, 680.0f };
	static constexpr float kMarginX = 16.0f; // 要素ごとの横余白

private:
	// =========================================================
	// Member Variables
	// =========================================================

	// 左上 UI
	std::unique_ptr<Cygnus::Sprite> spriteMenuButton_;
	std::unique_ptr<Cygnus::Sprite> spritePauseText_;

	// 右下 UI
	std::unique_ptr<Cygnus::Sprite> spriteStick_;
	std::unique_ptr<Cygnus::Sprite> spriteMoveText_;
	std::unique_ptr<Cygnus::Sprite> spriteRb_;
	std::unique_ptr<Cygnus::Sprite> spriteReturnText_;
};

