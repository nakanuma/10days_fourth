#include "ResultSceneUI.h"
// ※SpriteCollisionのパスは実際のプロジェクトに合わせて変更してください
#include "../../../SpriteCollision/SpriteCollision.h" 
#include "ImguiWrapper.h"

using namespace Cygnus;

void ResultSceneUI::Init(SpriteCommon* spriteCommon)
{
	input_ = Input::GetInstance();
	spriteCommon_ = spriteCommon;

	sprites_[RESULT_IMG].sprite = std::make_unique<Sprite>();
	sprites_[RESULT_IMG].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/ResultImg.png"));
	sprites_[RESULT_IMG].size = sprites_[RESULT_IMG].sprite->GetSize();

	// リザルトロゴ（テキスト）用スプライト
	sprites_[RESULT_TEX].sprite = std::make_unique<Sprite>();
	sprites_[RESULT_TEX].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/ResultText.png"));
	sprites_[RESULT_TEX].sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_[RESULT_TEX].pos = { 640.0f, 150.0f };
	sprites_[RESULT_TEX].size = sprites_[RESULT_TEX].sprite->GetSize();
	sprites_[RESULT_TEX].sprite->SetPosition(sprites_[RESULT_TEX].pos);

	// 次へ（またはタイトルへ戻る）ボタン用スプライト
	sprites_[NEXT_TEX].sprite = std::make_unique<Sprite>();
	sprites_[NEXT_TEX].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/TitleSpace.png"));
	sprites_[NEXT_TEX].sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_[NEXT_TEX].pos = { 640.0f, 450.0f };
	sprites_[NEXT_TEX].size = sprites_[NEXT_TEX].sprite->GetSize();
	sprites_[NEXT_TEX].sprite->SetPosition(sprites_[NEXT_TEX].pos);
}

void ResultSceneUI::Update()
{
#ifdef _DEBUG
#endif // _DEBUG

	for (auto& sprite : sprites_)
	{
		sprite.sprite->Update();
	}

	NextButtonUpdate();

	// マウスカーソルが重なった際のボタン拡大処理
	SpriteCollision collision;
	if (collision.IsMouseHover(*sprites_[NEXT_TEX].sprite.get()))
	{
		sprites_[NEXT_TEX].scale = 1.2f; // マウスオーバー時の拡大率
	}
	else
	{
		sprites_[NEXT_TEX].scale = 1.0f;
	}

	sprites_[NEXT_TEX].sprite->SetSize(sprites_[NEXT_TEX].size * sprites_[NEXT_TEX].scale);
}

void ResultSceneUI::Draw()
{
	for (auto& sprite : sprites_)
	{
		sprite.sprite->Draw();
	}

#ifdef USE_IMGUI
	ImGui::Begin("ResultUI");

	ImGui::Text("bool : %d", isNext_);

	ImGui::End();
#endif
}

void ResultSceneUI::NextButtonUpdate()
{
	SpriteCollision collision;
	// スペースキーが押されたか、ボタンがクリックされたらフラグを立てる
	if (input_->TriggerKey(DIK_SPACE) || collision.IsMouseClicked(*sprites_[NEXT_TEX].sprite.get()))
	{
		isNext_ = true;
	}
}