#pragma once
#include <memory>
#include <map>

// Application
#include <src/Game/UI/PartsUI/PartsInventoryUI.h> // プレイヤーのパーツ所持数UI
#include <src/Game/UI/Player/PlayerHPUI.h> // プレイヤーのHPゲージUI
#include <src/Game/UI/Player/PlayerOxygenUI.h> // プレイヤーの残り酸素UI
#include <src/Game/UI/Spaceship/SpaceshipDurabilityUI.h> // 宇宙船の耐久度UI
#include <src/Game/UI/Guide/ControlGuideUI.h> // 操作UI
#include <src/Game/UI/Timer/GameTimerUI.h> // 残り時間UI

/// <summary>
/// ゲーム説明UI(チュートリアルで使用)
/// </summary>
class ExplainUI {
public:

	void Initialize(Cygnus::SpriteCommon* spriteCommon, Player* player);

	void Update();

	void Draw();

	/// <summary>
	/// 次の段階に進む
	/// </summary>
	void NextUp();

	/// <summary>
	/// 説明終了
	/// </summary>
	/// <returns></returns>
	bool IsFinish() {
		return time_ >= kExplainFinishTime_;
	}

private:

	static constexpr Cygnus::Float2 kDescriptionTextureSize_ = { 960.0f, 48.0f };


	/* 各UI */
	std::unique_ptr<PartsInventoryUI> partsInventoryUI_; // プレイヤーのパーツ所持数UI
	std::unique_ptr<PlayerHPUI> playerHPUI_; // プレイヤーのHPバーUI
	std::unique_ptr<PlayerOxygenUI> playerOxygenUI_; // プレイヤーの残り酸素UI
	std::unique_ptr<ControlGuideUI> controlGuideUI_; // 操作UI

	std::unique_ptr<Cygnus::Sprite> descriptionUI_;
	Cygnus::Float2 position_ = {};
	Cygnus::Float2 textureLT_ = { 0.0f,0.0f };

	float time_ = 0.0f;

	static constexpr float kO2TimeExplainTime_ = 5.0f;
	static constexpr float kMeteorExplainTime_ = 10.0f;
	static constexpr float kExplainFinishTime_ = 15.0f;

};

/// <summary>
/// 行動判定
/// </summary>
class ActionJudgment {
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns></returns>
	static ActionJudgment* GetInstance();

	/// <summary>
	/// 行動判定の取得
	/// </summary>
	void AddAction(bool isFlag, int32_t order) {
		// 配列方に
		int32_t num = order - 1;
		if (numbers_ > num) {
			actionFlags_[num] = isFlag;
			return;
		}
		else if (numbers_ < num || progress_ > num) {
			return;
		}

		numbers_++;

		actionFlags_.push_back(isFlag);
	}

	/// <summary>
	/// 順番に行動判定を取る(1 2 3 ...)
	/// </summary>
	/// <returns></returns>
	bool first() {
		if (progress_ >= actionFlags_.size()) {
			return false;
		}
		return actionFlags_[progress_];
	}

	/// <summary>
	/// 成功　次の行動判定に
	/// </summary>
	void Success() {
		progress_++;
	}

	/// <summary>
	/// 全てリセット
	/// </summary>
	void Clear() {
		actionFlags_.clear();
		numbers_ = 0;
		progress_ = 0;
	}

private:
	std::vector<bool> actionFlags_;

	int32_t numbers_ = 0;
	int32_t progress_ = 0;
};