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

// Application
#include <src/Game/Objects/Player/Player.h>
#include <src/Game/Objects/Spaceship/Spaceship.h>
#include <src/Game/Objects/Tether/Tether.h>
#include <src/Game/Objects/FlyingObject/FlyingObjectManager.h>
#include <src/Game/Util/Transition/FadeTransition.h>
#include <src/Game/Scene/PauseMenu.h>
#include <src/Game/UI/GameHUD.h>

// =========================================================
// ゲームプレイシーンクラス
// =========================================================
class GamePlayScene : public Cygnus::BaseScene {
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

	/// <summary>
	/// カメラの更新処理（プレイヤー位置に応じて画角を変える）
	/// </summary>
	void UpdateCamera();

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

	const float kMaxGameTime = 100.0f; // ゲーム制限時間（秒）

	// =========================================================
	// Member Variables
	// =========================================================

	// ----- System -----
	std::unique_ptr<Cygnus::Camera> camera_ = nullptr;             /* 3Dカメラクラス */
	std::unique_ptr<Cygnus::SpriteCommon> spriteCommon_ = nullptr; /* スプライト共通処理 */
	Cygnus::Input* input_ = nullptr;                               /* 入力管理クラス */
	Cygnus::LightManager* lightManager_ = nullptr;                 /* 各ライト管理クラス */

	// ポーズメニュー
	std::unique_ptr<PauseMenu> pauseMenu_ = nullptr;
	// ゲームUI
	std::unique_ptr<GameHUD> gameHUD_ = nullptr;

	// ゲーム進行用
	float gameTimer_ = 0.0f; // ゲーム経過時間タイマー
	bool isTransitionStarted_ = false; // シーン遷移開始フラグ

	// ----- Parameters -----
	Cygnus::Float3 cameraTopPos_ = {0.0f, 5.0f, -90.0f}; // プレイヤーが最も上にいる時のカメラ位置
	Cygnus::Float3 cameraBottomPos_ = {0.0f, -55.0f, -170.0f}; // プレイヤーが最も下にいる時のカメラ位置

	float playerTopY_ = 0.0f; // 基準となるプレイヤー最上部Y座標
	float playerBottomY_ = -60.0f; // 基準となるプレイヤー最下部Y座標
	float cameraInterpolation_ = 0.1f; // カメラ移動の追従スピード

	// ----- Objects -----
	std::unique_ptr<Player> player_; /* プレイヤー */
	std::unique_ptr<Spaceship> spaceship_;	/* 宇宙船 */
	std::unique_ptr<Tether> tether_; /* 命綱 */
	std::unique_ptr<FlyingObjectManager> flyingObjectManager_; /* 飛翔物管理クラス */

	// ----- Others -----
	uint32_t shadowMapHandle_;                                     /* シャドウマップテクスチャ */
	std::unique_ptr<Cygnus::PostEffectManager> postEffectManager_; /* ポストエフェクト管理クラス */
};