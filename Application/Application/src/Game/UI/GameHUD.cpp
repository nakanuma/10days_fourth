#include "GameHUD.h"

void GameHUD::Initialize(Cygnus::SpriteCommon* spriteCommon, const Player* player) {
	/* 各UI生成+初期化 */
	
	// プレイヤーのパーツ所持数UI
	partsInventoryUI_ = std::make_unique<PartsInventoryUI>();
	partsInventoryUI_->Initialize(spriteCommon, player);
}

void GameHUD::Update() {
	// プレイヤーのパーツ所持数UI更新
	partsInventoryUI_->Update();
}

void GameHUD::Draw() {
	// プレイヤーのパーツ所持数UI描画
	partsInventoryUI_->Draw();
}