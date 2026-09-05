#include "PartsInventoryUI.h"

// Engine
#include <TextureManager.h>

// Application
#include <src/Game/Objects/Player/Player.h>

void PartsInventoryUI::Initialize(Cygnus::SpriteCommon* spriteCommon, const Player* player) {
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

		// 「x」記号
		slot.timesSymbol = std::make_unique<Cygnus::Sprite>();
		slot.timesSymbol->Initialize(spriteCommon, texTimes);
		slot.timesSymbol->SetAnchorPoint(kAnchorPointCenter);
		slot.timesSymbol->SetPosition({baseX + kOffsetTimesX, kYPos + kOffsetNumbersY });
		slot.timesSymbol->SetColor(kNumberColor);

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

		// 初期値の設定（初回Updateで必ず描画更新するため）
		slot.currentCount = -1;
	}
}

void PartsInventoryUI::Update() {
	// インベントリ背景更新
	spriteBackground_->Update();

	if(!player_) return;

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

void PartsInventoryUI::UpdateSlot(PartSlot& slot, int32_t count){
	// 負の値にならないようクリップ
	count = std::clamp(count, 0, 99);

	// 値に変更があった場合のみ数字のUVを再計算
	if(slot.currentCount != count) {
		slot.currentCount = count;

		int32_t tens = count / 10;
		int32_t ones = count % 10;

		SetDigitValue(slot.digitTens.get(), tens);
		SetDigitValue(slot.digitOnes.get(), ones);
	}

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
