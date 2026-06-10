#include "DroppedGear.h"

// Engine
#include <TimeManager.h>
#include <ParticleEffect/ParticleEffectManager.h>

// Application
#include <src/Game/Player/Player.h>


void DroppedGear::Initialize(const Cygnus::Float3 translate) {
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Gear");
	object_->transform_.translate_ = translate;
	object_->transform_.scale_ = kColliderSize;
	basePosition_ = translate; // 初期位置の保存（上下揺れアニメーション用）

	// コライダー生成 + 登録
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("DroppedGear");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize(kColliderSize);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());
}

void DroppedGear::Update() {
	// 上下揺れ + 回転を行うアニメーション
	BobbingAnimation();

	// コライダー更新
	collider_->Update();
	// オブジェクト更新
	object_->UpdateMatrix();
}

void DroppedGear::Draw() {
	// オブジェクト描画
	object_->Draw();
}

void DroppedGear::OnCollision(Cygnus::Collider* other) {
	// プレイヤーとの衝突
	if (other->GetTag() == "Player") {
		Player* player = static_cast<Player*>(other->GetOwner());

		// プレイヤーが歯車を拾える場合
		if (player->CanPickUpGear()) {
			isPickedUp_ = true;    // 自身の取得フラグを立てる（消す準備）
			player->AddGearCount(); // プレイヤーの歯車所持数を1増やす
			//取得パーティクルを発生
			Cygnus::ParticleEffectManager::GetInstance()->Emit("get", player->GetPosition(), 20);
		}
	}

	if (other->GetTag() == "Sandstorm") {
		flyAway_.InSandstorm();
	}
}

void DroppedGear::BobbingAnimation() {
	// タイマー加算
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();
	timer_ += dt;

	// Y軸回転
	object_->transform_.rotate_.y += kRotateSpeed * dt;

	flyAway_.Update(velocity_);

	// 上下揺れ（sin波）
	float offsettTop = std::sinf(timer_ * kBobbingSpeed) * kBobbingAmplitude;
	object_->transform_.translate_.y = basePosition_.y + offsettTop + velocity_.y;;
}
