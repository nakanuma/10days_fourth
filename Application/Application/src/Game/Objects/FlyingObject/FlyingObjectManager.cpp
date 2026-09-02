#include "FlyingObjectManager.h"

// Engine
#include <ImguiWrapper.h>

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
		Spawn<MeteorSmall>(Cygnus::Float3{25.0f, -10.0f, 0.0f});
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