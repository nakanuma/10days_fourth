#include "HeartItem.h"

// Engine
#include <Collider/CollisionManager.h>
#include <RandomGenerator.h>

void HeartItem::Initialize(const Cygnus::Float3& position, bool isRightToLeft) {
	FlyingObject::Initialize(position, isRightToLeft);

	object_->transform_.rotate_ = {0.0f, 0.0f, 0.0f}; // 回転リセット

	// 各パラメーター設定
	category_ = ObjectCategory::HeartItem;
	speed_ = kSpeed;
	rotationSpeed_ = Cygnus::RandomGenerator::GetInstance()->RandomValue({0.0f, -0.015f, 0.0f }, { 0.0f, 0.015f, 0.0f });
	hitRadius_ = kHitRadius;

	// モデル設定
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("HeartItem");

	// コライダー生成
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("HeartItem");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize({hitRadius_, hitRadius_, hitRadius_});
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());
}