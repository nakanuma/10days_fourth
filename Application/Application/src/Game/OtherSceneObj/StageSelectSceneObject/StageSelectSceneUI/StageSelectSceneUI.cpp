#include "StageSelectSceneUI.h"
#include "ImguiWrapper.h"

using namespace Cygnus;

void StageSelectSceneUI::Init(SpriteCommon* spriteCommon)
{
	// 1. ベースクラスの共通初期化（input_ や spriteCommon_ の保持）
	OtherSceneUIBase::Init(spriteCommon);

	// 2. このシーンで必要なスプライト数分 vector のメモリを確保
	sprites_.resize(SpriteNum);


	// スタート（決定）ボタンの生成
	sprites_[START_TEX].sprite = std::make_unique<Sprite>();
	sprites_[START_TEX].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/TitleSpace.png"));
	sprites_[START_TEX].sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_[START_TEX].pos = { 1040.0f, 590.0f };
	sprites_[START_TEX].size = sprites_[START_TEX].sprite->GetSize();
	sprites_[START_TEX].sprite->SetPosition(sprites_[START_TEX].pos);
}

void StageSelectSceneUI::Update()
{
	// 全スプライトの基本更新（全要素の sprite->Update() を実行）
	OtherSceneUIBase::Update();

	ResetTrigger();

	// スタートボタンのマウスホバー・クリック・スペースキー処理を共通関数で実行
	UpdateButtonLogic(START_TEX);
}