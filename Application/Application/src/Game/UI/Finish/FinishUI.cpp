#include "FinishUI.h"
#define NOMINMAX

// C++
#include <algorithm>

// Application
#include <src/Game/Util/GameResult/GameResultManager.h>
#include <src/Game/Util/Transition/FadeTransition.h>

// Engine
#include <TextureManager.h>
#include <Easing.h>
#include <Math/MyMath.h>
#include <TimeManager.h>
#include <RandomGenerator.h>
#include <Engine/Scene/SceneManager.h>
#include <Collider/CollisionManager.h>

void FinishUI::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	// テクスチャ読み込み
	texFinish_ = Cygnus::TextureManager::Load("finish.png");
	// スプライト生成
	spriteFinish_ = std::make_unique<Cygnus::Sprite>();
	spriteFinish_->Initialize(spriteCommon, texFinish_);
	spriteFinish_->SetAnchorPoint({ 0.5f, 0.5f });
	spriteFinish_->SetPosition({ 1280.0f + 640.0f, 360.0f }); // 画面中央に配置
	spriteFinish_->SetSize({ 1280.0f,720.0f });
	spriteFinish_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 初期状態では透明に設定
}

void FinishUI::Update() {

	//STARTスプライトスライド更新
	if (isSlide_) {
		//タイマーを減少
		slideTimer_ -= Cygnus::TimeManager::GetInstance()->GetDeltaTime();

		float t = slideTimer_ / slideTime_;

		//位置
		float posX = 640.0f + Cygnus::Easing::EaseOutSine(t) * 1280.0f;
		spriteFinish_->SetPosition({ posX,360.0f });

		//色
		spriteFinish_->SetColor({ 1,1,1,1 });

		//タイマーが時間を迎えたら
		if (slideTimer_ < 0.0f) {
			isSlide_ = false;
			isFinished_ = true;
			slideTimer_ = 0.0f;
			spriteFinish_->SetPosition({ 640.0f,360.0f });

			//シーン遷移先をセット
			switch (td_) {
			case TransitionDest::GameClear:
				GameResultManager::SetResult(GameResult::Clear);
				break;
			case TransitionDest::GameOver:
				GameResultManager::SetResult(GameResult::GameOver);
				break;
			default:
				break;
			}
		}
	}

	//全てのスプライトを更新
	spriteFinish_->Update();

}

void FinishUI::Draw() {
	//全てのスプライトを描画
	spriteFinish_->Draw();

}

void FinishUI::StartFadeOut() {
	FadeTransition::GetInstance()->StartFadeOut(
		1.0f,
		[]() {
			Cygnus::SceneManager::GetInstance()->ChangeScene("RESULT");
			Cygnus::CollisionManager::GetInstance()->Clear();
		},
		0.5f
	);
}
