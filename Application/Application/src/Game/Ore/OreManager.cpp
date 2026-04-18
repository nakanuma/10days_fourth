#include "OreManager.h"

OreManager* OreManager::GetInstance() {
	static OreManager instance;
	return &instance;
}

void OreManager::Initialize() {
	ores_.clear();
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
}

void OreManager::Draw() {
	// 全ての鉱石を描画
	for(auto& ore : ores_) {
		ore->Draw();
	}
}