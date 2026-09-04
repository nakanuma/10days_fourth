#include "DirectXBase.h"
#include "ImguiWrapper.h"
#include "RTVManager.h"
#include "SRVManager.h"
#include "SpriteCommon.h"
#include "ResultScene.h"
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

// Application

void ResultScene::Initialize() {
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
	
	// ゲーム結果の取得
	result_ = GameResultManager::GetResult();

	// 初期選択状態のリセット
	currentMenu_ = MenuIndex::Retry;
	isStickMoved_ = false;
	isSelected_ = false;

	/* 3Dオブジェクト生成 */

	// プレイヤー生成
	objectPlayer_ = std::make_unique<Cygnus::Object3D>();
	objectPlayer_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Player");

	// 宇宙船生成
	objectSpaceship_ = std::make_unique<Cygnus::Object3D>();
	objectSpaceship_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Spaceship");

	// 勝敗に応じた初期化処理
	if (result_ == GameResult::Clear) {
		// SkyBoxのパラメーター設定
		Cygnus::SkyBoxManager::GetInstance()->SetTranslate({ 0.0f, 0.0f, 1500.0f });
		Cygnus::SkyBoxManager::GetInstance()->SetRotate({ -0.31f, 2.31f, 0.31f });
		Cygnus::SkyBoxManager::GetInstance()->SetColor({ 0.65f, 1.0f, 1.0f, 1.0f });

		// 宇宙船の初期トランスフォーム設定と基準保存
		spaceshipClearInitPos_ = {0.0f, 0.0f, 0.0f};
		spaceshipClearInitRot_ = {0.0f, 0.0f, 0.0f};

		// プレイヤーは宇宙船の子オブジェクトとして配置
		objectPlayer_->SetParent(objectSpaceship_.get());
		objectPlayer_->transform_.translate_ = {-0.910f, 0.44f, 0.0f};
		objectPlayer_->transform_.rotate_ = {0.0f, 0.0f, -1.57f};
	} else {
		// SkyBoxのパラメーター設定
		Cygnus::SkyBoxManager::GetInstance()->SetTranslate({ 0.0f, 0.0f, 1500.0f });
		Cygnus::SkyBoxManager::GetInstance()->SetRotate({ -3.04, 1.39f, 0.29f });
		Cygnus::SkyBoxManager::GetInstance()->SetColor({ 0.1f, 0.08f, 1.0f, 1.0f });

		playerGameOverInitPos_ = {0.0f, 0.0f, 0.0f};
		objectPlayer_->transform_.translate_ = playerGameOverInitPos_;
		objectPlayer_->transform_.rotate_ = {-1.18f, 0.72f, 0.0f};
	}

	/* スプライト生成 */

	// ゲームクリアテキスト
	uint32_t texGameClear = Cygnus::TextureManager::Load("result_gameClearText.png");
	spriteGameClearText_ = std::make_unique<Cygnus::Sprite>();
	spriteGameClearText_->Initialize(spriteCommon_.get(), texGameClear);
	spriteGameClearText_->SetPosition(kResultTextInitPos);
	spriteGameClearText_->SetAnchorPoint({ 0.5f, 0.5f });

	// ゲームオーバーテキスト
	uint32_t texGameOver = Cygnus::TextureManager::Load("result_gameOverText.png");
	spriteGameOverText_ = std::make_unique<Cygnus::Sprite>();
	spriteGameOverText_->Initialize(spriteCommon_.get(), texGameOver);
	spriteGameOverText_->SetPosition(kResultTextInitPos);
	spriteGameOverText_->SetAnchorPoint({ 0.5f, 0.5f });

	// リトライボタン
	uint32_t texRetryButton = Cygnus::TextureManager::Load("result_retryButton.png");
	spriteRetryButton_ = std::make_unique<Cygnus::Sprite>();
	spriteRetryButton_->Initialize(spriteCommon_.get(), texRetryButton);
	spriteRetryButton_->SetPosition(kRetryButtonInitPos);
	spriteRetryButton_->SetAnchorPoint({ 0.5f, 0.5f });
	baseRetryButtonSize_ = spriteRetryButton_->GetSize();

	// タイトルへ戻るボタン
	uint32_t texReturnButton = Cygnus::TextureManager::Load("result_returnButton.png");
	spriteReturnButton_ = std::make_unique<Cygnus::Sprite>();
	spriteReturnButton_->Initialize(spriteCommon_.get(), texReturnButton);
	spriteReturnButton_->SetPosition(kReturnButtonInitPos);
	spriteReturnButton_->SetAnchorPoint({ 0.5f, 0.5f });
	baseReturnButtonSize_ = spriteReturnButton_->GetSize();

	// Aボタン
	uint32_t texButtonA = Cygnus::TextureManager::Load("button_a.png");
	spriteButtonA_ = std::make_unique<Cygnus::Sprite>();
	spriteButtonA_->Initialize(spriteCommon_.get(), texButtonA);
	spriteButtonA_->SetAnchorPoint({ 0.5f, 0.5f });
	baseButtonASize_ = spriteButtonA_->GetSize();



	// シーンの開始時にフェードインを実行
	FadeTransition::GetInstance()->StartFadeIn(1.0f, 0.5f);
}

