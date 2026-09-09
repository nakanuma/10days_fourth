#include "FlyingObjectManager.h"

// Engine
#include <ImguiWrapper.h>
#include <TimeManager.h>
#include <RandomGenerator.h>
#include <SoundManager.h>

// Application
#include <src/Game/Objects/FlyingObject/Meteor/MeteorSmall/MeteorSmall.h> // 隕石（小）
#include <src/Game/Objects/FlyingObject/Meteor/MeteorLarge/MeteorLarge.h> // 隕石（大）

#include <src/Game/Objects/FlyingObject/RepairPart/RepairPartLow/RepairPartLow.h> // 修理パーツ（低品質）
#include <src/Game/Objects/FlyingObject/RepairPart/RepairPartMedium/RepairPartMedium.h> // 修理パーツ（中品質）
#include <src/Game/Objects/FlyingObject/RepairPart/RepairpartHigh/RepairPartHigh.h> // 修理パーツ（高品質）

#include <src/Game/Objects/FlyingObject/HeartItem/HeartItem.h> // ハート（回復）
#include <src/Game/Objects/FlyingObject/BombItem/BombItem.h> // 爆弾（隕石全破壊）

void FlyingObjectManager::Initialize() {
	objects_.clear();
	while (!destroyQueue_.empty()) destroyQueue_.pop();
	destroyTimer_ = 0.0f;
}

void FlyingObjectManager::Update(bool isAutoSpawn) {
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();
	rng = Cygnus::RandomGenerator::GetInstance();

	// 自動スポーン処理
	// チュートリアルの時はなしに
	if (isAutoSpawn) {
		AutoSpawn();
	}

	/* 隕石の連続破壊処理 */
	if (!destroyQueue_.empty()) {
		destroyTimer_ += dt;
		while (destroyTimer_ >= kDestroyInterval && !destroyQueue_.empty()) {
			destroyTimer_ -= kDestroyInterval;

			FlyingObject* target = destroyQueue_.front();
			destroyQueue_.pop();

			// まだ生きている場合のみ破壊を実行
			if (target && !target->IsDead()) {
				target->Dead();

				Cygnus::SoundManager::GetInstance()->Play("se_explosion", false, 0.5f); // SE再生（爆発）

				// カメラシェイク用コールバック発火
				if(onDestroyMeteorCallback_) {
					onDestroyMeteorCallback_();
				}
			}
		}
	}

	// オブジェクト更新
	for (auto& obj : objects_) {
		obj->Update();
	}

	// 画面外に出た or 破壊されたオブジェクトを自動削除
	auto it = std::remove_if(objects_.begin(), objects_.end(), 
		[](const std::unique_ptr<FlyingObject>& obj) {
			if (obj->IsDead()) {
				obj->UnregisterCollider();
				return true;
			}
			return false;
		});
	objects_.erase(it, objects_.end());
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
	if (ImGui::Button("Spawn : RepairPartMedium")) {
		Spawn<RepairPartMedium>(Cygnus::Float3{ 25.0f, -10.0f, 0.0f });
	}
	if (ImGui::Button("Spawn : RepairPartHigh")) {
		Spawn<RepairPartHigh>(Cygnus::Float3{ 25.0f, -10.0f, 0.0f });
	}
	if (ImGui::Button("Spawn : BombItem")) {
		Spawn<BombItem>(Cygnus::Float3{kSpawnX, -50.0f, 0.0f});
	}

	ImGui::End();
#endif
}

void FlyingObjectManager::DestroyAllMeteorsSequential() {
	// 現在存在するオブジェクトの中から隕石だけを抽出してキューに追加
	for (auto& obj : objects_) {
		if (!obj->IsDead() && obj->GetCategory() == ObjectCategory::Meteor) {
			destroyQueue_.push(obj.get());
		}
	}
	destroyTimer_ = 0.0f;
}

