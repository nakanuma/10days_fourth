#include "StartCountdownUI.h"
#define NOMINMAX

// C++
#include <algorithm>

// Engine
#include <TextureManager.h>
#include <Easing.h>
#include <Math/MyMath.h>
#include <TimeManager.h>
#include <RandomGenerator.h>

void StartCountdownUI::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	// テクスチャ読み込み
	texNumbers_ = Cygnus::TextureManager::Load("numbers.png");
	texStart_ = Cygnus::TextureManager::Load("start.png");
	// カウントダウン用スプライト生成
	spriteCountdown_ = std::make_unique<Cygnus::Sprite>();
	spriteCountdown_->Initialize(spriteCommon, texNumbers_);
	spriteCountdown_->SetAnchorPoint({ 0.5f, 0.5f });
	spriteCountdown_->SetPosition({ 640.0f, 360.0f }); // 画面中央に配置
	spriteCountdown_->SetTextureSize({ 64.0f, 64.0f }); // テクスチャサイズに合わせる
	spriteCountdown_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 初期状態では透明に設定
	//スタート用スプライト生成
	spriteStart_ = std::make_unique<Cygnus::Sprite>();
	spriteStart_->Initialize(spriteCommon, texStart_);
	spriteStart_->SetAnchorPoint({ 0.5f, 0.5f });
	spriteStart_->SetPosition({ 640.0f + 1280.0f, 360.0f }); // 画面中央に配置
	spriteStart_->SetColor({ 1.0f, 1.0f, 1.0f, 0.0f }); // 初期状態では透明に設定
}

void StartCountdownUI::Update() {
	//カウントダウン更新
	if (isCdStart_) {
		// タイマーを減少
		countdownTimer_ -= Cygnus::TimeManager::GetInstance()->GetDeltaTime();

		float timer = countdownTimer_ - (int)countdownTimer_;

		float t = timer / 1.0f; // 0.0f ~ 1.0f の範囲に正規化

		//テクスチャの位置
		spriteCountdown_->SetTextureLeftTop({ ((int)countdownTimer_ + 1) * 64.0f ,0.0f });

		//サイズ
		float size = Cygnus::Easing::EaseInSine(t) * 720.0f;
		spriteCountdown_->SetSize({ size,size });

		//色
		Cygnus::Float4 color = { 1,1,1,1 };
		spriteCountdown_->SetColor(color);

		//タイマーが時間を迎えたら
		if (countdownTimer_ < 0.0f) {
			isCdStart_ = false;
			isSlide_ = true;
			isStarted_ = true;
			countdownTimer_ = 0.0f;
			startAppearTimer_ = startAppearTime_;
			spriteCountdown_->SetColor({ 1,1,1,0 });
			spriteStart_->SetColor({ 1,1,1,1 });
			spriteStart_->SetPosition({ 1280.0f + 640.0f,360.0f });
		}

	}

	//STARTスプライトスライド更新
	if (isSlide_) {
		//タイマーを減少
		startAppearTimer_ -= Cygnus::TimeManager::GetInstance()->GetDeltaTime();

		float t = startAppearTimer_ / startAppearTime_;

		//位置
		float posX = 640.0f + Cygnus::Easing::EaseOutSine(t) * 1280.0f;
		spriteStart_->SetPosition({ posX,360.0f });

		//色
		spriteStart_->SetColor({ 1,1,1,1 });

		//タイマーが時間を迎えたら
		if (startAppearTimer_ < 0.0f) {
			isSlide_ = false;
			isDisappear_ = true;
			startAppearTimer_ = 0.0f;
			startDisappearTimer_ = startDisappearTime_;
			spriteStart_->SetPosition({ 640.0f,360.0f });
		}

	}

	//STARTスプライト消失更新
	if (isDisappear_) {
		//タイマー減少
		startDisappearTimer_ -= Cygnus::TimeManager::GetInstance()->GetDeltaTime();

		//タイマーが時間を迎えたら
		if (startDisappearTimer_ < 0.0f) {
			isDisappear_ = false;
			startDisappearTimer_ = 0.0f;
			spriteStart_->SetColor({ 1,1,1,0 });
		}

		float t = startDisappearTimer_ / startAppearTime_;

		//位置ランダムにシェイク
		auto randGen = Cygnus::RandomGenerator::GetInstance();
		Cygnus::Float2 shakeValue = { 640.0f + randGen->RandomValue(0.0f,3.0f),360.0f + randGen->RandomValue(0.0f,3.0f) };
		spriteStart_->SetPosition(shakeValue);

		//色
		float alpha = Cygnus::Easing::EaseInSine(t);
		spriteStart_->SetColor({ 1,1,1,alpha });

	}

	//全てのスプライトを更新
	spriteCountdown_->Update();
	spriteStart_->Update();


}

void StartCountdownUI::Draw() {
	//全てのスプライトを描画
	spriteCountdown_->Draw();
	spriteStart_->Draw();

}
