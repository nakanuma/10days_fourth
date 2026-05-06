#include "OreManager.h"

OreManager* OreManager::GetInstance() {
	static OreManager instance;
	return &instance;
}

void OreManager::Initialize() {
	ores_.clear();

	return;

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

void OreManager::AddPoint(const Cygnus::Float3& translate, const Cygnus::Float3& size) {

	//追加する鉱石を作成
	
	/// ---- 偶数か調べる ---
	//中心点をtranslateにするように
	float halfX{}, halfZ{}, slippageX = 0.0f, slippageZ = 0.0f;

	HalfChecker(halfX, slippageX, size.x);
	HalfChecker(halfZ, slippageZ, size.z);

	/// ----------------
	
	// 鉱石の塊を作る(サイズにより変更可能)
	for (float i = -halfZ + slippageZ; i <= halfZ - slippageZ; ++i) {
		for (float j = -halfX + slippageX; j <= halfX - slippageX; ++j) {
			//鉱石を作成
			auto newOre = std::make_unique<Ore>();
			newOre->Initialize(translate + Cygnus::Float3{ 2.0f * j ,0.0f ,2.0f * i });
			ores_.push_back(std::move(newOre));
		}
	}
}

void OreManager::HalfChecker(float& half, float& slippagePoint, float size) {
	if (std::fmod(size, 2.0f) == 0) {
		//偶数
		half = size * kHalf_;//半分にする;
		slippagePoint = kHalf_;//座標をずらす
	}
	else {
		//奇数
		half = (size - 1) * kHalf_;//偶数にした後、半分にする;
	}
}
