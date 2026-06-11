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
#include "SelectSceneState.h"
#include <src/Game/GameData/GameDataManager.h>

void StageSelectScene::Initialize()
{
	// 各シーン共通の初期化を呼び出す
	CommonInitialize();
	cameraPosition_ = kCameraInitialPosition; // カメラの初期位置
	// カメラの個別設定
	camera_ = std::make_unique<Cygnus::Camera>(cameraPosition_, Cygnus::Float3{ 1.0f, 0.0f, 0.0f }, 0.45f);
	Cygnus::Camera::Set(camera_.get()); // 現在のカメラをセット


	// UIの生成
	stageSelectSceneUI_ = std::make_unique<StageSelectSceneUI>();
	stageSelectSceneUI_->Init(spriteCommon_.get());

	// 地面オブジェクトの設定
	objectGround_ = std::make_unique<Cygnus::Object3D>();
	objectGround_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Plane"); // モデル設定
	objectGround_->transform_.rotate_ = { -Cygnus::PIf / 2.0f, 0.0f, 0.0f }; // 上向き
	objectGround_->transform_.scale_ = { 500.0f, 500.0f, 1.0f }; // スケール変更
	objectGround_->materialCB_.data_->color = { 0.85f, 0.625f, 0.4f, 1.0f }; // 色変更

	// 固有オブジェクトの生成
	selectObjects_ = std::make_unique<SelectObjectManager>();
	selectObjects_->Initialize();

	// 3. ステートマシンの初期化とステートの登録
	stateMachine_.RegisterState<FadeInState>(SelectSceneState::FADE_IN, "FadeIn");
	stateMachine_.RegisterState<SelectingState>(SelectSceneState::SELECTING, "Selecting");
	stateMachine_.RegisterState<MovingState>(SelectSceneState::MOVING, "Moving");
	stateMachine_.RegisterState<FadeOutState>(SelectSceneState::FADE_OUT, "FadeOut");

	// 最初の状態をフェードインに設定
	stateMachine_.ChangeState(SelectSceneState::FADE_IN);

	GameDataManager::GetInstance()->SetTargetStage(0);
	Cygnus::SoundManager::GetInstance()->Play("sandstorm", true, 0.5f);
}

void StageSelectScene::Finalize()
{
	Cygnus::SoundManager::GetInstance()->Stop("sandstorm");
}

void StageSelectScene::Update()
{
	// 共通更新処理
	Cygnus::LightManager::GetInstance()->ClearEmissiveLights();
	Cygnus::LightManager::GetInstance()->ClearAreaLights();
	Cygnus::SkyBoxManager::GetInstance()->Update();
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	// 地面オブジェクト更新
	if (objectGround_) objectGround_->UpdateMatrix();

	selectObjects_->Update(dt);
	stageSelectSceneUI_->Update();

	if(stateMachine_.GetCurrentState() !=  SelectSceneState::FADE_OUT)
	{
		selectObjects_->SetPlayerObjectPosX(camera_->transform_.translate_.x);
	}

	Cygnus::ParticleEffectManager::GetInstance()->Update(dt);

	stateMachine_.UpdateState(*this, dt);
}

void StageSelectScene::CheckSceneTransition()
{
	if (IsTransition())
	{
		TransitionGamePlay();
	}
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

	stateMachine_.DebugImGui("SelectState");
}

void StageSelectScene::TransitionGamePlay()
{
	Cygnus::SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
}