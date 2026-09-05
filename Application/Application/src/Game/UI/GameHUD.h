#pragma once

// Engine
#include <SpriteCommon.h>

// Application
#include <src/Game/UI/PartsUI/PartsInventoryUI.h> // プレイヤーのパーツ所持数UI

// =========================================================
// ゲーム中の全UIマネージャー
// =========================================================
class GameHUD
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
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
	// Member Variables
	// =========================================================

	/* 各UI */
	std::unique_ptr<PartsInventoryUI> partsInventoryUI_; // プレイヤーのパーツ所持数UI
};

