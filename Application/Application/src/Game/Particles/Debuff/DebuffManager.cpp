#include "DebuffManager.h"

//application
#include <ParticleEffect/ParticleEffectManager.h>

//Engine
#include <ImguiWrapper.h>

DebuffManager* DebuffManager::GetInstance() {
	static DebuffManager instance;
	return &instance;
}

void DebuffManager::Update(float dt) {
	// 全てのパーティクル更新
	for (size_t i = 0; i < debuffs_.size(); ) {

		auto& debuff = debuffs_[i];

		// =====================================================
		// 時間更新
		// =====================================================
		debuff.time += dt;





		++i;
	}
}

void DebuffManager::Debug() {
#ifdef _DEBUG
	ImGui::Begin("Debuffs Manager Debug");
	//発生位置の入力
	ImGui::DragFloat3("Emit Position", &emitPosition_.x, 0.1f);
	//発生
	if (ImGui::Button("Create")) {
		Create(emitPosition_);
	}

	ImGui::End();
#endif // _DEBUG

}

void DebuffManager::Create(const Cygnus::Float3& position) {
	// インタラクトの生成
	DebuffParticle debuff;
	debuff.position = position;
	debuff.time = 0.0f;

	// パーティクルをマップに追加
	debuffs_.push_back(debuff);
}
