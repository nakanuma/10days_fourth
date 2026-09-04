#include "DirectXBase.h"
#include "TitleScene.h"
#include "ImguiWrapper.h"
#include "RTVManager.h"
#include "SRVManager.h"
#include "SpriteCommon.h"
#include <Engine/3D/LightCamera.h>
#include <Engine/3D/LineDrawer.h>
#include <Engine/DirectX/ShadowMapManager.h>
#include <Engine/Model/SkyBoxManager.h>
#include <Engine/ParticleEffect/ParticleEffectManager.h>

// C++
#include <numbers>

// Engine
#include <Collider/CollisionManager.h>
#include <CommandManager.h>
#include <Engine/Scene/SceneManager.h>
#include <MyMath.h>

// Application
#include <src/Game/Util/Transition/FadeTransition.h>

void TitleScene::Initialize() {
	Cygnus::DirectXBase* dxBase = Cygnus::DirectXBase::GetInstance();

	// カメラのインスタンスを生成
	camera_ = std::make_unique<Cygnus::Camera>(Cygnus::Float3{0.0f, 0.0f, -50.0f}, Cygnus::Float3{0.0f, 0.0f, 0.0f}, 0.45f);
	Cygnus::Camera::Set(camera_.get()); // 現在のカメラをセット

	// SpriteCommonの生成と初期化
	spriteCommon_ = std::make_unique<Cygnus::SpriteCommon>();
	spriteCommon_->Initialize(Cygnus::DirectXBase::GetInstance());

	// TextureManagerの初期化
	Cygnus::TextureManager::Initialize(dxBase->GetDevice(), Cygnus::SRVManager::GetInstance());

	// Inputの初期化
	input_ = Cygnus::Input::GetInstance();

	// LightManagerの初期化
	lightManager_ = Cygnus::LightManager::GetInstance();
	lightManager_->Initialize();

	// シャドウマップ生成
	shadowMapHandle_ = Cygnus::ShadowMapManager::GetInstance()->CreateShadowMap(Cygnus::Window::GetWidth(), Cygnus::Window::GetHeight());

	// ポストエフェクト管理
	postEffectManager_ = std::make_unique<Cygnus::PostEffectManager>();
	postEffectManager_->Initialize();

	///
	///	↓ シーン用
	///

	// シーンの開始時にフェードインを実行
	FadeTransition::GetInstance()->StartFadeIn(1.0f, 0.5f);

	// 初期選択状態のリセット
	currentMenu_ = MenuIndex::Start;
	isStickMoved_ = false;
	isSelected_ = false;

	/* オブジェクト */ 

	// プレイヤー生成
	objectPlayer_ = std::make_unique<Cygnus::Object3D>();
	objectPlayer_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Player");
	objectPlayer_->transform_.translate_.z = 5.0f;

	/* スプライト */
	
	// タイトルロゴ生成
	uint32_t texTitleLogo = Cygnus::TextureManager::Load("titleLogo.png");
	spriteTitleLogo_ = std::make_unique<Cygnus::Sprite>();
	spriteTitleLogo_->Initialize(spriteCommon_.get(), texTitleLogo);
	spriteTitleLogo_->SetPosition(kTitleLogoInitPos);
	spriteTitleLogo_->SetAnchorPoint({0.5f, 0.5f});

	// スタートボタン生成
	uint32_t texStartButton = Cygnus::TextureManager::Load("title_startButton.png");
	spriteStartButton_ = std::make_unique<Cygnus::Sprite>();
	spriteStartButton_->Initialize(spriteCommon_.get(), texStartButton);
	spriteStartButton_->SetPosition(kStartButtonInitPos);
	spriteStartButton_->SetAnchorPoint({ 0.5f, 0.5f });
	baseStartButtonSize_ = spriteStartButton_->GetSize();

	// 終了ボタン生成
	uint32_t texExitButton = Cygnus::TextureManager::Load("title_exitButton.png");
	spriteExitButton_ = std::make_unique<Cygnus::Sprite>();
	spriteExitButton_->Initialize(spriteCommon_.get(), texExitButton);
	spriteExitButton_->SetPosition(kExitButtonInitPos);
	spriteExitButton_->SetAnchorPoint({ 0.5f, 0.5f });
	baseExitButtonSize_ = spriteExitButton_->GetSize();

	// Aボタン生成
	uint32_t texButtonA = Cygnus::TextureManager::Load("button_a.png");
	spriteButtonA_ = std::make_unique<Cygnus::Sprite>();
	spriteButtonA_->Initialize(spriteCommon_.get(), texButtonA);
	spriteButtonA_->SetAnchorPoint({0.5f, 0.5f});
	baseButtonASize_ = spriteButtonA_->GetSize();

	// 初期スケールの適用
	UpdateUI();
}

