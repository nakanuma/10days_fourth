#pragma once

// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include <Engine/Math/MyMath.h>
#include <Engine/Model/ModelManager.h>
#include <Engine/ParticleEffect/BaseParticleEffect.h>

/// <summary>
/// パーティクルデータ
/// </summary>
struct ParticleData {
	Cygnus::Transform initTransform;	/* 初期状態 */
	Cygnus::Transform updateTransform;	/* 更新状態 */
	Cygnus::Float4 color;				/* 色 */
	float lifeTime;						/* 生存時間 */
	float currentTime;					/* 経過時間 */
};

/// <summary>
/// 定数データ
/// </summary>
struct ConstantsData {
	Cygnus::Float3 minScale;				/* 最小スケール */
	Cygnus::Float3 maxScale;				/* 最大スケール */
	Cygnus::Float3 minVelocity;				/* 最小速度 */
	Cygnus::Float3 maxVelocity;				/* 最大速度 */
	Cygnus::Float4 startColor;				/* 開始色 */
	Cygnus::Float4 endColor;				/* 終了色 */
	float minLifeTime;						/* 最小生存時間 */
	float maxLifeTime;						/* 最大生存時間 */
	Cygnus::Float3 minRotationSpeed;		/* 最小回転速度 */
	Cygnus::Float3 maxRotationSpeed;		/* 最大回転速度 */
	Cygnus::Float3 minAccerelation;			/* 最小加速度 */
	Cygnus::Float3 maxAccerelation;			/* 最大加速度 */
	bool isBillboard;						/* ビルボードの有無 */
	Cygnus::BlendMode blendMode;			/* ブレンドモード */
};

class Particle : public Cygnus::BaseParticleEffect<ParticleData> {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// コンストラクタ
	/// </summary>
	/// <param name="model">モデルデータ</param>
	Particle(Cygnus::ModelManager::ModelData& model);

	/// <summary>
	/// JSONデータの読み込み
	/// </summary>
	/// <param name="fileName"></param>
	void LoadJsonData(const std::string& fileName);

protected:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// パーティクル固有の生成処理を行います。
	/// </summary>
	/// <param name="pos">初期位置</param>
	/// <param name="velocity">速度ベクトル</param>
	/// <param name="angle">初期回転角</param>
	/// <returns>パーティクルデータ</returns>
	ParticleData CreateParticle(const Cygnus::Float3& pos, const Cygnus::Float3& velocity, float angle) override;

	/// <summary>
	/// パーティクルの毎フレームの更新処理を行います。
	/// </summary>
	/// <param name="p">パーティクルデータ</param>
	/// <param name="dt">デルタタイム</param>
	void UpdateParticle(ParticleData& p, float dt) override;

private:
	// =========================================================
	// Constants
	// =========================================================

	//定数データ
	ConstantsData constantsData_;

};

