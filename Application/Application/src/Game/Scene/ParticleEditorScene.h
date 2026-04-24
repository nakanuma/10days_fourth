#pragma once

// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include <BaseScene.h>
#include <Camera.h>
#include <Engine/Collider/CollisionManager.h>
#include <Engine/Texture/PostEffectManager.h>
#include <Engine/Util/TimeManager.h>
#include <Input.h>
#include <LightManager.h>
#include <ModelManager.h>
#include <Object3D.h>
#include <SoundManager.h>
#include <Sprite.h>
#include <SpriteCommon.h>
#include <TextureManager.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>

// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------


// =========================================================
// ゲームプレイシーンクラス
// =========================================================
class ParticleEditorScene : public Cygnus::BaseScene {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// ゲームシーンの初期化処理を行います。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// ゲームシーンの終了処理を行います。
	/// </summary>
	void Finalize() override;

	/// <summary>
	/// 毎フレームの更新処理を行います。
	/// </summary>
	void Update() override;

	/// <summary>
	/// シーンの描画処理を行います。
	/// </summary>
	void Draw() override;

	/// <summary>
	/// デバッグ表示を行います。
	/// </summary>
	void Debug();

	// =========================================================
	// Accessor
	// =========================================================

private:
	// =========================================================
	// Internal Methods
	// =========================================================

private:
	// =========================================================
	// Constants
	// =========================================================

	// =========================================================
	// Member Variables
	// =========================================================

	// ----- System -----
	std::unique_ptr<Cygnus::Camera> camera_ = nullptr;             /* 3Dカメラクラス */
	std::unique_ptr<Cygnus::SpriteCommon> spriteCommon_ = nullptr; /* スプライト共通処理 */
	Cygnus::Input* input_ = nullptr;                               /* 入力管理クラス */
	Cygnus::LightManager* lightManager_ = nullptr;                 /* 各ライト管理クラス */

	// ----- Objects -----
	std::unique_ptr<Cygnus::Object3D> objectGround_;	/* 地面オブジェクト（仮） */

	// ----- Others -----
	uint32_t shadowMapHandle_;                                     /* シャドウマップテクスチャ */
	std::unique_ptr<Cygnus::PostEffectManager> postEffectManager_; /* ポストエフェクト管理クラス */
};