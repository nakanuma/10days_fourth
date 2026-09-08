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

	//
	descriptionUI_ = std::make_unique<Cygnus::Sprite>();
	descriptionUI_->Initialize(spriteCommon,Cygnus::TextureManager::GetInstance().Load("descriptionUI.png"));
	descriptionUI_->SetAnchorPoint({ 0.5f, 0.5f });
	descriptionUI_->SetPosition({ float(Cygnus::Window::GetWidth()) / 2.0f ,50.0f});
	descriptionUI_->SetTextureSize(kDescriptionTextureSize_);
	descriptionUI_->SetSize(kDescriptionTextureSize_);
	descriptionUI_->SetTextureLeftTop(textureLT_);

	skipUI_ = std::make_unique<Cygnus::Sprite>();
	skipUI_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().Load("skipUI.png"));
	skipUI_->SetPosition(kSkipPosition_);
	skipUI_->SetSize(kSkipSize_);
}

void ExplainUI::Update(Player* player) {
	if (textureLT_.y >= kDescriptionTextureSize_.y * 4.0f) {
		// 自動で読み進めるタイマー
		autoReadTime_ += Cygnus::TimeManager::GetInstance()->GetDeltaTime();
	}

	ActionJudgment::GetInstance()->IsAction(isMovingTimeMax(player->IsMoving()), 1);//説明 動く
	ActionJudgment::GetInstance()->IsAction(player->IsRewinding(), 4);//説明 命綱を巻き、宇宙船に帰還する
		if (player->IsRewinding()) {
		partsInventoryUI_->StartConsuming();
	}
	ActionJudgment::GetInstance()->IsAction(autoReadTime_ >= kO2TimeExplainTime_, 5);//説明 自動で酸素がなくなる
	ActionJudgment::GetInstance()->IsAction(autoReadTime_ >= kMeteorExplainTime_, 6);//説明 隕石注意

	//各UIの更新
	partsInventoryUI_->Update();
	playerHPUI_->Update();
	playerOxygenUI_->Update();
	controlGuideUI_->Update();
	descriptionUI_->Update();
	skipUI_->Update();
	if (ActionJudgment::GetInstance()->first()) {
		NextUp();//次のステップに
		ActionJudgment::GetInstance()->Success();//手順成功
	}
}

void ExplainUI::Draw() {
	//各UIの描画
	partsInventoryUI_->Draw();
	playerHPUI_->Draw();
	playerOxygenUI_->Draw();
	controlGuideUI_->Draw();
	descriptionUI_->Draw();
	skipUI_->Draw();
}

bool ExplainUI::isMovingTimeMax(bool isMove) {
	if (isMove) {
		movingTime_ += Cygnus::TimeManager::GetInstance()->GetDeltaTime();
	}

	if (movingTime_ >= kMaxMoveTime_) {
		return true;
	}

	return false;
}


void ExplainUI::NextUp() {
	textureLT_.y += kDescriptionTextureSize_.y;
	descriptionUI_->SetTextureLeftTop(textureLT_);
}

ActionJudgment* ActionJudgment::GetInstance() {
	static ActionJudgment instance;
	return &instance;
}


