#pragma once
//C++
#include <vector>

//Engine
#include <Float3.h>

/// <summary>
///	花火単体の構造体
/// </summary>
struct FireworkParticle {
	Cygnus::Float3 position;	// パーティクルの位置
	float upTime;				// 上昇時間
};

/// <summary>
/// 花火パーティクルの管理クラス
/// </summary>
class FireworksManager {
public:
	// =========================================================
	// Public Methods
	// =========================================================
	
	/// <summary>
	/// インスタンスの取得を行います。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static FireworksManager* GetInstance();

	/// <summary>
	/// 更新処理を行います。
	/// </summary>
	void Update(float dt);

	/// <summary>
	/// デバッグ処理を行います。
	/// </summary>
	void Debug();

	/// <summary>
	/// 花火の生成を行います。
	/// </summary>
	/// <param name="position"></param>
	void CreateFireworks(const Cygnus::Float3& position);

private:
	// =========================================================
	// Member Variables
	// =========================================================

	std::vector<FireworkParticle> fireworks_;				/* 花火パーティクルのマップ */ 
	const float maxUpTime_ = 1.0f;					/* 花火の上昇時間の最大値 */
	const Cygnus::Float3 velocity = Cygnus::Float3(0.0f, 25.0f, 0.0f);


	Cygnus::Float3 emitPosition_;					/* (デバッグ用)発生位置 */

};

