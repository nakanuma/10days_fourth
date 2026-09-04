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
#include <src/Game/Util/Transition/FadeTransition.h>
#include <src/Game/Util/GameResult/GameResultManager.h>

// =========================================================
// リザルトシーンクラス
// =========================================================
class ResultScene : public Cygnus::BaseScene {
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
	/// メニュー入力処理
	/// </summary>
	void ProcessMenuInput();

	/// <summary>
	/// UIの見た目・アニメーション更新処理
	/// </summary>
	void UpdateUI();

	/// <summary>
	/// ゲームクリア時専用の更新処理
	/// </summary>
	void UpdateGameClear();

	/// <summary>
	/// ゲームオーバー時専用の更新処理
	/// </summary>
	void UpdateGameOver();

private:
	// =========================================================
	// Constants
	// =========================================================

	// スプライト初期位置
	static constexpr Cygnus::Float2 kResultTextInitPos = { 640.0f, 140.0f }; // リザルトロゴ初期位置
	static constexpr Cygnus::Float2 kRetryButtonInitPos = { 640.0f, 460.0f }; // リトライボタン初期位置
	static constexpr Cygnus::Float2 kReturnButtonInitPos = { 640.0f, 560.0f }; // タイトルへ戻るボタン初期位置

	// ボタン用
	static constexpr float kNormalButtonRate = 1.0f; // 通常時の倍率
	static constexpr float kSelectedBaseRate = 1.15f; // 選択時の倍率
	static constexpr float kPulseAmplitude = 0.05f; // 拡縮の振り幅
	static constexpr float kPulseSpeed = 6.0f; // 拡縮アニメーションの速度
	static constexpr float kButtonAMoveDuration = 0.2f; // 移動完了にかかる時間

	// テキストロゴ用
	static constexpr float kTextFloatAmplitude = 6.0f; // 上下の振幅
	static constexpr float kTextFloatSpeed = 1.0f; // 浮遊の速度

	// Aボタン用
	static constexpr Cygnus::Float2 kButtonAOffsetRetry = { -130.0f, 0.0f }; // リトライボタンから左に離すオフセット
	static constexpr Cygnus::Float2 kButtonAOffsetReturn = { -200.0f, 0.0f }; // タイトルに戻るボタンから左に離すオフセット
	static constexpr float kButtonABounceHeight = 2.0f; // 跳ねる高さ
	static constexpr float kButtonABounceSpeed = 6.0f; // バウンドの速度

	// プレイヤー（ゲームオーバー用）
	static constexpr float kPlayerFloatYAmplitude = 0.5f; // 上限の振幅
	static constexpr float kPlayerFloatYSpeed = 1.2f; // 漂うスピード

	// 宇宙船（ゲームクリア用）
	static constexpr float kSpaceshipFloatYAmplitude = 0.3f; // Y軸移動の振幅
	static constexpr float kSpaceshipFloatZAmplitude = 0.05f; // Z軸回転の振幅
	static constexpr float kSpaceshipFloatSpeed = 1.5f; // 漂うスピード

	// =========================================================
	// Member Variables
	// =========================================================

	// ----- System -----
	std::unique_ptr<Cygnus::Camera> camera_ = nullptr;             /* 3Dカメラクラス */
	std::unique_ptr<Cygnus::SpriteCommon> spriteCommon_ = nullptr; /* スプライト共通処理 */
	Cygnus::Input* input_ = nullptr;                               /* 入力管理クラス */
	Cygnus::LightManager* lightManager_ = nullptr;                 /* 各ライト管理クラス */

	// ゲーム結果受け取り用
	GameResult result_ = GameResult::GameOver;

	// ----- Parameters -----

	// メニュー項目の定義
	enum class MenuIndex {
		Retry,
		ReturnTitle,
		Count
	};

	MenuIndex currentMenu_ = MenuIndex::Retry; /* 現在選択中のメニュー */
	bool isStickMoved_ = false; /* スティックの倒し込み保持フラグ */
	bool isSelected_ = false; /* 決定ボタン押下済みフラグ */

	// ボタンの元サイズ保持
	Cygnus::Float2 baseRetryButtonSize_{ 0.0f, 0.0f };
	Cygnus::Float2 baseReturnButtonSize_ = { 0.0f, 0.0f };
	Cygnus::Float2 baseButtonASize_ = { 0.0f, 0.0f };

	float uiAnimationTimer_ = 0.0f; /* UIアニメーション用タイマー */

	// ゲームオーバー時のプレイヤー基準位置
	Cygnus::Float3 playerGameOverInitPos_ = {0.0f, 0.0f, 0.0f};

	// ゲームクリア時の宇宙船基準パラメーター保存用
	Cygnus::Float3 spaceshipClearInitPos_ = {0.0f, 0.0f, 0.0f};
	Cygnus::Float3 spaceshipClearInitRot_ = {0.0f, 0.0f, 0.0f};

	// Aボタンの補間移動用変数
	Cygnus::Float2 buttonAStartPos_{};  // 移動開始位置
	Cygnus::Float2 buttonATargetPos_{}; // 移動目標位置
	Cygnus::Float2 buttonACurrentPos_{}; // 現在位置
	float buttonAMoveTimer_ = 0.0f;     // 移動タイマー

	// ----- Objects -----

	// 3Dオブジェクト
	std::unique_ptr<Cygnus::Object3D> objectPlayer_ = nullptr; /* プレイヤーオブジェクト */
	std::unique_ptr<Cygnus::Object3D> objectSpaceship_ = nullptr; /* 宇宙船オブジェクト */

	// スプライト
	std::unique_ptr<Cygnus::Sprite> spriteGameClearText_ = nullptr; /* ゲームクリア文字 */
	std::unique_ptr<Cygnus::Sprite> spriteGameOverText_ = nullptr; /* ゲームオーバー文字 */
	std::unique_ptr<Cygnus::Sprite> spriteRetryButton_ = nullptr; /* リトライボタン */
	std::unique_ptr<Cygnus::Sprite> spriteReturnButton_ = nullptr; /* タイトルへ戻るボタン */
	std::unique_ptr<Cygnus::Sprite> spriteButtonA_ = nullptr; /* Aボタン */

	// ----- Others -----
	uint32_t shadowMapHandle_;                                     /* シャドウマップテクスチャ */
	std::unique_ptr<Cygnus::PostEffectManager> postEffectManager_; /* ポストエフェクト管理クラス */
};