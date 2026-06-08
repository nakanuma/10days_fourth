#include "Mummy.h"
#include "src/Game/Player/Player.h"
#include "ImguiWrapper.h"
#include <Collider/CollisionMath.h>

void Mummy::Initialize(const Cygnus::Float3& translate) {
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Mummy");
	object_->transform_.translate_ = translate;

	// コライダー生成 + 登録
	auto aabb = std::make_unique<Cygnus::OBBCollider>();
	aabb->SetTag("Mummy");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetFollowRotation(&object_->transform_.rotate_);
	aabb->SetSize(kColliderSize_);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());
}

void Mummy::Update(const Cygnus::Float3& playerPos, float deltaTime) {

	//生存時間
	lifeTime_ -= 1.0f / 60.0f;
	if (lifeTime_ < 0.0f) {
		isDead_ = true;//消滅
		Cygnus::CollisionManager::GetInstance()->Unregister(&*collider_);//コライダー削除
		return;
	}

	Cygnus::Float3 mummyPosition = playerPos - object_->transform_.translate_;
	Cygnus::Float3 moveNormal = Cygnus::Float3::Normalize(mummyPosition);//移動方向設定 [プレイヤーのいる方向に]

	object_->transform_.translate_ += moveNormal * kMoveSpeed_ * deltaTime;

	// コライダー更新
	collider_->Update();
	// オブジェクト更新
	object_->UpdateMatrix();
}

void Mummy::Draw() {
	object_->Draw();
}

void Mummy::Debug() {
	
}


void Mummy::OnCollision(Cygnus::Collider* other) {
	if (other->GetTag() == "Ore" || other->GetTag() == "WorkBench") {

		Cygnus::OBBCollider* myOBB = dynamic_cast<Cygnus::OBBCollider*>(collider_.get());
		Cygnus::AABBCollider* otherAABB = dynamic_cast<Cygnus::AABBCollider*>(other);

		// 押し戻し処理
		if (myOBB && other)
		{
			// 相手のAABBを一時的にOBBとして扱う
			Cygnus::OBBCollider otherAsOBB;
			otherAsOBB.SetCenter((otherAABB->GetMin() + otherAABB->GetMax()) * 0.5f);
			otherAsOBB.SetSize((otherAABB->GetMax() - otherAABB->GetMin()) * 0.5f);
			otherAsOBB.SetXAxis({ 1.0f, 0.0f, 0.0f });
			otherAsOBB.SetYAxis({ 0.0f, 1.0f, 0.0f });
			otherAsOBB.SetZAxis({ 0.0f, 0.0f, 1.0f });

			// 押し戻しベクトルを計算
			Cygnus::Float3 pushVec = Cygnus::CollisionMath::CalculatePushBackOBBvsOBB(myOBB, &otherAsOBB);

			// 位置を補正
			object_->transform_.translate_ += pushVec;
			object_->UpdateMatrix();

			// コライダーも更新
			myOBB->Update();
		}
	}

	if (other->GetTag() == "Player" || other->GetTag() == "Sphinx" || other->GetTag() == "SunLaser") {
		lifeTime_ = 0.0f;
	}

}


//namespace MummyState {
//	
//	void SummonState::Update(Mummy& mummy) {
//
//	}
//
//	void MoveState::Update(Mummy& mummy) {
//
//	}
//
//	void DeadState::Update(Mummy& mummy) {
//
//	}
//}