void ResultScene::Finalize() {}

void ResultScene::Update() {
	Cygnus::LightManager::GetInstance()->ClearEmissiveLights(); // エミッシブライトをクリア
	Cygnus::LightManager::GetInstance()->ClearAreaLights();     // エリアライトをクリア
	Cygnus::SkyBoxManager::GetInstance()->Update();             // SkyBox更新

	// メニュー入力処理
	ProcessMenuInput();

	// UI見た目・アニメーション更新
	UpdateUI();

	// ゲーム結果に応じた固有処理
	if(result_ == GameResult::Clear) {
		UpdateGameClear();
	} else if (result_ == GameResult::GameOver) {
		UpdateGameOver();
	}

	///
	///	オブジェクト更新処理
	///

	///
	///	スプライト更新処理
	///

	if(result_ == GameResult::Clear) {
		spriteGameClearText_->Update();
	} else {
		spriteGameOverText_->Update();
	}

	spriteRetryButton_->Update();
	spriteReturnButton_->Update();
	spriteButtonA_->Update();

	// フェードトランジション更新
	FadeTransition::GetInstance()->Update();

	///
	///	共通更新処理
	///

	// コリジョンマネージャー更新
	Cygnus::CollisionManager::GetInstance()->Update();
}

void ResultScene::Draw() {
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
	// 宇宙船描画
	objectSpaceship_->Draw();

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

	// 勝敗に応じたテキストロゴの描画
	if(result_ == GameResult::Clear) {
		spriteGameClearText_->Draw();
	} else {
		spriteGameOverText_->Draw();
	}

	// ボタンUIの描画
	spriteRetryButton_->Draw();
	spriteReturnButton_->Draw();
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

void ResultScene::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("ResultSceneInfo");

	if (result_ == GameResult::Clear) {
		ImGui::Text("GameClear");
	} else if (result_ == GameResult::GameOver) {
		ImGui::Text("GameOver");
	} else {
		ImGui::Text("None");
	}

	if (ImGui::Button("TITLE")) {
		Cygnus::SceneManager::GetInstance()->ChangeScene("TITLE");
	}
	ImGui::SameLine();
	if (ImGui::Button("GAMEPLAY")) {
		Cygnus::SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
	}

	ImGui::Text("fps:%.2f", ImGui::GetIO().Framerate);

	ImGui::DragFloat3("camera.translate", &camera_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("camera.rotate", &camera_->transform_.rotate_.x, 0.01f);

	ImGui::Separator();
	ImGui::DragFloat3("player.translate", &objectPlayer_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("player.rotate", &objectPlayer_->transform_.rotate_.x, 0.01f);

	ImGui::Separator();
	ImGui::DragFloat3("spaceship.translate", &objectSpaceship_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("spaceship.rotate", &objectSpaceship_->transform_.rotate_.x, 0.01f);

	ImGui::End();
#endif
}

void ResultScene::ProcessMenuInput() {
	// フェード未完了時 または 選択済みならスキップ
	if (!FadeTransition::GetInstance()->IsFinished() || isSelected_) return;

	/* 上下移動入力判定 */
	bool isUp = false;
	bool isDown = false;

	// キーボード入力
	if (input_->TriggerKey(DIK_W) || input_->TriggerKey(DIK_UP)) {
		isUp = true;
	}
	if (input_->TriggerKey(DIK_S) || input_->TriggerKey(DIK_DOWN)) {
		isDown = true;
	}

	// コントローラー入力
	XINPUT_STATE padState;
	if (input_->GetJoystickState(0, padState)) {
		// 十字キー
		if (input_->IsTriggerButton(0, XINPUT_GAMEPAD_DPAD_UP)) isUp = true;
		if (input_->IsTriggerButton(0, XINPUT_GAMEPAD_DPAD_DOWN)) isDown = true;

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

	// カーソル切り替え実行
	if (isDown && currentMenu_ == MenuIndex::Retry) {
		currentMenu_ = MenuIndex::ReturnTitle;
	} else if (isUp && currentMenu_ == MenuIndex::ReturnTitle) {
		currentMenu_ = MenuIndex::Retry;
	}

	/* 決定入力判定 */
	bool isConfirm = input_->TriggerKey(DIK_SPACE) || input_->TriggerKey(DIK_RETURN) || input_->IsTriggerButton(0, XINPUT_GAMEPAD_A);

	if (isConfirm) {
		isSelected_ = true; // 重複実行を防止

		if (currentMenu_ == MenuIndex::Retry) {
			// ゲームプレイシーンへ移行
			FadeTransition::GetInstance()->StartFadeOut(
				1.0f,
				[]() {
					Cygnus::SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
				},
				0.5f
			);
		} else if (currentMenu_ == MenuIndex::ReturnTitle) {
			// タイトルシーンへ移行
			FadeTransition::GetInstance()->StartFadeOut(
				1.0f,
				[]() {
					Cygnus::SceneManager::GetInstance()->ChangeScene("TITLE");
				},
				0.5f
			);
		}
	}
}

void ResultScene::UpdateUI() {
	// タイマー加算
	uiAnimationTimer_ += Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	/* タイトルロゴの上下浮遊 */
	float logoOffsetY = std::sinf(uiAnimationTimer_ * kTextFloatSpeed) * kTextFloatAmplitude;
	Cygnus::Float2 textPos = {kResultTextInitPos.x, kResultTextInitPos.y + logoOffsetY};

	if(result_ == GameResult::Clear) {
		spriteGameClearText_->SetPosition(textPos);
	} else {
		spriteGameOverText_->SetPosition(textPos);
	}

	/* 選択中ボタンの拡縮アニメーション */
	// サイン波で拡縮倍率を計算
	float pulseScale = kSelectedBaseRate + std::sinf(uiAnimationTimer_ * kPulseSpeed) * kPulseAmplitude;

	// 選択状態に応じた倍率の設定
	float retryRate = (currentMenu_ == MenuIndex::Retry) ? pulseScale : kNormalButtonRate;
	float returnRate = (currentMenu_ == MenuIndex::ReturnTitle) ? pulseScale : kNormalButtonRate;

	// 元のサイズに倍率を掛けて設定
	spriteRetryButton_->SetSize({ baseRetryButtonSize_.x * retryRate, baseRetryButtonSize_.y * retryRate });
	spriteReturnButton_->SetSize({ baseReturnButtonSize_.x * returnRate, baseReturnButtonSize_.y * returnRate });

	// Aボタンアイコンを選択中ボタンの左側に配置
	Cygnus::Float2 targetPos = {};
	Cygnus::Float2 currentOffset = {};

	if (currentMenu_ == MenuIndex::Retry) {
		targetPos = spriteRetryButton_->GetPosition();
		currentOffset = kButtonAOffsetRetry;
	} else if (currentMenu_ == MenuIndex::ReturnTitle) {
		targetPos = spriteReturnButton_->GetPosition();
		currentOffset = kButtonAOffsetReturn;
	}
	// 少し跳ねる動き
	float bounceOffsetY = -std::abs(std::sinf(uiAnimationTimer_ * kButtonABounceSpeed)) * kButtonABounceHeight;

	// 選択中ボタンの左側にバウンドオフセットを加算して配置
	spriteButtonA_->SetPosition({
		targetPos.x + currentOffset.x,
		targetPos.y + currentOffset.y + bounceOffsetY
		});
}

void ResultScene::UpdateGameClear() {
	// sin波による浮遊演出
	float sinVal = std::sinf(uiAnimationTimer_ * kSpaceshipFloatSpeed);

	objectSpaceship_->transform_.translate_.y = spaceshipClearInitPos_.y + sinVal * kSpaceshipFloatYAmplitude;
	objectSpaceship_->transform_.rotate_.z = spaceshipClearInitRot_.z + sinVal * kSpaceshipFloatZAmplitude;

	// プレイヤー更新
	objectPlayer_->UpdateMatrix();
	// 宇宙船更新
	objectSpaceship_->UpdateMatrix();
}

void ResultScene::UpdateGameOver() {
	// サイン波で漂いアニメーション
	float floatOffsetY = std::sinf(uiAnimationTimer_ * kPlayerFloatYSpeed) * kPlayerFloatYAmplitude;
	objectPlayer_->transform_.translate_.y = playerGameOverInitPos_.y + floatOffsetY;

	// プレイヤー更新
	objectPlayer_->UpdateMatrix();
}
