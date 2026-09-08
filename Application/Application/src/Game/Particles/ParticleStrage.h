#pragma once

// ---------------------------------------------------------
// C++ Includes
// ---------------------------------------------------------
#include <string>
#include <unordered_map>

// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include <src/Game/Particles/Particle.h>

/// <summary>
/// パーティクルのリソースのポインタを保持するクラス
/// （実体を所有しているのはEngine側のため、アプリケーション側で出し入れするのが目的）
/// </summary>
class ParticleStrage {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// インスタンスの取得を行います。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static ParticleStrage* GetInstance();

	/// <summary>
	/// 登録
	/// </summary>
	/// <param name="_name">名前</param>
	/// <param name="particle">パーティクルのポインタ</param>
	void Regist(const std::string& _name, Particle* particle);

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// エフェクトの取得
	/// </summary>
	/// <returns></returns>
	std::unordered_map<std::string, Particle*>& GetEffects() { return effects_; }	/* パーティクルエフェクトのマップの取得 */

private:
	// =========================================================
	// Member Variables
	// =========================================================

	std::unordered_map<std::string, Particle*> effects_;			/* パーティクルエフェクトのマップ */

};

