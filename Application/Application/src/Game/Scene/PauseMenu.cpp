#include "PauseMenu.h"

// Engine
#include <Input/Input.h>
#include <TextureManager.h>
#include <SceneManager.h>
#include <TimeManager.h>
#include <Easing.h>
#include <Collider/CollisionManager.h>

// Application
#include <src/Game/Util/Transition/FadeTransition.h>

void PauseMenu::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	// パラメーター初期化
	isPaused_ = false; 
	currentMenu_ = MenuIndex::Continue;
	isStickMoved_ = false;
	isSelected_ = false;
	animationTimer_ = 0.0f;

	/* スプライト生成 */

	// 背景
	uint32_t texBackground = Cygnus::TextureManager::Load("white.png");
	spriteBackground_ = std::make_unique<Cygnus::Sprite>();
	spriteBackground_->Initialize(spriteCommon, texBackground);
	spriteBackground_->SetSize({1280.0f, 720.0f});
	spriteBackground_->SetColor({0.0f, 0.0f, 0.0f, 0.5f});

	// ポーズ文字
	uint32_t texPauseText = Cygnus::TextureManager::Load("pauseText.png");
	spritePauseText_ = std::make_unique<Cygnus::Sprite>();
	spritePauseText_->Initialize(spriteCommon, texPauseText);
	spritePauseText_->SetAnchorPoint({0.5f, 0.5f});
	spritePauseText_->SetPosition(kPauseTextInitPos);

	// つづけるボタン
	uint32_t texContinueButton = Cygnus::TextureManager::Load("pause_continueButton.png");
	spriteContinueButton_ = std::make_unique<Cygnus::Sprite>();
	spriteContinueButton_->Initialize(spriteCommon, texContinueButton);
	spriteContinueButton_->SetAnchorPoint({0.5f, 0.5f});
	spriteContinueButton_->SetPosition(kContinueButtonInitPos);
	baseContinueButtonSize_ = spriteContinueButton_->GetSize();

	// タイトルへ戻るボタン
	uint32_t texReturnButton = Cygnus::TextureManager::Load("result_returnButton.png");
	spriteReturnButton_ = std::make_unique<Cygnus::Sprite>();
	spriteReturnButton_->Initialize(spriteCommon, texReturnButton);
	spriteReturnButton_->SetAnchorPoint({0.5f, 0.5f});
	spriteReturnButton_->SetPosition(kReturnButtonInitPos);
	baseReturnButtonSize_ = spriteReturnButton_->GetSize();

	// Aボタン
	uint32_t texButtonA = Cygnus::TextureManager::Load("button_a.png");
	spriteButtonA_ = std::make_unique<Cygnus::Sprite>();
	spriteButtonA_->Initialize(spriteCommon, texButtonA);
	spriteButtonA_->SetAnchorPoint({ 0.5f, 0.5f });
	// 初期ターゲット座標を設定
	buttonACurrentPos_ = {
		kContinueButtonInitPos.x + kButtonAOffsetContinue.x,
		kContinueButtonInitPos.y + kButtonAOffsetContinue.y
	};
	buttonAStartPos_ = buttonACurrentPos_;
	buttonATargetPos_ = buttonACurrentPos_;
	buttonAMoveTimer_ = kButtonAMoveDuration;
}

void PauseMenu::Update() { 
	isJustUnpaused_ = false; // 毎フレーム初期化

	Cygnus::Input* input = Cygnus::Input::GetInstance();

	// ESCAPEまたはSTARTボタンの押下判定
	bool isTogglePause = false;

	// キーボード入力
	if(input->TriggerKey(DIK_ESCAPE)) {
		isTogglePause = true;
	}

	// コントローラー入力
	XINPUT_STATE padState;
	if(input->GetJoystickState(0, padState)) {
		if(input->IsTriggerButton(0, XINPUT_GAMEPAD_START)) {
			isTogglePause = true;
		}
	}

	// フェード完了時のみポーズ切り替えを実行
	if(isTogglePause && FadeTransition::GetInstance()->IsFinished()) {
		if(isPaused_) {
			isPaused_ = false;
			isJustUnpaused_ = false; // ポーズ解除フラグを立てる
		} else {
			isPaused_ = true;
			isSelected_ = false; // ポーズを開いたときにフラグ初期化
		}
	}

	if(!isPaused_) return;

	// 入力処理
	ProcessMenuInput();

	// UIアニメーション処理
	UpdateUI();

	// 背景更新
	spriteBackground_->Update();
	// ポーズ文字更新
	spritePauseText_->Update();
	// つづけるボタン更新
	spriteContinueButton_->Update();
	// タイトルへ戻るボタン更新
	spriteReturnButton_->Update();
	// Aボタン更新
	spriteButtonA_->Update();
}

void PauseMenu::Draw() {
	if (!isPaused_) return;

	// 背景描画
	spriteBackground_->Draw();
	// ポーズ文字描画
	spritePauseText_->Draw();
	// つづけるボタン描画
	spriteContinueButton_->Draw();
	// タイトルへ戻るボタン描画
	spriteReturnButton_->Draw();
	// Aボタン描画
	spriteButtonA_->Draw();
}

