#include "OreManager.h"

OreManager* OreManager::GetInstance() {
	static OreManager instance;
	return &instance;
}

void OreManager::Initialize() {
	ores_.clear();
	droppedOres_.clear();
	// デバッグ用にベタ打ちで鉱石を追加（Todo : エディタで追加できるように変更する）
	for(size_t i = 0; i < 3; ++i) {
		for(size_t j = 0; j < 3; ++j) {
			auto newOre = std::make_unique<Ore>();
			newOre->Initialize(Cygnus::Float3{static_cast<float>(i) * 2.0f, 1.0f, -10.0f + static_cast<float>(j) * 2.0f});
			ores_.push_back(std::move(newOre));
		}
	}
}

void OreManager::Update() {
	// 全ての鉱石を更新
	for(auto& ore : ores_) {
		ore->Update();
	}

	// 全ての落ちている鉱石（ドロップアイテム）を更新 + 削除判定
	auto it = std::remove_if(droppedOres_.begin(), droppedOres_.end(), [](const std::unique_ptr<DroppedOre>& droppedOre) { 
		droppedOre->Update();

		// プレイヤーに拾われた際の処理
		if (droppedOre->IsPickedUp()) {
			droppedOre->UnregisterCollider(); // コライダー登録解除
			return true;
		}
		return false;
	});
	if (it != droppedOres_.end()) {
		droppedOres_.erase(it, droppedOres_.end()); // 配列から削除
	}
}

void OreManager::Draw() {
	// 全ての鉱石を描画
	for(auto& ore : ores_) {
		ore->Draw();
	}

	// 全ての落ちている鉱石（ドロップアイテム）を更新
	for (auto& droppedOre : droppedOres_) {
		droppedOre->Draw();
	}
}

bool OreManager::TryBreakAt(const Cygnus::Float3& targetPos, float range) { 
	int closestIndex = -1;
	float minDistanceSq = range * range;	// 射程範囲を初期値にする

	// 全ての鉱石から、射程内で最も近い鉱石を探索
	for (size_t i = 0; i < ores_.size(); ++i) {
		Cygnus::Float3 orePos = ores_[i]->GetTranslate();
		// 鉱石との距離を計算
		float dx = orePos.x - targetPos.x;
		float dz = orePos.z - targetPos.z;
		float distSq = dx * dx + dz * dz;

		// 最短距離かつ、射程内なら更新
		if (distSq < minDistanceSq) {
			minDistanceSq = distSq;
			closestIndex = i;
		}
	}

	// 見つかった際の処理
	if (closestIndex != -1) {
		Cygnus::Float3 dropPos = ores_[closestIndex]->GetTranslate();	// 削除される鉱石の位置を保存
		ores_[closestIndex]->UnregisterCollider();	// コライダー登録解除
		ores_.erase(ores_.begin() + closestIndex);	// 配列から削除

		// 落ちている鉱石（ドロップアイテム）の生成
		auto newDroppedOre = std::make_unique<DroppedOre>();
		newDroppedOre->Initialize(dropPos);
		droppedOres_.push_back(std::move(newDroppedOre));

		return true;
	}

	return false;
}
