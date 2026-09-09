#pragma once

// C++
#include <vector>

// Engine
#include <Float2.h>
#include <Sprite.h>
#include <SpriteCommon.h>

/// <summary>
/// 遷移先
/// </summary>
enum class TransitionDest {
	GameClear,
	GameOver
};

/// <summary>
/// ゲーム終了通知UI
/// </summary>
class FinishUI {
public:

	void Initialize(Cygnus::SpriteCommon* spriteCommon);

	void Update();

	void Draw();

	void Finish(TransitionDest td) { td_ = td; isSlide_ = true;  slideTimer_ = slideTime_; }
	void StartFadeOut();

	//accessor

	bool IsFinished() { return isFinished_; }

private:

	//テクスチャハンドル
	uint32_t texFinish_ = 0;

	//スプライト
	std::unique_ptr<Cygnus::Sprite> spriteFinish_;

	//フラグ
	bool isFinished_ = false;
	bool isSlide_ = false;

	//タイマー
	float slideTimer_ = 0.0f;
	float slideTime_ = 0.3f;

	//遷移先
	TransitionDest td_;

};

