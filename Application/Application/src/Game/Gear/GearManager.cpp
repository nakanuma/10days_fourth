#include "GearManager.h"

GearManager* GearManager::GetInstance() {
	static GearManager instance;
	return &instance;
}

void GearManager::Initialize() { 
	droppedGears_.clear(); 
	// デバッグ用にベタ打ちで歯車を追加
	auto gear1 = std::make_unique<DroppedGear>();
	gear1->Initialize({-15.0f, 1.0f, 0.0f});
	droppedGears_.push_back(std::move(gear1));
}

void GearManager::Update() {
	// 全ての落ちている歯車（ドロップアイテム）を更新 + 削除判定
	auto it = std::remove_if(droppedGears_.begin(), droppedGears_.end(), [](const std::unique_ptr<DroppedGear>& droppedOre) {
		droppedOre->Update();

		// プレイヤーに拾われた際の処理
		if (droppedOre->IsPickedUp()) {
			droppedOre->UnregisterCollider(); // コライダー登録解除
			return true;
		}
		return false;
	});
	if (it != droppedGears_.end()) {
		droppedGears_.erase(it, droppedGears_.end()); // 配列から削除
	}
}

void GearManager::Draw() {
	// 全ての落ちている歯車（ドロップアイテム）を更新
	for (auto& droppedGear : droppedGears_) {
		droppedGear->Draw();
	}
}



