#include "InteractGuide.h"

// Engine
#include <TextureManager.h>

void InteractGuide::Initialize(Cygnus::SpriteCommon* spriteCommon, ActionType actionType) {
	// ボタン用スプライト生成+初期化
	spriteButton_ = std::make_unique<Cygnus::Sprite>();
	spriteButton_->Initialize(spriteCommon, Cygnus::TextureManager::Load("UI/button_a.png"));
	spriteButton_->SetAnchorPoint(kAnchorPoint);
	spriteButton_->SetSize(kButtonSize);

	// 文字用スプライト生成+初期化
	uint32_t textTex = 0;
	switch (actionType) { // 指定されたタイプに応じてテクスチャ変更
	case ActionType::Insert:
		textTex = Cygnus::TextureManager::Load("UI/text_hameru.png"); // 「はめる」文字
		break;
	case ActionType::Craft:
		textTex = Cygnus::TextureManager::Load("UI/text_tsukuru.png"); // 「作る」文字
		break;
	case ActionType::Mine:
		textTex = Cygnus::TextureManager::Load("UI/text_horu.png"); // 「掘る」文字
		break;
	}
	spriteText_ = std::make_unique<Cygnus::Sprite>();
	spriteText_->Initialize(spriteCommon, textTex);
	spriteText_->SetAnchorPoint(kAnchorPoint);
	spriteText_->SetSize(kTextSize);
}

void InteractGuide::Update(const Cygnus::Float3& screenPos) {
	if (!isActive_) return;

	// UI全体の横幅を計算
	float totalWidth = kButtonSize.x + kSpacing + kTextSize.x;
	// UI全体の左端となる位置を逆算
	float leftEdgeX = screenPos.x - totalWidth * 0.5f;
	// ボタンの中心位置を計算
	Cygnus::Float2 buttonCenterPos = {
		leftEdgeX + kButtonSize.x * 0.5f, 
		screenPos.y - kYOffsetY
	};
	spriteButton_->SetPosition(buttonCenterPos);
	// 文字の中心位置を計算
	Cygnus::Float2 textCenterPos = {
		buttonCenterPos.x + kButtonSize.x * 0.5f + kSpacing + kTextSize.x * 0.5f, 
		screenPos.y - kYOffsetY
	};
	spriteText_->SetPosition(textCenterPos);

	// ボタン更新
	spriteButton_->Update();
	// 文字更新
	spriteText_->Update();
}

void InteractGuide::Draw() {
	if (!isActive_) return;

	// ボタン描画
	spriteButton_->Draw();
	// 文字描画
	spriteText_->Draw();
}

