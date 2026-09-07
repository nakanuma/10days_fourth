#include "PartsInventoryUI.h"

// Engine
#include <TextureManager.h>
#include <TimeManager.h>
#include <Easing.h>
#include <SoundManager.h>

// Application
#include <src/Game/Objects/Player/Player.h>

void PartsInventoryUI::Initialize(Cygnus::SpriteCommon* spriteCommon, Player* player) {
	player_ = player;

	/* 各スプライト生成+初期化 */

	// インベントリ背景
	uint32_t texBackground = Cygnus::TextureManager::Load("inventoryBackground.png");
	spriteBackground_ = std::make_unique<Cygnus::Sprite>();
	spriteBackground_->Initialize(spriteCommon, texBackground);
	spriteBackground_->SetAnchorPoint(kAnchorPointCenter);
	spriteBackground_->SetPosition(kBackgroundInitPos);

	// 共通テクスチャのロード
	uint32_t texTimes = Cygnus::TextureManager::Load("x.png");
	texNumbers_ = Cygnus::TextureManager::Load("numbers.png");

	// 各アイコンテクスチャのロード
	std::array<uint32_t, PartType::Count> texIcons = {
		Cygnus::TextureManager::Load("icon_partLow.png"), // 低品質アイコン
		Cygnus::TextureManager::Load("icon_partMedium.png"), // 中品質アイコン
		Cygnus::TextureManager::Load("icon_partHigh.png"), // 高品質愛顧音
	};

	// 各スロットの共通初期化処理
	for(size_t i = 0; i < PartType::Count; ++i) {
		auto& slot = slots_[i];
		float baseX = kSlotXPositions[i];

		// アイコン
		slot.icon = std::make_unique<Cygnus::Sprite>();
		slot.icon->Initialize(spriteCommon, texIcons[i]);
		slot.icon->SetAnchorPoint(kAnchorPointCenter);
		slot.icon->SetPosition({baseX + kOffsetX, kYPos});
		slot.iconBaseSize = slot.icon->GetSize();

		// 「x」記号
		slot.timesSymbol = std::make_unique<Cygnus::Sprite>();
		slot.timesSymbol->Initialize(spriteCommon, texTimes);
		slot.timesSymbol->SetAnchorPoint(kAnchorPointCenter);
		slot.timesSymbol->SetPosition({baseX + kOffsetTimesX, kYPos + kOffsetNumbersY });
		slot.timesSymbol->SetColor(kNumberColor);
		slot.timesBaseSize = slot.timesSymbol->GetSize();

		// 十の位の数字
		slot.digitTens = std::make_unique<Cygnus::Sprite>();
		slot.digitTens->Initialize(spriteCommon, texNumbers_);
		slot.digitTens->SetAnchorPoint(kAnchorPointCenter);
		slot.digitTens->SetPosition({baseX + kOffsetTensX, kYPos + kOffsetNumbersY });
		slot.digitTens->SetSize(kDigitDisplaySize); // 描画サイズ設定
		slot.digitTens->SetTextureSize(kDigitTextureSize); // 切り出しサイズ
		slot.digitTens->SetColor(kNumberColor);

		// 一の位の数字
		slot.digitOnes = std::make_unique<Cygnus::Sprite>();
		slot.digitOnes->Initialize(spriteCommon, texNumbers_);
		slot.digitOnes->SetAnchorPoint(kAnchorPointCenter);
		slot.digitOnes->SetPosition({baseX + kOffsetOnesX, kYPos + kOffsetNumbersY });
		slot.digitOnes->SetSize(kDigitDisplaySize); // 描画サイズ設定
		slot.digitOnes->SetTextureSize(kDigitTextureSize); // 切り出しサイズ
		slot.digitOnes->SetColor(kNumberColor);

		slot.digitBaseSize = kDigitDisplaySize;

		// 初期値の設定（初回Updateで必ず描画更新するため）
		slot.currentCount = -1;

		slot.popTimer = 0.0f;
	}
}

