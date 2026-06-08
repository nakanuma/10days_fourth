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
	float diffusionHeight_ = 20.0f;						/* 花火の拡散高さ */

	Cygnus::Float3 emitPosition_;					/* (デバッグ用)発生位置 */

};

