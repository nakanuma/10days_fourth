#pragma once

// C++
#include <queue>

// Application
#include <src/Game/Objects/FlyingObject/Base/FlyingObject.h>

// =========================================================
// 飛翔物の管理クラス
// =========================================================
class FlyingObjectManager {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Debug();

	/// <summary>
	/// 飛翔物のスポーン処理
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="position"></param>
	/// <param name="speed"></param>
	template<typename T> void Spawn(const Cygnus::Float3& position, bool isRightToLeft = true) { 
		auto newObj = std::make_unique<T>();
		newObj->Initialize(position, isRightToLeft);
		objects_.push_back(std::move(newObj));
	}

	/// <summary>
	/// 画面内の全隕石を時間差で連続破壊する
	/// </summary>
	void DestroyAllMeteorsSequential();

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// 飛翔物の配列を取得
	/// </summary>
	/// <returns></returns>
	const std::vector<std::unique_ptr<FlyingObject>>& GetObjects() const { return objects_; }

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// 自動スポーン処理
	/// </summary>
	void AutoSpawn();

private:
	// =========================================================
	// Constants
	// =========================================================

	// Y座標の範囲
	static constexpr float kSpawnMaxY = -10.0f;
	static constexpr float kSpawnMinY = -55.0f;

	// 各層の境界
	static constexpr float kUpperLimitY = -25.0f;
	static constexpr float kMiddleLimitY = -40.0f;

	// スポーン時のX座標
	static constexpr float kSpawnX = 50.0f;

	// 仮のスポーン間隔
	static constexpr float kIntervalMeteorSmall = 3.0f;
	static constexpr float kIntervalMeteorLarge = 5.0f;
	static constexpr float kIntervalRepairLow = 3.0f;
	static constexpr float kIntervalRepairMid = 4.0f;
	static constexpr float kIntervalRepairHigh = 5.0f;
	static constexpr float kIntervalHeartItem = 15.0f;
	static constexpr float kIntervalBombItem = 30.0f;

	// 連続破壊用の設定
	static constexpr float kDestroyInterval = 0.05f; // 破壊間隔（秒）

	// =========================================================
	// Member Variables
	// =========================================================

	// 飛翔物の配列
	std::vector<std::unique_ptr<FlyingObject>> objects_;

	// 各オブジェクトのタイマー
	float timerMeteorSmall_ = 0.0f;
	float timerMeteorLarge_ = 0.0f;
	float timerRepairLow_ = 0.0f;
	float timerRepairMid_ = 0.0f;
	float timerRepairHigh_ = 0.0f;
	float timerHeartItem_ = 0.0f;
	float timerBombItem_ = 0.0f;

	// 連続破壊管理用
	std::queue<FlyingObject*> destroyQueue_; // 破壊待ちリスト
	float destroyTimer_ = 0.0f; // タイマー
};
