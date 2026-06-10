#include "OreManager.h"

//Engine
#include <ParticleEffect/ParticleEffectManager.h>

OreManager* OreManager::GetInstance() {
	static OreManager instance;
	return &instance;
}

void OreManager::Initialize() {
	ores_.clear(); 
	droppedOres_.clear();
	breakRequests_.clear();

	// デバッグ用にベタ打ちで鉱石を追加（Todo : エディタで追加できるように変更する）
	for(size_t i = 0; i < 4; ++i) {
		for(size_t j = 0; j < 4; ++j) {
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

	BulkDestruction();
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

void OreManager::AddPoint(const Cygnus::Float3& translate, const Cygnus::Float3& size) {

	if (ores_.size() >= kMaxOre_) return;

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

void OreManager::BulkDestruction()
{
	if (!breakRequests_.empty())
	{
		for (const auto& request : breakRequests_)
		{
			float rangeSq = request.range * request.range;

			auto oreIt = std::remove_if(ores_.begin(), ores_.end(), [&](const std::unique_ptr<Ore>& ore)
				{
					Cygnus::Float3 orePos = ore->GetTranslate();
					float dx = orePos.x - request.targetPos.x;
					float dz = orePos.z - request.targetPos.z;
					float distSq = dx * dx + dz * dz;

					if (distSq <= rangeSq)
					{
						// 削除される鉱石の位置を保存
						Cygnus::Float3 dropPos = ore->GetTranslate();

						// コライダー登録解除
						ore->UnregisterCollider();

						// 落ちている鉱石（ドロップアイテム）を生成してリストに追加
						auto newDroppedOre = std::make_unique<DroppedOre>();
						newDroppedOre->Initialize(dropPos);
						droppedOres_.push_back(std::move(newDroppedOre));

						return true; // 削除対象
					}
					return false;
				});

			if (oreIt != ores_.end())
			{
				ores_.erase(oreIt, ores_.end());
			}
		}
		// 処理が終わったらリクエストをクリア
		breakRequests_.clear();
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

		// 鉱石を破壊した際のパーティクルエフェクト生成
		Cygnus::ParticleEffectManager::GetInstance()->Emit("ore_break", dropPos, 20);

		return true;
	}

	return false;
}

bool OreManager::IsBreakableAt(const Cygnus::Float3& targetPos, float range) { 
	float minDistanceSq = range * range; // 射程範囲を初期値にする

	// 全ての鉱石から射程内にあるか探索
	for (size_t i = 0; i < ores_.size(); ++i) {
		Cygnus::Float3 orePos = ores_[i]->GetTranslate();
		// 鉱石との距離を計算
		float dx = orePos.x - targetPos.x;
		float dz = orePos.z - targetPos.z;
		float distSq = dx * dx + dz * dz;

		// 射程内に1つでも鉱石があれば、その時点で破壊可能とみなす
		if (distSq < minDistanceSq) {
			return true;
		}
	}

	return false;
}

bool OreManager::BreakAllAt(const Cygnus::Float3& targetPos, float range)
{
	breakRequests_.push_back({ targetPos, range });

	return true;
}
