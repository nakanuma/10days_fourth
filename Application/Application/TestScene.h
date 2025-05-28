#pragma once
#include "BaseScene.h"
#include "Camera.h"
#include "SpriteCommon.h"
#include "TextureManager.h"
#include "Sprite.h"
#include "ModelManager.h"
#include "Object3D.h"
#include "SoundManager.h"
#include "Input.h"
#include "LightManager.h"

class TestScene : public BaseScene {
public:
	void Initialize() override;

	void Finalize() override;

	void Update() override;

	void Draw() override;

private:
	std::unique_ptr<Camera> camera = nullptr;
	std::unique_ptr<SpriteCommon> spriteCommon = nullptr;
	std::unique_ptr<SoundManager> soundManager = nullptr;
	Input* input = nullptr;
	LightManager* lightManager = nullptr;

	///
	/// ↓ ゲームシーン用
	///

	// モデルデータ
	ModelManager::ModelData model_;
	// 3Dオブジェクト
	std::unique_ptr<Object3D> object_;

	///
	/// レンダーテクスチャ関連
	/// 

	// レンダーテクスチャ
	uint32_t renderTextureGH_;
	uint32_t postEffectGH_;

	// スプライト用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> vertexResourceSprite;
	// 頂点データ
	Sprite::VertexData* vertexDataSprite;
	// 頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};
	// インデックスデータ
	uint32_t* indexDataSprite = nullptr;
	// 頂点インデックス
	Microsoft::WRL::ComPtr<ID3D12Resource> indexResourceSprite;
	// インデックスバッファビュー
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};
	// アウトライン用のマテリアルリソース
	ConstBuffer<Sprite::Material> outlineMaterial;
	// Sprite用のTransformationMatrix用のリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> transformationMatrixResourceSprite;
	// Sprite用のマテリアルのリソース
	Microsoft::WRL::ComPtr<ID3D12Resource> materialResourceSprite;
	// トランスフォーム用データ
	Object3D::TransformationMatrix* transformationMatrixDataSprite;
	// マテリアル用データ
	Object3D::Material* materialDataSprite;
};
