#include "DroppedOre.h"

// Engine
#include <TimeManager.h>

// Application
#include <src/Game/Player/Player.h>

void DroppedOre::Initialize(const Cygnus::Float3 translate) {
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Ore");
	object_->transform_.translate_ = translate;
	object_->transform_.scale_ = kColliderSize;
	basePosition_ = translate;	// 初期位置の保存（上下揺れアニメーション用）

	// コライダー生成 + 登録
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("DroppedOre");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize(kColliderSize);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());
}

void DroppedOre::Update() {
	// 上下揺れ + 回転を行うアニメーション
	BobbingAnimation();

	// コライダー更新
	collider_->Update();
	// オブジェクト更新
	object_->UpdateMatrix();
}

void DroppedOre::Draw() {
	// オブジェクト描画
	object_->Draw();
}

void DroppedOre::OnCollision(Cygnus::Collider* other) {
	// プレイヤーとの衝突
	if (other->GetTag() == "Player") {
		Player* player = static_cast<Player*>(other->GetOwner());

		// プレイヤーが鉱石を拾える場合
		if (player->CanPickUpOre()) {
			isPickedUp_ = true;	// 自身の取得フラグを立てる（消す準備）
			player->AddOreCount();	// プレイヤーの鉱石所持数を1増やす
		}
	}

	//砂嵐に当たったら
	if (other->GetTag() == "Sandstorm") {
		flyAway_.InSandstorm();
	}
}

void DroppedOre::BobbingAnimation() {
	// タイマー加算
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();
	timer_ += dt;

	// Y軸回転
	object_->transform_.rotate_.y += kRotateSpeed * dt;


	//砂嵐で飛ばされる
	flyAway_.Update(velocity_);

	// 上下揺れ（sin波）
	float offsettTop = std::sinf(timer_ * kBobbingSpeed) * kBobbingAmplitude;
	object_->transform_.translate_.y = basePosition_.y + offsettTop + velocity_.y;
}
