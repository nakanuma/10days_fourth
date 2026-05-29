#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// =========================================================
// 列車の歯車残り持続時間UI
// =========================================================
class GearTimerBar {
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
	/// <param name="screenPos">列車のスクリーン座標</param>
	void Update(const Cygnus::Float3& screenPos);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// =========================================================
	// Constants
	// =========================================================
	const Cygnus::Float2 kAnchorPoint = {0.5f, 0.5f};
	const Cygnus::Float2 kBarSize = {100.0f, 20.0f};
	const float kYOffsetY = 60.0f;

	// =========================================================
	// Member Variables
	// =========================================================
	std::unique_ptr<Cygnus::Sprite> spriteBar_;	// 残り時間バー
};
