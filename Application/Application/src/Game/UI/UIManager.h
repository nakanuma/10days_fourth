#pragma once

// Application
#include <src/Game/UI/HUD.h>
#include <src/Game/UI/InterctionUI.h>

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
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// =========================================================
	// Member Variables
	// =========================================================
	std::unique_ptr<Cygnus::SpriteCommon> spriteCommon_;	// スプライト共通クラス

	std::unique_ptr<HUD> hud_;	// 常に表示されるUI
	std::unique_ptr<InterctionUI> interctionUI_;	// 入力操作UI
};