#include "O2TimeUI.h"
#include "TextureManager.h"
#include "TimeManager.h"
#include <algorithm>

void O2TimeUI::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	o2Meter_ = std::make_unique<Cygnus::Sprite>();
	o2Meter_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().Load("o2Max.png"));
	o2Meter_->SetPosition({20.0f,500.0f });

	o2Null_ = std::make_unique<Cygnus::Sprite>();
	o2Null_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().Load("o2Min.png"));
	o2Null_->SetPosition({ 20.0f,500.0f });
}

void O2TimeUI::Update(float nowTime, float maxTime) {
	
	float downMeter = (kUIDefaultSize_ / maxTime) * (maxTime - nowTime);

	o2Null_->SetTextureSize({ kUIDefaultSize_,kUIDefaultSize_- downMeter });
	o2Null_->SetSize({kUIDefaultSize_, kUIDefaultSize_ - downMeter});

	o2Null_->Update();
	o2Meter_->Update();
}

void O2TimeUI::Draw() {
	o2Meter_->Draw();
	o2Null_->Draw();
}
