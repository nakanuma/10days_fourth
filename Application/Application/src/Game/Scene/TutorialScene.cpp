#include <src/Game/Scene/TutorialScene.h>
#include <SkyBoxManager.h>
#include <SceneManager.h>
#include <ImguiWrapper.h>
#include <LightCamera.h>
#include <CommandManager.h>
#include <ShadowMapManager.h>
#include <ParticleEffect/ParticleEffectManager.h>
#include <LineDrawer.h>
#include <random>

#include <src/Game/Objects/FlyingObject/RepairPart/RepairPartLow/RepairPartLow.h>
#include <src/Game/Objects/FlyingObject/RepairPart/RepairPartHigh/RepairPartHigh.h>
#include <src/Game/Objects/FlyingObject/RepairPart/RepairPartMedium/RepairPartMedium.h>
#include <src/Game/Objects/FlyingObject/Meteor/MeteorLarge/MeteorLarge.h>
#include <src/Game/Objects/FlyingObject/Meteor/MeteorSmall/MeteorSmall.h>
#include <src/Game/Objects/FlyingObject/HeartItem/HeartItem.h>
#include <src/Game/Objects/FlyingObject/BombItem/BombItem.h>

void TutorialScene::Initialize() {
	Cygnus::DirectXBase* dxBase = Cygnus::DirectXBase::GetInstance();

	// カメラ生成
	camera_ = std::make_unique<Cygnus::Camera>(Cygnus::Float3{ 0.0f, -12.5f, -80.0f }, Cygnus::Float3{ 0.0f, 0.0f, 0.0f }, 0.45f);
	Cygnus::Camera::Set(camera_.get());

	// SpriteCommonの生成と初期化
	spriteCommon_ = std::make_unique<Cygnus::SpriteCommon>();
	spriteCommon_->Initialize(Cygnus::DirectXBase::GetInstance());

	// TextureManagerの初期化
	Cygnus::TextureManager::Initialize(dxBase->GetDevice(), Cygnus::SRVManager::GetInstance());

	// Input初期化
	input_ = Cygnus::Input::GetInstance();

	// ライトマネージャの初期化
	lightManager_ = Cygnus::LightManager::GetInstance();
	lightManager_->Initialize();

	// シャドウマップ生成
	shadowMapHandle_ = Cygnus::ShadowMapManager::GetInstance()->CreateShadowMap(Cygnus::Window::GetWidth(), Cygnus::Window::GetHeight());

	// ポストエフェクト管理
	postEffectManager_ = std::make_unique<Cygnus::PostEffectManager>();
	postEffectManager_->Initialize();


	// SkyBoxのパラメーター設定
	Cygnus::SkyBoxManager::GetInstance()->SetTranslate({ 0.0f, 0.0f, 1500.0f });
	Cygnus::SkyBoxManager::GetInstance()->SetRotate({ 0.37f, 1.29f, 0.26f });
	Cygnus::SkyBoxManager::GetInstance()->SetColor({ 0.5f, 0.3f, 1.0f, 1.0f });
	
	Cygnus::ParticleEffectManager::GetInstance()->Clear();

	//スペースシップ
	spaceship_ = std::make_unique<Spaceship>();
	spaceship_->Initialize();
	
	//プレイヤー
	player_ = std::make_unique<Player>();
	player_->Initialize(spaceship_.get(),spriteCommon_.get());
	//カメラシェイク発火用
	player_->SetOnDamageCallback([this](float intensity, float duration) {
		StartCameraShake(intensity, duration);
		});

	// 命綱
	tether_ = std::make_unique<Tether>();
	tether_->Initialize(spaceship_.get() ,player_.get());

	// 飛行物体クラス
	flyingObjectManager_ = std::make_unique<FlyingObjectManager>();
	flyingObjectManager_->Initialize();
	flyingObjectManager_->SetOnDestroyMeteorCallback([this]() {
		StartCameraShake(4.0f, 0.25f);
	});


	// シーンの開始時にフェードインを実行
	FadeTransition::GetInstance()->StartFadeIn(1.0f, 0.5f);

	// BGM再生
	Cygnus::SoundManager::GetInstance()->Play("bgm_tutorial", true, 0.5f);

	// ゲームUI作成
	gameHUD_ = std::make_unique<GameHUD>();
	gameHUD_->Initialize(spriteCommon_.get(), player_.get(), spaceship_.get());
	// プレイヤーのパーツ取得時ポップアップコールバックをセット
	player_->SetOnPickupPartCallback([this](PartType type, const Cygnus::Float3& worldPos) {
		if (gameHUD_) {
			gameHUD_->SpawnPlayerPopup(type, [this]() { return player_->GetTranslate(); });
		}
	});

	explainUI_ = std::make_unique<ExplainUI>();
	explainUI_->Initialize(spriteCommon_.get(),player_.get());
}

