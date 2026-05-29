#include "InteractManager.h"

//application
#include <ParticleEffect/ParticleEffectManager.h>

//Engine
#include <ImguiWrapper.h>

InteractManager* InteractManager::GetInstance() {
	static InteractManager instance;
	return &instance;
}

void InteractManager::Update(float dt) {
	// 全てのパーティクル更新
	for (size_t i = 0; i < interacts_.size(); ) {

		auto& interact = interacts_[i];

		// =====================================================
		// 時間更新
		// =====================================================
		interact.time += dt;

		// 正規化時間
		float t = interact.time / kMaxTime_;

		// 終了判定
		if (t >= 1.0f) {
			interacts_[i] = interacts_.back();
			interacts_.pop_back();
			continue;
		}

		// =====================================================
		// 螺旋パラメータ
		// =====================================================

		// 回転角度
		float angle = t * kAngleSpeed_;

		// 半径（徐々に広がる）
		float radius = kRadius_;

		// 高さ
		float height = t * kRiseHeight_;

		// =====================================================
		// 基準座標から螺旋移動
		// =====================================================

		Cygnus::Float3 emitPos;

		emitPos.x =
			interact.position.x +
			std::cos(angle) * radius;

		emitPos.y =
			interact.position.y +
			height;

		emitPos.z =
			interact.position.z +
			std::sin(angle) * radius;

		// =====================================================
		// パーティクル発生
		// =====================================================

		Cygnus::ParticleEffectManager::GetInstance()->Emit(
			"interact",
			emitPos,
			1
		);

		++i;
	}
}

void InteractManager::Debug() {
#ifdef _DEBUG
	ImGui::Begin("Interacts Manager Debug");
	//発生位置の入力
	ImGui::DragFloat3("Emit Position", &emitPosition_.x, 0.1f);
	//花火の発生
	if (ImGui::Button("Create")) {
		Create(emitPosition_);
	}

	ImGui::End();
#endif // _DEBUG

}

void InteractManager::Create(const Cygnus::Float3& position) {
	// インタラクトの生成
	InteractParticle interact;
	interact.position = position;
	interact.time = 0.0f;

	// 花火パーティクルをマップに追加
	interacts_.push_back(interact);
}
