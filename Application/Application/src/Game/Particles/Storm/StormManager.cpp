#include "StormManager.h"
//C++
#include <random>


//application
#include <src/Game/Particles/SandRain/SandRainManager.h>

//Engine
#include <ImguiWrapper.h>
#include <ParticleEffect/ParticleEffectManager.h>

StormManager* StormManager::GetInstance() {
	static StormManager instance;
	return &instance;
}

void StormManager::Update(float dt) {
	//竜巻の情報が存在しない場合は処理を行わない
	if (stormParams_.empty()) {
		//砂嵐をオフにする
		SandRainManager::GetInstance()->SetEmitting(false);

		return;
	}
	//砂嵐をオンにする
	SandRainManager::GetInstance()->SetEmitting(true);

	//各竜巻の位置に竜巻のパーティクルを発生させる
	for (auto& param : stormParams_) {
		//基準座標（竜巻の中心座標）
		Cygnus::Float3 basePosition = *param.position;
		basePosition.y += 0.0f;

		//各エミッターの更新（竜巻の座標に合わせて動かす）
		for (auto& emitterParam : param.emitterPositions) {
			//angleの更新
			emitterParam.angle += emitterParam.speed * dt;

			//オフセットの計算
			Cygnus::Float3 offset;
			offset.x = std::cos(emitterParam.angle) * emitterParam.radius;
			offset.y = 0.0f;
			offset.z = std::sin(emitterParam.angle) * emitterParam.radius;

			//エミッターの座標を基準座標にオフセットを加算して計算
			emitterParam.position = offset + Cygnus::Float3(0.0f, emitterParam.position.y, 0.0f);

			//実際の発生座標を計算
			Cygnus::Float3 emitterPosition = basePosition + emitterParam.position;
			//竜巻のパーティクルを発生させる
			Cygnus::ParticleEffectManager::GetInstance()->Emit("sand_grain", emitterPosition, 1);
		}
	}
}

void StormManager::Debug() {
#ifdef _DEBUG
	ImGui::Begin("Storm Manager Debug");

	if (CheckExist(debugName_)) {
		//存在伝達
		ImGui::Text("Storm with name '%s' already exists.", debugName_.c_str());
		//座標の入力
		ImGui::DragFloat3("Position", &debugPosition_.x);
		//削除ボタン
		if (ImGui::Button("Delete Storm")) {
			Delete(debugName_);
		}
	}
	else {
		//存在しないことの伝達
		ImGui::Text("No storm with name '%s' exists.", debugName_.c_str());
		//作成ボタン
		if (ImGui::Button("Create Storm")) {
			Create(debugName_, &debugPosition_);
		}
	}


	ImGui::End();

#endif // _DEBUG

}

bool StormManager::CheckExist(const std::string& name) {
	for (const auto& param : stormParams_) {
		if (param.name == name) {
			return true; //同じ名前の竜巻が存在するため、存在確認に成功
		}
	}
	return false; //同じ名前の竜巻が存在しないため、存在確認に失敗
}

bool StormManager::Create(const std::string& name, Cygnus::Float3* position) {
	//同じ名前の竜巻が存在するか確認
	for (const auto& param : stormParams_) {
		if (param.name == name) {
			return false; //同じ名前の竜巻が存在するため、作成に失敗
		}
	}
	//ランダムエンジンの初期化
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> disAngle(0.0f, 360.0f);

	//新しい竜巻の情報を作成してコンテナに追加
	StormParam newParam;
	newParam.name = name;
	newParam.position = position;
	//エミッターの情報を書き込む→0番が一番低い値で徐々に高くしていく（線形補完）
	for (int i = 0; i < emitterCount_; ++i) {
		EmitterParam emitterParam;
		float t = static_cast<float>(i) / (emitterCount_ - 1); //0から1の範囲で補間係数を計算
		emitterParam.position = Cygnus::Float3(0.0f, heightRange_.x + t * (heightRange_.y - heightRange_.x), 0.0f);
		emitterParam.radius = radiusRange_.x + t * (radiusRange_.y - radiusRange_.x);
		emitterParam.speed = speedRange_.x + t * (speedRange_.y - speedRange_.x);
		//angleはランダムに設定
		emitterParam.angle = disAngle(gen);

		newParam.emitterPositions.push_back(emitterParam);
	}

	stormParams_.push_back(newParam);
}

bool StormManager::Delete(const std::string& name) {
	//同じ名前の竜巻が存在するか確認
	for (auto it = stormParams_.begin(); it != stormParams_.end(); ++it) {
		if (it->name == name) {
			stormParams_.erase(it); //同じ名前の竜巻が存在するため、削除して成功
			return true;
		}
	}
	return false; //同じ名前の竜巻が存在しないため、削除に失敗
}
