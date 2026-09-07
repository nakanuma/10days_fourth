#include "PlayerHPUI.h"

// Engine
#include <TextureManager.h>

// Application
#include <src/Game/Objects/Player/Player.h>
#include <src/Game/Util/Utility.h>

void PlayerHPUI::Initialize(Cygnus::SpriteCommon* spriteCommon, const Player* player) {
	player_ = player;

	uint32_t texWhite = Cygnus::TextureManager::Load("white.png");

	// 背景スプライト初期化
	spriteBackground_ = std::make_unique<Cygnus::Sprite>();
	spriteBackground_->Initialize(spriteCommon, texWhite);
	spriteBackground_->SetAnchorPoint(kAnchorPointLeftCenter);
	spriteBackground_->SetSize(kBarSize);
	spriteBackground_->SetColor(kColorBackground);

	// 前景スプライト初期化
	spriteForeground_ = std::make_unique<Cygnus::Sprite>();
	spriteForeground_->Initialize(spriteCommon, texWhite);
	spriteForeground_->SetAnchorPoint(kAnchorPointLeftCenter);
	spriteForeground_->SetSize(kBarSize);
	spriteForeground_->SetColor(kColorForeground);
}

void PlayerHPUI::Update() {
	if(!player_) return;

	/* HP割合の計算とバー横幅の適用 */
	int32_t currentHP = player_->GetHP();
	int32_t maxHP = player_->GetMaxHP();

	float hpRate = 0.0f;
	if(maxHP > 0) {
		hpRate = static_cast<float>(currentHP) / static_cast<float>(maxHP);
	}
	hpRate = std::clamp(hpRate, 0.0f, 1.0f);

	// 前景の横幅をHP割合に応じて変化させる
	Cygnus::Float2 currentFgSize = {kBarSize.x * hpRate, kBarSize.y};
	spriteForeground_->SetSize(currentFgSize);

	/* ワールド座標->スクリーン座標による追従計算 */ 
	Cygnus::Float3 worldPos = player_->GetTranslate() + kWorldOffset;

	// 変換関数を実行して2Dスクリーン座標を取得
	Cygnus::Float3 screenPos3D = Utility::WorldToScreen(worldPos);

	// 左端アンカーのバーを中央揃えで配置
	Cygnus::Float2 screenPos = {
		screenPos3D.x - (kBarSize.x * 0.5f),
		screenPos3D.y
	};

	// 前景と背景の位置を設定
	spriteBackground_->SetPosition(screenPos);
	spriteForeground_->SetPosition(screenPos);

	// 各スプライト行列更新
	spriteBackground_->Update();
	spriteForeground_->Update();
}

void PlayerHPUI::Draw() {
	if(!player_ || player_->IsDead()) return;

	spriteBackground_->Draw();
	spriteForeground_->Draw();
}