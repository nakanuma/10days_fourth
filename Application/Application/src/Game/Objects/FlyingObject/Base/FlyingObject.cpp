#include "FlyingObject.h"

// Engine
#include <Collider/CollisionManager.h>
#include <RandomGenerator.h>

void FlyingObject::Initialize(const Cygnus::Float3& position, bool isRightToLeft) {
	isDead_ = false;

	// 移動方向の設定
	directionX_ = isRightToLeft ? -1.0f : 1.0f;

	// 基準Y座標と位相オフセットの初期化
	basePositionY_ = position.y;
	waveTimer_ = 0.0f;
	sinPhaseOffset_ = position.x * 0.1f; // 出現X座標から少し位相をずらす

	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->transform_.translate_ = position;

	// 初期の回転角度をランダムに設定
	auto random = Cygnus::RandomGenerator::GetInstance();
	object_->transform_.rotate_ = random->RandomValue({0.0f, 0.0f, 0.0f}, {6.28f, 6.28f, 6.28f});
}

void FlyingObject::Update() {
	if (isDead_) return;

	// 右から左への移動
	object_->transform_.translate_.x += speed_ * directionX_;

	// 漂い計算
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();
	waveTimer_ += dt * kWaveFrequency;

	// 基準Y座標に対してsinのオフセットを加算
	object_->transform_.translate_.y = basePositionY_ + std::sinf(waveTimer_ + sinPhaseOffset_) * kWaveAmplitude;

	// 回転処理（仮）
	object_->transform_.rotate_ += rotationSpeed_;

	// 画面外に達したら自動で削除
	if(directionX_ < 0.0f && object_->transform_.translate_.x < -kDespawnX) { // 右から左へ移動している場合
		isDead_ = true;
	} else if (directionX_ > 0.0f && object_->transform_.translate_.x > kDespawnX) { // 左から右へ移動している場合
		isDead_ = true;
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
