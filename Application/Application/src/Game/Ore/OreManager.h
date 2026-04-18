#pragma once

// Application
#include <src/Game/Ore/Ore.h>

// =========================================================
// 鉱石オブジェクト管理クラス
// =========================================================
class OreManager
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static OreManager* GetInstance();

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

private:
	// =========================================================
	// Member Variables
	// =========================================================

	std::vector<std::unique_ptr<Ore>> ores_;	// 全ての鉱石オブジェクトを管理
};

