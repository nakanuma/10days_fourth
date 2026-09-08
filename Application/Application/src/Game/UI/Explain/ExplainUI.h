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

#include <src/Game/Objects/Player/Player.h>

/// <summary>
/// ゲーム説明UI(チュートリアルで使用)
/// </summary>
class ExplainUI {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="spriteCommon"></param>
	/// <param name="player"></param>
	void Initialize(Cygnus::SpriteCommon* spriteCommon, Player* player);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Player* player);

	/// <summary>
	/// 描画処理
	/// </summary>
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
		return autoReadTime_ >= kExplainFinishTime_;
	}

private:

	bool isMovingTimeMax(bool isMove);

	static constexpr Cygnus::Float2 kDescriptionTextureSize_ = { 960.0f, 48.0f };
	static constexpr float kMaxMoveTime_ = 4.0f;
	static constexpr float kO2TimeExplainTime_ = 5.0f;
	static constexpr float kMeteorExplainTime_ = 10.0f;
	static constexpr float kExplainFinishTime_ = 15.0f;

	static constexpr Cygnus::Float2 kSkipPosition_ = { 20.0f,668.0f };
	static constexpr Cygnus::Float2 kSkipSize_ = { 240.0f,32.0f };

	/* 各UI */
	std::unique_ptr<PartsInventoryUI> partsInventoryUI_; // プレイヤーのパーツ所持数UI
	std::unique_ptr<PlayerHPUI> playerHPUI_; // プレイヤーのHPバーUI
	std::unique_ptr<PlayerOxygenUI> playerOxygenUI_; // プレイヤーの残り酸素UI
	std::unique_ptr<ControlGuideUI> controlGuideUI_; // 操作UI


	// チュートリアルのみのUI
	std::unique_ptr<Cygnus::Sprite> skipUI_;

	std::unique_ptr<Cygnus::Sprite> descriptionUI_;
	Cygnus::Float2 textureLT_ = { 0.0f,0.0f };

	float movingTime_ = 0.0f;
	float autoReadTime_ = 0.0f;
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
	void IsAction(bool isFlag, int32_t order) {
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