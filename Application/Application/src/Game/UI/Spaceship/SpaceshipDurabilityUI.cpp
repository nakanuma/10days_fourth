#include "SpaceshipDurabilityUI.h"

// Engine
#include <TextureManager.h>

// Application
#include <src/Game/Objects/Spaceship/Spaceship.h>
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

	// 前景スプライト（緑色）初期化
	spriteForeground_ = std::make_unique<Cygnus::Sprite>();
	spriteForeground_->Initialize(spriteCommon, texWhite);
	spriteForeground_->SetAnchorPoint(kAnchorPointLeftCenter);
	spriteForeground_->SetSize(kBarSize);
	spriteForeground_->SetColor(kColorForeground);
}

void SpaceshipDurabilityUI::Update() {
	if(!spaceship_) return;

	/* 修理耐久度の割合合計と横幅の適用 */
	float currentDurability = spaceship_->GetDurability();
	float maxDurability = spaceship_->GetMaxDurability();

	float progressRate = 0.0f;
	if(maxDurability > 0.0f) {
		progressRate = currentDurability / maxDurability;
	}
	progressRate = std::clamp(progressRate, 0.0f, 1.0f);

	// 前景の横幅を耐久度の増加分だけ右へ伸ばす
	Cygnus::Float2 currentFgSize = {kBarSize.x * progressRate, kBarSize.y};
	spriteForeground_->SetSize(currentFgSize);

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
	spriteForeground_->SetPosition(screenPos);

	spriteBackground_->Update();
	spriteForeground_->Update();
}

void SpaceshipDurabilityUI::Draw() {
	if(!spaceship_) return;

	spriteBackground_->Draw();
	spriteForeground_->Draw();
}