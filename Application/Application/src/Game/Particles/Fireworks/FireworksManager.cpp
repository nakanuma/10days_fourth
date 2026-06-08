#include "FireworksManager.h"

//application
#include <ParticleEffect/ParticleEffectManager.h>

//Engine
#include <ImguiWrapper.h>

FireworksManager* FireworksManager::GetInstance() {
	static FireworksManager instance;
	return &instance;
}

void FireworksManager::Update(float dt) {
	//全ての花火パーティクルの更新
	for (size_t i = 0; i < fireworks_.size(); ) {
		auto& firework = fireworks_[i];

		Cygnus::ParticleEffectManager::GetInstance()->Emit(
			"fireworks_diffusion",
			firework.position,
			150,
			Cygnus::Float3(0.0f, 0.0f, 0.0f),
			0.0f
		);

		fireworks_[i] = fireworks_.back();
		fireworks_.pop_back();
	}
}

void FireworksManager::Debug() {
#ifdef _DEBUG
	ImGui::Begin("Fireworks Manager Debug");
	//発生位置の入力
	ImGui::DragFloat3("Emit Position", &emitPosition_.x, 0.1f);
	//花火の発生
	if(ImGui::Button("Create")) {
		CreateFireworks(emitPosition_);
	}

	ImGui::End();
#endif // _DEBUG

}

void FireworksManager::CreateFireworks(const Cygnus::Float3& position) {
	// 花火の生成
	FireworkParticle firework;
	firework.position = position;
	firework.position.y = diffusionHeight_;

	// 花火パーティクルをマップに追加
	fireworks_.push_back(firework);
}
