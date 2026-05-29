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

// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include <src/Game/Player/Player.h>
#include <src/Game/Carrier/Carrier.h>
#include <src/Game/Sphinx/Sphinx.h>
#include <src/Game/TitleSceneObject/TitleSceneUI/TitleSceneUI.h>

// =========================================================
// タイトルシーンクラス
// =========================================================
class TitleScene : public Cygnus::BaseScene
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// タイトルシーンの初期化処理を行います。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// タイトルシーンの終了処理を行います。
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
	void TransitionTitle();

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
	//std::unique_ptr<Player> player_;	/* プレイヤー */
	//std::unique_ptr<Carrier> carrier_;	/* 経路に沿って移動するオブジェクト */
	//std::unique_ptr<Sphinx> sphinx_;	/* スフィンクス */

	// ----- Others -----
	uint32_t shadowMapHandle_;                                     /* シャドウマップテクスチャ */
	std::unique_ptr<Cygnus::PostEffectManager> postEffectManager_; /* ポストエフェクト管理クラス */
	std::unique_ptr<TitleSceneUI> titleSceneUI_; /* タイトルシーンのUIクラス */
};