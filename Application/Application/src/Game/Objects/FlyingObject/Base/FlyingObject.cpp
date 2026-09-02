#include "FlyingObject.h"

// Engine
#include <Collider/CollisionManager.h>

void FlyingObject::Initialize(const Cygnus::Float3& position, bool isRightToLeft) {
	isDead_ = false;

	// 移動方向の設定
	directionX_ = isRightToLeft ? -1.0f : 1.0f;

	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->transform_.translate_ = position;
}

void FlyingObject::Update() {
	if (isDead_) return;

	// 右から左への移動
	object_->transform_.translate_.x += speed_ * directionX_;
	// 回転処理（仮）
	object_->transform_.rotate_.z += rotationSpeed_;

	// 画面外に達したら自動で削除
	if(directionX_ < 0.0f && object_->transform_.translate_.x < -kDespawnX) { // 右から左へ移動している場合
		Destroy();
	} else if (directionX_ > 0.0f && object_->transform_.translate_.x > kDespawnX) { // 左から右へ移動している場合
		Destroy();
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

void FlyingObject::Destroy() {
	// 死亡フラグを立てる
	isDead_ = true;
	// コライダーの登録解除
	if (collider_) {
		Cygnus::CollisionManager::GetInstance()->Unregister(collider_.get());
	}
}