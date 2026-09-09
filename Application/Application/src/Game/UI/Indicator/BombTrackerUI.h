#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// 前方宣言
class FlyingObjectManager;

// =========================================================
// 爆弾追従UI
// =========================================================
class BombTrackerUI {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="spriteCommon"></param>
	void Initialize(Cygnus::SpriteCommon* spriteCommon);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="flyingObjectManager"></param>
	void Update(FlyingObjectManager* flyingObjectManager);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// スクリーン座標の計算
	/// </summary>
	/// <param name="rowScreenPos"></param>
	/// <param name="outIsOffScreen"></param>
	/// <returns></returns>
	Cygnus::Float2 CalculateScreenPosition(const Cygnus::Float2& rawScreenPos, bool& outIsOffScreen);

private:
	// =========================================================
	// Constants
	// =========================================================

	static constexpr float kScreenMargin = 50.0f; // 画面端からのマージン
	static constexpr float kArrowOffset = 24.0f; // アイコンからのオフセット距離

	static constexpr float kFadeDuration = 0.3f; // フェード時間
	static constexpr float kBlinkSpeed = 6.0f; // 画面内点滅速度

	static constexpr float kMinInScreenAlpha = 0.1f; // 点滅時最小Alpha
	static constexpr float kMaxInScreenAlpha = 0.5f; // 点滅時最大Alpha

	// =========================================================
	// Member Variables
	// =========================================================

	// スプライト
	std::unique_ptr<Cygnus::Sprite> spriteIcon_; // 爆弾アイコン
	std::unique_ptr<Cygnus::Sprite> spriteArrow_; // 逆三角

	// パラメーター
	Cygnus::Float2 currentPos_ = { 0.0f, 0.0f };
	float arrowAngle_ = 0.0f;
	bool isVisible_ = false;
	bool isOffScreen_ = false;

	// アニメーション用
	float fadeTimer_ = 0.0f;
	float blinkTimer_ = 0.0f;
	float currentAlpha_ = 0.0f;
};
