#pragma once

// C++
#include <functional>

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

enum class PartType {Low, Medium, High};

// =========================================================
// 修理パーツのポップアップUI
// =========================================================
class ItemPopupUI {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="spriteCommon"></param>
	/// <param name="type"></param>
	/// <param name="startScreenPos"></param>
	void Initialize(Cygnus::SpriteCommon* spriteCommon, PartType type, std::function<Cygnus::Float3()> getWorldPosFunc);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void DebugUI(int index);

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// 死亡フラグの取得
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; } 

private:
	// =========================================================
	// Internal Methods
	// =========================================================

private:
	// =========================================================
	// Constants
	// =========================================================

	static constexpr float kFadeInTime = 0.15f;
	static constexpr float kDisplayTime = 0.5f;
	static constexpr float kFadeOutTime = 0.25f;
	static constexpr float kSlideDistance = 30.0f;

	// =========================================================
	// Member Variables
	// =========================================================

	// スプライト
	std::unique_ptr<Cygnus::Sprite> spriteIcon_;
	std::unique_ptr<Cygnus::Sprite> spriteText_;

	// 追従対象のワールド座標を取得するコールバック関数
	std::function<Cygnus::Float3()> getWorldPosFunc_;

	Cygnus::Float2 currentPosition_;

	// タイマー
	float timer_ = 0.0f;

	// 透明度
	float alpha_ = 0.0f;
	
	// 死亡フラグ
	bool isDead_ = false;
};
