#include "FlyingObject.h"

// Engine
#include <Collider/CollisionManager.h>
#include <ParticleEffect/ParticleEffectManager.h>

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
		isDead_ = true;
	} else if (directionX_ > 0.0f && object_->transform_.translate_.x > kDespawnX) { // 左から右へ移動している場合
		isDead_ = true;
	}

	// 分類ごとに移動パーティクルを生成
	if(subCategory_ == "meteor_large") {
		Cygnus::ParticleEffectManager::GetInstance()->Emit("move_large_meteor", object_->transform_.translate_,
			1,
			Cygnus::Float3(0, 0, 0),
			0.0f
		);
	} else if(subCategory_ == "meteor_small") {
		Cygnus::ParticleEffectManager::GetInstance()->Emit("move_small_meteor", object_->transform_.translate_,
			1,
			Cygnus::Float3(0, 0, 0),
			0.0f
		);
	} else if(subCategory_ == "repair") {
		Cygnus::ParticleEffectManager::GetInstance()->Emit("move_repair", object_->transform_.translate_,
			1,
			Cygnus::Float3(0, 0, 0),
			0.0f
		);
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

void FlyingObject::OnCollision(Cygnus::Collider* other)
{
	// 衝突相手がプレイヤーの場合、死亡させる
	if(other->GetTag() == "Player") {
		isDead_ = true;
	}
}

void FlyingObject::UnregisterCollider()
{
	if(collider_) {
		Cygnus::CollisionManager::GetInstance()->Unregister(collider_.get());
		collider_.reset();
	}
}
