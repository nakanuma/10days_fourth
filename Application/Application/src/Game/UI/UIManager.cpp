#include "UIManager.h"

UIManager* UIManager::GetInstance() {
	static UIManager instance;
	return &instance;
}

void UIManager::Initialize() {
	// スプライト基盤機能生成
	spriteCommon_ = std::make_unique<Cygnus::SpriteCommon>();
	spriteCommon_->Initialize(Cygnus::DirectXBase::GetInstance());

	// 常に表示されるUIの生成+初期化
	hud_ = std::make_unique<HUD>();
	hud_->Initialize(spriteCommon_.get());

	// 入力操作UIの生成+初期化
	interctionUI_ = std::make_unique<InterctionUI>();
	interctionUI_->Initialize(spriteCommon_.get());
}

void UIManager::Update() {
	// 常に表示されるUI更新
	hud_->Update();
	// 入力操作UI更新
	interctionUI_->Update();
}

void UIManager::Draw() {
	// 常に表示されるUI描画
	hud_->Draw();
	// 入力操作UI描画
	interctionUI_->Draw();
}