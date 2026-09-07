#include "GameHUD.h"

// Engine
#include <TimeManager.h>
#include <ImguiWrapper.h>

void GameHUD::Initialize(Cygnus::SpriteCommon* spriteCommon, Player* player, const Spaceship* spaceship) {
	spriteCommon_ = spriteCommon;
	player_ = player;

	/* 各UI生成+初期化 */
	
	// プレイヤーのパーツ所持数UI
	partsInventoryUI_ = std::make_unique<PartsInventoryUI>();
	partsInventoryUI_->Initialize(spriteCommon_, player);

	// プレイヤーのHPバーUI
	playerHPUI_ = std::make_unique<PlayerHPUI>();
	playerHPUI_->Initialize(spriteCommon_, player);

	// プレイヤーの残り酸素UI
	playerOxygenUI_ = std::make_unique<PlayerOxygenUI>();
	playerOxygenUI_->Initialize(spriteCommon_, player);

	// 宇宙船の耐久度UI
	spaceshipDurabilityUI_= std::make_unique<SpaceshipDurabilityUI>();
	spaceshipDurabilityUI_->Initialize(spriteCommon_, spaceship);

	// 操作UI
	controlGuideUI_ = std::make_unique<ControlGuideUI>();
	controlGuideUI_->Initialize(spriteCommon_);

	// 残り時間UI
	gameTimerUI_ = std::make_unique<GameTimerUI>();
	gameTimerUI_->Initialize(spriteCommon_);

	// 危険マークUI
	dangerWarningUI_ = std::make_unique<DangerWarningUI>();
	dangerWarningUI_->Initialize(spriteCommon_);
}

void GameHUD::Update(float remainingTime, Tether* tether, FlyingObjectManager* flyingObjectManager) {
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	/* 修理パーツポップアップUI更新 */
	if (!depositQueue_.empty()) {
		depositTimer_ += dt;
		if (depositTimer_ >= kDepositInterval) {
			depositTimer_ = 0.0f;

			PartType type = depositQueue_.front();
			depositQueue_.pop();

			// 宇宙船ゲージのスクリーンの位置を取得してポップアップ生成
			auto popup = std::make_unique<ItemPopupUI>();
			popup->Initialize(spriteCommon_, type, [this]() { 
				return spaceshipDurabilityUI_->GetSpaceshipTranslate();
			});
			activePopups_.push_back(std::move(popup));
		}
	}
	// 各アクティブUIの更新・自動破棄
	for (auto& popup : activePopups_) {
		popup->Update();
	}
	auto it = std::remove_if(activePopups_.begin(), activePopups_.end(), 
		[](const std::unique_ptr<ItemPopupUI>& p) { return p->IsDead(); });
	activePopups_.erase(it, activePopups_.end());

	/* 各UI更新 */
	partsInventoryUI_->Update();
	playerHPUI_->Update();
	playerOxygenUI_->Update();
	spaceshipDurabilityUI_->Update();
	controlGuideUI_->Update();
	gameTimerUI_->Update(remainingTime);
	dangerWarningUI_->Update(player_, tether, flyingObjectManager);
}

void GameHUD::Draw() {
	/* 各UI更新 */
	partsInventoryUI_->Draw();
	playerHPUI_->Draw();
	playerOxygenUI_->Draw();
	spaceshipDurabilityUI_->Draw();
	controlGuideUI_->Draw();
	gameTimerUI_->Draw();
	dangerWarningUI_->Draw();

	// ポップアップの描画
	for (auto& popup : activePopups_) {
		popup->Draw();
	}
}

void GameHUD::Debug() { 
#ifdef USE_IMGUI
	ImGui::Begin("GameHUD");

	ImGui::Text("Deposit Queue Size: %zu", depositQueue_.size());
	ImGui::Text("Active popups Count: %zu", activePopups_.size());

	if (ImGui::TreeNode("Active Popups Detail")) {
		for (size_t i = 0; i < activePopups_.size(); ++i) {
			// 各ポップアップのデバッグ情報を表示
			activePopups_[i]->DebugUI(static_cast<int>(i));
		}
		ImGui::TreePop();
	}

	ImGui::End();
#endif
}

void GameHUD::SpawnPlayerPopup(PartType type, std::function<Cygnus::Float3()> targetPosFunc) { 
	auto popup = std::make_unique<ItemPopupUI>(); 
	popup->Initialize(spriteCommon_, type, targetPosFunc);
	activePopups_.push_back(std::move(popup));
}

void GameHUD::QueueSpaceshipDeposit(PartType type, int count) {
	for (int i = 0; i < count; ++i) {
		depositQueue_.push(type);
	}
}
