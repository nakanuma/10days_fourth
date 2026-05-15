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

	std::string fullPath = "resources/Particles/" + fileName;

	std::optional<json> jsonData =
		JsonUtil::GetJsonData(fullPath);

	assert(jsonData && "don't exist json file");

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

	// 初期位置
	p.transform.translate_ = pos;
	// 初期スケール
	p.transform.scale_ = rand->RandomValue(constantsData_.minScale, constantsData_.maxScale);
	// 初期回転
	angle;
	p.transform.rotate_ = { 0.0f, 0.0f, 0.0f};
	
	// 更新速度ベクトル
	velocity;
	p.updateTransform.translate_ = rand->RandomValue(constantsData_.minVelocity, constantsData_.maxVelocity);
	//更新スケール
	p.updateTransform.scale_ = rand->RandomValue(constantsData_.minScaleSpeed, constantsData_.maxScaleSpeed);
	// 更新回転
	p.updateTransform.rotate_ = rand->RandomValue(constantsData_.minRotationSpeed, constantsData_.maxRotationSpeed);

	// 更新加速度
	p.accerelation = rand->RandomValue(constantsData_.minAccerelation, constantsData_.maxAccerelation);
	
	// 色
	p.color = constantsData_.startColor;
	// 生存時間
	p.lifeTime = rand->RandomValue(constantsData_.minLifeTime, constantsData_.maxLifeTime);
	// 経過時間
	p.currentTime = 0.0f;

	// ビルボード設定
	isBillboard_ = { constantsData_.isBillboard, constantsData_.isBillboard, constantsData_.isBillboard };
	// ブレンドモード設定
	blendMode_ = constantsData_.blendMode;

	return p;
}

void Particle::UpdateParticle(ParticleData& p, float dt) {

	// 色の線形補間
	p.color = Cygnus::Float4::Lerp(constantsData_.startColor, constantsData_.endColor, p.currentTime / p.lifeTime);

	// スケールの更新
	p.transform.scale_ += p.updateTransform.scale_ * dt;
	// 回転の更新
	p.transform.rotate_ += p.updateTransform.rotate_ * dt;
	// 速度に加速度を加算
	p.updateTransform.translate_ += p.accerelation * dt;
	// 位置の更新
	p.transform.translate_ += p.updateTransform.translate_ * dt;


}
