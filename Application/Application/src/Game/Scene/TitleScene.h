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

// =========================================================
// タイトルシーンクラス
// =========================================================
class TitleScene : public Cygnus::BaseScene {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// シーンの初期化処理を行います。
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// シーンの終了処理を行います。
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

	/// <summary>
	/// タイトルメニューの入力処理
	/// </summary>
	void ProcessMenuInput();

	/// <summary>
	/// タイトルUIの見た目更新処理
	/// </summary>
	void UpdateUI();

	/// <summary>
	/// プレイヤーオブジェクトのアニメーション更新処理
	/// </summary>
	void UpdatePlayerAnimation();

private:
	// =========================================================
	// Constants
	// =========================================================

	// スプライト初期位置
	static constexpr Cygnus::Float2 kTitleLogoInitPos = {640.0f, 140.0f}; // タイトルロゴ初期位置
	static constexpr Cygnus::Float2 kStartButtonInitPos = {640.0f, 460.0f}; // スタートボタン初期位置
	static constexpr Cygnus::Float2 kExitButtonInitPos = { 640.0f, 560.0f }; // 終了ボタン初期位置

	// ボタン用
	static constexpr float kNormalButtonRate = 1.0f; // 通常時の倍率
	static constexpr float kSelectedBaseRate = 1.15f; // 選択時の倍率

	static constexpr float kPulseAmplitude = 0.05f; // 拡縮の振り幅
	static constexpr float kPulseSpeed = 6.0f; // 拡縮アニメーションの速度

	// タイトルロゴ用
	static constexpr float kLogoFloatAmplitude = 8.0f; // 上下の振幅
	static constexpr float kLogoFloatSpeed = 2.5f; // 浮遊の速度

	// Aボタン用
	static constexpr Cygnus::Float2 kButtonAOffsetStart = {-120.0f, 0.0f}; // 選択中ボタンから左に離すオフセット
	static constexpr Cygnus::Float2 kButtonAOffsetExit = { -100.0f, 0.0f };
	static constexpr float kButtonABounceHeight = 2.0f; // 跳ねる高さ
	static constexpr float kButtonABounceSpeed = 6.0f; // バウンドの速度
	static constexpr float kButtonAMoveDuration = 0.2f; // 移動完了にかかる時間

	// プレイヤー用
	static constexpr float kPlayerStartX = -25.0f; // 開始X位置
	static constexpr float kPlayerEndX = 25.0f; // 終了X位置
	static constexpr float kPlayerMoveSpeed = 2.5f; // 横移動速度
	static constexpr float kPlayerRotSpeedZ = 0.75f; // Z軸回転速度
	static constexpr float kPlayerFloatYAmplitude = 0.5f; // Y軸方向の上下振幅
	static constexpr float kPlayerFloatYSpeed = 2.0f; // Y軸方向の振動速度
	static constexpr float kPlayerPulseBaseScale = 1.0f; // 基本スケール
	static constexpr float kPlayerPulseMaxScale = 1.1f; // 拍動時の最大スケール
	static constexpr float kPlayerPulseCycleTime = 0.8f; // 1パルスの周期
	static constexpr float kPlayerPulseDuration = 0.25f; // 拍動の時間

	// =========================================================
	// Member Variables
	// =========================================================

	// ----- System -----
	std::unique_ptr<Cygnus::Camera> camera_ = nullptr;             /* 3Dカメラクラス */
	std::unique_ptr<Cygnus::SpriteCommon> spriteCommon_ = nullptr; /* スプライト共通処理 */
	Cygnus::Input* input_ = nullptr;                               /* 入力管理クラス */
	Cygnus::LightManager* lightManager_ = nullptr;                 /* 各ライト管理クラス */

	// ----- Parameters -----
	
	// メニュー項目の定義
	enum class MenuIndex {
		Start,
		Exit,
		Count
	};

	MenuIndex currentMenu_ = MenuIndex::Start; /* 現在選択中のメニュー */
	bool isStickMoved_ = false; /* スティックの倒し込み保持フラグ */
	bool isSelected_ = false; /* 決定ボタン押下済みフラグ */

	// ボタンの元サイズ保持
	Cygnus::Float2 baseStartButtonSize_ {0.0f, 0.0f};
	Cygnus::Float2 baseExitButtonSize_ = {0.0f, 0.0f};
	Cygnus::Float2 baseButtonASize_ = {0.0f, 0.0f};

	float uiAnimationTimer_ = 0.0f; /* UIアニメーション用タイマー */

	// Aボタンの補完移動用変数
	Cygnus::Float2 buttonAStartPos_{}; // 移動開始位置
	Cygnus::Float2 buttonATargetPos_{}; // 移動目標位置
	Cygnus::Float2 buttonACurrentPos_{}; // 現在位置
	float buttonAMoveTimer_ = 0.0f; // 移動タイマー

	// ----- Objects -----

	// 3Dオブジェクト
	std::unique_ptr<Cygnus::Object3D> objectPlayer_ = nullptr; /* プレイヤーオブジェクト */

	// スプライト
	std::unique_ptr<Cygnus::Sprite> spriteTitleLogo_ = nullptr; /* タイトルロゴ */
	std::unique_ptr<Cygnus::Sprite> spriteStartButton_ = nullptr; /* スタートボタン */
	std::unique_ptr<Cygnus::Sprite> spriteExitButton_ = nullptr; /* 終了ボタン */
	std::unique_ptr<Cygnus::Sprite> spriteButtonA_ = nullptr; /* Aボタン */

	// ----- Others -----
	uint32_t shadowMapHandle_;                                     /* シャドウマップテクスチャ */
	std::unique_ptr<Cygnus::PostEffectManager> postEffectManager_; /* ポストエフェクト管理クラス */
};