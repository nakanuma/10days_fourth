#include "SpaceshipDurabilityUI.h"

// Engine
#include <TextureManager.h>
#include <TimeManager.h>

// Application
#include <src/Game/Util/Utility.h>

void SpaceshipDurabilityUI::Initialize(Cygnus::SpriteCommon* spriteCommon, const Spaceship* spaceship) {
	spaceship_ = spaceship;

	// 単色表示用の白テクスチャをロード
	uint32_t texWhite = Cygnus::TextureManager::Load("white.png");

	// 背景スプライト初期化
	spriteBackground_ = std::make_unique<Cygnus::Sprite>();
	spriteBackground_->Initialize(spriteCommon, texWhite);
	spriteBackground_->SetAnchorPoint(kAnchorPointLeftCenter);
	spriteBackground_->SetSize(kBarSize);
	spriteBackground_->SetColor(kColorBackground);

	// 予測回復スプライト初期化
	spritePrediction_ = std::make_unique<Cygnus::Sprite>();
	spritePrediction_->Initialize(spriteCommon, texWhite);
	spritePrediction_->SetAnchorPoint(kAnchorPointLeftCenter);
	spritePrediction_->SetSize(kBarSize);
	spritePrediction_->SetColor(kColorPrediction);

	// 前景スプライト（緑色）初期化
	spriteForeground_ = std::make_unique<Cygnus::Sprite>();
	spriteForeground_->Initialize(spriteCommon, texWhite);
	spriteForeground_->SetAnchorPoint(kAnchorPointLeftCenter);
	spriteForeground_->SetSize(kBarSize);
	spriteForeground_->SetColor(kColorForeground);

	// 初期表示耐久度のセット
	if(spaceship) {
		displayDurability_ = spaceship_->GetDurability();
	}
}

void SpaceshipDurabilityUI::Update() {
	if(!spaceship_) return;

	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	// 宇宙船のデータ数値
	float targetDurability = spaceship_->GetDurability(); // 最終目標値
	float maxDurability = spaceship_->GetMaxDurability();

	// 実際の緑ゲージを表示用数値に向けて徐々に増加させる
	if(displayDurability_ < targetDurability) {
		displayDurability_ += kGaugeCatchupSpeed * dt * maxDurability;

		if(displayDurability_ > targetDurability) {
			displayDurability_ = targetDurability;
		} 
	}
	
	// 実際の緑ゲージ（連続して伸びる部分）の割合
	float currentRate = (maxDurability > 0.0f) ? (displayDurability_ / maxDurability) : 0.0f;
	currentRate = std::clamp(currentRate, 0.0f, 1.0f);
	spriteForeground_->SetSize({kBarSize.x * currentRate, kBarSize.y});

	// 薄い緑ゲージ（最終到達地点）の割合
	float predictedRate = (maxDurability > 0.0f) ? (targetDurability / maxDurability) : 0.0f;
	predictedRate = std::clamp(predictedRate, 0.0f, 1.0f);
	spritePrediction_->SetSize({kBarSize.x * predictedRate, kBarSize.y});

	/* 宇宙船頭上用への追従 */
	Cygnus::Float3 worldPos = spaceship_->GetTranslate() + kWorldOffset;
	Cygnus::Float3 screenPos3D = Utility::WorldToScreen(worldPos);

	// 宇宙船の真上に配置されるよう補正
	Cygnus::Float2 screenPos = {
		screenPos3D.x - (kBarSize.x * 0.5f),
		screenPos3D.y
	};

	// 座標を設定して行列更新
	spriteBackground_->SetPosition(screenPos);
	spritePrediction_->SetPosition(screenPos);
	spriteForeground_->SetPosition(screenPos);

	spriteBackground_->Update();
	spritePrediction_->Update();
	spriteForeground_->Update();
}

void SpaceshipDurabilityUI::Draw() {
	if(!spaceship_) return;

	spriteBackground_->Draw();
	spritePrediction_->Draw();
	spriteForeground_->Draw();
}