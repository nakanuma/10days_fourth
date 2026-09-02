#include "Player.h"

// Engine
#include <Collider/CollisionManager.h>
#include <Input/Input.h>
#include <ImguiWrapper.h>
#include <TimeManager.h>

// Application
#include <src/Game/Objects/Spaceship/Spaceship.h>

void Player::Initialize(Spaceship* spaceship) {
	spaceship_ = spaceship;

	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Player");
	object_->transform_.translate_ = { 0.0f, -10.0f, 0.0f };

	// 各パラメーター初期化
	velocity_ = { 0.0f, 0.0f, 0.0f };
	driftTimer_ = 0.0f;

	isRewinding_ = false;
	autoRewindTimer_ = 0.0f;

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

	ImGui::Checkbox("IsRewinding", &isRewinding_);
	ImGui::Text("RewindTimer: %.2f", autoRewindTimer_);

	ImGui::End();
#endif
}

void Player::StartRewind()
{
	// 巻取りを実行する
	if(!isRewinding_) {
		isRewinding_ = true;
	}
}

void Player::Move()
{
	auto input = Cygnus::Input::GetInstance();
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	/* 巻取りトリガー判定 */

	if(!isRewinding_) {
		// タイマー更新（宇宙船より下にいる間）
		if(object_->transform_.translate_.y < -1.0f) {
			autoRewindTimer_ += dt;
			// 自動巻き取りの限界時間（酸素ゲージ）に達したら自動巻き取り開始
			if(autoRewindTimer_ >= kDefaultAutoRewindTime) {
				StartRewind();
			}
		} else {
			autoRewindTimer_ = 0.0f; // 上部にいる間はリセット
		}

		// ボタン入力での巻取り
		if(input->TriggerKey(DIK_SPACE)) {
			StartRewind();
		}
	}

	/* 移動力の計算 */

	// 入力に基づく加速度の計算
	Cygnus::Float3 accel = { 0.0f, 0.0f, 0.0f };
	bool isInputting = false;

	if(isRewinding_) {
		// 巻取り中の移動計算
		ProcessRewind();
	} else {
		// 通常操作時の移動計算
		if (input->PushKey(DIK_W)) { accel.y += kAcceleration; isInputting = true; }
		if (input->PushKey(DIK_S)) { accel.y -= kAcceleration; isInputting = true; }
		if (input->PushKey(DIK_A)) { accel.x -= kAcceleration; isInputting = true; }
		if (input->PushKey(DIK_D)) { accel.x += kAcceleration; isInputting = true; }

		// 速度に加速度を加算
		velocity_.x += accel.x;
		velocity_.y += accel.y;
		velocity_.z += accel.z;

		// 無入力時の処理（漂い）
		Cygnus::Float3 driftOffset = { 0.0f, 0.0f, 0.0f };
		if (!isInputting) {
			driftOffset = Drift();
		}

		// 減衰処理
		velocity_.x *= kDamping;
		velocity_.y *= kDamping;
		velocity_.z *= kDamping;

		// 最高速度の制限
		float currentSpeed = std::sqrt(velocity_.x * velocity_.x + velocity_.y * velocity_.y + velocity_.z * velocity_.z);
		if (currentSpeed > kMaxSpeed) {
			velocity_.x = (velocity_.x / currentSpeed) * kMaxSpeed;
			velocity_.y = (velocity_.y / currentSpeed) * kMaxSpeed;
			velocity_.z = (velocity_.z / currentSpeed) * kMaxSpeed;
		}

		// 座標への適用
		object_->transform_.translate_.x += velocity_.x + driftOffset.x;
		object_->transform_.translate_.y += velocity_.y + driftOffset.y;
		object_->transform_.translate_.z += velocity_.z + driftOffset.z;
	}

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

void Player::ProcessRewind()
{
	if(!spaceship_) return;

	// 宇宙船への方向ベクトルを算出
	Cygnus::Float3 targetPos = spaceship_->GetTranslate();
	Cygnus::Float3 diff = {
		targetPos - object_->transform_.translate_
	};
	float distance = std::sqrt(Cygnus::Float3::Length(diff));

	// 宇宙船に到達したら巻取り終了
	if(distance <= kRewindStopDistance) {
		isRewinding_ = false;
		autoRewindTimer_ = 0.0f;
		velocity_ = {0.0f, 0.0f, 0.0f};
		return;
	}

	// 宇宙船に向かう加速度を足す
	Cygnus::Float3 dir = {diff.x / distance, diff.y / distance, diff.z / distance};
	velocity_ += dir * kDefaultRewindAccel;

	// 巻取り中の最高速度制御
	float currentSpeed = std::sqrt(Cygnus::Float3::Length(velocity_));
	if(currentSpeed > kDefaultRewindMaxSpeed) {
		velocity_ = (velocity_ / currentSpeed) * kDefaultRewindMaxSpeed;
	}

	// 座標への適用
	object_->transform_.translate_ += velocity_;
}
