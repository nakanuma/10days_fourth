#include "ResultScene.h"
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

void ResultScene::Initialize()
{
	// 各シーン共通の初期化を呼び出す
	CommonInitialize();

	// カメラの個別設定
	camera_ = std::make_unique<Cygnus::Camera>(Cygnus::Float3{ 0.0f, 80.0f, -60.0f }, Cygnus::Float3{ 1.0f, 0.0f, 0.0f }, 0.45f);
	Cygnus::Camera::Set(camera_.get()); // 現在のカメラをセット

	// UIの生成
	resultSceneUI_ = std::make_unique<ResultSceneUI>();
	resultSceneUI_->Init(spriteCommon_.get());

	// 地面オブジェクトの設定
	objectGround_ = std::make_unique<Cygnus::Object3D>();
	objectGround_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Plane"); // モデル設定
	objectGround_->transform_.rotate_ = { -Cygnus::PIf / 2.0f, 0.0f, 0.0f }; // 上向き
	objectGround_->transform_.scale_ = { 500.0f, 500.0f, 1.0f }; // スケール変更
	objectGround_->materialCB_.data_->color = { 0.5f, 0.5f, 0.5f, 1.0f }; // 色変更
}

void ResultScene::Finalize()
{}

void ResultScene::Update()
{
	// 共通更新処理
	Cygnus::LightManager::GetInstance()->ClearEmissiveLights();
	Cygnus::LightManager::GetInstance()->ClearAreaLights();
	Cygnus::SkyBoxManager::GetInstance()->Update();
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	// 地面オブジェクト更新
	if (objectGround_) objectGround_->UpdateMatrix();

	resultSceneUI_->Update();

	Cygnus::ParticleEffectManager::GetInstance()->Update(dt);
}

void ResultScene::CheckSceneTransition()
{
	if (resultSceneUI_->IsNext())
	{
		Cygnus::SoundManager::GetInstance()->Play("enter");
		TransitionResult();
	}
}

void ResultScene::Debug()
{
#ifdef USE_IMGUI
	ImGui::Begin("ResultSceneInfo");

	ImGui::Text("fps:%.2f", ImGui::GetIO().Framerate);

	if (ImGui::Button("Go To TitleScene"))
	{
		TransitionResult();
	}

	ImGui::End();
#endif

#ifdef USE_IMGUI
	ImGui::Begin("Camera");

	ImGui::DragFloat3("Translate", &camera_->transform_.translate_.x, 0.01f);
	ImGui::DragFloat3("Rotate", &camera_->transform_.rotate_.x, 0.01f);

	ImGui::End();
#endif
}

void ResultScene::TransitionResult()
{
	Cygnus::SceneManager::GetInstance()->ChangeScene("TITLE");
}