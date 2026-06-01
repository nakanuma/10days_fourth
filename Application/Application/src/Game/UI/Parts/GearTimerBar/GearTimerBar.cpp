#include "GearTimerBar.h"

// Engine
#include <TextureManager.h>

void GearTimerBar::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	// 残り時間バー用スプライト生成 + 初期化
	spriteBar_ = std::make_unique<Cygnus::Sprite>();
	spriteBar_->Initialize(spriteCommon, Cygnus::TextureManager::Load("white.png"));
	spriteBar_->SetSize(kBarSize);
}

void GearTimerBar::Update(const Cygnus::Float3& screenPos, float energyRatio) {
	// 位置を設定
	Cygnus::Float2 spritePos = {
		screenPos.x - (kBarSize.x / 2.0f),	// 真ん中に来るよう調整
		screenPos.y - kBarSize.y - kOffsetY // Y軸オフセットを適用
	};
	spriteBar_->SetPosition(spritePos);
	// 横幅を設定（残り稼働時間）
	float newWidth = kBarSize.x * energyRatio;
	spriteBar_->SetSize({newWidth, kBarSize.y});

	// 残り時間バー更新
	spriteBar_->Update();
}

void GearTimerBar::Draw() {
	// 残り時間バー描画
	spriteBar_->Draw();
}