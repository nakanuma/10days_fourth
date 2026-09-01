#include "Player.h"

// Engine
#include <Collider/CollisionManager.h>
#include <Input/Input.h>
#include <ImguiWrapper.h>
#include <TimeManager.h>

void Player::Initialize() {
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Player");
	object_->transform_.translate_ = { 0.0f, -10.0f, 0.0f };

	// 速度の初期化
	velocity_ = { 0.0f, 0.0f, 0.0f };
	driftTimer_ = 0.0f;

	// コライダー生成
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("Player");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize(kColliderSize);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());
}

void Player::Update() {
	// 移動処理
	Move();

	// コライダー更新
	collider_->Update();

	// オブジェクト更新
	object_->UpdateMatrix();
}

void Player::Draw() {
	// オブジェクト描画
	object_->Draw();
}

void Player::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("Player");

	ImGui::DragFloat3("translate", &object_->transform_.translate_.x, 0.01f);

	ImGui::End();
#endif
}

void Player::Move()
{
	auto input = Cygnus::Input::GetInstance();

	// 入力に基づく加速度の計算
	Cygnus::Float3 accel = { 0.0f, 0.0f, 0.0f };
	bool isInputting = false;

	if (input->PushKey(DIK_W)) { accel.y += kAcceleration; isInputting = true; }
	if (input->PushKey(DIK_S)) { accel.y -= kAcceleration; isInputting = true; }
	if (input->PushKey(DIK_A)) { accel.x -= kAcceleration; isInputting = true; }
	if (input->PushKey(DIK_D)) { accel.x += kAcceleration; isInputting = true; }

	// 速度に加速度を加算
	velocity_.x += accel.x;
	velocity_.y += accel.y;
	velocity_.z += accel.z;

	// 無入力時の処理（漂い）
	Cygnus::Float3 driftOffset = {0.0f, 0.0f, 0.0f};
	if(!isInputting) {
		driftOffset = Drift();
	}

	// 減衰処理
	velocity_.x *= kDamping;
	velocity_.y *= kDamping;
	velocity_.z *= kDamping;

	// 最高速度の制限
	float currentSpeed = std::sqrt(velocity_.x * velocity_.x + velocity_.y * velocity_.y + velocity_.z * velocity_.z);
	if(currentSpeed > kMaxSpeed) {
		velocity_.x = (velocity_.x / currentSpeed) * kMaxSpeed;
		velocity_.y = (velocity_.y / currentSpeed) * kMaxSpeed;
		velocity_.z = (velocity_.z / currentSpeed) * kMaxSpeed;
	}

	// 座標への適用
	object_->transform_.translate_.x += velocity_.x + driftOffset.x;
	object_->transform_.translate_.y += velocity_.y + driftOffset.y;
	object_->transform_.translate_.z += velocity_.z + driftOffset.z;

	// 移動範囲制限
	float clampedX = std::clamp(object_->transform_.translate_.x, -kDefaultLimitX, kDefaultLimitX);
	float clampedY = std::clamp(object_->transform_.translate_.y, kDefaultLimitMinY, kDefaultLimitMaxY);

	if (clampedX != object_->transform_.translate_.x) {
		velocity_.x = 0.0f;
	}
	if (clampedY != object_->transform_.translate_.y) {
		velocity_.y = 0.0f;
	}

	object_->transform_.translate_.x = clampedX;
	object_->transform_.translate_.y = clampedY;
}

Cygnus::Float3 Player::Drift() {
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	// 毎フレームタイマー加算
	driftTimer_ += dt * kDriftFrequency;

	// XとYでそれぞれ異なる周期にして不規則な漂い動作を行う
	float driftX = std::sinf(driftTimer_) * std::cosf(driftTimer_ * 0.7f) * kDriftAmplitude;
	float driftY = std::cosf(driftTimer_ * 1.3f) * std::sinf(driftTimer_ * 0.5f) * kDriftAmplitude;

	// 移動量を返す
	return {driftX, driftY, 0.0f};
}
