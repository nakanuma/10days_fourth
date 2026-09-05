#include "Player.h"

// Engine
#include <Collider/CollisionManager.h>
#include <Input/Input.h>
#include <ImguiWrapper.h>
#include <TimeManager.h>
#include <LineDrawer.h>

// Application
#include <src/Game/Objects/Spaceship/Spaceship.h>
#include <src/Game/Objects/FlyingObject/Base/FlyingObject.h>
#include <src/Game/Scene/PauseMenu.h>

void Player::Initialize(Spaceship* spaceship, Cygnus::SpriteCommon* spriteCommon) {
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

	hp_ = kMaxHP;
	isDead_ = false;

	// コライダー生成
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("Player");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize(kColliderSize);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());

	// 所持パーツ数UI初期化
	partsCountUI_ = std::make_unique<PartsCountUI>();
	partsCountUI_->Initialize(spriteCommon);

	// 酸素ゲージのUI初期化
	o2TimeUI_ = std::make_unique<O2TimeUI>();
	o2TimeUI_->Initialize(spriteCommon);

}

void Player::Update() {
	// 移動処理
	Move();

	// コライダー更新
	collider_->Update();

	// オブジェクト更新
	object_->UpdateMatrix();

	// 所持パーツ数UI更新
	partsCountUI_->Update();

	// 酸素ゲージのUI更新
	o2TimeUI_->Update(autoRewindTimer_, kDefaultAutoRewindTime);
}

void Player::Draw() {
	// オブジェクト描画
	object_->Draw();

	// 移動制限エリアの描画
	DrawAreaLimit();
}

void Player::DrawUI() {
	// 所持パーツ数UI描画
	/*partsCountUI_->Draw();*/
	// 酸素ゲージのUI描画
	/*o2TimeUI_->Draw();*/
}

void Player::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("Player");

	ImGui::Text("HP: %d / %d", hp_, kMaxHP);
	ImGui::Checkbox("IsDead", &isDead_);

	ImGui::Separator();

	ImGui::DragFloat3("translate", &object_->transform_.translate_.x, 0.01f);

	ImGui::Separator();

	ImGui::Checkbox("IsRewinding", &isRewinding_);
	ImGui::Text("RewindTimer: %.2f", autoRewindTimer_);

	ImGui::Separator();

	ImGui::Text("RepairPartLow Count: %d", repairPartLowCount_);
	ImGui::Text("RepairPartMidium Count: %d", repairPartMediumCount_);
	ImGui::Text("RepairPartHigh Count: %d", repairPartHighCount_);

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

void Player::OnCollision(Cygnus::Collider* other)
{
	const std::string& tag = other->GetTag();

	/* 隕石との衝突処理 */
	if (tag == "MeteorSmall" || tag == "MeteorLarge") {
		ApplyDamage(1); // ダメージを与える
	}

	/* 各修理パーツとの衝突 */
	if(other->GetTag() == "RepairPartLow") {
		repairPartLowCount_++;
		partsCountUI_->AddParts();
	}
	if(other->GetTag() == "RepairPartMedium") {
		repairPartMediumCount_++;
		partsCountUI_->AddParts();
	}
	if(other->GetTag() == "RepairPartHigh") {
		repairPartHighCount_++;
		partsCountUI_->AddParts();
	}
}

void Player::ApplyDamage(int32_t damage) {
	if (isDead_) return;

	hp_ -= damage;

	// 被ダメージ時にシェイクのコールバックを呼ぶ
	if(onDamageCallback_) {
		onDamageCallback_(5.0f, 1.0f);
	}

	// HPが0になれば死亡フラグを立てる
	if (hp_ <= 0) {
		hp_ = 0;
		isDead_ = true;
	}
}

