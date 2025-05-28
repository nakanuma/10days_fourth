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
	///	レンダーテクスチャ関連
	/// 

	// レンダーテクスチャ生成
	renderTextureGH_ = RTVManager::CreateRenderTargetTexture(Window::GetWidth(), Window::GetHeight());
	postEffectGH_ = RTVManager::CreateRenderTargetTexture(Window::GetWidth(), Window::GetHeight(), {0.0f, 0.0f, 0.0f, 0.0f});


	// Sprite用のリソースを作る
	vertexResourceSprite = CreateBufferResource(dxBase->GetDevice(), sizeof(Sprite::VertexData) * 4);
	// 頂点バッファビューを作成する
	vertexBufferViewSprite = {};
	// リソースの先頭のアドレスから使う
	vertexBufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
	// 使用するリソースのサイズは頂点4つ分のサイズ
	vertexBufferViewSprite.SizeInBytes = sizeof(Sprite::VertexData) * 4;
	// 1頂点あたりのサイズ
	vertexBufferViewSprite.StrideInBytes = sizeof(Sprite::VertexData);

	// 頂点データを設定
	vertexDataSprite = nullptr;
	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));
	// 左下
	vertexDataSprite[0].position = {-1.0f, -1.0f, 0.0f, 1.0f};
	vertexDataSprite[0].texcoord = {0.0f, 1.0f};
	vertexDataSprite[0].normal = {0.0f, 0.0f, -1.0f};
	// 左上
	vertexDataSprite[1].position = { -1.0f, 1.0f, 0.0f, 1.0f};
	vertexDataSprite[1].texcoord = {0.0f, 0.0f};
	vertexDataSprite[1].normal = {0.0f, 0.0f, -1.0f};
	// 右下
	vertexDataSprite[2].position = {1.0f, -1.0f, 0.0f, 1.0f};
	vertexDataSprite[2].texcoord = {1.0f, 1.0f};
	vertexDataSprite[2].normal = {0.0f, 0.0f, -1.0f};
	// 右上
	vertexDataSprite[3].position = {1.0f, 1.0f, 0.0f, 1.0f};
	vertexDataSprite[3].texcoord = {1.0f, 0.0f};
	vertexDataSprite[3].normal = {0.0f, 0.0f, -1.0f};


	// 頂点インデックスの作成
	indexResourceSprite = CreateBufferResource(dxBase->GetDevice(), sizeof(uint32_t) * 6);
	// インデックスバッファビューの作成
	indexBufferViewSprite = {};
	// リソースの先頭のアドレスから使う
	indexBufferViewSprite.BufferLocation = indexResourceSprite->GetGPUVirtualAddress();
	// 使用するリソースのサイズはインデックス6つ分のサイズ
	indexBufferViewSprite.SizeInBytes = sizeof(uint32_t) * 6;
	// インデックスはuin32_tとする
	indexBufferViewSprite.Format = DXGI_FORMAT_R32_UINT;

	// インデックスリソースにデータを書き込む
	indexDataSprite = nullptr;
	indexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSprite));
	indexDataSprite[0] = 0; indexDataSprite[1] = 1; indexDataSprite[2] = 2; 
	indexDataSprite[3] = 1; indexDataSprite[4] = 3; indexDataSprite[5] = 2;

	// Outline用のマテリアルのリソースを作る
	outlineMaterial.data_->color = {0.0f, 0.0f, 0.0f, 1.0f};
	outlineMaterial.data_->enableLighting = false;
	outlineMaterial.data_->uvTransform = Matrix::Identity();


	// Sprite用のTransformationMatrix用のリソースを作る。Matrix 1つ分のサイズを用意する
	transformationMatrixResourceSprite = CreateBufferResource(dxBase->GetDevice(), sizeof(Object3D::TransformationMatrix));
	// データを書き込む
	transformationMatrixDataSprite = nullptr;
	// 書き込むためのアドレスを取得
	transformationMatrixResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	// 単位行列を書き込んでおく
	transformationMatrixDataSprite->WVP = Matrix::Identity();


	// Sprite用のマテリアルのリソースを作る
	materialResourceSprite = CreateBufferResource(dxBase->GetDevice(), sizeof(Object3D::Material));
	// マテリアルにデータを書き込む
	materialDataSprite = nullptr;
	// 書き込むためのアドレスを取得
	materialResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite));
	// 輝度を白に設定
	materialDataSprite->color = Float4(1.0f, 1.0f, 1.0f, 1.0f);
	// SpriteはLightingしないのでfalseを設定
	materialDataSprite->enableLighting = false;
	// UVTransform行列を単位行列で初期化
	materialDataSprite->uvTransform = Matrix::Identity();
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
	ImGuiUtil::ImageWindow("rendertexture", renderTextureGH_);
	ImGuiUtil::ImageWindow("postEffect", postEffectGH_);
	#endif

	///
	///	↓ ここから3Dオブジェクトの描画コマンド
	///

	// レンダーターゲットをレンダーテクスチャにセット
	RTVManager::SetRenderTarget(renderTextureGH_);
	RTVManager::ClearRTV(renderTextureGH_);

	// オブジェクトの描画
	object_->Draw();

	RTVManager::SetRTtoBB();
	// ポストエフェクト用のPSOを設定
	dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineStateVignette());
	// VBVを設定
	dxBase->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite);
	// IBVを設定
	dxBase->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);
	// マテリアルCBufferの場所を設定
	dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	// TransformationMatrixCBufferの場所を設定
	dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());
	// SRVのDescriptorTableの先頭を設定
	TextureManager::SetDescriptorTable(2, dxBase->GetCommandList(), renderTextureGH_);
	// 描画
	dxBase->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);

	//#pragma region ポストエフェクト適用
	//// レンダーターゲットの設定
	//RTVManager::SetRenderTarget(postEffectGH_);
	//RTVManager::ClearRTV(postEffectGH_, {0.0f, 0.0f, 0.0f, 0.0f});
	//// ポストエフェクト用のPSOを設定
	//dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineStateGrayscale());
	//// VBVを設定
	//dxBase->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite);
	//// IBVを設定
	//dxBase->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);
	//// マテリアルCBufferの場所を設定
	//dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(0, outlineMaterial.resource_->GetGPUVirtualAddress());
	//// TransformationMatrixCBufferの場所を設定
	//dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());
	//// SRVのDescriptorTableの先頭を設定
	//TextureManager::SetDescriptorTable(2, dxBase->GetCommandList(), RTVManager::GetDepthSRVHandle(renderTextureGH_));
	//// 描画。6個のインデックスを使用し1つのインスタンスを描画
	//dxBase->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	//#pragma endregion


	//#pragma region renderTextureGHをバックバッファにそのまま描画
	//// レンダーターゲットの設定
	//RTVManager::SetRTtoBB();
	//// 通常のパイプラインを設定
	//dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineState());
	//// VBVを設定
	//dxBase->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite);
	//// IBVを設定
	//dxBase->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);
	//// マテリアルCBufferの場所を設定
	//dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	//// TransformationMatrixCBufferの場所を設定
	//dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());
	//// SRVのDescriptorTableの先頭を設定
	//TextureManager::SetDescriptorTable(2, dxBase->GetCommandList(), renderTextureGH_);
	//// 描画。6個のインデックスを使用し1つのインスタンスを描画
	//dxBase->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	//#pragma endregion


	//#pragma region outlineをバックバッファにそのまま描画
	//// 通常のパイプラインを設定
	//dxBase->GetCommandList()->SetPipelineState(dxBase->GetPipelineState());
	//// VBVを設定
	//dxBase->GetCommandList()->IASetVertexBuffers(0, 1, &vertexBufferViewSprite);
	//// IBVを設定
	//dxBase->GetCommandList()->IASetIndexBuffer(&indexBufferViewSprite);
	//// マテリアルCBufferの場所を設定
	//dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(0, materialResourceSprite->GetGPUVirtualAddress());
	//// TransformationMatrixCBufferの場所を設定
	//dxBase->GetCommandList()->SetGraphicsRootConstantBufferView(1, transformationMatrixResourceSprite->GetGPUVirtualAddress());
	//// SRVのDescriptorTableの先頭を設定
	//TextureManager::SetDescriptorTable(2, dxBase->GetCommandList(), postEffectGH_);
	//// 描画。6個のインデックスを使用し1つのインスタンスを描画
	//dxBase->GetCommandList()->DrawIndexedInstanced(6, 1, 0, 0, 0);
	//#pragma endregion

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

	ImGui::DragFloat3("translation", &object_->transform_.translate.x, 0.01f);
	ImGui::DragFloat3("rotation", &object_->transform_.rotate.x, 0.01f);

	ImGui::ColorEdit3("outlineColor", &outlineMaterial.data_->color.x);

	ImGui::End();
#endif // _DEBUG

	// ImGuiの内部コマンドを生成する
	ImguiWrapper::Render(dxBase->GetCommandList());
	// 描画後処理
	dxBase->PostDraw();
	// フレーム終了処理
	dxBase->EndFrame();
}
