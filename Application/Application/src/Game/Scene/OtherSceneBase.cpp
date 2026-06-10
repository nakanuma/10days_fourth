#include "OtherSceneBase.h"

void OtherSceneBase::CommonInitialize()
{
	// 必要なシステムを取得
	Cygnus::DirectXBase* dxBase = Cygnus::DirectXBase::GetInstance();

	input_ = Cygnus::Input::GetInstance();

	// ライトの初期化
	lightManager_ = Cygnus::LightManager::GetInstance();
	lightManager_->Initialize();

	// SpriteCommonの生成と初期化
	spriteCommon_ = std::make_unique<Cygnus::SpriteCommon>();
	spriteCommon_->Initialize(dxBase);

	// TextureManagerの初期化
	Cygnus::TextureManager::Initialize(dxBase->GetDevice(), Cygnus::SRVManager::GetInstance());

	// シャドウマップ生成
	shadowMapHandle_ = Cygnus::ShadowMapManager::GetInstance()->CreateShadowMap(Cygnus::Window::GetWidth(), Cygnus::Window::GetHeight());

	// ポストエフェクト管理クラスの生成と初期化
	postEffectManager_ = std::make_unique<Cygnus::PostEffectManager>();
	postEffectManager_->Initialize();
}

void OtherSceneBase::Draw()
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

	/// ↓ 通常モデルのシャドウマップ描画
	DrawShadowNormal(cmd); // 各シーン固有の通常モデル描画

	// スキニングモデル用PSOをセット
	cmd->SetPipelineState(Cygnus::ShadowMapManager::GetInstance()->GetShadowSkinnedPSO());

	/// ↓ スキニングモデルのシャドウマップ描画
	DrawShadowSkinned(cmd); // 各シーン固有のスキニングモデル描画

	Cygnus::ShadowMapManager::GetInstance()->EndShadowPass(shadowMapHandle_);

	/// ↓ 3Dオブジェクト描画

#pragma region
	postEffectManager_->BeginMainScene();

	// スカイボックス描画
	Cygnus::SkyBoxManager::GetInstance()->Draw();
	// -----------------------------------------------

	// 地面オブジェクト描画
	if (objectGround_) objectGround_->Draw();

	// シーン固有のメインシーンオブジェクト描画
	DrawMainScene3D();

	// -----------------------------------------------
	postEffectManager_->EndMainScene();
#pragma endregion

#pragma region バックバッファへの直接描画
	postEffectManager_->RestoreBackBuffer(true);
	// -----------------------------------------------

	// シーン固有のバックバッファ直接描画（プレイヤーなどがあれば）
	DrawBackBuffer3D();

	// パーティクルエフェクト描画
	Cygnus::ParticleEffectManager::GetInstance()->Draw();
	// ライン描画
	Cygnus::LineDrawer::GetInstance()->Draw();

	// -----------------------------------------------
	postEffectManager_->RestoreDepthBufferState();
#pragma endregion

	// Spriteの描画準備。全ての描画に共通のグラフィックスコマンドを積む
	spriteCommon_->PreDraw();

	/// ↓ スプライト描画
	DrawUI(); // 各シーン固有のUI描画

#ifdef _DEBUG // デバッグ表示
	Cygnus::CollisionManager::GetInstance()->Draw();
	Debug();
#endif

	// シーンチェンジ・画面遷移チェック
	CheckSceneTransition();

	// ImGuiの内部コマンドを生成する
	Cygnus::ImguiWrapper::Render(cmd);
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}