#include "Ore.h"

void Ore::Initialize(const Cygnus::Float3 translate) {
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Ore");
	object_->transform_.translate_ = translate;

	// コライダー生成 + 登録
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("Ore");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize(kColliderSize);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());
}

void Ore::Update() {
	// コライダー更新
	collider_->Update();
	// オブジェクト更新
	object_->UpdateMatrix();
}

void Ore::Draw() {
	// オブジェクト描画
	object_->Draw();
}

void Ore::OnCollision(Cygnus::Collider* other)
{

}
