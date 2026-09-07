#pragma once

// C++
#include <queue>

// Engine
#include <SpriteCommon.h>

// Application
#include <src/Game/UI/PartsUI/PartsInventoryUI.h> // プレイヤーのパーツ所持数UI
#include <src/Game/UI/Player/PlayerHPUI.h> // プレイヤーのHPゲージUI
#include <src/Game/UI/Player/PlayerOxygenUI.h> // プレイヤーの残り酸素UI
#include <src/Game/UI/Spaceship/SpaceshipDurabilityUI.h> // 宇宙船の耐久度UI
#include <src/Game/UI/Guide/ControlGuideUI.h> // 操作UI
#include <src/Game/UI/Timer/GameTimerUI.h> // 残り時間UI
#include <src/Game/UI/PartsUI/ItemPopupUI.h> // 修理パーツポップアップUI
#include <src/Game/UI/Player/DangerWarningUI.h> // 隕石接近危険マークUI
 
// =========================================================
// ゲーム中の全UIマネージャー
// =========================================================
class GameHUD
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Cygnus::SpriteCommon* spriteCommon, Player* player, const Spaceship* spaceship);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(float remainingTime, Tether* tether, FlyingObjectManager* flyingObjectManager);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ用
	/// </summary>
	void Debug();

	/// <summary>
	/// パーツの連続消費アニメーションを発火
	/// </summary>
	void StartConsumingParts() {
		if(partsInventoryUI_) {
			partsInventoryUI_->StartConsuming();
		}
	}

	/// <summary>
	/// 消費中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsConsumingParts() const {
		return partsInventoryUI_ ? partsInventoryUI_->IsConsuming() : false;
	}

	/// <summary>
	/// プレイヤー頭上に1つ生成
	/// </summary>
	/// <param name="type"></param>
	/// <param name="playerScreenPos"></param>
	void SpawnPlayerPopup(PartType type, std::function<Cygnus::Float3()> targetPosFunc);

	/// <summary>
	/// 宇宙船へ納品
	/// </summary>
	/// <param name="type"></param>
	/// <param name="count"></param>
	void QueueSpaceshipDeposit(PartType type, int count);

private:
	// =========================================================
	// Constants
	// =========================================================

	static constexpr float kDepositInterval = 0.08f; // 連続表示の間隔

	// =========================================================
	// Member Variables
	// =========================================================

	Cygnus::SpriteCommon* spriteCommon_ = nullptr;
	Player* player_;

	/* 各UI */
	std::unique_ptr<PartsInventoryUI> partsInventoryUI_; // プレイヤーのパーツ所持数UI
	std::unique_ptr<PlayerHPUI> playerHPUI_; // プレイヤーのHPバーUI
	std::unique_ptr<PlayerOxygenUI> playerOxygenUI_; // プレイヤーの残り酸素UI
	std::unique_ptr<SpaceshipDurabilityUI> spaceshipDurabilityUI_; // 宇宙船の耐久度UI
	std::unique_ptr<ControlGuideUI> controlGuideUI_; // 操作UI
	std::unique_ptr<GameTimerUI> gameTimerUI_; // 残り時間UI
	std::unique_ptr<DangerWarningUI> dangerWarningUI_; // 隕石接近危険マークUI

	/* 修理パーツポップアップUI管理 */
	std::vector<std::unique_ptr<ItemPopupUI>> activePopups_; // プレイヤーの修理パーツ取得時ポップアップ
	std::queue<PartType> depositQueue_; // 宇宙船納品用のポップアップ
	float depositTimer_ = 0.0f;
};