void TitleScene::Finalize() {}

void TitleScene::Update() {
	Cygnus::LightManager::GetInstance()->ClearEmissiveLights(); // エミッシブライトをクリア
	Cygnus::LightManager::GetInstance()->ClearAreaLights();     // エリアライトをクリア
	Cygnus::SkyBoxManager::GetInstance()->Update();             // SkyBox更新

	// メニュー入力処理
	ProcessMenuInput();

	// UI見た目更新
	UpdateUI();

	// プレイヤーアニメーション更新
	UpdatePlayerAnimation();

	///
	///	オブジェクト更新処理
	///

	// プレイヤー更新
	objectPlayer_->UpdateMatrix();

	// タイトルロゴ更新
	spriteTitleLogo_->Update();
	// スタートボタン更新
	spriteStartButton_->Update();
	// 終了ボタン更新
	spriteExitButton_->Update();
	// Aボタン更新
	spriteButtonA_->Update();

	///
	///	スプライト更新処理
	/// 

	// フェードトランジション更新
	FadeTransition::GetInstance()->Update();

	///
	///	共通更新処理
	///

	// コリジョンマネージャー更新
	Cygnus::CollisionManager::GetInstance()->Update();
}

void TitleScene::Draw() {
	Cygnus::DirectXBase* dxBase = Cygnus::DirectXBase::GetInstance();
	Cygnus::SRVManager* srvManager = Cygnus::SRVManager::GetInstance();
	auto* cmd = Cygnus::CommandManager::GetInstance()->GetCommandList();

	// 描画前処理
	dxBase->PreDraw();
	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = {srvManager->descriptorHeap_.heap_.Get()};
	cmd->SetDescriptorHeaps(1, descriptorHeaps);
	// ImGuiのフレーム開始処理
	Cygnus::ImguiWrapper::NewFrame();
	// カメラの定数バッファを設定
	Cygnus::Camera::TransferConstantBuffer();
	// ライトの定数バッファを設定
	lightManager_->TransferContantBuffer();
	// LightCameraの定数バッファを送信
	Cygnus::LightCamera::GetInstance()->TransferConstantBuffer();

	// ---------------------------------------------------------
	// シャドウマップ描画前処理
	// ---------------------------------------------------------

	// ライトカメラの更新
	Cygnus::LightCamera::GetInstance()->SetDirectionalLight(Cygnus::LightManager::GetInstance()->directionalLightCB_.data_->direction);

	// シャドウマップ描画開始
	Cygnus::ShadowMapManager::GetInstance()->BeginShadowPass(shadowMapHandle_);

	/// =========================================================
	/// ↓ ここから通常モデルのシャドウマップ描画
	/// =========================================================

	/// =========================================================
	/// ↑ ここまで通常モデルのシャドウマップ描画
	/// =========================================================

	// スキニングモデル用PSOをセット
	cmd->SetPipelineState(Cygnus::ShadowMapManager::GetInstance()->GetShadowSkinnedPSO());

	/// =========================================================
	/// ↓ ここからスキニングモデルのシャドウマップ描画
	/// =========================================================

	/// =========================================================
	/// ↑ ここまでスキニングモデルのシャドウマップ描画
	/// =========================================================

	Cygnus::ShadowMapManager::GetInstance()->EndShadowPass(shadowMapHandle_);

	/// =========================================================
	/// ↓ ここから3Dオブジェクト描画
	/// =========================================================

#pragma region メインシーンの3Dオブジェクトのレンダリングを開始
	/*postEffectManager_->BeginMainScene();*/

	// スカイボックス描画
	Cygnus::SkyBoxManager::GetInstance()->Draw();
	// -----------------------------------------------

	// プレイヤー描画
	objectPlayer_->Draw();

	// -----------------------------------------------
	/*postEffectManager_->EndMainScene();*/
#pragma endregion

#pragma region バックバッファへの直接描画
	/*postEffectManager_->RestoreBackBuffer(true);*/
	// -----------------------------------------------

	// パーティクルエフェクト描画
	Cygnus::ParticleEffectManager::GetInstance()->Draw();
	// ライン描画
	Cygnus::LineDrawer::GetInstance()->Draw();

	// -----------------------------------------------
	/*postEffectManager_->RestoreDepthBufferState();*/
#pragma endregion

	/// =========================================================
	/// ↑ ここまで3Dオブジェクト描画
	/// =========================================================

	// Spriteの描画準備。全ての描画に共通のグラフィックスコマンドを積む
	spriteCommon_->PreDraw();

	/// =========================================================
	/// ↓ ここからスプライト描画
	/// =========================================================

	// タイトルロゴ描画
	spriteTitleLogo_->Draw();
	// スタートボタン描画
	spriteStartButton_->Draw();
	// 終了ボタン描画
	spriteExitButton_->Draw();
	// Aボタン描画
	spriteButtonA_->Draw();

	// フェードトランジション描画
	FadeTransition::GetInstance()->Draw();

	/// =========================================================
	/// ↑ ここまでスプライト描画
	/// =========================================================

#ifdef _DEBUG
	// ゲームシーンデバッグ表示
	Debug();
#endif

	// ImGuiの内部コマンドを生成する
	Cygnus::ImguiWrapper::Render(cmd);
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}

