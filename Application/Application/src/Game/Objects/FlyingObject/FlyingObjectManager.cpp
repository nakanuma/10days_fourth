#include "FlyingObjectManager.h"

// Engine
#include <ImguiWrapper.h>
#include <TimeManager.h>
#include <RandomGenerator.h>

// Application
#include <src/Game/Objects/FlyingObject/Meteor/MeteorSmall/MeteorSmall.h> // 隕石（小）
#include <src/Game/Objects/FlyingObject/Meteor/MeteorLarge/MeteorLarge.h> // 隕石（大）

#include <src/Game/Objects/FlyingObject/RepairPart/RepairPartLow/RepairPartLow.h> // 修理パーツ（低品質）
#include <src/Game/Objects/FlyingObject/RepairPart/RepairPartMidium/RepairPartMidium.h> // 修理パーツ（中品質）
#include <src/Game/Objects/FlyingObject/RepairPart/RepairpartHigh/RepairPartHigh.h> // 修理パーツ（高品質）

void FlyingObjectManager::Initialize() {
	objects_.clear();
}

void FlyingObjectManager::Update() {
	// 自動スポーン処理
	AutoSpawn();

	// オブジェクト更新
	for (auto& obj : objects_) {
		obj->Update();
	}

	// 画面外に出た or 破壊されたオブジェクトを自動削除
	std::erase_if(objects_, [](const std::unique_ptr<FlyingObject>& obj) {
		return obj->IsDead();
		});
}

void FlyingObjectManager::Draw() {
	// オブジェクト描画
	for (auto& obj : objects_) {
		obj->Draw();
	}
}

void FlyingObjectManager::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("FlyingObjectManager");

	if (ImGui::Button("Spawn : MeteorSmall")) {
		Spawn<MeteorSmall>(Cygnus::Float3{ 25.0f, -10.0f, 0.0f });
	}
	if (ImGui::Button("Spawn : MeteorLarge")) {
		Spawn<MeteorLarge>(Cygnus::Float3{ 25.0f, -10.0f, 0.0f });
	}
	ImGui::Separator();
	if (ImGui::Button("Spawn : RepairPartLow")) {
		Spawn<RepairPartLow>(Cygnus::Float3{ 25.0f, -10.0f, 0.0f });
	}
	if (ImGui::Button("Spawn : RepairPartMidium")) {
		Spawn<RepairPartMidium>(Cygnus::Float3{ 25.0f, -10.0f, 0.0f });
	}
	if (ImGui::Button("Spawn : RepairPartHigh")) {
		Spawn<RepairPartHigh>(Cygnus::Float3{ 25.0f, -10.0f, 0.0f });
	}

	ImGui::End();
#endif
}

void FlyingObjectManager::AutoSpawn() {
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();
	auto rng = Cygnus::RandomGenerator::GetInstance();

	// スポーン方向と初期位置を決定するラムダ関数
	auto GetRandomSpawnPos = [&](float spawnY, bool& outIsRightToLeft) -> Cygnus::Float3 {
		// 50％の確率で右スポーンか左スポーンかを決定
		outIsRightToLeft = rng->RandomValueBool(0.5f);
		float spawnX = outIsRightToLeft ? kSpawnX : -kSpawnX;
		return Cygnus::Float3{ spawnX, spawnY, 0.0f };
	};

	// 隕石（小）
	timerMeteorSmall_ += dt;
	if (timerMeteorSmall_ >= kIntervalMeteorSmall) {
		timerMeteorSmall_ = 0.0f;
		bool isRightToLeft = true;
		auto pos = GetRandomSpawnPos(rng->RandomValue(kSpawnMinY, kSpawnMaxY), isRightToLeft);
		Spawn<MeteorSmall>(pos, isRightToLeft);
	}

	// 隕石（大）
	timerMeteorLarge_ += dt;
	if (timerMeteorLarge_ >= kIntervalMeteorLarge) {
		timerMeteorLarge_ = 0.0f;
		bool isRightToLeft = true;
		auto pos = GetRandomSpawnPos(rng->RandomValue(kSpawnMinY, kSpawnMaxY), isRightToLeft);
		Spawn<MeteorLarge>(pos, isRightToLeft);
	}

	// 修理パーツ（低品質）
	timerRepairLow_ += dt;
	if (timerRepairLow_ >= kIntervalRepairLow) {
		timerRepairLow_ = 0.0f;
		bool isRightToLeft = true;
		auto pos = GetRandomSpawnPos(rng->RandomValue(kUpperLimitY, kSpawnMaxY), isRightToLeft);
		Spawn<RepairPartLow>(pos, isRightToLeft);
	}

	// 修理パーツ（中品質）
	timerRepairMid_ += dt;
	if (timerRepairMid_ >= kIntervalRepairMid) {
		timerRepairMid_ = 0.0f;
		bool isRightToLeft = true;
		auto pos = GetRandomSpawnPos(rng->RandomValue(kMiddleLimitY, kUpperLimitY), isRightToLeft);
		Spawn<RepairPartMidium>(pos, isRightToLeft);
	}

	// 修理パーツ（高品質）
	timerRepairHigh_ += dt;
	if (timerRepairHigh_ >= kIntervalRepairHigh) {
		timerRepairHigh_ = 0.0f;
		bool isRightToLeft = true;
		auto pos = GetRandomSpawnPos(rng->RandomValue(kSpawnMinY, kMiddleLimitY), isRightToLeft);
		Spawn<RepairPartHigh>(pos, isRightToLeft);
	}
}
