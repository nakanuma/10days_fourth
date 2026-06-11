#pragma once
//C++
#include <vector>

//Engine
#include <Float3.h>

/// <summary>
///	単体の構造体
/// </summary>
struct InteractParticle {
	Cygnus::Float3 position;	// パーティクルの位置
	float time;				// 上昇時間
};

/// <summary>
/// インタラクト演出の管理クラス
/// </summary>
class InteractManager {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// インスタンスの取得を行います。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static InteractManager* GetInstance();

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

	std::vector<InteractParticle> interacts_;	/* 花火パーティクルのマップ */

	const float kMaxTime_ = 0.5f;						/* パーティクルの最大時間 */
	const float kRiseHeight_ = 7.0f;						/* 上昇高さ */
	const float kRadius_ = 2.2f;						/* 回転縁の半径 */
	const float kAngleSpeed_ = 22.0f;					/* 回転速度 */

	Cygnus::Float3 emitPosition_;						/* (デバッグ用)発生位置 */

};

