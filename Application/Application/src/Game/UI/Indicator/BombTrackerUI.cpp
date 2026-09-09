#include "BombTrackerUI.h"

// Engine
#include <TextureManager.h>
#include <MyWindow.h>
#include <TimeManager.h>
#include <Easing.h>

// Application
#include <src/Game/Objects/FlyingObject/FlyingObjectManager.h>
#include <src/Game/Util/Utility.h>

void BombTrackerUI::Initialize(Cygnus::SpriteCommon* spriteCommon) { 
	uint32_t texIcon = Cygnus::TextureManager::Load("bombIcon.png"); 
	uint32_t texArrow = Cygnus::TextureManager::Load("tri.png"); 

	// 各スプライト生成
	spriteIcon_ = std::make_unique<Cygnus::Sprite>();
	spriteIcon_->Initialize(spriteCommon, texIcon);
	spriteIcon_->SetAnchorPoint({0.5f, 0.5f});
	
	spriteArrow_ = std::make_unique<Cygnus::Sprite>();
	spriteArrow_->Initialize(spriteCommon, texArrow);
	spriteArrow_->SetAnchorPoint({0.5f, 0.5f});

	fadeTimer_ = 0.0f;
	blinkTimer_ = 0.0f;
	currentAlpha_ = 0.0f;
}

void BombTrackerUI::Update(FlyingObjectManager* flyingObjectManager) {
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	/* フィールド上のアクティブな爆弾オブジェクトを探索 */
	const FlyingObject* targetBomb = nullptr;
	if (flyingObjectManager) {
		for (const auto& obj : flyingObjectManager->GetObjects()) {
			if (!obj->IsDead() && obj->GetCategory() == ObjectCategory::BombItem) {
				targetBomb = obj.get();
				break;
			}
		}
	}

	/* 出現 / 消滅のフェードタイマー更新 */
	if (targetBomb) {
		// 存在時 0->1
		fadeTimer_ = (std::min)(fadeTimer_ + dt, kFadeDuration);
	} else {
		// 不在時 0に向かうように
		fadeTimer_ = (std::max)(fadeTimer_ - dt, 0.0f);
		if (fadeTimer_ <= 0.0f) {
			blinkTimer_ = 0.0f; // 完全に消えたらタイマーリセット
		}
	}
	// タイマーが0なら処理を中断
	if (fadeTimer_ <= 0.0f) {
		currentAlpha_ = 0.0f;
		return;
	}

	/* 画面内 / 外の位置・角度の計算 */
	if (targetBomb) {
		Cygnus::Float3 worldPos = targetBomb->GetTranslate();
		Cygnus::Float3 rawScreen = Utility::WorldToScreen(worldPos);
		Cygnus::Float2 rawScreenPos = {rawScreen.x, rawScreen.y};

		// アイコンの位置を画面内にクランプ処理
		currentPos_ = CalculateScreenPosition(rawScreenPos, isOffScreen_);

		// 画面外にある場合のみ矢印UIの位置と角度を計算
		if (isOffScreen_) {
			// 爆弾位置へ向かうベクトルと角度
			Cygnus::Float2 dir = {rawScreenPos.x - currentPos_.x, rawScreenPos.y - currentPos_.y};
			arrowAngle_ = std::atan2f(dir.y, dir.x);

			// アイコンの周囲にオフセット配置
			Cygnus::Float2 arrowPos = {currentPos_.x + std::cosf(arrowAngle_) * kArrowOffset, currentPos_.y + std::sinf(arrowAngle_) * kArrowOffset};

			spriteArrow_->SetPosition(arrowPos);
			spriteArrow_->SetRotation(arrowAngle_);
		}
	}

	/* アルファ値のアニメーション計算 */
	float fadeFactor = Cygnus::Easing::EaseOutQuad(fadeTimer_ / kFadeDuration);

	if (isOffScreen_) {
		// 画面外の場合 1.0
		currentAlpha_ = 1.0f * fadeFactor;
		blinkTimer_ = 0.0f; // 画面外に出たら点滅タイマーリセット
	} else {
		// 画面内の場合 往復
		blinkTimer_ += dt * kBlinkSpeed;

		// sin波で往復
		float sinWave = (std::sinf(blinkTimer_) + 1.0f) * 0.5f;
		float blinkAlpha = kMinInScreenAlpha + sinWave * (kMaxInScreenAlpha - kMinInScreenAlpha);

		currentAlpha_ = blinkAlpha * fadeFactor;
	}

	/* 各スプライトへアルファ値を適用して更新 */
	Cygnus::Float4 color = {1.0f, 1.0f, 1.0f, currentAlpha_};

	spriteIcon_->SetPosition(currentPos_);
	spriteIcon_->SetColor(color);
	spriteIcon_->Update();

	if (isOffScreen_) {
		spriteArrow_->SetColor(color);
		spriteArrow_->Update();
	}
}

void BombTrackerUI::Draw() {
	if (currentAlpha_ <= 0.001f) return;

	// 画面外のときだけ矢印描画
	if (isOffScreen_) {
		spriteArrow_->Draw();
	}

	// アイコン描画
	spriteIcon_->Draw();
}


Cygnus::Float2 BombTrackerUI::CalculateScreenPosition(const Cygnus::Float2& rawScreenPos, bool& outIsOffScreen) { 
	float screenW = static_cast<float>(Cygnus::Window::GetWidth()); 
	float screenH = static_cast<float>(Cygnus::Window::GetHeight());

	float minX = kScreenMargin;
	float maxX = screenW - kScreenMargin;
	float minY = kScreenMargin;
	float maxY = screenH - kScreenMargin;

	// 画面外判定
	outIsOffScreen = (rawScreenPos.x < minX || rawScreenPos.x > maxX) || (rawScreenPos.y < minY || rawScreenPos.y > maxY);

	// 画面内にクランプ
	return Cygnus::Float2{
		std::clamp(rawScreenPos.x, minX, maxX), 
		std::clamp(rawScreenPos.y, minY, maxY)
	};
}