void TutorialScene::Finalize() {
	Cygnus::SoundManager::GetInstance()->Stop("bgm_tutorial");
}

void TutorialScene::Update() {
	Cygnus::LightManager::GetInstance()->ClearEmissiveLights(); // エミッシブライトをクリア
	Cygnus::LightManager::GetInstance()->ClearAreaLights();     // エリアライトをクリア
	Cygnus::SkyBoxManager::GetInstance()->Update(); // SkyBox更新
	
	// フェードトランジション更新（ポーズの前で更新）
	FadeTransition::GetInstance()->Update();

	if (FadeTransition::GetInstance()->IsFinished()) {
		// -- チュートリアルをスキップ --
		if (isSkipTutorial_ || explainUI_->IsFinish()) {
			FadeTransition::GetInstance()->StartFadeOut(
				1.0f,
				[]() {
					Cygnus::CollisionManager::GetInstance()->Clear();
					ActionJudgment::GetInstance()->Clear();
					Cygnus::SceneManager::GetInstance()->ChangeScene("GAMEPLAY");
				},
				0.5f
			);
		}
	}

	if (isSkipTutorial_ || explainUI_->IsFinish()) { return; }

	// -- スキップボタン
	if (input_->TriggerKey(DIK_TAB) || input_->IsTriggerButton(0, XINPUT_GAMEPAD_START)) {
		isSkipTutorial_ = true;
	}

	bool wasRewinding = player_->IsRewinding(); // プレイヤーの巻取り状態を保持して更新
	//プレイヤー更新
	player_->Update();

	// 巻取り完了時のUI発火
	if (wasRewinding && !player_->IsRewinding()) {
		// 所持している全パーツの納品用ポップアップをキューへ追加
		if (player_->GetRepairPartLowCount() > 0) {
			gameHUD_->QueueSpaceshipDeposit(PartType::Low, player_->GetRepairPartLowCount());
		}
		if (player_->GetRepairPartMediumCount() > 0) {
			gameHUD_->QueueSpaceshipDeposit(PartType::Medium, player_->GetRepairPartMediumCount());
		}
		if (player_->GetRepairPartHighCount() > 0) {
			gameHUD_->QueueSpaceshipDeposit(PartType::High, player_->GetRepairPartHighCount());
		}


		// インベントリのパーツ連続消費
		if (gameHUD_) {
			gameHUD_->StartConsumingParts();
		}
	}

	//宇宙船更新
	spaceship_->Update();
	//命綱更新
	tether_->Update();

	flyingObjectManager_->Update(false);
	if (player_->IsTriggerBomb()) { // プレイヤーが爆弾アイテムを取得したら一括隕石破壊
		flyingObjectManager_->DestroyAllMeteorsSequential();
	}

	AddFlyObject();

	// 命綱と飛翔物の衝突判定
	tether_->CheckCollisionWithFlyingObjects(flyingObjectManager_.get());

	// カメラの更新処理
	UpdateCamera();

	// ゲームUI更新
	float remainingTime = kMaxGameTime - gameTimer_;
	gameHUD_->Update(remainingTime, tether_.get(), flyingObjectManager_.get());

	//説明UIの更新
	explainUI_->Update(player_.get());

	// コリジョンマネージャー更新
	Cygnus::CollisionManager::GetInstance()->Update();

	// パーティクルエフェクト管理クラス更新
	Cygnus::ParticleEffectManager::GetInstance()->Update(Cygnus::TimeManager::GetInstance()->GetDeltaTime());
}

void TutorialScene::Draw() {
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

	// プレイヤーUI描画
	player_->DrawUI();

	gameHUD_->Draw();
	explainUI_->Draw();

	// フェードトランジション描画
	FadeTransition::GetInstance()->Draw();

	/// =========================================================
	/// ↑ ここまでスプライト描画
	/// =========================================================

#ifdef _DEBUG
	// ゲームシーンデバッグ表示
	//Debug();

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
	// スカイボックスデバッグ表示
	Cygnus::SkyBoxManager::GetInstance()->Debug();
#endif

	// ImGuiの内部コマンドを生成する
	Cygnus::ImguiWrapper::Render(cmd);
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}