void PauseMenu::ProcessMenuInput() {
	// 決定ボタン押下済みなら入力を受け付けない
	if(isSelected_) return;

	Cygnus::Input* input = Cygnus::Input::GetInstance();

	bool isUp = false;
	bool isDown = false;

	// キーボード入力
	if (input->TriggerKey(DIK_W) || input->TriggerKey(DIK_UP)) {
		isUp = true;
	}
	if (input->TriggerKey(DIK_S) || input->TriggerKey(DIK_DOWN)) {
		isDown = true;
	}

	// コントローラー入力
	XINPUT_STATE padState;
	if (input->GetJoystickState(0, padState)) {
		// 十字キー
		if (input->IsTriggerButton(0, XINPUT_GAMEPAD_DPAD_UP)) isUp = true;
		if (input->IsTriggerButton(0, XINPUT_GAMEPAD_DPAD_DOWN)) isDown = true;

		// Lスティック Y軸
		float lStickY = padState.Gamepad.sThumbLY / 32767.0f;
		constexpr float kThreshold = 0.5f;

		if (std::abs(lStickY) > kThreshold) {
			if (!isStickMoved_) {
				if (lStickY > kThreshold) isUp = true;
				if (lStickY < -kThreshold) isDown = true;
				isStickMoved_ = true;
			}
		} else {
			isStickMoved_ = false;
		}
	}

	// カーソル切り替えチェック用の旧メニュー保持
	MenuIndex prevMenu = currentMenu_;

	// カーソル切り替え
	if (isDown && currentMenu_ == MenuIndex::Continue) {
		currentMenu_ = MenuIndex::ReturnTitle;
	} else if (isUp && currentMenu_ == MenuIndex::ReturnTitle) {
		currentMenu_ = MenuIndex::Continue;
	}

	// メニューが変更された場合、Aボタンの補間移動を開始
	if(prevMenu != currentMenu_) {
		buttonAStartPos_ = buttonACurrentPos_; // 現在地を開始地点に

		// 新しい目標座標を設定
		if(currentMenu_ == MenuIndex::Continue) {
			buttonATargetPos_ = {
				kContinueButtonInitPos.x + kButtonAOffsetContinue.x,
				kContinueButtonInitPos.y + kButtonAOffsetContinue.y
			};
		} else if (currentMenu_ == MenuIndex::ReturnTitle) {
			buttonATargetPos_ = {
				kReturnButtonInitPos.x + kButtonAOffsetReturn.x,
				kReturnButtonInitPos.y + kButtonAOffsetReturn.y
			};
		}

		buttonAMoveTimer_ = 0.0f; // タイマーリセット
	}

	/* 決定入力判定 */
	bool isConfirm = input->TriggerKey(DIK_SPACE) || input->TriggerKey(DIK_RETURN) || input->IsTriggerButton(0, XINPUT_GAMEPAD_A);

	if (isConfirm) {
		if(currentMenu_ == MenuIndex::Continue) {
			isPaused_ = false; // ポーズ解除 
			isJustUnpaused_ = true; // つづけるを押して解除した瞬間もフラグを立てる
		} else if (currentMenu_ == MenuIndex::ReturnTitle) {
			isSelected_ = true; // 多重入力防止

			// タイトルシーンへ移行
			FadeTransition::GetInstance()->StartFadeOut(
				1.0f,
				[]() {
					Cygnus::SceneManager::GetInstance()->ChangeScene("TITLE");
					Cygnus::CollisionManager::GetInstance()->Clear();
				},
				0.5f
			);
		}
	}
}

void PauseMenu::UpdateUI() {
	float deltaTime = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	// タイマー加算
	animationTimer_ += deltaTime;

	// 選択中ボタンのアニメーション
	float pulseScale = kSelectedBaseRate + std::sinf(animationTimer_ * kPulseSpeed) * kPulseAmplitude;

	float continueRate = (currentMenu_ == MenuIndex::Continue) ? pulseScale : kNormalButtonRate;
	float returnRate = (currentMenu_ == MenuIndex::ReturnTitle) ? pulseScale : kNormalButtonRate;

	spriteContinueButton_->SetSize({baseContinueButtonSize_.x * continueRate, baseContinueButtonSize_.y * continueRate});
	spriteReturnButton_->SetSize({baseReturnButtonSize_.x * returnRate, baseReturnButtonSize_.y * returnRate});

	// Aボタンのイージング移動
	if(buttonAMoveTimer_ < kButtonAMoveDuration) {
		buttonAMoveTimer_ += deltaTime;
		if(buttonAMoveTimer_ > kButtonAMoveDuration) {
			buttonAMoveTimer_ = kButtonAMoveDuration;
		}

		// 進行度
		float t = buttonAMoveTimer_ / kButtonAMoveDuration;

		// イージング関数
		float easedT = Cygnus::Easing::EaseOutExpo(t);

		// 座標の補間
		buttonACurrentPos_.x = buttonAStartPos_.x + (buttonATargetPos_.x - buttonAStartPos_.x) * easedT;
		buttonACurrentPos_.y = buttonAStartPos_.y + (buttonATargetPos_.y - buttonAStartPos_.y) * easedT;
	} else {
		buttonACurrentPos_ = buttonATargetPos_;
	}

	// 毎フレームのバウンドオフセット加算
	float bounceOffsetY = -std::abs(std::sinf(animationTimer_ * kButtonABounceSpeed)) * kButtonABounceHeight;

	spriteButtonA_->SetPosition({
		buttonACurrentPos_.x,
		buttonACurrentPos_.y + bounceOffsetY
		});
}