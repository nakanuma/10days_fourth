#include "StageSelectSceneUI.h"
#include "../../../SpriteCollision/SpriteCollision.h"
#include "ImguiWrapper.h"

using namespace Cygnus;

void StageSelectSceneUI::Init(SpriteCommon* spriteCommon)
{
	input_ = Input::GetInstance();
	spriteCommon_ = spriteCommon;

	/*sprites_[SELECT_IMG].sprite = std::make_unique<Sprite>();
	sprites_[SELECT_IMG].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/StageSelectImg.png"));
	sprites_[SELECT_IMG].size = sprites_[SELECT_IMG].sprite->GetSize();


	sprites_[SELECT_TEX].sprite = std::make_unique<Sprite>();
	sprites_[SELECT_TEX].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/StageSelectText.png"));
	sprites_[SELECT_TEX].sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_[SELECT_TEX].pos = { 640.0f, 150.0f };
	sprites_[SELECT_TEX].size = sprites_[SELECT_TEX].sprite->GetSize();
	sprites_[SELECT_TEX].sprite->SetPosition(sprites_[SELECT_TEX].pos);*/


	sprites_[START_TEX].sprite = std::make_unique<Sprite>();
	sprites_[START_TEX].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/TitleSpace.png"));
	sprites_[START_TEX].sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_[START_TEX].pos = { 1040.0f, 590.0f };
	sprites_[START_TEX].size = sprites_[START_TEX].sprite->GetSize();
	sprites_[START_TEX].sprite->SetPosition(sprites_[START_TEX].pos);
}

void StageSelectSceneUI::Update()
{
#ifdef _DEBUG
#endif // _DEBUG

	for (auto& sprite : sprites_)
	{
		sprite.sprite->Update();
	}

	StartButtonUpdate();

	SpriteCollision collision;
	if (collision.IsMouseHover(*sprites_[START_TEX].sprite.get()))
	{
		sprites_[START_TEX].scale = 1.2f; // マウスオーバー時の拡大率
	}
	else
	{
		sprites_[START_TEX].scale = 1.0f;
	}


	sprites_[START_TEX].sprite->SetSize(sprites_[START_TEX].size * sprites_[START_TEX].scale);
}

void StageSelectSceneUI::Draw()
{
	for (auto& sprite : sprites_)
	{
		sprite.sprite->Draw();
	}
#ifdef USE_IMGUI
	ImGui::Begin("UI");

	ImGui::Text("bool : %d", isStart_);

	ImGui::End();
#endif
}

void StageSelectSceneUI::StartButtonUpdate()
{
	SpriteCollision collision;
	if (input_->TriggerKey(DIK_SPACE) || collision.IsMouseClicked(*sprites_[START_TEX].sprite.get()))
	{
		isStart_ = true;
	}
	else
	{
		isStart_ = false;
	}
}