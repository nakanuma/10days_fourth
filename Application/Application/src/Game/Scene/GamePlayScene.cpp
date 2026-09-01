#include "GamePlayScene.h"
#include "DirectXBase.h"
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
#include <Engine/Scene/SceneManager.h>
#include <CommandManager.h>
#include <Collider/CollisionManager.h>

// Application

void GamePlayScene::Initialize() {
	Cygnus::DirectXBase* dxBase = Cygnus::DirectXBase::GetInstance();

	// カメラのインスタンスを生成
	camera_ = std::make_unique<Cygnus::Camera>(Cygnus::Float3{ 0.0f, -12.5f, -80.0f }, Cygnus::Float3{ 0.0f, 0.0f, 0.0f }, 0.45f);
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
	///	↓ ゲームシーン用
	///

	// プレイヤー生成 + 初期化
	player_ = std::make_unique<Player>();
	player_->Initialize();

	// 宇宙船生成 + 初期化
	spaceship_ = std::make_unique<Spaceship>();
	spaceship_->Initialize();

	// 命綱生成 + 初期化
	tether_ = std::make_unique<Tether>();
	tether_->Initialize(spaceship_.get(), player_.get());

	// 飛翔物管理クラス生成 + 初期化
	flyingObjectManager_ = std::make_unique<FlyingObjectManager>();
	flyingObjectManager_->Initialize();
}

void GamePlayScene::Finalize() { }

void GamePlayScene::Update() {
	Cygnus::LightManager::GetInstance()->ClearEmissiveLights(); // エミッシブライトをクリア
	Cygnus::LightManager::GetInstance()->ClearAreaLights();     // エリアライトをクリア
	Cygnus::SkyBoxManager::GetInstance()->Update(); // SkyBox更新

	///
	///	オブジェクト更新処理
	/// 
	
	// プレイヤー更新
	player_->Update();
	// 宇宙船更新
	spaceship_->Update();
	// 命綱更新
	tether_->Update();
	// 飛翔物管理クラス更新
	flyingObjectManager_->Update();

	// 命綱と飛翔物の衝突判定
	tether_->CheckCollisionWithFlyingObjects(flyingObjectManager_.get());

	// カメラの更新処理
	UpdateCamera();

	///
	///	共通更新処理
	/// 
	
	// コリジョンマネージャー更新
	Cygnus::CollisionManager::GetInstance()->Update();
}

void GamePlayScene::Draw() {
	Cygnus::DirectXBase* dxBase = Cygnus::DirectXBase::GetInstance();
	Cygnus::SRVManager* srvManager = Cygnus::SRVManager::GetInstance();
	auto* cmd = Cygnus::CommandManager::GetInstance()->GetCommandList();

	// 描画前処理
	dxBase->PreDraw();
	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = { srvManager->descriptorHeap_.heap_.Get() };
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
	postEffectManager_->BeginMainScene();

	// スカイボックス描画
	Cygnus::SkyBoxManager::GetInstance()->Draw();
	// -----------------------------------------------

	// プレイヤー描画
	player_->Draw();
	// 宇宙船描画
	spaceship_->Draw();
	// 命綱描画
	tether_->Draw();
	// 飛翔物管理クラス描画
	flyingObjectManager_->Draw();

	// -----------------------------------------------
	postEffectManager_->EndMainScene();
#pragma endregion

#pragma region バックバッファへの直接描画
	postEffectManager_->RestoreBackBuffer(true);
	// -----------------------------------------------

	// パーティクルエフェクト描画
	Cygnus::ParticleEffectManager::GetInstance()->Draw();
	// ライン描画
	Cygnus::LineDrawer::GetInstance()->Draw();

	// -----------------------------------------------
	postEffectManager_->RestoreDepthBufferState();
#pragma endregion

	/// =========================================================
	/// ↑ ここまで3Dオブジェクト描画
	/// =========================================================

	// Spriteの描画準備。全ての描画に共通のグラフィックスコマンドを積む
	spriteCommon_->PreDraw();

	/// =========================================================
	/// ↓ ここからスプライト描画
	/// =========================================================



	/// =========================================================
	/// ↑ ここまでスプライト描画
	/// =========================================================

#ifdef _DEBUG
	// ゲームシーンデバッグ表示
	Debug();

	// プレイヤーデバッグ表示
	player_->Debug();
	// 宇宙船デバッグ表示
	spaceship_->Debug();
	// 命綱デバッグ表示
	tether_->Debug();
	// 飛翔物管理クラスデバッグ表示
	flyingObjectManager_->Debug();

	// コライダーデバッグ表示
	Cygnus::CollisionManager::GetInstance()->Debug();
#endif

	// ImGuiの内部コマンドを生成する
	Cygnus::ImguiWrapper::Render(cmd);
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}

void GamePlayScene::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("GamePlaySceneInfo");

	ImGui::Text("fps:%.2f", ImGui::GetIO().Framerate);

	ImGui::DragFloat3("camera.translate", &camera_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("camera.rotate", &camera_->transform_.rotate_.x, 0.01f);

	ImGui::End();
#endif
}

void GamePlayScene::UpdateCamera() {
	if (!camera_ || !player_) return;

	// プレイヤーのY座標を取得
	float playerY = player_->GetTranslate().y;

	// プレイヤーのY座標から進行割合を計算
	float t = 0.0f;
	float rangeY = playerBottomY_ - playerTopY_;
	if (std::abs(rangeY) > 0.0001f) {
		t = (playerY - playerTopY_) / rangeY;
	}

	// 割合tに基づいて目標カメラ座標を線形補間で計算
	Cygnus::Float3 targetCameraPos = Cygnus::Float3::Lerp(cameraTopPos_, cameraBottomPos_, t);

	// 現在のカメラ位置から目標位置へ滑らかに追従移動
	Cygnus::Float3 currentCameraPos = camera_->transform_.translate_;
	camera_->transform_.translate_ = Cygnus::Float3::Lerp(currentCameraPos, targetCameraPos, cameraInterpolation_);
}
