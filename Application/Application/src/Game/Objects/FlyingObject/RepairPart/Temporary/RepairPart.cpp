#include "RepairPart.h"

// Engine
#include <Collider/CollisionManager.h>

void RepairPart::Initialize(const Cygnus::Float3& position) {
	FlyingObject::Initialize(position);

	// 各パラメーター設定
	category_ = ObjectCategory::RepairPart;
	speed_ = kSpeed;
	rotationSpeed_ = kRotationSpeed;

	// モデル設定
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("RepairPart");

	// コライダー生成
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("RepairPart");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize(kColliderSize);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());
}
