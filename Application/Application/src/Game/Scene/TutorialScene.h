#pragma once
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
#include <src/Game/UI/Explain/ExplainUI.h>

class TutorialScene : public Cygnus::BaseScene {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize() override;
	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize() override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;

private:


	/// <summary>
	/// カメラ更新
	/// </summary>
	void UpdateCamera();

	/// <summary>
	/// カメラシェイクの開始
	/// </summary>
	/// <param name="intensity"></param>
	/// <param name="duration"></param>
	void StartCameraShake(float intensity = 1.0f, float duration = 0.3f);


private:

	// -- システム --
	std::unique_ptr<Cygnus::Camera> camera_ = nullptr;
	std::unique_ptr<Cygnus::SpriteCommon> spriteCommon_ = nullptr;
	Cygnus::Input* input_ = nullptr;
	Cygnus::LightManager* lightManager_ = nullptr;

	// -- チュートリアルスキップ --
	bool isSkipTutorial_ = false;

	// -- カメラ演出 --
	Cygnus::Float3 cameraTopPos_ = { 0.0f,7.5f,-90.0f };//プレイヤーが最も上にいる時
	Cygnus::Float3 cameraBottomPos_ = { 0.0f,-50.0f,-180.0f };//プレイヤーが最も下にいる時

	float playerTopY_ = 0.0f; // 基準となるプレイヤー最上部Y座標
	float playerBottomY_ = -60.0f; // 基準となるプレイヤー最下部Y座標
	float cameraInterpolation_ = 0.1f; // カメラ移動の追従スピード

	// 上下左右移動のパラメーター
	float playerLimitX_ = 25.0f;
	float playerLimitY_ = 30.0f;
	Cygnus::Float2 maxCameraShift_ = { 4.0f, 4.0f }; // カメラ移動の最大オフセット
	Cygnus::Float2 maxCameraAngle_ = { 0.00f, 0.08f }; // カメラ回転の最大角度

	// カメラシェイク用
	float shakeDuration_ = 0.0f; // 全体の揺れ指定時間
	float shakeTimer_ = 0.0f;
	float shakeIntensity_ = 0.0f;


	// ----- ゲームオブジェクト -----
	std::unique_ptr<Player> player_; /* プレイヤー */
	std::unique_ptr<Spaceship> spaceship_;	/* 宇宙船 */
	std::unique_ptr<Tether> tether_; /* 命綱 */
	std::unique_ptr<FlyingObjectManager> flyingObjectManager_; /* 飛翔物管理クラス */

	// ----- Others -----
	uint32_t shadowMapHandle_;                                     /* シャドウマップテクスチャ */
	std::unique_ptr<Cygnus::PostEffectManager> postEffectManager_; /* ポストエフェクト管理クラス */

	std::unique_ptr<ExplainUI> explainUI_;

};

