#pragma once
// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include "SpriteCommon.h"
#include "Sprite.h"
#include "ImguiWrapper.h"

/// <summary>
/// フィールドイベント 基盤クラス
/// </summary>
class BaseFieldEvent {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Initialize(Cygnus::SpriteCommon* spriteCommon) = 0;
	/// <summary>
	/// 更新処理(イベント効果をここに書く)
	/// </summary>
	virtual void Update() = 0;
	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() = 0;
	/// <summary>
	/// デバッグ処理
	/// </summary>
	virtual void Debug() = 0;

	/// <summary>
	/// 時間が進む処理
	/// </summary>
	void EventTimeUpdate(float deltaTime);

	/// <summary>
	/// 時間経過し終了
	/// </summary>
	/// <returns>タイマーが0ならばtrue</returns>
	bool EventEnd() {
		return eventTimer_ <= 0.0f;
	}

	/// <summary>
	/// getter_イベントタイマー
	/// </summary>
	/// <returns>イベントタイマーの進み具合</returns>
	float GetEventTime() { return eventTimer_; }

protected:
	std::unique_ptr<Cygnus::Sprite> effect_;//目印用

	//時間
	float eventTimer_ = 1.0f;
};

