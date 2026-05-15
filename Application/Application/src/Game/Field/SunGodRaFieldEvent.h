#pragma once
// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include "BaseFieldEvent.h"

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
};