void Player::Move()
{
	auto input = Cygnus::Input::GetInstance();
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	/* 巻取りトリガー判定 */
	if(!isRewinding_) {
		// タイマー更新（宇宙船より下にいる間）
		if(object_->transform_.translate_.y < -5.0f) {
			autoRewindTimer_ += dt;
			// 自動巻き取りの限界時間（酸素ゲージ）に達したら自動巻き取り開始
			if(autoRewindTimer_ >= kDefaultAutoRewindTime) {
				StartRewind();
			}
		} else {
			autoRewindTimer_ = 0.0f; // 上部にいる間はリセット
		}

		// 巻取り入力判定（キーボード: SPACE / コントローラー: Aボタン or RBボタン）
		bool triggerKeyboard = input->TriggerKey(DIK_SPACE);
		bool triggerPad = input->IsTriggerButton(0, XINPUT_GAMEPAD_A) || input->IsTriggerButton(0, XINPUT_GAMEPAD_RIGHT_SHOULDER);
		if (triggerKeyboard || triggerPad) {
			StartRewind();
		}
	}

	/* 移動力の計算 */
	if(isRewinding_) {
		// 巻取り中の移動計算
		ProcessRewind();
	} else {
		// キーボードとゲームパッド双方の入力ベクトルを合算
		Cygnus::Float3 inputVec = {0.0f, 0.0f, 0.0f};
		inputVec += GetKeyInput();
		inputVec += GetPadInput();

		bool isInputting = (Cygnus::Float3::Length(inputVec) > 0.01f);

		// 入力がある場合は正規化して加速度を掛ける
		if (isInputting) {
			if (Cygnus::Float3::Length(inputVec) > 1.0f) {
				inputVec = Cygnus::Float3::Normalize(inputVec);
			}
			// 速度に加速度を加算
			velocity_.x += inputVec.x * kAcceleration;
			velocity_.y += inputVec.y * kAcceleration;
		}

		// 無入力時の処理（漂い）
		Cygnus::Float3 driftOffset = {0.0f, 0.0f, 0.0f};
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

	/* 移動範囲制限 */
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

Cygnus::Float3 Player::GetKeyInput() { 
	auto input = Cygnus::Input::GetInstance(); 
	Cygnus::Float3 dir = {0.0f, 0.0f, 0.0f};

	if (input->PushKey(DIK_W)) dir.y += 1.0f;
	if (input->PushKey(DIK_S)) dir.y -= 1.0f;
	if (input->PushKey(DIK_A)) dir.x -= 1.0f;
	if (input->PushKey(DIK_D)) dir.x += 1.0f;

	return dir;
}

Cygnus::Float3 Player::GetPadInput() { 
	auto input = Cygnus::Input::GetInstance(); 
	XINPUT_STATE state;

	Cygnus::Float3 dir = {0.0f, 0.0f, 0.0f};

	// コントローラー接続確認と状態取得
	if (input->GetJoystickState(0, state)) {
		// 左スティック入力
		dir.x = state.Gamepad.sThumbLX / 32767.0f;
		dir.y = state.Gamepad.sThumbLY / 32767.0f;
	}

	return dir;
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
		// 宇宙船に所持パーツを渡して自動修理を実行
		spaceship_->Repair(repairPartLowCount_, repairPartMediumCount_, repairPartHighCount_);

		// 所持している修理パーツを消費（リセット）
		repairPartLowCount_ = 0;
		repairPartMediumCount_ = 0;
		repairPartHighCount_ = 0;

		// 巻取り完了処理
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

void Player::DrawAreaLimit()
{
	float z = object_->transform_.translate_.z;

	Cygnus::Float3 topLeft = { -kDefaultLimitX, kDefaultLimitMaxY, z};
	Cygnus::Float3 topRight = { kDefaultLimitX, kDefaultLimitMaxY, z };
	Cygnus::Float3 bottomLeft = { -kDefaultLimitX, kDefaultLimitMinY, z };
	Cygnus::Float3 bottomRight = { kDefaultLimitX, kDefaultLimitMinY, z };

	Cygnus::Float4 lineColor = { 0.0f, 1.0f, 0.0f, 1.0f };

	auto lineDrawer = Cygnus::LineDrawer::GetInstance();
	lineDrawer->RegisterLine(topLeft, topRight, lineColor); // 上辺
	lineDrawer->RegisterLine(topRight, bottomRight, lineColor); // 右辺
	lineDrawer->RegisterLine(bottomRight, bottomLeft, lineColor); // 下辺
	lineDrawer->RegisterLine(bottomLeft, topLeft, lineColor); // 左辺
}
