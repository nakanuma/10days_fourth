#include "Spaceship.h"

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

	driftTimer_ = 0.0f;

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

	ImGui::DragFloat3("translate", &object_->transform_.translate_.x, 0.01f);

	ImGui::End();
#endif
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