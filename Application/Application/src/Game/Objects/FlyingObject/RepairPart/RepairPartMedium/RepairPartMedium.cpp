#include "RepairPartMedium.h"

// Engine
#include <Collider/CollisionManager.h>

void RepairPartMedium::Initialize(const Cygnus::Float3& position, bool isRightToLeft) {
	FlyingObject::Initialize(position, isRightToLeft);

	// 各パラメーター設定
	category_ = ObjectCategory::RepairPart;
	speed_ = kSpeed;
	rotationSpeed_ = kRotationSpeed;
	hitRadius_ = kHitRadius;
	subCategory_ = "repair";

	// モデル設定
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("RepairPartMedium");

	// コライダー生成
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("RepairPartMedium");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize({ hitRadius_, hitRadius_, hitRadius_ });
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());
}
