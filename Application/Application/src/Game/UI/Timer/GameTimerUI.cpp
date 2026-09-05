#include "GameTimerUI.h"
#define NOMINMAX

// C++
#include <algorithm>

// Engine
#include <TextureManager.h>

void GameTimerUI::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	// テクスチャ読み込み
	uint32_t texClock = Cygnus::TextureManager::Load("icon_clock.png");
	uint32_t texDot = Cygnus::TextureManager::Load("dot.png");
	texNumbers_ = Cygnus::TextureManager::Load("numbers.png");

	// 時計アイコン生成
	spriteClockIcon_ = std::make_unique<Cygnus::Sprite>();
	spriteClockIcon_->Initialize(spriteCommon, texClock);
	spriteClockIcon_->SetAnchorPoint(kAnchorPointCenter);
	spriteClockIcon_->SetPosition(kClockIconPos);
	spriteClockIcon_->SetSize(kClockIconSize);

	// 数字スプライト生成ヘルパーラムダ
	auto CreateDigitSprite = [&](float posX) {
		auto sprite = std::make_unique<Cygnus::Sprite>();
		sprite->Initialize(spriteCommon, texNumbers_);
		sprite->SetAnchorPoint(kAnchorPointCenter);
		sprite->SetPosition({ posX, kYPos });
		sprite->SetSize(kDigitDisplaySize);
		sprite->SetTextureSize(kDigitTextureSize);
		return sprite;
		};

	// 桁ごとのスプライト初期化
	spriteDigitHundreds_ = CreateDigitSprite(kOffsetHundredsX);
	spriteDigitTens_ = CreateDigitSprite(kOffsetTensX);
	spriteDigitOnes_ = CreateDigitSprite(kOffsetOnesX);
	spriteDigitDecimal_ = CreateDigitSprite(kOffsetDecimalX);

	// ドット「.」生成
	spriteDot_ = std::make_unique<Cygnus::Sprite>();
	spriteDot_->Initialize(spriteCommon, texDot);
	spriteDot_->SetAnchorPoint(kAnchorPointCenter);
	spriteDot_->SetPosition({ kOffsetDotX, kYPos });
	spriteDot_->SetSize(kDotDisplaySize);

	currentDisplayValue_ = -1;
}

void GameTimerUI::Update(float remainingTime) {
	// 負の値にならないようクリップ
	float time = (std::max)(0.0f, remainingTime);

	// 整数化した値
	int32_t totalValue = static_cast<int32_t>(std::floor(time * 10.0f));

	// 値に変更があった場合のみ切り出し領域を再計算
	if(currentDisplayValue_ != totalValue) {
		currentDisplayValue_ = totalValue;

		int32_t hundreds = (totalValue / 1000) % 10;
		int32_t tens = (totalValue / 100) % 10;
		int32_t ones = (totalValue / 10) % 10;
		int32_t decimal = totalValue % 10;

		SetDigitValue(spriteDigitHundreds_.get(), hundreds);
		SetDigitValue(spriteDigitTens_.get(), tens);
		SetDigitValue(spriteDigitOnes_.get(), ones);
		SetDigitValue(spriteDigitDecimal_.get(), decimal);
	}

	// 各スプライトの行列更新
	spriteClockIcon_->Update();
	spriteDigitHundreds_->Update();
	spriteDigitTens_->Update();
	spriteDigitOnes_->Update();
	spriteDot_->Update();
	spriteDigitDecimal_->Update();
}

void GameTimerUI::Draw() {
	spriteClockIcon_->Draw();
	spriteDigitHundreds_->Draw();
	spriteDigitTens_->Draw();
	spriteDigitOnes_->Draw();
	spriteDot_->Draw();
	spriteDigitDecimal_->Draw();
}

void GameTimerUI::SetDigitValue(Cygnus::Sprite* sprite, int32_t digit) {
	if (!sprite) return;

	// 数字に応じてテクスチャの左上ピクセル位置をオフセット設定
	sprite->SetTextureLeftTop({ digit * kDigitTextureSize.x, 0.0f });
}