#pragma once

// Application
#include <src/Game/Ore/Ore.h>
#include <src/Game/Ore/DroppedOre.h>

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

	/// <summary>
	/// 指定座標の一定範囲内にある鉱石を削除（プレイヤー側で採掘時に呼び出し）
	/// </summary>
	/// <param name="targetPos"></param>
	/// <param name="range"></param>
	/// <returns></returns>
	bool TryBreakAt(const Cygnus::Float3& targetPos, float range);

private:
	// =========================================================
	// Member Variables
	// =========================================================

	std::vector<std::unique_ptr<Ore>> ores_;	// 全ての鉱石オブジェクト
	std::vector<std::unique_ptr<DroppedOre>> droppedOres_;	// 全ての落ちている（ドロップアイテム）鉱石オブジェクト
};

