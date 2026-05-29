#pragma once
#include "src/Game/Field/BaseFieldEvent.h"

/// <summary>
/// フィールドイベント「アヌビス」
/// ミイラを大量発生する
/// </summary>
class AnubisFieldEvent : public BaseFieldEvent {
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
	/// 敵召喚
	/// </summary>
	void Summon();
	/// <summary>
	/// 召喚クールタイム
	/// </summary>
	void SummonCoolTime();

	//イベント時間を伸ばす
	const float kEventTime_ = 10.0f;

	float nextSummonTimer_ = 0.0f;         //召喚時間を測定
	const float kNextSummonTimeMax_ = 0.1f;//召喚までの時間

	bool isSummon_ = false;
};

