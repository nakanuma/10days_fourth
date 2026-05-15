#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// =========================================================
// 操作表示UIクラス
// =========================================================
class HUD {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
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
	// Constants
	// =========================================================

	Cygnus::Float2 kAnchorPoint = {0.5f, 0.5f};	// 中心アンカーポイント

	Cygnus::Float2 kOreIconInitialPosition = {50.0f, 50.0f};	// 鉱石アイコン初期位置
	Cygnus::Float2 kGearIconInitialPosition = {50.0f, 120.0f};	// 歯車アイコン初期位置

	// =========================================================
	// Member Variables
	// =========================================================

	std::unique_ptr<Cygnus::Sprite> spriteOreIcon_;	// 鉱石アイコン
	std::unique_ptr<Cygnus::Sprite> spriteGearIcon_; // 歯車アイコン
};