void TutorialScene::UpdateCamera() {
	if (!camera_ || !player_) return;

	float deltaTime = Cygnus::TimeManager::GetInstance()->GetDeltaTime();
	Cygnus::Float3 playerPos = player_->GetTranslate();

	/* 基準となる高さ（Y座標）に応じた引きカメラの位置計算 */
	float tBaseY = 0.0f;
	float rangeY = playerBottomY_ - playerTopY_;
	if (std::abs(rangeY) > 0.0001f) {
		tBaseY = (std::clamp)((playerPos.y - playerTopY_) / rangeY, 0.0f, 1.0f);
	}
	Cygnus::Float3 baseCameraPos = Cygnus::Float3::Lerp(cameraTopPos_, cameraBottomPos_, tBaseY);

	/* 上下左右への移動に伴うわずかなカメラシフト & 回転計算 */
	// X軸の正規化割合
	float tX = (std::clamp)(playerPos.x / playerLimitX_, -1.0f, 1.0f);

	// Y軸の正規化割合
	float tY = (std::clamp)(playerPos.y / playerLimitY_, -1.0f, 1.0f);

	// 移動オフセット（右に行けば+X, 上に行けば+Y へわずかにカメラをずらす）
	Cygnus::Float3 targetPosOffset = {
		tX * maxCameraShift_.x,
		tY * maxCameraShift_.y,
		0.0f
	};

	// 回転角度
	Cygnus::Float3 targetRotate = {
		-tY * maxCameraAngle_.x,
		tX * maxCameraAngle_.y,
		0.0f
	};

	/* 被弾時のカメラシェイク（徐々に減衰） */
	Cygnus::Float3 shakeOffset = { 0.0f, 0.0f, 0.0f };

	if (shakeTimer_ > 0.0f) {
		shakeTimer_ -= deltaTime;

		float decay = (std::clamp)(shakeTimer_ / shakeDuration_, 0.0f, 1.0f);
		float currentIntensity = shakeIntensity_ * decay;

		static std::random_device rd;
		static std::mt19937 gen(rd());
		std::uniform_real_distribution<float> dist(-1.0f, 1.0f);

		shakeOffset.x = dist(gen) * currentIntensity;
		shakeOffset.y = dist(gen) * currentIntensity;

		if (shakeTimer_ <= 0.0f) {
			shakeTimer_ = 0.0f;
		}
	}

	/* カメラ座標および角度の線形補間適用 */
	Cygnus::Float3 finalTargetPos = {
		baseCameraPos.x + targetPosOffset.x + shakeOffset.x,
		baseCameraPos.y + targetPosOffset.y + shakeOffset.y,
		baseCameraPos.z + targetPosOffset.z
	};

	// 計算結果をカメラに適用
	camera_->transform_.translate_ = Cygnus::Float3::Lerp(camera_->transform_.translate_, finalTargetPos, cameraInterpolation_);
	camera_->transform_.rotate_ = Cygnus::Float3::Lerp(camera_->transform_.rotate_, targetRotate, cameraInterpolation_);

}


void TutorialScene::StartCameraShake(float intensity, float duration) {
	shakeIntensity_ = intensity;
	shakeDuration_ = duration;
	shakeTimer_ = duration;
}


void TutorialScene::AddFlyObject() {
	spawnTime_ += Cygnus::TimeManager::GetInstance()->GetDeltaTime();
	if (spawnTime_ >= kSpawnMaxTime_) {
		spawnTime_ = 0.0f;
	}
	else {
		return;//時間になるまでスポーンさせない
	}
	// 常にスポーン
	flyingObjectManager_->SpawnRepairPartLow();

	//説明3(黄色パーツを取ったら)スポーンする
	if (ActionJudgment::GetInstance()->NowExplain() >= 3) {
		flyingObjectManager_->SpawnRepairPartMedium();
		flyingObjectManager_->SpawnRepairPartHigh();
	}

	//説明4(紫パーツを取ったら)スポーンする
	if (ActionJudgment::GetInstance()->NowExplain() >= 4) {
		flyingObjectManager_->SpawnMeteorSmall();
		flyingObjectManager_->SpawnMeteorLarge();
		flyingObjectManager_->SpawnHeartItem();
		flyingObjectManager_->SpawnBombItem();
	}
}



