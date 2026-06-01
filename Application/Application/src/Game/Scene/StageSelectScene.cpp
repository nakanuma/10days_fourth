#include "StageSelectScene.h"
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

// Application
#include <src/Game/Scene/SceneFactory.h>

void StageSelectScene::Initialize()
{
	Cygnus::DirectXBase* dxBase = Cygnus::DirectXBase::GetInstance();

	// カメラのインスタンスを生成
	camera_ = std::make_unique<Cygnus::Camera>(Cygnus::Float3{ 0.0f, 80.0f, -45.0f }, Cygnus::Float3{ 1.0f, 0.0f, 0.0f }, 0.45f);
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

	stageSelectSceneUI_ = std::make_unique<StageSelectSceneUI>();
	stageSelectSceneUI_->Init(spriteCommon_.get());

	// 地面オブジェクト生成
	objectGround_ = std::make_unique<Cygnus::Object3D>();
	objectGround_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Plane"); // モデル設定
	objectGround_->transform_.rotate_ = { -Cygnus::PIf / 2.0f, 0.0f, 0.0f }; // 上向き
	objectGround_->transform_.scale_ = { 500.0f, 500.0f, 1.0f }; // スケール変更
	objectGround_->materialCB_.data_->color = { 0.5f, 0.5f, 0.5f, 1.0f }; // 色変更

	selectObjects_ = std::make_unique<SelectObjectManager>();
	selectObjects_->Initialize();

}

void StageSelectScene::Finalize()
{}

void StageSelectScene::Update()
{
	///
	///	共通更新処理
	/// 
	Cygnus::LightManager::GetInstance()->ClearEmissiveLights(); // エミッシブライトをクリア
	Cygnus::LightManager::GetInstance()->ClearAreaLights();     // エリアライトをクリア
	Cygnus::SkyBoxManager::GetInstance()->Update(); // SkyBox更新
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();	// デルタタイム取得


	///
	///	オブジェクト更新処理
	/// 

	// 地面オブジェクト更新
	objectGround_->UpdateMatrix();

	selectObjects_->Update(dt);

	stageSelectSceneUI_->Update();	// ステージセレクトシーンUI更新
	///
	///	
	/// 

	Cygnus::ParticleEffectManager::GetInstance()->Update(dt);	// パーティクルエフェクト管理クラス更新
}

void StageSelectScene::Draw()
{
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

#pragma region
	postEffectManager_->BeginMainScene();

	// スカイボックス描画
	Cygnus::SkyBoxManager::GetInstance()->Draw();
	// -----------------------------------------------

	// 地面オブジェクト描画
	objectGround_->Draw();


	selectObjects_->Draw();

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

	stageSelectSceneUI_->Draw();

	/// =========================================================
	/// ↑ ここまでスプライト描画
	/// =========================================================

#ifdef _DEBUG // デバッグ表示

	// ゲームシーン
	Debug();

#endif
	// シーンチェンジは描画処理後に行う
	if (stageSelectSceneUI_->IsStart() && selectObjects_->GetCurrentStage() == 0/*今は初期ステージのみ*/)
	{
		TransitionGamePlay();
	}

	// ImGuiの内部コマンドを生成する
	Cygnus::ImguiWrapper::Render(cmd);
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}

void StageSelectScene::Debug()
{
#ifdef USE_IMGUI
	ImGui::Begin("StageSelectSceneInfo");

	ImGui::Text("fps:%.2f", ImGui::GetIO().Framerate);

	if (ImGui::Button("GamePlayScene"))
	{
		TransitionGamePlay();
	}

	ImGui::End();
#endif

#ifdef USE_IMGUI
	ImGui::Begin("Camera");

	ImGui::DragFloat3("Translate", &camera_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &camera_->transform_.rotate_.x, 0.01f);

	ImGui::End();
#endif
	selectObjects_->Debug();
}

void StageSelectScene::TransitionGamePlay()
{
	Cygnus::SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
}