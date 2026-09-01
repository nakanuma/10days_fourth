#include "FlyingObject.h"

// Engine
#include <Collider/CollisionManager.h>

void FlyingObject::Initialize(const Cygnus::Float3& position) { 
	isDead_ = false;

	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->transform_.translate_ = position;
}

void FlyingObject::Update() {
	if (isDead_) return;

	// 右から左への移動
	object_->transform_.translate_.x -= speed_;
	// 回転処理（仮）
	object_->transform_.rotate_.z += rotationSpeed_;

	// Xが一定の距離に達したら自動で死亡
	if (object_->transform_.translate_.x < kDespawnX) {
		isDead_ = true;
		// コライダーの登録解除
		if (collider_) {
			Cygnus::CollisionManager::GetInstance()->Unregister(collider_.get());
		}
	}

	// コライダー更新
	if (collider_) collider_->Update();
	// オブジェクト更新
	object_->UpdateMatrix();
}

void FlyingObject::Draw() {
	if (isDead_) return;

	// オブジェクト描画
	object_->Draw();
}