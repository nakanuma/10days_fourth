#include "InteractGuide.h"

// Engine
#include <TextureManager.h>

void InteractGuide::Initialize(Cygnus::SpriteCommon* spriteCommon, ActionType actionType) {
	// ボタン画像生成+初期化
	spriteButton_ = std::make_unique<Cygnus::Sprite>();
	spriteButton_->Initialize(spriteCommon, Cygnus::TextureManager::Load("UI/button_a.png"));

	// 文字画像生成+初期化
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
}

void InteractGuide::Update() {
	if (!isActive_) return;

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
	spriteButton_->Draw();
}

