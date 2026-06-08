#pragma once
// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include "BaseFieldEvent.h"
#include "src/Game/SunLaser/SunLaser.h"

/// <summary>
/// 太陽神ラーのフィールドイベント
/// </summary>
class SunGodRaFieldEvent : public BaseFieldEvent {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Cygnus::SpriteCommon* spriteCommon) override;
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update() override;
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw() override;
	/// <summary>
	/// デバッグ処理
	/// </summary>
	void Debug() override;

private:

	std::unique_ptr<SunLaser> sunLaser_ = nullptr;
	const float kEventTime_ = 21.0f;
};

