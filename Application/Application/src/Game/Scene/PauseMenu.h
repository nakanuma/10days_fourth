#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// =========================================================
// ポーズメニュークラス
// =========================================================
class PauseMenu {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Cygnus::SpriteCommon* spriteCommon);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// ポーズ中かどうかを取得
	/// </summary>
	/// <returns></returns>
	bool IsPaused() const { return isPaused_; }

	/// <summary>
	/// 今フレームでポーズが解除されたかどうかを取得
	/// </summary>
	/// <returns></returns>
	bool IsJustUnpaused() const { return isJustUnpaused_; }

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

private:
	// =========================================================
	// Constants
	// =========================================================

	// スプライト初期位置
	static constexpr Cygnus::Float2 kPauseTextInitPos = { 640.0f, 240.0f }; // ポーズ文字初期位置
	static constexpr Cygnus::Float2 kContinueButtonInitPos = { 640.0f, 360.0f }; // つづけるボタン初期位置
	static constexpr Cygnus::Float2 kReturnButtonInitPos = { 640.0f, 440.0f }; // タイトルへ戻るボタン初期位置

	// ボタン用
	static constexpr float kNormalButtonRate = 1.0f; // 通常時の倍率
	static constexpr float kSelectedBaseRate = 1.15f; // 選択時の倍率
	static constexpr float kPulseAmplitude = 0.05f; // 拡縮の振り幅
	static constexpr float kPulseSpeed = 6.0f; // 拡縮アニメーションの速度

	// Aボタン用
	static constexpr Cygnus::Float2 kButtonAOffsetContinue = { -140.0f, 0.0f }; // つづけるボタンから左に離すオフセット
	static constexpr Cygnus::Float2 kButtonAOffsetReturn = { -200.0f, 0.0f }; // タイトルに戻るボタンから左に離すオフセット
	static constexpr float kButtonABounceHeight = 2.0f; // 跳ねる高さ
	static constexpr float kButtonABounceSpeed = 6.0f; // バウンドの速度
	static constexpr float kButtonAMoveDuration = 0.2f; // 移動完了にかかる時間

	// =========================================================
	// Member Variables
	// =========================================================

	// メニュー項目の定義
	enum class MenuIndex {
		Continue,
		ReturnTitle,
		Count
	};

	MenuIndex currentMenu_ = MenuIndex::Continue; /* 現在選択中のメニュー */
	bool isStickMoved_ = false;
	bool isSelected_ = false; // 決定ボタン押下済みフラグ

	// ボタンの元サイズ保持
	Cygnus::Float2 baseContinueButtonSize_{0.0f, 0.0f};
	Cygnus::Float2 baseReturnButtonSize_{0.0f, 0.0f};

	// Aボタンの補完移動用変数
	Cygnus::Float2 buttonAStartPos_{}; // 移動開始位置
	Cygnus::Float2 buttonATargetPos_{}; // 移動目標位置
	Cygnus::Float2 buttonACurrentPos_{}; // 現在位置
	float buttonAMoveTimer_ = 0.0f; // 移動タイマー

	float animationTimer_ = 0.0f; // アニメーション用タイマー

	// スプライト
	std::unique_ptr<Cygnus::Sprite> spriteBackground_ = nullptr; // 背景（半透明黒）
	std::unique_ptr<Cygnus::Sprite> spritePauseText_ = nullptr; // ポーズ文字
	std::unique_ptr<Cygnus::Sprite> spriteContinueButton_ = nullptr; // つづけるボタン
	std::unique_ptr<Cygnus::Sprite> spriteReturnButton_ = nullptr; // タイトルへ戻るボタン
	std::unique_ptr<Cygnus::Sprite> spriteButtonA_ = nullptr; // Aボタン

	// ポーズ状態フラグ
	bool isPaused_ = false;
	bool isJustUnpaused_ = false;
};
