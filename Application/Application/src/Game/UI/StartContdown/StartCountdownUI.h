#pragma once

// C++
#include <vector>

// Engine
#include <Float2.h>
#include <Sprite.h>
#include <SpriteCommon.h>

/// <summary>
/// スタートカウントダウンUI
/// </summary>
class StartCountdownUI {
public:

	void Initialize(Cygnus::SpriteCommon* spriteCommon);

	void Update();

	void Draw();

	void StartCountdown() { isCdStart_ = true; countdownTimer_ = countdownDuration_; }

	//accessor

	bool IsStarted() const { return isStarted_; }

private:

	//テクスチャハンドル
	uint32_t texNumbers_ = 0;
	uint32_t texStart_ = 0;

	//スプライト
	std::unique_ptr<Cygnus::Sprite> spriteCountdown_;
	std::unique_ptr<Cygnus::Sprite> spriteStart_;

	//開始フラグ
	bool isCdStart_ = false;
	bool isSlide_ = false;
	bool isDisappear_ = false;
	bool isStarted_ = false;
	//タイマー
	float countdownTimer_ = 0.0f;
	float countdownDuration_ = 3.5f; // カウントダウンの総時間（秒）
	float startAppearTimer_ = 0.0f;
	float startAppearTime_ = 0.3f;
	float startDisappearTimer_ = 0.0f;
	float startDisappearTime_ = 1.0f;

};

