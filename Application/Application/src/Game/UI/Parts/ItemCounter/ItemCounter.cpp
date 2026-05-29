#define NOMINMAX
#include "ItemCounter.h"

// C++
#include <algorithm>

// Engine
#include <TextureManager.h>

void ItemCounter::Initialize(Cygnus::SpriteCommon* spriteCommon, uint32_t textureHandle, const Cygnus::Float2 position) {
	initialPosition = position;
	
	// アイコン生成+初期化
	spriteIcon_ = std::make_unique<Cygnus::Sprite>();
	spriteIcon_->Initialize(spriteCommon, textureHandle);
	spriteIcon_->SetPosition(initialPosition);

	// xマーク生成+初期化
	spriteXSign_ = std::make_unique<Cygnus::Sprite>();
	spriteXSign_->Initialize(spriteCommon, Cygnus::TextureManager::Load("UI/multiply.png"));
	spriteXSign_->SetPosition(initialPosition + kOffsetXSign);

	// 数字スプライト生成+初期化
	digitSprites_.resize(2);
	digitValues_.resize(2, 0);
	for (uint32_t i = 0; i < 2; i++) {
		digitSprites_[i] = std::make_unique<Cygnus::Sprite>();
		digitSprites_[i]->Initialize(spriteCommon, Cygnus::TextureManager::Load("UI/numbers.png"));
		digitSprites_[i]->SetSize(kDisplayDigitSize); 
		digitSprites_[i]->SetTextureSize(kDigitSize);

		if (i == 0) {
			// 10の位の位置を固定
			digitSprites_[i]->SetPosition(initialPosition + kOffsetNumber10);
		} else {
			// 1の位の位置を固定
			digitSprites_[i]->SetPosition(initialPosition + kOffsetNumber10 + Cygnus::Float2{kDigitSpacing, 0.0f});
		}
	}
}

void ItemCounter::Update(uint32_t itemCount, bool isMax) {
	// アイコン更新
	spriteIcon_->Update();

	// xマーク更新
	spriteXSign_->Update();

	// 個数が99を超えたらカンストさせる
	uint32_t displayCount = std::min(itemCount, 99u);
	// 桁の分解
	digitValues_[0] = displayCount / 10; // 10の位
	digitValues_[1] = displayCount % 10; // 1の位
	// 最大数に達していたら赤にする
	Cygnus::Float4 color = isMax ? kColorRed : kColorWhite;
	// 数字のUV切り出しと更新
	for (uint32_t i = 0; i < 2; i++) {
		// 現在の数値に応じたテクスチャ位置を設定
		digitSprites_[i]->SetTextureLeftTop({digitValues_[i] * kDigitSize.x, 0.0f});
		digitSprites_[i]->SetColor(color);
		digitSprites_[i]->Update();
	}
}

void ItemCounter::Draw() {
	// アイコン描画
	spriteIcon_->Draw();

	// xマーク描画
	spriteXSign_->Draw();

	// 数字2桁の描画
	for (auto& digit : digitSprites_) {
		digit->Draw();
	}
}