void TitleScene::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("TitleSceneInfo");

	if (ImGui::Button("GAMEPLAY")) {
		Cygnus::SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}
	ImGui::SameLine();
	if (ImGui::Button("RESULT")) {
		Cygnus::SceneManager::GetInstance()->ChangeScene("RESULT");
	}

	ImGui::Text("fps:%.2f", ImGui::GetIO().Framerate);

	ImGui::DragFloat3("camera.translate", &camera_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("camera.rotate", &camera_->transform_.rotate_.x, 0.01f);

	ImGui::Separator();

	ImGui::DragFloat3("Player_Translate", &objectPlayer_->transform_.translate_.x, 0.1f);
	ImGui::DragFloat3("Player_Rotate", &objectPlayer_->transform_.rotate_.x, 0.1f);

	ImGui::End();
#endif
}

void TitleScene::ProcessMenuInput()
{
	// フェード未完了時 または 選択済みならスキップ
	if(!FadeTransition::GetInstance()->IsFinished() || isSelected_) return;

	/* 上下移動入力判定 */
	bool isUp = false;
	bool isDown = false;

	// キーボード入力
	if(input_->TriggerKey(DIK_W) || input_->TriggerKey(DIK_UP)) {
		isUp = true;
	}
	if(input_->TriggerKey(DIK_S) || input_->TriggerKey(DIK_DOWN)) {
		isDown = true;
	}

	// コントローラー入力
	XINPUT_STATE padState;
	if(input_->GetJoystickState(0, padState)) {
		// 十字キー
		if(input_->IsTriggerButton(0, XINPUT_GAMEPAD_DPAD_UP)) isUp = true;
		if(input_->IsTriggerButton(0, XINPUT_GAMEPAD_DPAD_DOWN)) isDown = true;

		// Lスティック Y軸
		float lStickY = padState.Gamepad.sThumbLY / 32767.0f;
		constexpr float kThreshold = 0.5f;

		if(std::abs(lStickY) > kThreshold) {
			if(!isStickMoved_) {
				if(lStickY > kThreshold) isUp = true;
				if(lStickY < -kThreshold) isDown = true;
				isStickMoved_ = true;
			}
		} else {
			isStickMoved_ = false;
		}
	}

	// カーソル切り替え実行
	if(isDown && currentMenu_ == MenuIndex::Start) {
		currentMenu_ = MenuIndex::Exit;
	} else if (isUp && currentMenu_ == MenuIndex::Exit) {
		currentMenu_ = MenuIndex::Start;
	}

	/* 決定入力判定 */
	bool isConfirm = input_->TriggerKey(DIK_SPACE) || input_->TriggerKey(DIK_RETURN) || input_->IsTriggerButton(0, XINPUT_GAMEPAD_A);

	if(isConfirm) {
		isSelected_ = true; // 重複実行を防止

		if(currentMenu_ == MenuIndex::Start) {
			// ゲームプレイシーンへ移行
			FadeTransition::GetInstance()->StartFadeOut(
				1.0f,
				[]() {
					Cygnus::SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
				},
				0.5f
			);
		} else if (currentMenu_ == MenuIndex::Exit) {
			// ゲーム終了処理
			FadeTransition::GetInstance()->StartFadeOut(
				1.0f,
				[]() {
					SendMessage(Cygnus::Window::GetHandle(), WM_CLOSE, 0, 0);
				},
				0.5f
			);
		}
	}
}

