#pragma once

// Application
#include <src/Game/UI/Parts/ItemCounter/ItemCounter.h>

// =========================================================
// すべてのUIを管理するクラス
// =========================================================
class UIManager {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static UIManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(uint32_t oreCount, bool isOreMax, uint32_t gearCount, bool isGearMax);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// =========================================================
	// Constants
	// =========================================================
	Cygnus::Float2 kOreCounterInitialPosition = {25.0f, 25.0f};   // プレイヤー鉱石所持数UI初期位置
	Cygnus::Float2 kGearCounterInitialPosition = {25.0f, 95.0f}; // プレイヤー歯車所持数UI初期位置

	// =========================================================
	// Member Variables
	// =========================================================
	std::unique_ptr<Cygnus::SpriteCommon> spriteCommon_;	// スプライト共通クラス

	std::unique_ptr<ItemCounter> oreCounter_;  // プレイヤー鉱石所持数UI
	std::unique_ptr<ItemCounter> gearCounter_;	// プレイヤー歯車所持数UI
};