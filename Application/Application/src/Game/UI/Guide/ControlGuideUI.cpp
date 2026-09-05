#include "ControlGuideUI.h"

// Engine
#include <TextureManager.h>

void ControlGuideUI::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	// テクスチャのロード
	uint32_t texMenuButton = Cygnus::TextureManager::Load("button_menu.png");
	uint32_t texPauseText = Cygnus::TextureManager::Load("guide_pauseText.png");
	uint32_t texStick = Cygnus::TextureManager::Load("guide_stick.png");
	uint32_t texMoveText = Cygnus::TextureManager::Load("guide_moveText.png");
	uint32_t texRb = Cygnus::TextureManager::Load("guide_rb.png");
	uint32_t texReturnText = Cygnus::TextureManager::Load("guide_returnText.png");

	Cygnus::Float2 leftCenterAnchor = { 0.5f, 0.5f };

	/* 左上UI（メニューボタン + ポーズ文字） */
	spriteMenuButton_ = std::make_unique<Cygnus::Sprite>();
	spriteMenuButton_->Initialize(spriteCommon, texMenuButton);
	spriteMenuButton_->SetAnchorPoint(leftCenterAnchor);
	spriteMenuButton_->SetSize(kButtonSize);
	spriteMenuButton_->SetPosition(kPauseGroupPos);

	// ポーズテキスト（サイズを半分に指定）
	spritePauseText_ = std::make_unique<Cygnus::Sprite>();
	spritePauseText_->Initialize(spriteCommon, texPauseText);
	spritePauseText_->SetAnchorPoint(leftCenterAnchor);
	spritePauseText_->SetSize(kTextSize);
	spritePauseText_->SetPosition({ kPauseGroupPos.x + kPauseTextOffset, kPauseGroupPos.y });

	/* 右下UI（左スティック -> 移動 -> RB -> 帰還） */
	float currentX = kRightBottomGroupPos.x;
	float currentY = kRightBottomGroupPos.y;

	// 左スティック
	spriteStick_ = std::make_unique<Cygnus::Sprite>();
	spriteStick_->Initialize(spriteCommon, texStick);
	spriteStick_->SetAnchorPoint(leftCenterAnchor);
	spriteStick_->SetSize(kButtonSize);
	spriteStick_->SetPosition({ currentX, currentY });
	currentX += kButtonSize.x + kMarginX;

	// 移動テキスト
	spriteMoveText_ = std::make_unique<Cygnus::Sprite>();
	spriteMoveText_->Initialize(spriteCommon, texMoveText);
	spriteMoveText_->SetAnchorPoint(leftCenterAnchor);
	spriteMoveText_->SetSize(kTextSize);
	spriteMoveText_->SetPosition({ currentX, currentY });
	currentX += kTextSize.x + (kMarginX * 2.0f); // ボタンとグループを分けるため余白を少し広めに

	// RBボタン
	spriteRb_ = std::make_unique<Cygnus::Sprite>();
	spriteRb_->Initialize(spriteCommon, texRb);
	spriteRb_->SetAnchorPoint(leftCenterAnchor);
	spriteRb_->SetSize(kButtonSize);
	spriteRb_->SetPosition({ currentX, currentY });
	currentX += kButtonSize.x + kMarginX;

	// 帰還テキスト
	spriteReturnText_ = std::make_unique<Cygnus::Sprite>();
	spriteReturnText_->Initialize(spriteCommon, texReturnText);
	spriteReturnText_->SetAnchorPoint(leftCenterAnchor);
	spriteReturnText_->SetSize(kTextSize);
	spriteReturnText_->SetPosition({ currentX, currentY });
}

void ControlGuideUI::Update() {
	spriteMenuButton_->Update();
	spritePauseText_->Update();

	spriteStick_->Update();
	spriteMoveText_->Update();
	spriteRb_->Update();
	spriteReturnText_->Update();
}

void ControlGuideUI::Draw() {
	// 左上UIの描画
	spriteMenuButton_->Draw();
	spritePauseText_->Draw();

	// 右下UIの描画
	spriteStick_->Draw();
	spriteMoveText_->Draw();
	spriteRb_->Draw();
	spriteReturnText_->Draw();
}
