#include "O2TimeUI.h"
#include "TextureManager.h"

void O2TimeUI::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	// _‘f‚ ‚èó‘Ô
	o2Max_ = std::make_unique<Cygnus::Sprite>();
	o2Max_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().Load("o2Max.png"));
	o2Max_->SetPosition(kUIPosition_);

	// _‘f‚ª‹ó‚Á‚Ûó‘Ô
	o2Null_ = std::make_unique<Cygnus::Sprite>();
	o2Null_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().Load("o2Min.png"));
	o2Null_->SetPosition(kUIPosition_);
}

void O2TimeUI::Update(float nowTime, float maxTime) {
	
	// _‘fƒ[ƒ^‚Ìİ’è
	float downMeter = (kUIDefaultSize_ / maxTime) * (maxTime - nowTime);
	o2Null_->SetTextureSize({ kUIDefaultSize_,kUIDefaultSize_- downMeter });
	o2Null_->SetSize({kUIDefaultSize_, kUIDefaultSize_ - downMeter});

	o2Null_->Update();// _‘f‚ª‹ó‚Á‚Ûó‘Ô XV
	o2Max_->Update();// _‘f‚ ‚èó‘Ô XV
}

void O2TimeUI::Draw() {
	o2Max_->Draw();// _‘f‚ ‚èó‘Ô •`‰æ
	o2Null_->Draw();// _‘f‚ª‹ó‚Á‚Ûó‘Ô •`‰æ(™X‚ÉŒ¸‚é‚æ‚¤‚É‚·‚é‚½‚ß‚±‚¿‚ç‚ªŒã‚É•`‰æ)
}