void FlyingObjectManager::AutoSpawn() {
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	// 隕石（小）
	timerMeteorSmall_ += dt;
	if (timerMeteorSmall_ >= kIntervalMeteorSmall) {
		timerMeteorSmall_ = 0.0f;
		SpawnMeteorSmall();
	}

	// 隕石（大）
	timerMeteorLarge_ += dt;
	if (timerMeteorLarge_ >= kIntervalMeteorLarge) {
		timerMeteorLarge_ = 0.0f;
		SpawnMeteorLarge();
	}

	// 修理パーツ（低品質）
	timerRepairLow_ += dt;
	if (timerRepairLow_ >= kIntervalRepairLow) {
		timerRepairLow_ = 0.0f;
		SpawnRepairPartLow();
	}

	// 修理パーツ（中品質）
	timerRepairMid_ += dt;
	if (timerRepairMid_ >= kIntervalRepairMid) {
		timerRepairMid_ = 0.0f;
		SpawnRepairPartMedium();
	}

	// 修理パーツ（高品質）
	timerRepairHigh_ += dt;
	if (timerRepairHigh_ >= kIntervalRepairHigh) {
		timerRepairHigh_ = 0.0f;
		SpawnRepairPartHigh();
	}

	// ハート（回復）
	timerHeartItem_ += dt;
	if (timerHeartItem_ >= kIntervalHeartItem) {
		timerHeartItem_ = 0.0f;
		SpawnHeartItem();
	}

	// 爆弾（隕石全破壊）
	timerBombItem_ += dt;
	if (timerBombItem_ >= kIntervalBombItem) {
		timerBombItem_ = 0.0f;
		SpawnBombItem();
	}
}

// スポーン方向と初期位置を決定するラムダ関数
const Cygnus::Float3 FlyingObjectManager::GetRandomSpawnPos(float spawnY, bool& outIsRightToLeft) {
	// 50％の確率で右スポーンか左スポーンかを決定
	outIsRightToLeft = rng->RandomValueBool(0.5f);
	float spawnX = outIsRightToLeft ? kSpawnX : -kSpawnX;
	return Cygnus::Float3{ spawnX, spawnY, 0.0f };
};

void FlyingObjectManager::SpawnMeteorSmall() {
	//隕石(小)生成
	bool isRightToLeft = true;
	auto pos = GetRandomSpawnPos(rng->RandomValue(kSpawnMinY, kSpawnMaxY), isRightToLeft);
	Spawn<MeteorSmall>(pos, isRightToLeft);
}

void FlyingObjectManager::SpawnMeteorLarge() {
	//隕石(大)生成
	bool isRightToLeft = true;
	auto pos = GetRandomSpawnPos(rng->RandomValue(kSpawnMinY, kSpawnMaxY), isRightToLeft);
	Spawn<MeteorLarge>(pos, isRightToLeft);
}

void FlyingObjectManager::SpawnRepairPartLow() {
	//パーツ(低)生成
	bool isRightToLeft = true;
	auto pos = GetRandomSpawnPos(rng->RandomValue(kUpperLimitY, kSpawnMaxY), isRightToLeft);
	Spawn<RepairPartLow>(pos, isRightToLeft);
}

void FlyingObjectManager::SpawnRepairPartMedium() {
	//パーツ(中)生成
	bool isRightToLeft = true;
	auto pos = GetRandomSpawnPos(rng->RandomValue(kMiddleLimitY, kUpperLimitY), isRightToLeft);
	Spawn<RepairPartMedium>(pos, isRightToLeft);
}

void FlyingObjectManager::SpawnRepairPartHigh() {
	//パーツ(高)生成
	bool isRightToLeft = true;
	auto pos = GetRandomSpawnPos(rng->RandomValue(kSpawnMinY, kMiddleLimitY), isRightToLeft);
	Spawn<RepairPartHigh>(pos, isRightToLeft);
}

void FlyingObjectManager::SpawnHeartItem() {
	//アイテム(ハート)生成
	bool isRightToLeft = true;
	auto pos = GetRandomSpawnPos(rng->RandomValue(kMiddleLimitY, kUpperLimitY), isRightToLeft); // 中品質パーツと同じ高さ
	Spawn<HeartItem>(pos, isRightToLeft);
}

void FlyingObjectManager::SpawnBombItem() {
	//アイテム(爆弾)生成
	bool isRightToLeft = true;
	auto pos = GetRandomSpawnPos(rng->RandomValue(kSpawnMinY, kMiddleLimitY), isRightToLeft); // 高品質パーツと同じ高さ
	Spawn<BombItem>(pos, isRightToLeft);
}
