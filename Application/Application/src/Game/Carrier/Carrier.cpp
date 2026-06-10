#include "Carrier.h"

// Engine
#include <ImguiWrapper.h>
#include <Input/Input.h>
#include <ParticleEffect/ParticleEffectManager.h>

// Application
#include <src/Game/Path/PathManager.h>
#include <src/Game/Player/Player.h>
#include <src/Game/UI/UIManager.h>

void Carrier::Initialize() {
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Carrier");
	object_->transform_.translate_ = PathManager::GetInstance()->GetPoint(0); // 経路の始点座標をセット

	// コライダー生成 + 登録
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("Carrier");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize(kColliderSize);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());

	auto sensor = std::make_unique<Cygnus::AABBCollider>();
	sensor->SetTag("carrierSensor");
	sensor->SetFollowTarget(&object_->transform_.translate_);
	sensor->SetSize(kSensorSize);
	sensor->SetOwner(this);

	colliderSensor_ = std::move(sensor);
	Cygnus::CollisionManager::GetInstance()->Register(colliderSensor_.get());
}

void Carrier::Update(float deltaTime) {
	// 時間経過でエネルギー消費
	if (energyTimer_ > 0.0f) {
		energyTimer_ -= deltaTime;
		if (energyTimer_ < 0.0f) energyTimer_ = 0.0f;
	}

	// 速度倍率の計算
	if (energyTimer_ > 0.0f && !isGoal_) {
		// 加速（速度倍率を徐々に1.0fへ）
		currentVelocityRate_ += deltaTime / kAccelerationTime;
	} else {
		// 減速（速度倍率を徐々に0.0fへ）
		currentVelocityRate_ -= deltaTime / kDecelerationTime;
	}
	currentVelocityRate_ = std::clamp(currentVelocityRate_, 0.0f, 1.0f);	// 0.0f ~ 1.0fの範囲にクランプ

	// 経路に沿った移動処理
	MoveAlongPath(deltaTime);

	// コライダー更新
	collider_->Update();
	colliderSensor_->Update();
	// オブジェクト更新
	object_->UpdateMatrix();
}

void Carrier::Draw() {
	// オブジェクト描画
	object_->Draw();
}

void Carrier::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("Carrier");

	ImGui::Checkbox("IsActive", &isActive_);
	ImGui::Text("EnergyTimer : %.f", energyTimer_);
	ImGui::Separator();
	ImGui::DragFloat3("Translate", &object_->transform_.translate_.x, 0.01f);
	ImGui::Checkbox("IsGoal", &isGoal_);

	ImGui::End();
#endif
}

void Carrier::OnCollision(Cygnus::Collider* other) {
	auto input = Cygnus::Input::GetInstance();

	if (other->GetTag() == "Player") {
		Player* player = static_cast<Player*>(other->GetOwner());

		// プレイヤーが必要数歯車を所持しているか確認
		if (player->GetGearCount() >= kRequiredGearCount) {
			// インタラクトUIの表示を要求
			UIManager::GetInstance()->RequestInteract(InteractGuide::ActionType::Insert);

			// キー or ボタン入力操作
			if (input->TriggerKey(DIK_SPACE) || input->IsTriggerButton(0, XINPUT_GAMEPAD_A)) {
				// プレイヤーの歯車を消費させる
				player->ConsumeGear(kRequiredGearCount);
				// 列車に歯車を注入された際の処理
				SupplyGear();
				// インタラクトエフェクトの表示を要求
				Cygnus::ParticleEffectManager::GetInstance()->Emit("get", object_->transform_.translate_,30);

			}
		}
	}
}

void Carrier::MoveAlongPath(float deltaTime)
{
	// 無効化状態 or ゴール済みなら終了
	if (!isActive_ || isGoal_) return;

	// 速度が完全に0なら計算しない
	if (currentVelocityRate_ <= 0.0f && energyTimer_ <= 0.0f) return;

	// 全てのポイントを通過したらゴール済みにする
	auto pathManager = PathManager::GetInstance();
	if(targetIndex_ >= pathManager->GetPointCount()) {
		isGoal_ = true;
		return;
	}

	// 現在地->目標地点への距離を計算
	Cygnus::Float3& currentPos = object_->transform_.translate_;
	Cygnus::Float3 targetPos = pathManager->GetPoint(targetIndex_);

	Cygnus::Float3 diff = targetPos - currentPos;
	float distance = Cygnus::Float3::Length(diff);

	// 現在フレームの移動速度を計算
	float frameSpeed = kMoveSpeed * currentVelocityRate_ * deltaTime;

	// ポイント到着判定 + オブジェクト移動処理
	if(distance < kMoveSpeed * deltaTime) {
		currentPos = targetPos;
		targetIndex_++;	// 次のポイントへ
	} else {
		currentPos += (diff / distance) * frameSpeed;
	}
}

void Carrier::SupplyGear() {
	energyTimer_ = kMaxEnergy;	// 動作時間を設定
	isActive_ = true;	// 有効化
}
