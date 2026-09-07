#include "PlayerOxygenUI.h"

// Engine
#include <TextureManager.h>

// Application
#include <src/Game/Objects/Player/Player.h>
#include <src/Game/Util/Utility.h>

void PlayerOxygenUI::Initialize(Cygnus::SpriteCommon* spriteCommon, const Player* player) {
	player_ = player;

	// 単色表示用の白テクスチャをロード
	uint32_t texWhite = Cygnus::TextureManager::Load("white.png");

	// 1. 背景スプライト初期化
	spriteBackground_ = std::make_unique<Cygnus::Sprite>();
	spriteBackground_->Initialize(spriteCommon, texWhite);
	spriteBackground_->SetAnchorPoint(kAnchorPointBottomCenter);
	spriteBackground_->SetSize(kBarSize);
	spriteBackground_->SetColor(kColorBackground);

	// 2. 前景スプライト（水色）初期化
	spriteForeground_ = std::make_unique<Cygnus::Sprite>();
	spriteForeground_->Initialize(spriteCommon, texWhite);
	spriteForeground_->SetAnchorPoint(kAnchorPointBottomCenter);
	spriteForeground_->SetSize(kBarSize);
	spriteForeground_->SetColor(kColorForeground);
}

void PlayerOxygenUI::Update() {
	if (!player_) return;

	/* 酸素残り割合の計算と縦幅の適用 */
	float currentTimer = player_->GetAutoRewindTimer();
	float maxTimer = Player::GetMaxAutoRewindTime();

	// 経過時間から残り酸素割合を算出
	float oxygenRate = 1.0f;
	if(maxTimer > 0.0f) {
		oxygenRate = 1.0f - (currentTimer / maxTimer);
	}
	oxygenRate = std::clamp(oxygenRate, 0.0f, 1.0f);

	// 前景の縦幅を割合に応じて縮小
	Cygnus::Float2 currentFgSize = {kBarSize.x, kBarSize.y * oxygenRate};
	spriteForeground_->SetSize(currentFgSize);

	/* プレイヤー右側への追従座標計算 */
	Cygnus::Float3 worldPos = player_->GetTranslate() + kWorldOffset;
	Cygnus::Float3 screenPos3D = Utility::WorldToScreen(worldPos);

	Cygnus::Float2 screenPos = {
		screenPos3D.x, 
		screenPos3D.y + (kBarSize.y * 0.5f)
	};

	// 座標を設定して行列更新
	spriteBackground_->SetPosition(screenPos);
	spriteForeground_->SetPosition(screenPos);

	spriteBackground_->Update();
	spriteForeground_->Update();
}

void PlayerOxygenUI::Draw() {
	if(!player_ || player_->IsDead()) return;

	spriteBackground_->Draw();
	spriteForeground_->Draw();
}