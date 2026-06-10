#pragma once

// Application
#include <src/Game/WorkBench/WorkBench.h>

// =========================================================
// 工作台オブジェクト管理クラス
// =========================================================
class WorkBenchManager {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static WorkBenchManager* GetInstance();

	void Initialize();
	
	void Update();

	void Draw();

	/// <summary>
	/// ワークベンチを追加配置する
	/// </summary>
	void AddWorkBench(const Cygnus::Float3& position);

private:
	// =========================================================
	// Member Variables
	// =========================================================

	std::vector<std::unique_ptr<WorkBench>> workBenches_;	// 全ての工作台オブジェクト
};
