#pragma once
// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include "Sprite.h"
#include <memory>
// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include "BaseFieldEvent.h"
/// <summary>
/// フィールドイベント「鉱石ザクザク」
/// ランダムな場所に鉱石が増えます
/// </summary>
class LotsOfOreFieldEvent : public BaseFieldEvent {
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

	/// <summary>
	/// 鉱石を生み出す
	/// </summary>
	void BornOres();

	//生み出す時間
	float addOreTimer_ = 0.0f;
	const float kAddOreTime_ = 2.0f;

	//生成範囲
	const float kBornPositionX_ = 30.0f;
	const float kBornPositionZ_ = 20.0f;

	//生成サイズ
	const uint32_t kMaxSize_ = 2;

	//変更なしの値(posY,sizeY)
	float positionY_ = 2.0f;
	float oreSizeY_ = 2.0f;

	//イベント時間を伸ばす
	const float kEventTime_ = 1.0f;
};
