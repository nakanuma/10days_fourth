#include "GameHUD.h"

void GameHUD::Initialize(Cygnus::SpriteCommon* spriteCommon, const Player* player, const Spaceship* spaceship) {
	/* 各UI生成+初期化 */
	
	// プレイヤーのパーツ所持数UI
	partsInventoryUI_ = std::make_unique<PartsInventoryUI>();
	partsInventoryUI_->Initialize(spriteCommon, player);

	// プレイヤーのHPバーUI
	playerHPUI_ = std::make_unique<PlayerHPUI>();
	playerHPUI_->Initialize(spriteCommon, player);

	// プレイヤーの残り酸素UI
	playerOxygenUI_ = std::make_unique<PlayerOxygenUI>();
	playerOxygenUI_->Initialize(spriteCommon, player);

	// 宇宙船の耐久度UI
	spaceshipDurabilityUI_= std::make_unique<SpaceshipDurabilityUI>();
	spaceshipDurabilityUI_->Initialize(spriteCommon, spaceship);

	// 操作UI
	controlGuideUI_ = std::make_unique<ControlGuideUI>();
	controlGuideUI_->Initialize(spriteCommon);

	// 残り時間UI
	gameTimerUI_ = std::make_unique<GameTimerUI>();
	gameTimerUI_->Initialize(spriteCommon);
}

void GameHUD::Update(float remainingTime) {
	partsInventoryUI_->Update();
	playerHPUI_->Update();
	playerOxygenUI_->Update();
	spaceshipDurabilityUI_->Update();
	controlGuideUI_->Update();
	gameTimerUI_->Update(remainingTime);
}

void GameHUD::Draw() {
	partsInventoryUI_->Draw();
	playerHPUI_->Draw();
	playerOxygenUI_->Draw();
	spaceshipDurabilityUI_->Draw();
	controlGuideUI_->Draw();
	gameTimerUI_->Draw();
}