#pragma once

// Application
#include <src/Game/Gear/DroppedGear.h>

// =========================================================
// 歯車オブジェクト管理クラス
// =========================================================
class GearManager {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static GearManager* GetInstance();

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

	std::vector<std::unique_ptr<DroppedGear>> droppedGears_; // 全ての落ちている（ドロップアイテム）歯車オブジェクトを管理
};
