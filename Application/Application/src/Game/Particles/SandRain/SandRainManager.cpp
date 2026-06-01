#include "SandRainManager.h"
//C++
#include <random>


//application
#include <ParticleEffect/ParticleEffectManager.h>

//Engine
#include <ImguiWrapper.h>

SandRainManager* SandRainManager::GetInstance() {
	static SandRainManager instance;
	return &instance;
}

void SandRainManager::Update(float dt) {
	//発生フラグが立っていない場合は処理を行わない
	if (!isEmitting_) {
		return;
	}

	//ランダムな位置に砂嵐のパーティクルを発生させる
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_real_distribution<float> disX(-fieldSize_.x / 2.0f, fieldSize_.x / 2.0f);
	std::uniform_real_distribution<float> disZ(-fieldSize_.y / 2.0f, fieldSize_.y / 2.0f);

	//1fに数箇所から砂粒を発生させる
	for (int i = 0; i < 15; ++i) {
		Cygnus::Float3 position = Cygnus::Float3(disX(gen), emitHeight_, disZ(gen));

		Cygnus::ParticleEffectManager::GetInstance()->Emit("sand_rain", position, 1);
	}

}

void SandRainManager::Debug() {
#ifdef _DEBUG
	ImGui::Begin("Sand Rain Manager Debug");

	//発生フラグ
	ImGui::Checkbox("Emitting", &isEmitting_);

	ImGui::End();

#endif // _DEBUG

}
