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

	static constexpr float kMaxMoveTime_ = 1.5f;//移動するまでの時間
	static constexpr float kO2TimeExplainTime_ = 5.0f;//酸素ゲージの説明時間
	static constexpr float kMeteorExplainTime_ = kO2TimeExplainTime_ + 5.0f;//隕石の説明時間
	static constexpr float kItemExplainTime_ = kMeteorExplainTime_ + 4.5f;//アイテムの説明時間
	static constexpr float kPurposeExplainTime_ = kItemExplainTime_ + 4.5f;//ゲーム目的の説明時間
	static constexpr float kExplainFinishTime_ = kPurposeExplainTime_ + 0.5f;//good luck

	static constexpr Cygnus::Float2 kSkipPosition_ = { 20.0f,668.0f };
	static constexpr Cygnus::Float2 kSkipSize_ = { 240.0f,32.0f };

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
	/// 進捗
	/// </summary>
	/// <returns>現在の進捗</returns>
	int32_t NowExplain() {
		return progress_ + 1;
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