#pragma once

// Engine
#include <SpriteCommon.h>

// Application
#include <src/Game/UI/PartsUI/PartsInventoryUI.h> // プレイヤーのパーツ所持数UI
#include <src/Game/UI/Player/PlayerHPUI.h> // プレイヤーのHPゲージUI
#include <src/Game/UI/Player/PlayerOxygenUI.h> // プレイヤーの残り酸素UI
#include <src/Game/UI/Spaceship/SpaceshipDurabilityUI.h> // 宇宙船の耐久度UI
 
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
	void Initialize(Cygnus::SpriteCommon* spriteCommon, const Player* player, const Spaceship* spaceship);

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
	std::unique_ptr<PlayerHPUI> playerHPUI_; // プレイヤーのHPバーUI
	std::unique_ptr<PlayerOxygenUI> playerOxygenUI_; // プレイヤーの残り酸素UI
	std::unique_ptr<SpaceshipDurabilityUI> spaceshipDurabilityUI_; // 宇宙船の耐久度UI
};

