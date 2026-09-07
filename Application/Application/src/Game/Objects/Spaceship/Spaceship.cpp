#include "Spaceship.h"
#define NOMINMAX

// Engine
#include <Collider/CollisionManager.h>
#include <ImguiWrapper.h>
#include <TimeManager.h>

void Spaceship::Initialize() {
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Spaceship");

	// 初期座標の設定
	basePosition_ = {0.0f, 0.0f, 0.0f};
	object_->transform_.translate_ = basePosition_;

	// 漂いタイマー初期化
	driftTimer_ = 0.0f;

	// 耐久力初期化
	durability_ = 0.0f;

	// コライダー生成
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("Spaceship");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize(kColliderSize);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());
}

void Spaceship::Update() {
	// 漂い処理の更新
	Drift();

	// コライダー更新
	collider_->Update();

	// オブジェクト更新
	object_->UpdateMatrix();
}

void Spaceship::Draw() {
	// オブジェクト描画
	object_->Draw();
}

void Spaceship::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("Spaceship");

	// 耐久力ゲージの表示
	float progress = durability_ / kMaxDurability;
	ImGui::ProgressBar(progress, ImVec2(0.0f, 0.0f), "Durability");
	ImGui::Text("Durability: %.1f / %.1f", durability_, kMaxDurability);
	ImGui::Text("Is Fully Repaired: %s", IsFullyRepaired() ? "YES" : "NO");

	ImGui::Separator();

	if (ImGui::Button("Test Repair (Low x1)")) {
		Repair(1, 0, 0);
	}
	ImGui::SameLine();
	if (ImGui::Button("Test Repair (Mid x3)")) {
		Repair(0, 3, 0);
	}
	ImGui::SameLine();
	if (ImGui::Button("Test Repair (High x5)")) {
		Repair(0, 0, 5);
	}

	if (ImGui::Button("Reset Durability")) {
		durability_ = 0.0f;
	}

	ImGui::Separator();

	ImGui::DragFloat3("translate", &object_->transform_.translate_.x, 0.01f);

	ImGui::End();
#endif
}

void Spaceship::Repair(int32_t low, int32_t medium, int32_t high) { 
	int32_t totalCount = low + medium + high; 
	if (totalCount <= 0) return;

	// 品質ごとの基礎回復量
	float baseRepair = (low * kRepairPointLow) + (medium * kRepairPointMedium) + (high * kRepairPointHigh);

	// 数に応じた倍率計算
	float multiplier = 1.0f + (totalCount - 1) * kBonusMultiplierPerItem;

	// 最終回復量の適用
	float actualRepair = baseRepair * multiplier;
	durability_ = std::min(kMaxDurability, durability_ + actualRepair);
}

void Spaceship::Drift() {
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	// 毎フレームタイマー加算
	driftTimer_ += dt * kDriftFrequency;

	// 基準座標からのオフセットを計算
	float offsetX = std::sinf(driftTimer_) * kDriftAmplitudeX;
	float offsetY = std::cosf(driftTimer_ * 0.8f) * kDriftAmplitudeY;

	// 基準座標に直接オフセットを代入（基準座標からずれないように）
	object_->transform_.translate_.x = basePosition_.x + offsetX;
	object_->transform_.translate_.y = basePosition_.y + offsetY;
	object_->transform_.translate_.z = basePosition_.z;
}