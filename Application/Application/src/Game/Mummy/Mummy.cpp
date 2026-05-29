#include "Mummy.h"
#include "src/Game/Player/Player.h"
#include "ImguiWrapper.h"

void Mummy::Initialize(const Cygnus::Float3& translate) {
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Ore");
	object_->transform_.translate_ = translate;

	// コライダー生成 + 登録
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("Mummy");
	aabb->SetFollowTarget(&object_->transform_.translate_);
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

	Cygnus::Float3 mummyPosition = object_->transform_.translate_;
	Cygnus::Float3 moveNormal = Cygnus::Float3::Normalize(playerPos - mummyPosition);//移動方向設定 [プレイヤーのいる方向に]

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

		Cygnus::AABBCollider* myAABB = dynamic_cast<Cygnus::AABBCollider*>(collider_.get());
		Cygnus::AABBCollider* otherAABB = dynamic_cast<Cygnus::AABBCollider*>(other);

		// 押し戻し処理
		if (myAABB && otherAABB) {
			// 押し戻しベクトル取得
			Cygnus::Float3 pushVec = myAABB->GetPushBackVector(*otherAABB);
			// プレイヤー位置を補正
			object_->transform_.translate_ += pushVec;
			object_->UpdateMatrix();

			// コライダーも更新
			Cygnus::Float3 currentMin = myAABB->GetMin();
			Cygnus::Float3 currentMax = myAABB->GetMax();
			myAABB->SetMin(currentMin + pushVec);
			myAABB->SetMax(currentMax + pushVec);
		}
	}

	if (other->GetTag() == "Player" || other->GetTag() == "Sphinx") {
		lifeTime_ = 0.0f;
	}

}


