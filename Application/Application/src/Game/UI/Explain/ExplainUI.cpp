#include "ExplainUI.h"
#include <TextureManager.h>
#include "Input.h"
#include "TimeManager.h"

void ExplainUI::Initialize(Cygnus::SpriteCommon* spriteCommon, Player* player) {
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

	// 操作UI
	controlGuideUI_ = std::make_unique<ControlGuideUI>();
	controlGuideUI_->Initialize(spriteCommon);

	position_ = { float(Cygnus::Window::GetWidth()) / 2.0f ,50.0f};

	//
	descriptionUI_ = std::make_unique<Cygnus::Sprite>();
	descriptionUI_->Initialize(spriteCommon,Cygnus::TextureManager::GetInstance().Load("descriptionUI.png"));
	descriptionUI_->SetAnchorPoint({ 0.5f, 0.5f });
	descriptionUI_->SetPosition(position_);
	descriptionUI_->SetTextureSize(kDescriptionTextureSize_);
	descriptionUI_->SetSize(kDescriptionTextureSize_);
	descriptionUI_->SetTextureLeftTop(textureLT_);


}

void ExplainUI::Update() {
	Cygnus::Input* input = Cygnus::Input::GetInstance();


	if (textureLT_.y >= kDescriptionTextureSize_.y * 4.0f) {
		time_ += Cygnus::TimeManager::GetInstance()->GetDeltaTime();
	}

	ActionJudgment::GetInstance()->AddAction(input->TriggerKey(DIK_W) || input->TriggerKey(DIK_A) ||
		input->TriggerKey(DIK_S) || input->TriggerKey(DIK_D), 1);

	ActionJudgment::GetInstance()->AddAction(Cygnus::Input::GetInstance()->TriggerKey(DIK_SPACE), 4);

	ActionJudgment::GetInstance()->AddAction(time_ >= kO2TimeExplainTime_, 5);
	ActionJudgment::GetInstance()->AddAction(time_ >= kMeteorExplainTime_, 6);

	partsInventoryUI_->Update();
	playerHPUI_->Update();
	playerOxygenUI_->Update();
	controlGuideUI_->Update();
	descriptionUI_->Update();

	if (ActionJudgment::GetInstance()->first()) {
		NextUp();
		ActionJudgment::GetInstance()->Success();
	}
}

void ExplainUI::Draw() {
	
	partsInventoryUI_->Draw();
	
	playerHPUI_->Draw();
	
	playerOxygenUI_->Draw();

	controlGuideUI_->Draw();

	descriptionUI_->Draw();
}


void ExplainUI::NextUp() {
	textureLT_.y += kDescriptionTextureSize_.y;
	descriptionUI_->SetTextureLeftTop(textureLT_);
}

ActionJudgment* ActionJudgment::GetInstance() {
	static ActionJudgment instance;
	return &instance;
}


