#include "TitleSceneUI.h"
#include "../../../SpriteCollision/SpriteCollision.h"
#include "ImguiWrapper.h"

using namespace Cygnus;

void TitleSceneUI::Init(SpriteCommon* spriteCommon)
{
	input_ = Input::GetInstance();
	spriteCommon_ = spriteCommon;

	sprites_[TITLE_IMG].sprite = std::make_unique<Sprite>();
	sprites_[TITLE_IMG].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/TitleImg.png"));
	sprites_[TITLE_IMG].size = sprites_[TITLE_IMG].sprite->GetSize();


	sprites_[TITLE_TEX].sprite = std::make_unique<Sprite>();
	sprites_[TITLE_TEX].sprite->Initialize(spriteCommon_, Cygnus::TextureManager::GetInstance().Load("UI/TitleText.png"));
	sprites_[TITLE_TEX].sprite->SetAnchorPoint({ 0.5f, 0.5f });
	sprites_[TITLE_TEX].pos = { 640.0f, 150.0f };
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

void TitleSceneUI::Draw()
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

void TitleSceneUI::StartButtonUpdate()
{
	SpriteCollision collision;
	if(input_->TriggerKey(DIK_SPACE) || collision.IsMouseClicked(*sprites_[START_TEX].sprite.get()))
	{
		isStart_ = true;
	}
}
