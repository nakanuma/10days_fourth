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

	// タイトルの仮画像生成
	uint32_t textureTitle = Cygnus::TextureManager::Load("title_temp.png");
	spriteTitle_ = std::make_unique<Cygnus::Sprite>();
	spriteTitle_->Initialize(spriteCommon_.get(), textureTitle);

	// シーンの開始時にフェードインを実行
	FadeTransition::GetInstance()->StartFadeIn(1.0f, 0.5f);
}

void TitleScene::Finalize() {}

void TitleScene::Update() {
	Cygnus::LightManager::GetInstance()->ClearEmissiveLights(); // エミッシブライトをクリア
	Cygnus::LightManager::GetInstance()->ClearAreaLights();     // エリアライトをクリア
	Cygnus::SkyBoxManager::GetInstance()->Update();             // SkyBox更新

	// ここにゲームシーンへ移行する条件を記入（現在は仮でスペースキー入力）
	if (Cygnus::Input::GetInstance()->TriggerKey(DIK_SPACE) && FadeTransition::GetInstance()->IsFinished()) {
		FadeTransition::GetInstance()->StartFadeOut(
			1.0f, 
			[]() {
			    Cygnus::SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
		    }, 
			0.5f
		);
	}

	///
	///	オブジェクト更新処理
	///

	// タイトルの仮画像更新
	spriteTitle_->Update();

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

	// タイトルの仮画像描画
	spriteTitle_->Draw();

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

	ImGui::End();
#endif
}
