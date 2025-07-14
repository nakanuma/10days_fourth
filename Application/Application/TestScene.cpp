#include "TestScene.h"
#include "ImguiWrapper.h" 
#include "DirectXBase.h"
#include "RTVManager.h"
#include "SRVManager.h"
#include "SpriteCommon.h"

// C++
#include <numbers>

void TestScene::Initialize() {
	DirectXBase* dxBase = DirectXBase::GetInstance();

	// カメラのインスタンスを生成
	camera = std::make_unique<Camera>(Float3{0.0f, 15.0f, -40.0f}, Float3{0.3f, 0.0f, 0.0f}, 0.45f);
	Camera::Set(camera.get()); // 現在のカメラをセット

	// SpriteCommonの生成と初期化
	spriteCommon = std::make_unique<SpriteCommon>();
	spriteCommon->Initialize(DirectXBase::GetInstance());

	// TextureManagerの初期化
	TextureManager::Initialize(dxBase->GetDevice(), SRVManager::GetInstance());

	// SoundManagerの初期化
	soundManager = std::make_unique<SoundManager>();
	soundManager->Initialize();

	// Inputの初期化
	input = Input::GetInstance();

	// LightManagerの初期化
	lightManager = LightManager::GetInstance();
	lightManager->Initialize();

	///
	///	↓ ゲームシーン用
	///

	// Texture読み込み
	uint32_t uvCheckerGH = TextureManager::Load("resources/Images/grass.png", dxBase->GetDevice());

	// モデルの読み込みとテクスチャの設定
	model_ = ModelManager::LoadModelFile("resources/Models", "terrain.obj", dxBase->GetDevice());
	model_.material.textureHandle = uvCheckerGH;

	// オブジェクトの生成とモデル設定
	object_ = std::make_unique<Object3D>();
	object_->model_ = &model_;
	object_->transform_.rotate = {0.0f, std::numbers::pi_v<float> * 1.5f, 0.0f};
	object_->materialCB_.data_->color = {1.0f, 1.0f, 1.0, 1.0f};

	///
	///	ポストエフェクト
	/// 
	
	postEffectManager_ = std::make_unique<PostEffectManager>();
	postEffectManager_->Initialize();
}

void TestScene::Finalize() {}

void TestScene::Update() { 
	object_->UpdateMatrix(); 
}

void TestScene::Draw() {
	DirectXBase* dxBase = DirectXBase::GetInstance();
	SRVManager* srvManager = SRVManager::GetInstance();

	// 描画前処理
	dxBase->PreDraw();
	// 描画用のDescriptorHeapの設定
	ID3D12DescriptorHeap* descriptorHeaps[] = {srvManager->descriptorHeap.heap_.Get()};
	dxBase->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);
	// ImGuiのフレーム開始処理
	ImguiWrapper::NewFrame();
	// カメラの定数バッファを設定
	Camera::TransferConstantBuffer();
	// ライトの定数バッファを設定
	lightManager->TransferContantBuffer();

	#ifdef _DEBUG
	ImGuiUtil::ImageWindow("rendertexture", postEffectManager_->GetRenderTextureHandle());
	#endif

	///
	///	↓ ここから3Dオブジェクトの描画コマンド
	///

	// レンダーターゲットをレンダーテクスチャにセット
	postEffectManager_->BeginRenderToTexture();

	// オブジェクトの描画
	object_->Draw();

	// ポストエフェクト適用
	postEffectManager_->ApplyEffect();

	///
	///	↑ ここまで3Dオブジェクトの描画コマンド
	///

	// Spriteの描画準備。全ての描画に共通のグラフィックスコマンドを積む
	spriteCommon->PreDraw();

	///
	/// ↓ ここからスプライトの描画コマンド
	///

	///
	/// ↑ ここまでスプライトの描画コマンド
	///

#ifdef _DEBUG
	ImGui::Begin("window");

	ImGui::Text("fps : %.2f", ImGui::GetIO().Framerate);

	if (ImGui::Button("RadialBlur")) {
		postEffectManager_->SetEffectType(PostEffectType::RadialBlur);
	}
	if (ImGui::Button("GrayScale")) {
		postEffectManager_->SetEffectType(PostEffectType::GrayScale);
	}
	if (ImGui::Button("Vignette")) {
		postEffectManager_->SetEffectType(PostEffectType::Vignette);
	}
	if (ImGui::Button("BoxFilter")) {
		postEffectManager_->SetEffectType(PostEffectType::BoxFilter);
	}
	if (ImGui::Button("GaussianFilter")) {
		postEffectManager_->SetEffectType(PostEffectType::GaussianFilter);
	}

	ImGui::End();
#endif // _DEBUG

	// ImGuiの内部コマンドを生成する
	ImguiWrapper::Render(dxBase->GetCommandList());
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}
