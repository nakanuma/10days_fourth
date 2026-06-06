#include "UIManager.h"

// Engine
#include <TextureManager.h>
#include <Math/MathUtil.h>

UIManager* UIManager::GetInstance() {
	static UIManager instance;
	return &instance;
}

void UIManager::Initialize() {
	// スプライト基盤機能生成
	spriteCommon_ = std::make_unique<Cygnus::SpriteCommon>();
	spriteCommon_->Initialize(Cygnus::DirectXBase::GetInstance());

	/* プレイヤーのアイテム所持数UI */

	// プレイヤー鉱石所持数UI生成 + 初期化
	oreCounter_ = std::make_unique<ItemCounter>();
	oreCounter_->Initialize(spriteCommon_.get(), Cygnus::TextureManager::Load("UI/oreIcon.png"), kOreCounterInitialPosition);

	// プレイヤー歯車所持数UI生成 + 初期化
	gearCounter_ = std::make_unique<ItemCounter>();
	gearCounter_->Initialize(spriteCommon_.get(), Cygnus::TextureManager::Load("UI/gearIcon.png"), kGearCounterInitialPosition);

	/* 操作ボタン表示UI */

	// インタラクトUIを生成
	interactGuides_[InteractGuide::ActionType::Insert] = std::make_unique<InteractGuide>();
	interactGuides_[InteractGuide::ActionType::Insert]->Initialize(spriteCommon_.get(), InteractGuide::ActionType::Insert);

	interactGuides_[InteractGuide::ActionType::Craft] = std::make_unique<InteractGuide>();
	interactGuides_[InteractGuide::ActionType::Craft]->Initialize(spriteCommon_.get(), InteractGuide::ActionType::Craft);

	interactGuides_[InteractGuide::ActionType::Mine] = std::make_unique<InteractGuide>();
	interactGuides_[InteractGuide::ActionType::Mine]->Initialize(spriteCommon_.get(), InteractGuide::ActionType::Mine);

	/* 列車 */

	// 列車の稼働時間UIを生成
	gearTimeBar_ = std::make_unique<GearTimerBar>();
	gearTimeBar_->Initialize(spriteCommon_.get());
}

void UIManager::Update(uint32_t oreCount, bool isOreMax, uint32_t gearCount, bool isGearMax, const Cygnus::Float3 playerPos, const Cygnus::Float3 carrierPos, float carrierEneryRatio) {
	/* プレイヤーUI */

	// プレイヤー鉱石所持数UI更新
	oreCounter_->Update(oreCount, isOreMax);
	// プレイヤー歯車所持数UI更新
	gearCounter_->Update(gearCount, isGearMax);

	/* 操作ボタンUI */

	// インタラクトUI更新
	for (auto& pair : interactGuides_) {
		pair.second->Update(Cygnus::MathUtil::WorldToScreen(playerPos)); // スクリーン座標に変換して渡す
	}

	/* 列車UI */

	// 列車残り稼働時間UI更新
	gearTimeBar_->Update(Cygnus::MathUtil::WorldToScreen(carrierPos), carrierEneryRatio);
}

void UIManager::Draw() {
	/* プレイヤーUI */

	// プレイヤー鉱石所持数UI描画
	oreCounter_->Draw();
	// プレイヤー鉱石所持数UI描画
	gearCounter_->Draw();

	/* 操作ボタンUI */

	// インタラクトUI描画
	for (auto& pair : interactGuides_) {
		pair.second->Draw();
	}

	/* 列車UI */

	// 列車残り稼働時間UI描画
	gearTimeBar_->Draw();
}

void UIManager::ClearInteractRequests() {
	for (auto& pair : interactGuides_) {
		pair.second->SetActive(false);
	}
}

void UIManager::RequestInteract(InteractGuide::ActionType type) {
	// リクエストのあった種類だけ有効化
	if (interactGuides_.count(type)) {
		interactGuides_[type]->SetActive(true);
	}
}
