#include "PartsCountUI.h"
#include "SpriteCommon.h"

#include "TextureManager.h"

void PartsCountUI::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	// 一の位の数字 初期化処理
	spriteCountOneUI_ = std::make_unique<Cygnus::Sprite>();
	spriteCountOneUI_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().Load("Number.png"));
	spriteCountOneUI_->SetPosition({ kUISpace_ + kUISize_,kUISpace_ });
	spriteCountOneUI_->SetSize({ kUISize_,kUISize_ });
	spriteCountOneUI_->SetTextureSize({kUISize_,kUISize_});
	spriteCountOneUI_->SetTextureLeftTop({ 0,0 });

	// 十の位の数字 初期化処理
	spriteCountTenUI_ = std::make_unique<Cygnus::Sprite>();
	spriteCountTenUI_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().Load("Number.png"));
	spriteCountTenUI_->SetPosition({ kUISpace_,kUISpace_ });
	spriteCountTenUI_->SetSize({ kUISize_,kUISize_ });
	spriteCountTenUI_->SetTextureSize({ kUISize_,kUISize_ });
	spriteCountTenUI_->SetTextureLeftTop({ 0,0 });

}

void PartsCountUI::Update() {

	uint32_t nowPartsCount = partsCounter_;

	spriteCountOneUI_->Update();// 一の位 更新
	spriteCountTenUI_->Update();// 十の位 更新

	// 前回の数と同じなら通さない
	if (nowPartsCount == prevPartsCount_) return;
	// 現在の数字が前回の数字となる
	prevPartsCount_ = nowPartsCount;

	if (nowPartsCount >= kTenPlace_) {
		uint32_t TenPlace = nowPartsCount / kTenPlace_;
		spriteCountTenUI_->SetTextureLeftTop({ float(TenPlace * kUISize_),0 });
		nowPartsCount = nowPartsCount % kTenPlace_;//あまり(一の位にする)
	}
	else {
		spriteCountTenUI_->SetTextureLeftTop({ 0,0 });//十の位「0」
	}

	spriteCountOneUI_->SetTextureLeftTop({ float(nowPartsCount * kUISize_),0 });
}

void PartsCountUI::Draw() {
	spriteCountOneUI_->Draw();// 一の位 描画
	spriteCountTenUI_->Draw();// 十の位 描画
}

void PartsCountUI::AddParts() {
	partsCounter_++;// 加算
}

void PartsCountUI::ReleaseParts() {
	if (partsCounter_ <= 0) return;
	partsCounter_--;
}

