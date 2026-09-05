#include "GameHUD.h"

void GameHUD::Initialize(Cygnus::SpriteCommon* spriteCommon, const Player* player) {
	/* 各UI生成+初期化 */
	
	// プレイヤーのパーツ所持数UI
	partsInventoryUI_ = std::make_unique<PartsInventoryUI>();
	partsInventoryUI_->Initialize(spriteCommon, player);

	// プレイヤーのHPバーUI
	playerHPUI_ = std::make_unique<PlayerHPUI>();
	playerHPUI_->Initialize(spriteCommon, player);
}

void GameHUD::Update() {
	partsInventoryUI_->Update();
	playerHPUI_->Update();
}

void GameHUD::Draw() {
	partsInventoryUI_->Draw();
	playerHPUI_->Draw();
}