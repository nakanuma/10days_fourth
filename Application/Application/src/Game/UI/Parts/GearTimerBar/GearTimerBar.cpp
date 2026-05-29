#include "GearTimerBar.h"

// Engine
#include <TextureManager.h>

void GearTimerBar::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	// 残り時間バー用スプライト生成 + 初期化
	spriteBar_ = std::make_unique<Cygnus::Sprite>();
	spriteBar_->Initialize(spriteCommon, Cygnus::TextureManager::Load("white.png"));
	spriteBar_->SetAnchorPoint(kAnchorPoint);
	spriteBar_->SetSize(kBarSize);
}

void GearTimerBar::Update(const Cygnus::Float3& screenPos) {

}

void GearTimerBar::Draw() {
	
}