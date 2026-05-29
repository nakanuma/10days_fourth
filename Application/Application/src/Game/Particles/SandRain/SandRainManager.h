#pragma once
//C++
#include <vector>

//Engine
#include <Float2.h>
#include <Float3.h>

/// <summary>
/// 砂嵐パーティクル管理クラス
/// </summary>
class SandRainManager {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// インスタンスの取得を行います。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static SandRainManager* GetInstance();

	/// <summary>
	/// 更新処理を行います。
	/// </summary>
	void Update(float dt);

	/// <summary>
	/// デバッグ処理を行います。
	/// </summary>
	void Debug();

	/// <summary>
	/// 発生フラグの設定を行います。
	/// </summary>
	/// <param name="isEmitting">発生させるか</param>
	void SetEmitting(bool isEmitting) { isEmitting_ = isEmitting; }

private:
	// =========================================================
	// Member Variables
	// =========================================================

	//フィールド変数
	const Cygnus::Float2 fieldSize_ = Cygnus::Float2(80.0f, 80.0f);	/* 砂嵐の発生範囲 */
	const float emitHeight_ = 60.0f;						/* 砂嵐の発生高さ */

	//砂嵐を降らせるかのフラグ
	bool isEmitting_ = false;						/* 砂嵐の発生フラグ */

};

