#pragma once

// Application
#include <src/Game/UI/HUD.h>
#include <src/Game/UI/InterctionUI.h>

// =========================================================
// UI管理クラス
// =========================================================
class UIManager {
public:
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
	
};