#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// =========================================================
// プレイヤーの操作表示UI（オブジェクトに近づいた際のみ表示）
// =========================================================
class InteractGuide {
public:
	/// <summary>
	/// 行動の種類
	/// </summary>
	enum class ActionType {
		Insert,	// はめる
		Craft, // 作る
		Mine, // 掘る
	};

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="spriteCommon"></param>
	/// <param name="actionType"></param>
	/// <param name="position"></param>
	void Initialize(Cygnus::SpriteCommon* spriteCommon, ActionType actionType);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const Cygnus::Float3& screenPos);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// 有効化状態の設定
	/// </summary>
	/// <param name="flag"></param>
	void SetActive(bool flag) { isActive_ = flag; }

	/// <summary>
	/// 有効化状態の取得
	/// </summary>
	/// <returns></returns>
	bool IsActive() const { return isActive_; }

private:
	// =========================================================
	// Constants
	// =========================================================
	const Cygnus::Float2 kButtonSize = {32.0f, 32.0f}; // ボタンのサイズ
	const Cygnus::Float2 kTextSize = {96.0f, 32.0f}; // テキストのサイズ
	const float kSpacing = -12.0f;	// ボタンと文字の隙間

	const Cygnus::Float2 kAnchorPoint = {0.5f, 0.5f};
	const float kYOffsetY = 60.0f;

	// =========================================================
	// Member Variables
	// =========================================================
	std::unique_ptr<Cygnus::Sprite> spriteButton_;	// ボタンの画像
	std::unique_ptr<Cygnus::Sprite> spriteText_; // 文字の画像

	bool isActive_ = false;	// 有効化フラグ
};
