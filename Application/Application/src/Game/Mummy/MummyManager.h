#pragma once
#include "Mummy.h"
#include <vector>
class MummyManager {
public:
	/// <summary>
	/// インスタンス生成
	/// </summary>
	/// <returns></returns>
	static MummyManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="player">プレイヤーの</param>
	/// <param name="dt"></param>
	void Update(const Cygnus::Float3& playerPos, float dt);
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
	/// <summary>
	/// デバッグ
	/// </summary>
	void Debug();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="position"></param>
	void AddMummy(const Cygnus::Float3& position);

	/// <summary>
	/// ミイラ召喚上限
	/// 出しすぎ防止
	/// </summary>
	/// <returns>Maxでtrue</returns>
	bool MummyMax() {
		return mummies_.size() >= mummyCountMax;
	}

private:
	//ミイラの配列
	std::list<std::unique_ptr<Mummy>> mummies_;

	const uint32_t mummyCountMax = 20;//最大召喚数
};

