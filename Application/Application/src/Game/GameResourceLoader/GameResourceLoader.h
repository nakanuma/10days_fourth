#pragma once

// ---------------------------------------------------------
// C++ Includes
// ---------------------------------------------------------
#include <string>
#include <memory>

// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include <ParticleEffect/ParticleEffectManager.h>
#include <ModelManager.h>

#include <src/Game/Particles/ParticleStrage.h>

// =========================================================
// テクスチャやモデルなどのリソースを予め読み込んでおくクラス
// =========================================================
class GameResourceLoader
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// インスタンスの取得を行います。
	/// </summary>
	/// <returns>シングルトンインスタンス</returns>
	static GameResourceLoader* GetInstance();

	/// <summary>
	/// 初期化処理を行います。
	/// </summary>
	void Initialize();

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// パーティクル登録を行います。
	/// </summary>
	/// <param name="particleEffectName">呼び出し時パーティクルエフェクト名</param>
	/// <param name="modelName">適用モデル名</param>
	void RegisterParticle(const std::string& particleEffectName, const std::string& modelName) {
		auto particle = std::make_unique<Particle>(Cygnus::ModelManager::GetInstance()->GetModel(modelName));
		particle->LoadJsonData(particleEffectName);	//新しく追加
		ParticleStrage::GetInstance()->Regist(particleEffectName, particle.get());   //新しく追加
		Cygnus::ParticleEffectManager::GetInstance()->Register(particleEffectName, std::move(particle));
	}


private:
	/// <summary>
	/// 全てのモデルデータの読み込みを行います。
	/// </summary>
	void LoadAllModelData();

	/// <summary>
	/// 全てのパーティクルの生成と登録を行います。
	/// </summary>
	void RegisterAllParticleEffect();

	/// <summary>
	/// 全てのサウンドデータの読み込みを行います。
	/// </summary>
	void LoadAllSoundData();
};