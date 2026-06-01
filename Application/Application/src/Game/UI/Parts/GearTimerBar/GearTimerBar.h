#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// =========================================================
// 列車の歯車残り持続時間UI
// =========================================================
class GearTimerBar {
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
	/// <param name="screenPos">列車のスクリーン座標</param>
	/// <param name="energyRatio">列車の残り稼働時間割合</param>
	void Update(const Cygnus::Float3& screenPos, float energyRatio);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// =========================================================
	// Constants
	// =========================================================
	const Cygnus::Float2 kBarSize = {80.0f, 15.0f};
	const float kOffsetY = 40.0f;

	// =========================================================
	// Member Variables
	// =========================================================
	std::unique_ptr<Cygnus::Sprite> spriteBar_;	// 残り時間バー
};
