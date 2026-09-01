#include "FlyingObjectManager.h"

// Engine
#include <ImguiWrapper.h>

// Application
#include <src/Game/Objects/FlyingObject/Meteor/Temporary/Meteor.h> // 隕石（仮）
#include <src/Game/Objects/FlyingObject/RepairPart/Temporary/RepairPart.h> // 修理パーツ（仮）

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
	ImGui::Begin("FlyingObjectManager"); 

	if (ImGui::Button("Spawn : Meteor")) {
		Spawn<Meteor>(Cygnus::Float3{25.0f, -10.0f, 0.0f});
	}
	if (ImGui::Button("Spawn : RepairPart")) {
		Spawn<RepairPart>(Cygnus::Float3{25.0f, -10.0f, 0.0f});
	}

	ImGui::End();
}