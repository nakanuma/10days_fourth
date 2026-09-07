#include "ControlGuideUI.h"

// Engine
#include <TextureManager.h>
#include <TimeManager.h>
#include <Input/Input.h>

void ControlGuideUI::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	// タイマー初期化
	pausePressTimer_ = 0.0f;
	isPausePressed_ = false;
	rbPressTimer_ = 0.0f;
	isRbPressed_ = false;

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
	Cygnus::Input* input = Cygnus::Input::GetInstance();
	float deltaTime = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	/* 入力判定（押し下げトリガーの検知） */

	// ポーズボタン（ESCAPE または コントローラー START）
	bool isPauseTriggered = input->TriggerKey(DIK_ESCAPE);
	// RBボタン（Eキー または コントローラー RB）
	bool isRbTriggered = input->TriggerKey(DIK_E);

	XINPUT_STATE padState;
	if (input->GetJoystickState(0, padState)) {
		if (input->IsTriggerButton(0, XINPUT_GAMEPAD_START)) {
			isPauseTriggered = true;
		}
		if (input->IsTriggerButton(0, XINPUT_GAMEPAD_RIGHT_SHOULDER)) {
			isRbTriggered = true;
		}
	}

	if (isPauseTriggered) {
		isPausePressed_ = true;
		pausePressTimer_ = 0.0f;
	}

	if (isRbTriggered) {
		isRbPressed_ = true;
		rbPressTimer_ = 0.0f;
	}

	/* ポーズ（メニュー）ボタンのアニメーション計算 */
	float menuScaleFactor = 1.0f;
	Cygnus::Float4 menuColor = kButtonNormalColor;

	if (isPausePressed_) {
		pausePressTimer_ += deltaTime;
		if (pausePressTimer_ >= kButtonPressDuration) {
			pausePressTimer_ = kButtonPressDuration;
			isPausePressed_ = false; // アニメーション終了
		}

		float progress = pausePressTimer_ / kButtonPressDuration;
		float pressFactor = std::sinf(progress * Cygnus::PIf); // 0 -> 1 -> 0

		menuScaleFactor = 1.0f - pressFactor * (1.0f - kButtonPressMinScale);
		menuColor = kButtonNormalColor + (kButtonPressedColor - kButtonNormalColor) * pressFactor;
	}

	spriteMenuButton_->SetSize({kButtonSize.x * menuScaleFactor, kButtonSize.y * menuScaleFactor});
	spriteMenuButton_->SetColor(menuColor);

	/* RBボタンのアニメーション計算 */
	float rbScaleFactor = 1.0f;
	Cygnus::Float4 rbColor = kButtonNormalColor;

	if (isRbPressed_) {
		rbPressTimer_ += deltaTime;
		if (rbPressTimer_ >= kButtonPressDuration) {
			rbPressTimer_ = kButtonPressDuration;
			isRbPressed_ = false; // アニメーション終了
		}

		float progress = rbPressTimer_ / kButtonPressDuration;
		float pressFactor = std::sinf(progress * Cygnus::PIf); // 0 -> 1 -> 0

		rbScaleFactor = 1.0f - pressFactor * (1.0f - kButtonPressMinScale);
		rbColor = kButtonNormalColor + (kButtonPressedColor - kButtonNormalColor) * pressFactor;
	}

	spriteRb_->SetSize({kButtonSize.x * rbScaleFactor, kButtonSize.y * rbScaleFactor});
	spriteRb_->SetColor(rbColor);

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
