#pragma once
//C++
#include <vector>

//Engine
#include <Float2.h>
#include <Float3.h>

/// <summary>
///	単体の構造体
/// </summary>
struct DebuffParticle {
	Cygnus::Float3 position;	// 基礎座標
	float time;					// 演出時間
};

/// <summary>
/// デバフ演出の管理クラス
/// </summary>
class DebuffManager {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// インスタンスの取得を行います。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static DebuffManager* GetInstance();

	/// <summary>
	/// 更新処理を行います。
	/// </summary>
	void Update(float dt);

	/// <summary>
	/// デバッグ処理を行います。
	/// </summary>
	void Debug();

	/// <summary>
	/// 生成を行います。
	/// </summary>
	/// <param name="position"></param>
	void Create(const Cygnus::Float3& position);

private:
	// =========================================================
	// Member Variables
	// =========================================================

	std::vector<DebuffParticle> debuffs_;	/* パーティクルのマップ */

	const float kMaxTime_ = 0.8f;						/* パーティクルの最大時間 */
	const float kHeight_ = 5.0f;						/* 発生高度 */
	const Cygnus::Float2 kRange_ = { 2.0f,2.0f };		/* 発生範囲 */

	Cygnus::Float3 emitPosition_;						/* (デバッグ用)発生位置 */


};

