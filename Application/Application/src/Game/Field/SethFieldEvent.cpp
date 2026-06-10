#include "SethFieldEvent.h"
#include <TextureManager.h>
#include <src/Game/Sandstrom/SandstormManager.h>

void SethFieldEvent::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	//スプライト作成
	effect_ = std::make_unique<Cygnus::Sprite>();
	effect_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().GetSRVIndex("resources/Images/white.png"));
	effect_->SetPosition({ 0.0f,0.0f });
	effect_->SetSize({ 32.0f,32.0f });

	eventTimer_ = kEventStartTime_;//タイムを設定
	
	//砂嵐を召喚
	SandstormManager::GetInstance()->AddSandstrom(summonPoint_);
}

void SethFieldEvent::Update() {

	summonTimer_ += 1.0f / 60.0f;
	if (summonTimer_ >= kEventStartTime_ / kSummonFrequency_) {
		//砂嵐を召喚
		SandstormManager::GetInstance()->AddSandstrom(summonPoint_);
		summonTimer_ = 0.0f;
	}

	effect_->Update();
}

void SethFieldEvent::Draw() {
	effect_->Draw();
}

void SethFieldEvent::Debug() {
#ifdef USE_IMGUI
	ImGui::Text("Seth");
#endif // USE_IMGUI
}

