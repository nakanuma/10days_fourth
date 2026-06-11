#include "TitleSceneUI.h"
#include "ImguiWrapper.h"

using namespace Cygnus;

void TitleSceneUI::Init(SpriteCommon* spriteCommon)
{
	// 1. ベースクラスの初期化（ポインタの取得など）
	OtherSceneUIBase::Init(spriteCommon);

	// 2. vector のサイズをこのシーンで必要な分（SpriteNum）確保
	sprites_.resize(SpriteNum);

	// 以降はスプライトの生成処理（元コードのまま）
	sprites_[TITLE_IMG].sprite = std::make_unique<Sprite>();
	sprites_[TITLE_IMG].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/TitleImg.png"));
	sprites_[TITLE_IMG].size = sprites_[TITLE_IMG].sprite->GetSize();

	sprites_[TITLE_TEX].sprite = std::make_unique<Sprite>();
	sprites_[TITLE_TEX].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/TitleText.png"));
	sprites_[TITLE_TEX].sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_[TITLE_TEX].pos = { 640.0f, 350.0f };
	sprites_[TITLE_TEX].size = sprites_[TITLE_TEX].sprite->GetSize();
	sprites_[TITLE_TEX].sprite->SetPosition(sprites_[TITLE_TEX].pos);

	sprites_[START_TEX].sprite = std::make_unique<Sprite>();
	sprites_[START_TEX].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/TitleSpace.png"));
	sprites_[START_TEX].sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_[START_TEX].pos = { 640.0f, 450.0f };
	sprites_[START_TEX].size = sprites_[START_TEX].sprite->GetSize();
	sprites_[START_TEX].sprite->SetPosition(sprites_[START_TEX].pos);
}

void TitleSceneUI::Update()
{
	// 1. 全スプライトの更新処理（ベースクラスのUpdateを呼ぶ）
	OtherSceneUIBase::Update();

	// 2. スタートボタン（START_TEX）のホバー・クリック処理を呼ぶ
	UpdateButtonLogic(START_TEX);
}