#include "Meteor.h"

// Engine
#include <Collider/CollisionManager.h>

void Meteor::Initialize(const Cygnus::Float3& position) { 
	FlyingObject::Initialize(position); 

	// 各パラメーター設定
	category_ = ObjectCategory::Meteor;
	speed_ = kSpeed;
	rotationSpeed_ = kRotationSpeed;

	// モデル設定
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Meteor");

	// コライダー生成
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("Meteor");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize(kColliderSize);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());
}