void PartsInventoryUI::Update() {
	// インベントリ背景更新
	spriteBackground_->Update();

	if(!player_) return;

	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	/* 連続消費処理の更新 */
	if(isConsuming_) {
		consumeTimer_ += dt;

		if(consumeTimer_ >= kConsumeInterval) {
			consumeTimer_ -= kConsumeInterval;

			// 現在対象のパーツ数をプレイヤーから取得して1減らす
			int32_t low = player_->GetRepairPartLowCount();
			int32_t med = player_->GetRepairPartMediumCount();
			int32_t high = player_->GetRepairPartHighCount();

			if(currentConsumeIndex_ == PartType::Low) {
				if(low > 0) {
					player_->SetRepairPartLowCount(low - 1);
				} else {
					currentConsumeIndex_++; // 次のパーツ種へ
				}
			}

			if(currentConsumeIndex_ == PartType::Medium) {
				if(med > 0) {
					player_->SetRepairPartMediumCount(med - 1);
				} else {
					currentConsumeIndex_++; // 次のパーツ種へ
				}
			}

			if(currentConsumeIndex_ == PartType::High) {
				if (high > 0) {
					player_->SetRepairPartHighCount(high - 1);
				} else {
					// 全てのパーツを消費しきった
					isConsuming_ = false;
				}
			}
		}
	}

	/* 各スロットの描画+アニメーション更新 */

	// 各パーツのリアルタイム所持数を取得
	std::array<int32_t, PartType::Count> counts = {
		player_->GetRepairPartLowCount(),
		player_->GetRepairPartMediumCount(),
		player_->GetRepairPartHighCount()
	};

	// 各スロットの更新
	for(size_t i = 0; i < PartType::Count; ++i) {
		UpdateSlot(slots_[i], counts[i]);
	}
}

void PartsInventoryUI::Draw() {
	// インベントリ背景描画
	spriteBackground_->Draw();

	// 各スロット描画
	for(const auto& slot : slots_) {
		slot.icon->Draw();
		slot.timesSymbol->Draw();
		slot.digitTens->Draw();
		slot.digitOnes->Draw();
	}
}

void PartsInventoryUI::StartConsuming()
{
	isConsuming_ = true;
	consumeTimer_ = 0.0f;
	currentConsumeIndex_ = 0; // Lowパーツから順に減らす
}

void PartsInventoryUI::UpdateSlot(PartSlot& slot, int32_t count){
	// 負の値にならないようクリップ
	count = std::clamp(count, 0, 99);
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	// 値の変更チェック
	if(slot.currentCount != count) {
		// 初回以外でカウントが増減した場合のみアニメーション発火
		if(slot.currentCount != -1) {
			slot.popTimer = kPopDuration;

			// パーツが消費された場合のみSE再生
			if(count < slot.currentCount) {
				Cygnus::SoundManager::GetInstance()->Play("se_consume", false, 0.75f); // SE再生（消費）
			}
		}
		
		slot.currentCount = count;

		int32_t tens = count / 10;
		int32_t ones = count % 10;

		SetDigitValue(slot.digitTens.get(), tens);
		SetDigitValue(slot.digitOnes.get(), ones);
	}

	// イージングアニメーション
	float currentScale = 1.0f;
	if(slot.popTimer > 0.0f) {
		slot.popTimer -= dt;
		if(slot.popTimer < 0.0f) slot.popTimer = 0.0f;

		// 進行度
		float t = slot.popTimer / kPopDuration;
		float easeFactor = std::sinf(t * Cygnus::PIf);

		// 拡大スケールの算出
		currentScale = 1.0f + (kMaxScale - 1.0f) * easeFactor;
	}

	// スケールを書くスプライトに適用
	slot.icon->SetSize({slot.iconBaseSize.x * currentScale, slot.iconBaseSize.y * currentScale});
	slot.timesSymbol->SetSize({slot.timesBaseSize.x * currentScale, slot.timesBaseSize.y * currentScale});
	slot.digitTens->SetSize({slot.digitBaseSize.x * currentScale, slot.digitBaseSize.y * currentScale});
	slot.digitOnes->SetSize({slot.digitBaseSize.x * currentScale, slot.digitBaseSize.y * currentScale});

	// 各スプライトの更新
	slot.icon->Update();
	slot.timesSymbol->Update();
	slot.digitTens->Update();
	slot.digitOnes->Update();
}

void PartsInventoryUI::SetDigitValue(Cygnus::Sprite* sprite, int32_t digit){
	if(!sprite) return;

	// 数字に応じてテクスチャの左上ピクセル位置をオフセット設定
	sprite->SetTextureLeftTop({digit * kDigitTextureSize.x, 0.0f});
}
