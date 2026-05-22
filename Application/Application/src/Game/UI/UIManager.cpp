#include "UIManager.h"

// Engine
#include <TextureManager.h>

UIManager* UIManager::GetInstance() {
	static UIManager instance;
	return &instance;
}

void UIManager::Initialize() {
	// スプライト基盤機能生成
	spriteCommon_ = std::make_unique<Cygnus::SpriteCommon>();
	spriteCommon_->Initialize(Cygnus::DirectXBase::GetInstance());


	// プレイヤー鉱石所持数UI生成 + 初期化
	oreCounter_ = std::make_unique<ItemCounter>();
	oreCounter_->Initialize(spriteCommon_.get(), Cygnus::TextureManager::Load("UI/oreIcon.png"), kOreCounterInitialPosition);

	// プレイヤー歯車所持数UI生成 + 初期化
	gearCounter_ = std::make_unique<ItemCounter>();
	gearCounter_->Initialize(spriteCommon_.get(), Cygnus::TextureManager::Load("UI/gearIcon.png"), kGearCounterInitialPosition);


	// インタラクトUIを生成
	interactGuides_[InteractGuide::ActionType::Insert] = std::make_unique<InteractGuide>();
	interactGuides_[InteractGuide::ActionType::Insert]->Initialize(spriteCommon_.get())
}

void UIManager::Update(uint32_t oreCount, bool isOreMax, uint32_t gearCount, bool isGearMax) {
	// プレイヤー鉱石所持数UI更新
	oreCounter_->Update(oreCount, isOreMax);
	// プレイヤー歯車所持数UI更新
	gearCounter_->Update(gearCount, isGearMax);
}

void UIManager::Draw() {
	// プレイヤー鉱石所持数UI描画
	oreCounter_->Draw();
	// プレイヤー鉱石所持数UI描画
	gearCounter_->Draw();
}

void UIManager::RequestInteract(InteractGuide::ActionType type) {

}
