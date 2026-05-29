#pragma once
#include "src/Game/Field/BaseFieldEvent.h"

class SethFieldEvent : public BaseFieldEvent {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Cygnus::SpriteCommon* spriteCommon) override;
	/// <summary>
	/// 更新処理(イベント効果をここに書く)
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
	Cygnus::Float3 summonPoint_ = { -40, 0, 0 };

	float summonTimer_ = 0.0f;//砂嵐の召喚時間
	const float kSummonFrequency_ = 3.0f;//砂嵐の召喚頻度


	const float kEventStartTime_ = 10.0f;//時間設定
};

