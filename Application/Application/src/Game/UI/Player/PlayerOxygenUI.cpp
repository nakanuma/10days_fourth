#include "PlayerOxygenUI.h"

// Engine
#include <TextureManager.h>
#include <TimeManager.h>
#include <Math/Easing.h>
#include <ParticleEffect/ParticleEffectManager.h>
#include <RandomGenerator.h>

// Application
#include <src/Game/Objects/Player/Player.h>
#include <src/Game/Util/Utility.h>

void PlayerOxygenUI::Initialize(Cygnus::SpriteCommon* spriteCommon, const Player* player) {
	player_ = player;

	// 単色表示用の白テクスチャをロード
	uint32_t texWhite = Cygnus::TextureManager::Load("white.png");

	// 1. 背景スプライト初期化
	spriteBackground_ = std::make_unique<Cygnus::Sprite>();
	spriteBackground_->Initialize(spriteCommon, texWhite);
	spriteBackground_->SetAnchorPoint(kAnchorPointBottomCenter);
	spriteBackground_->SetSize(kBarSize);
	spriteBackground_->SetColor(kColorBackground);

	// 2. 前景スプライト（水色）初期化
	spriteForeground_ = std::make_unique<Cygnus::Sprite>();
	spriteForeground_->Initialize(spriteCommon, texWhite);
	spriteForeground_->SetAnchorPoint(kAnchorPointBottomCenter);
	spriteForeground_->SetSize(kBarSize);
	spriteForeground_->SetColor(kColorForeground);
}

void PlayerOxygenUI::Update() {
	if (!player_) return;

	/* 酸素残り割合の計算と縦幅の適用 */
	float currentTimer = player_->GetAutoRewindTimer();
	float maxTimer = Player::GetMaxAutoRewindTime();

	// 経過時間から残り酸素割合を算出
	float oxygenRate = 1.0f;
	if (maxTimer > 0.0f) {
		oxygenRate = 1.0f - (currentTimer / maxTimer);
	}
	oxygenRate = std::clamp(oxygenRate, 0.0f, 1.0f);

	// 前景の縦幅を割合に応じて縮小
	Cygnus::Float2 currentFgSize = { kBarSize.x, kBarSize.y * oxygenRate };
	spriteForeground_->SetSize(currentFgSize);

	/* プレイヤー右側への追従座標計算 */
	Cygnus::Float3 worldPos = player_->GetTranslate() + kWorldOffset;
	Cygnus::Float3 screenPos3D = Utility::WorldToScreen(worldPos);

	Cygnus::Float2 screenPos = {
		screenPos3D.x,
		screenPos3D.y + (kBarSize.y * 0.5f)
	};

	//点滅処理
	UpdateBlink();

	// 座標を設定して行列更新
	spriteBackground_->SetPosition(screenPos);
	spriteForeground_->SetPosition(screenPos);

	spriteBackground_->Update();
	spriteForeground_->Update();
}

void PlayerOxygenUI::Draw() {
	if (!player_ || player_->IsDead()) return;

	spriteBackground_->Draw();
	spriteForeground_->Draw();
}

void PlayerOxygenUI::UpdateBlink() {
	// 酸素が残り30%以下になった場合に点滅させる
	if (player_) {
		float currentTimer = player_->GetAutoRewindTimer();
		float maxTimer = Player::GetMaxAutoRewindTime();

		float oxygenRate = 1.0f;

		if (maxTimer > 0.0f) {
			oxygenRate = 1.0f - (currentTimer / maxTimer);
		}

		oxygenRate = std::clamp(oxygenRate, 0.0f, 1.0f);

		if (oxygenRate <= 0.3f) {

			// DeltaTimeで点滅タイマーを進める
			float deltaTime = Cygnus::TimeManager::GetInstance()->GetDeltaTime();
			blinkTimer_ += deltaTime;

			// 0.0 ～ 1.0に正規化
			float t = blinkTimer_ / kBlinkInterval_;

			if (t >= 1.0f) {
				t = 0.0f;
				blinkTimer_ = 0.0f;
			}

			// 0 → 1 → 0 の値を作る
			float blinkT;

			if (t < 0.5f) {
				blinkT = t * 2.0f;
			}
			else {
				blinkT = (1.0f - t) * 2.0f;
			}

			// イージング
			float easeT = Cygnus::Easing::EaseInSine(blinkT);

			// 0.2 ～ 1.0の範囲で透明度を変化
			float alpha = 0.2f + easeT * 0.8f;

			Cygnus::Float4 color = kColorForeground;
			color.w = alpha;

			spriteForeground_->SetColor(color);

			//パーティクルを出す
			float randomValue = Cygnus::RandomGenerator::GetInstance()->RandomValue(0.0f, 1.0f);

			if(randomValue < 0.3f) {
				// Emitパーティクルを出す
				Cygnus::ParticleEffectManager::GetInstance()->Emit("low_oxy", player_->GetTranslate(), 1, Cygnus::Float3(0.0f, 0.0f, 0.0f), 0.0f);
			}
		}
		else {
			// 酸素が30%以上の場合は点滅をリセットして不透明にする
			spriteForeground_->SetColor(kColorForeground);
			blinkTimer_ = 0.0f;
		}
	}
}
