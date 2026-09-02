#pragma once

// Application
#include <src/Game/Objects/FlyingObject/Base/FlyingObject.h>

// =========================================================
// 隕石（小）
// =========================================================
class MeteorSmall : public FlyingObject {
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
	static constexpr float kSpeed = 0.06f; // 移動速度
	static constexpr float kRotationSpeed = 0.02f; // 回転速度
	static constexpr float kHitRadius = 1.0f;      // コライダーの半径
};
