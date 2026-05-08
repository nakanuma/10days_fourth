#include "Particle.h"

//c++
#include <cassert>

//application
#include <src/Util/JsonUtil.h>

//engine
#include <Engine/Util/RandomGenerator.h>

Particle::Particle(Cygnus::ModelManager::ModelData& model) {
	// オブジェクト設定
	object_.model_ = &model;
	object_.gTransformationMatrices_.numMaxInstance_ = kMaxParticles;
	object_.gTransformationMatrices_.Create();

	// ビルボード適用設定
	isBillboard_ = { false, false, false };
	// ブレンドモード設定
	blendMode_ = Cygnus::BlendMode::Normal;
}

void Particle::LoadJsonData(const std::string& fileName) {
	// =========================================================
	// JSONファイルの読み込み
	// =========================================================

	std::string fullPath = "../resources/Particles/" + fileName;

	std::optional<json> jsonData =
		JsonUtil::GetJsonData(fullPath);

	assert(jsonData && "Jsonファイルが存在しません");

	const json& j = jsonData.value();

	// =========================================================
	// 読み込んだデータを格納
	// =========================================================

	constantsData_.minScale = {
		j["minScale"][0],
		j["minScale"][1],
		j["minScale"][2]
	};
	constantsData_.maxScale = {
		j["maxScale"][0],
		j["maxScale"][1],
		j["maxScale"][2]
	};
	constantsData_.minVelocity = {
		j["minVelocity"][0],
		j["minVelocity"][1],
		j["minVelocity"][2]
	};
	constantsData_.maxVelocity = {
		j["maxVelocity"][0],
		j["maxVelocity"][1],
		j["maxVelocity"][2]
	};
	constantsData_.startColor = {
		j["startColor"][0],
		j["startColor"][1],
		j["startColor"][2],
		j["startColor"][3]
	};
	constantsData_.endColor = {
		j["endColor"][0],
		j["endColor"][1],
		j["endColor"][2],
		j["endColor"][3]
	};
	constantsData_.minLifeTime = j["minLifeTime"];
	constantsData_.maxLifeTime = j["maxLifeTime"];
	constantsData_.minRotationSpeed = {
		j["minRotationSpeed"][0],
		j["minRotationSpeed"][1],
		j["minRotationSpeed"][2]
	};
	constantsData_.maxRotationSpeed = {
		j["maxRotationSpeed"][0],
		j["maxRotationSpeed"][1],
		j["maxRotationSpeed"][2]
	};
	constantsData_.minAccerelation = {
		j["minAccerelation"][0],
		j["minAccerelation"][1],
		j["minAccerelation"][2]
	};
	constantsData_.maxAccerelation = {
		j["maxAccerelation"][0],
		j["maxAccerelation"][1],
		j["maxAccerelation"][2]
	};
	constantsData_.isBillboard = j["isBillboard"];
	constantsData_.blendMode = static_cast<Cygnus::BlendMode>(j["blendMode"]);
}

ParticleData Particle::CreateParticle(const Cygnus::Float3& pos, const Cygnus::Float3& velocity, float angle) {
	ParticleData p;
	auto rand = Cygnus::RandomGenerator::GetInstance();

	// 位置
	p.transform.translate_ = pos;
	// スケール
	p.transform.scale_ = kScale;
	// 速度ベクトル
	Cygnus::Float3 baseDir = Cygnus::Float3::Normalize(velocity) * -1.0f;				// 引数で受け取った方向と逆向きにする
	Cygnus::Float3 randDir = rand->RandomValue({ -kDirectionSpread, 0.0f, -kDirectionSpread }, { kDirectionSpread, 0.0f, kDirectionSpread }); // 方向をバラつかせるためのオフセット
	p.velocity = Cygnus::Float3::Normalize(baseDir + randDir) * rand->RandomValue(kMinSpeed, kMaxSpeed);
	// 回転（進行方向を向くように）
	Cygnus::Float3 dir = Cygnus::Float3::Normalize(p.velocity);
	float yaw = std::atan2(dir.x, dir.z);
	float pitch = -std::asin(dir.y);
	p.transform.rotate_ = { -pitch, -yaw, 0.0f };
	// 色
	p.color = kInitialColor;
	// 生存時間
	p.lifeTime = rand->RandomValue(kMinLifeTime, kMaxLifeTime);
	// 経過時間
	p.currentTime = 0.0f;
	// 初期スケール
	p.initScale = p.transform.scale_;

	return p;
}

void Particle::UpdateParticle(ParticleData& p, float dt) {

}
