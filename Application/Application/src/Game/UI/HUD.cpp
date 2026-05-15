#include "HUD.h"

// Engine
#include <TextureManager.h>

void HUD::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	// 鉱石アイコン生成+初期化
	spriteOreIcon_ = std::make_unique<Cygnus::Sprite>();
	spriteOreIcon_->Initialize(spriteCommon, Cygnus::TextureManager::Load("UI/oreIcon.png"));
	spriteOreIcon_->SetAnchorPoint(kAnchorPoint);
	spriteOreIcon_->SetPosition(kOreIconInitialPosition);

	// 歯車アイコン生成+初期化
	spriteGearIcon_ = std::make_unique<Cygnus::Sprite>();
	spriteGearIcon_->Initialize(spriteCommon, Cygnus::TextureManager::Load("UI/gearIcon.png"));
	spriteGearIcon_->SetAnchorPoint(kAnchorPoint);
	spriteGearIcon_->SetPosition(kGearIconInitialPosition);
}

void HUD::Update() {
	// 鉱石アイコン更新
	spriteOreIcon_->Update();
	// 歯車アイコン更新
	spriteGearIcon_->Update();
}

void HUD::Draw() {
	//// 鉱石アイコン描画
	//spriteOreIcon_->Draw();
	//// 歯車アイコン描画
	//spriteGearIcon_->Draw();
}