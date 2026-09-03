#include "PartsCountUI.h"
#include "SpriteCommon.h"

#include "TextureManager.h"

void PartsCountUI::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	spriteCountOneUI_ = std::make_unique<Cygnus::Sprite>();
	spriteCountOneUI_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().Load("Number.png"));
	spriteCountOneUI_->SetPosition({ kUISpace_ + kUISize_,kUISpace_ });
	spriteCountOneUI_->SetSize({ kUISize_,kUISize_ });
	spriteCountOneUI_->SetTextureSize({kUISize_,kUISize_});
	spriteCountOneUI_->SetTextureLeftTop({ 0,0 });

	spriteCountTenUI_ = std::make_unique<Cygnus::Sprite>();
	spriteCountTenUI_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().Load("Number.png"));
	spriteCountTenUI_->SetPosition({ kUISpace_,kUISpace_ });
	spriteCountTenUI_->SetSize({ kUISize_,kUISize_ });
	spriteCountTenUI_->SetTextureSize({ kUISize_,kUISize_ });

	spriteCountTenUI_->SetTextureLeftTop({ 0,0 });

}

void PartsCountUI::Update() {

	uint32_t nowPartsCount = partsCounter_;

	spriteCountOneUI_->Update();
	spriteCountTenUI_->Update();

	if (nowPartsCount == prevPartsCount_) return;
	prevPartsCount_ = nowPartsCount;

	if (nowPartsCount >= kTenPlace_) {
		uint32_t TenPlace = nowPartsCount / kTenPlace_;
		spriteCountTenUI_->SetTextureLeftTop({ float(TenPlace * kUISize_),0 });
		nowPartsCount = nowPartsCount % kTenPlace_;//‚ ‚Ü‚è(ˆê‚ÌˆÊ)
	}
	else {
		spriteCountTenUI_->SetTextureLeftTop({ 0,0 });
	}

	spriteCountOneUI_->SetTextureLeftTop({ float(nowPartsCount * kUISize_),0 });
}

void PartsCountUI::Draw() {
	spriteCountOneUI_->Draw();
	spriteCountTenUI_->Draw();
}

void PartsCountUI::AddParts() {
	partsCounter_++;
}

void PartsCountUI::ReleaseParts() {
	if (partsCounter_ <= 0) return;
	partsCounter_--;
}

