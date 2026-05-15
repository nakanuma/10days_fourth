#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// =========================================================
// 操作表示UIクラス
// =========================================================
class InterctionUI {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(Cygnus::SpriteCommon* spriteCommon);

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
};