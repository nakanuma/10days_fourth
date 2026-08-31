#include "Spaceship.h"

// Engine
#include <Collider/CollisionManager.h>
#include <ImguiWrapper.h>

void Spaceship::Initialize() {
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Spaceship");

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