void TitleScene::UpdateUI()
{
	// タイマー加算
	uiAnimationTimer_ += Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	/* タイトルロゴの上下浮遊 */
	float logoOffsetY = std::sinf(uiAnimationTimer_ * kLogoFloatSpeed) * kLogoFloatAmplitude;
	spriteTitleLogo_->SetPosition({kTitleLogoInitPos.x, kTitleLogoInitPos.y + logoOffsetY});

	/* 選択中ボタンの拡縮アニメーション */
	// サイン波で拡縮倍率を計算
	float pulseScale = kSelectedBaseRate + std::sinf(uiAnimationTimer_ * kPulseSpeed) * kPulseAmplitude;

	// 選択状態に応じた倍率の設定
	float startRate = (currentMenu_ == MenuIndex::Start) ? pulseScale : kNormalButtonRate;
	float exitRate = (currentMenu_ == MenuIndex::Exit) ? pulseScale : kNormalButtonRate;

	// 元のサイズに倍率を掛けて設定
	spriteStartButton_->SetSize({baseStartButtonSize_.x * startRate, baseStartButtonSize_.y * startRate});
	spriteExitButton_->SetSize({baseExitButtonSize_.x * exitRate, baseExitButtonSize_.y * exitRate});

	// Aボタンアイコンを選択中ボタンの左側に配置
	Cygnus::Float2 targetPos = {};
	if(currentMenu_ == MenuIndex::Start) {
		targetPos = spriteStartButton_->GetPosition();
	} else if (currentMenu_ == MenuIndex::Exit) {
		targetPos = spriteExitButton_->GetPosition();
	}
	// 少し跳ねる動き
	float bounceOffsetY = -std::abs(std::sinf(uiAnimationTimer_ * kButtonABounceSpeed)) * kButtonABounceHeight;

	// 選択中ボタンの左側にバウンドオフセットを加算して配置
	spriteButtonA_->SetPosition({
		targetPos.x + kButtonAOffset.x,
		targetPos.y + kButtonAOffset.y + bounceOffsetY
	});
}

void TitleScene::UpdatePlayerAnimation()
{
	if(!objectPlayer_) return;

	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	// X軸方向移動
	objectPlayer_->transform_.translate_.x += kPlayerMoveSpeed * dt;
	// 右に達したら再度左から開始
	if(objectPlayer_->transform_.translate_.x >= kPlayerEndX) {
		objectPlayer_->transform_.translate_.x = kPlayerStartX;
	}
	
	// Y軸方向の上下振幅
	objectPlayer_->transform_.translate_.y = std::sinf(uiAnimationTimer_ * kPlayerFloatYSpeed) * kPlayerFloatYAmplitude;

	// Z軸回転
	objectPlayer_->transform_.rotate_.z -= kPlayerRotSpeedZ * dt;

	// 拡縮処理
	float cycleProgress = std::fmodf(uiAnimationTimer_, kPlayerPulseCycleTime);

	float currentScale = kPlayerPulseBaseScale;

	// アニメーション時間内のみ拡縮
	if(cycleProgress < kPlayerPulseDuration) {
		// 正規化
		float normalizedTime = cycleProgress / kPlayerPulseDuration;

		// powfで一瞬だけ膨らむように
		float pulseWave = std::powf(std::sinf(normalizedTime * Cygnus::PIf), 3.0f);

		// ベースサイズから最大サイズへ線形補間
		currentScale = kPlayerPulseBaseScale + pulseWave * (kPlayerPulseMaxScale - kPlayerPulseBaseScale);
	}

	// スケールを適用
	objectPlayer_->transform_.scale_ = {currentScale, currentScale, currentScale};
}
