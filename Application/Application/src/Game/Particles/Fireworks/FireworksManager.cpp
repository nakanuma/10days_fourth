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
	for(auto& firework : fireworks_) {
		//上昇時間を更新
		firework.upTime += dt;

		//上昇時間が最大値を超えた場合、花火を爆発させる
		if (firework.upTime >= maxUpTime_) {
			//花火の爆発エフェクトを発生させる
			Cygnus::ParticleEffectManager::GetInstance()->Emit("fireworks_diffusion", firework.position, 50, Cygnus::Float3(0.0f, 0.0f, 0.0f), 0.0f);
			//花火パーティクルを削除
			firework = fireworks_.back();
			fireworks_.pop_back();

			continue;
		}

		//花火の位置を更新
		firework.position.y += velocity.y * dt;

		//花火の上昇エフェクトを発生させる
		Cygnus::ParticleEffectManager::GetInstance()->Emit("fireworks_up", firework.position, 1, Cygnus::Float3(0.0f, 0.0f, 0.0f), 0.0f);

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
	firework.upTime = 0.0f;

	// 花火パーティクルをマップに追加
	fireworks_.push_back(firework);
}
