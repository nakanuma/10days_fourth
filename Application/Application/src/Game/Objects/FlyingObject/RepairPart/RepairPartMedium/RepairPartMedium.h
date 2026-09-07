#pragma once

// Application
#include <src/Game/Objects/FlyingObject/Base/FlyingObject.h>

// =========================================================
// 修理パーツ（中品質）
// =========================================================
class RepairPartMedium : public FlyingObject {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="position"></param>
	/// <param name="speed"></param>
	void Initialize(const Cygnus::Float3& position, bool isRightToLeft = true) override;

private:
	// =========================================================
	// Constants
	// =========================================================

	// 各パラメーター
	static constexpr float kSpeed = 0.07f;                               // 移動速度
	static constexpr float kHitRadius = 1.0f;                           // コライダーの半径
};
