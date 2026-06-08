#include "ResultSceneUI.h"
#include "ImguiWrapper.h"

using namespace Cygnus;

void ResultSceneUI::Init(SpriteCommon* spriteCommon)
{
	// 1. ベースクラスの共通初期化
	OtherSceneUIBase::Init(spriteCommon);

	// 2. スプライトの最大数に合わせてリサイズ
	sprites_.resize(SpriteNum);

	// 背景画像
	sprites_[RESULT_IMG].sprite = std::make_unique<Sprite>();
	sprites_[RESULT_IMG].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/ResultImg.png"));
	sprites_[RESULT_IMG].size = sprites_[RESULT_IMG].sprite->GetSize();

	// リザルトロゴ
	sprites_[RESULT_TEX].sprite = std::make_unique<Sprite>();
	sprites_[RESULT_TEX].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/ResultText.png"));
	sprites_[RESULT_TEX].sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_[RESULT_TEX].pos = { 640.0f, 150.0f };
	sprites_[RESULT_TEX].size = sprites_[RESULT_TEX].sprite->GetSize();
	sprites_[RESULT_TEX].sprite->SetPosition(sprites_[RESULT_TEX].pos);

	// 次へボタン
	sprites_[NEXT_TEX].sprite = std::make_unique<Sprite>();
	sprites_[NEXT_TEX].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/TitleSpace.png"));
	sprites_[NEXT_TEX].sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_[NEXT_TEX].pos = { 640.0f, 450.0f };
	sprites_[NEXT_TEX].size = sprites_[NEXT_TEX].sprite->GetSize();
	sprites_[NEXT_TEX].sprite->SetPosition(sprites_[NEXT_TEX].pos);
}

void ResultSceneUI::Update()
{
	// 1. 全スプライトの基本更新
	OtherSceneUIBase::Update();

	// 2. 次へボタン（NEXT_TEX）のホバー・クリック処理
	UpdateButtonLogic(NEXT_TEX);
}