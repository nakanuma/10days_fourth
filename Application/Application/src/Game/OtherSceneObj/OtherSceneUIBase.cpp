#include "OtherSceneUIBase.h"
#include "../SpriteCollision/SpriteCollision.h"

using namespace Cygnus;

void OtherSceneUIBase::Init(SpriteCommon* spriteCommon)
{
	input_ = Input::GetInstance();
	spriteCommon_ = spriteCommon;
}

void OtherSceneUIBase::Update()
{
	for (auto& sprite : sprites_)
	{
		if (sprite.sprite)
		{
			sprite.sprite->Update();
		}
	}
}

void OtherSceneUIBase::Draw()
{
	for (auto& sprite : sprites_)
	{
		if (sprite.sprite)
		{
			sprite.sprite->Draw();
		}
	}
}

void OtherSceneUIBase::UpdateButtonLogic(size_t buttonIndex)
{
	// 安全対策（インデックスオーバーや未生成の場合は弾く）
	if (buttonIndex >= sprites_.size() || !sprites_[buttonIndex].sprite) return;

	SpriteCollision collision;

	// --- 1. クリック判定 ---
	if (input_->TriggerKey(DIK_SPACE) || collision.IsMouseClicked(*sprites_[buttonIndex].sprite.get()) || input_->IsTriggerButton(0, XINPUT_GAMEPAD_A))
	{
		isTriggered_ = true;
	}

	// --- 2. ホバー時の拡大判定 ---
	if (collision.IsMouseHover(*sprites_[buttonIndex].sprite.get()))
	{
		sprites_[buttonIndex].scale = 1.2f;
	}
	else
	{
		sprites_[buttonIndex].scale = 1.0f;
	}

	// --- 3. スケールの適用 ---
	sprites_[buttonIndex].sprite->SetSize(sprites_[buttonIndex].size * sprites_[buttonIndex].scale);
}