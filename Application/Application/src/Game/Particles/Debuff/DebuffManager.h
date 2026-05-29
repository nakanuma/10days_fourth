#pragma once
//C++
#include <vector>

//Engine
#include <Float3.h>

/// <summary>
///	単体の構造体
/// </summary>
struct DebuffParticle {
	Cygnus::Float3 position;	// パーティクルの位置
	float time;				// 上昇時間
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

	const float kMaxTime_ = 1.0f;						/* パーティクルの最大時間 */
	const float kRiseHeight_ = 5.0f;						/* 上昇高さ */
	const float kRadius_ = 2.5f;						/* 回転縁の半径 */
	const float kAngleSpeed_ = 20.0f;					/* 回転速度 */

	Cygnus::Float3 emitPosition_;						/* (デバッグ用)発生位置 */


};

