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
	/// 鉱石の追加
	/// </summary>
	/// <param name="translate">座標位置(中心)</param>
	/// <param name="size">大きさ</param>
	void AddPoint(const Cygnus::Float3& translate, const Cygnus::Float3& size);

	/// 指定座標の一定範囲内にある鉱石を削除（プレイヤー側で採掘時に呼び出し）
	/// </summary>
	/// <param name="targetPos"></param>
	/// <param name="range"></param>
	/// <returns></returns>
	bool TryBreakAt(const Cygnus::Float3& targetPos, float range);

	/// <summary>
	/// 指定座標の射程範囲内に破壊可能な鉱石があるかチェックする
	/// </summary>
	/// <param name="targetPos"></param>
	/// <param name="range"></param>
	/// <returns></returns>
	bool IsBreakableAt(const Cygnus::Float3& targetPos, float range);
  

	/// <summary>
	/// 指定座標の一定範囲内にある全ての鉱石を削除（爆発や広範囲攻撃などを想定）
	/// </summary>
	/// <param name="targetPos">中心座標</param>
	/// <param name="range">半径</param>
	/// <returns>1つでも破壊したらtrue</returns>
	bool BreakAllAt(const Cygnus::Float3& targetPos, float range);
  
private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// 設定位置を鉱石の塊の中心点にする
	/// </summary>
	/// <param name="half">サイズの半分</param>
	/// <param name="slippagePoint">偶数の時のズレ</param>
	/// <param name="size">サイズ</param>
	void HalfChecker(float& half, float& slippagePoint, float size);

	/// <summary>
	/// 一括破壊を実行
	/// </summary>
	void BulkDestruction();
  
private:

	// =========================================================
	// Member Variables
	// =========================================================
	std::vector<std::unique_ptr<Ore>> ores_;	// 全ての鉱石オブジェクトを管理

	//半分
	const float kHalf_ = 0.5f;
	std::vector<std::unique_ptr<DroppedOre>> droppedOres_;	// 全ての落ちている（ドロップアイテム）鉱石オブジェクトを管理

	struct BreakRequest
	{
		Cygnus::Float3 targetPos;
		float range;
	};
	std::vector<BreakRequest> breakRequests_; // 範囲破壊時にここにリクエストを溜める
};

