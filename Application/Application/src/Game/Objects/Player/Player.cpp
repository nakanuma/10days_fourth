#include "Player.h"

// Engine
#include <Collider/CollisionManager.h>
#include <Input/Input.h>
#include <ImguiWrapper.h>
#include <TimeManager.h>
#include <LineDrawer.h>
#include <Easing.h>
#include <SoundManager.h>
#include <ParticleEffect/ParticleEffectManager.h>

// Application
#include <src/Game/Objects/Spaceship/Spaceship.h>
#include <src/Game/Objects/FlyingObject/Base/FlyingObject.h>
#include <src/Game/Scene/PauseMenu.h>
#include <src/Game/UI/GameHUD.h>

#include "src/Game/UI/Explain/ExplainUI.h"

void Player::Initialize(Spaceship* spaceship, Cygnus::SpriteCommon* spriteCommon) {
	spaceship_ = spaceship;

	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Player");
	object_->transform_.translate_ = { 0.0f, -4.0f, 0.0f };
	object_->materialCB_.data_->emissiveColor = {1.0f, 0.0f, 0.0f};

	// 各パラメーター初期化
	velocity_ = { 0.0f, 0.0f, 0.0f };
	driftTimer_ = 0.0f;

	isRewinding_ = false;
	autoRewindTimer_ = 0.0f;

	hp_ = kMaxHP;
	isDead_ = false;

	isTriggerBomb_ = false;

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
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	/* 被弾時の点滅 */
	if(damageFlashTimer_ > 0.0f) {
		damageFlashTimer_ -= dt;
		if(damageFlashTimer_ < 0.0f) {
			damageFlashTimer_ = 0.0f;
		}

		// 経過時間
		float elapsedTime = (kDamageFlashInDuration + kDamageFlashInDuration + kDamageFlashOutDuration) - damageFlashTimer_;
		float intensity = 0.0;

		// 0->1
		if(elapsedTime < kDamageFlashInDuration) {
			float t = elapsedTime / kDamageFlashInDuration;
			intensity = Cygnus::Easing::EaseOutExpo(t);
		// 待機
		} else if (elapsedTime < (kDamageFlashInDuration + kDamageFlashHoldDuration)) {
			intensity = 1.0f;
		// 1->0
		} else {
			float outProgress = (elapsedTime - kDamageFlashInDuration - kDamageFlashHoldDuration) / kDamageFlashOutDuration;
			intensity = Cygnus::Easing::Lerp(1.0f, 0.0f, Cygnus::Easing::EaseOutQuad(outProgress));
		}

		object_->materialCB_.data_->emissiveIntensity = intensity;
	} else {
		object_->materialCB_.data_->emissiveIntensity = 0.0f;
	}

	/* アイテム取得時拡縮アニメーション */
	if(pickupAnimTimer_ > 0.0f) {
		pickupAnimTimer_ -= dt;
		if(pickupAnimTimer_ < 0.0f) {
			pickupAnimTimer_ = 0.0f;
		}
	}
	float targetScale = 1.0f;
	if(pickupAnimTimer_ > 0.0f) {
		// 進行度
		float progress = 1.0f - (pickupAnimTimer_ / kPickupAnimDuration);
		float scaleOffset = std::sinf(progress * Cygnus::PIf) * (1.0f - kPickupMinScale);
		targetScale = 1.0f - scaleOffset;
	}
	// オブジェクトのスケールに適用
	object_->transform_.scale_ = {targetScale, targetScale, targetScale};

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

	ImGui::Separator();

	ImGui::DragFloat("EmissiveIntensity", &object_->materialCB_.data_->emissiveIntensity);

	ImGui::ColorPicker4("EmissiveColor", &object_->materialCB_.data_->emissiveColor.x);

	ImGui::End();
#endif
}

void Player::StartRewind() {
	// 巻取りを実行する
	if (!isRewinding_) {
		isRewinding_ = true;
	}
}

void Player::OnCollision(Cygnus::Collider* other) {
	const std::string& tag = other->GetTag();

	/* 隕石との衝突処理 */
	if (tag == "MeteorSmall" || tag == "MeteorLarge") {
		Cygnus::SoundManager::GetInstance()->Play("se_collide", false, 0.75f); // SE再生（衝突）
		ApplyDamage(1); // ダメージを与える
		pickupAnimTimer_ = kPickupAnimDuration;
	}

	/* 各修理パーツとの衝突 */
	if (other->GetTag() == "RepairPartLow") {
		repairPartLowCount_++;
		pickupAnimTimer_ = kPickupAnimDuration;
		Cygnus::SoundManager::GetInstance()->Play("se_pickup", false, 0.75f); // SE再生（取得）
		partsCountUI_->AddParts();
		gameHUD_->GetValueChangeUI()->RegistDigitValue(object_->transform_.translate_, 1, { 1,1,0,1 });
		if (onPickupPartCallback_) onPickupPartCallback_(PartType::Low, object_->transform_.translate_);
	}
	if (other->GetTag() == "RepairPartMedium") {
		repairPartMediumCount_++;
		pickupAnimTimer_ = kPickupAnimDuration;
		Cygnus::SoundManager::GetInstance()->Play("se_pickup", false, 0.75f); // SE再生（取得）
		partsCountUI_->AddParts();
		gameHUD_->GetValueChangeUI()->RegistDigitValue(object_->transform_.translate_, 1, { 0,0,1,1 });
		if (onPickupPartCallback_) onPickupPartCallback_(PartType::Medium, object_->transform_.translate_);
	}
	if (other->GetTag() == "RepairPartHigh") {
		repairPartHighCount_++;
		pickupAnimTimer_ = kPickupAnimDuration;
		Cygnus::SoundManager::GetInstance()->Play("se_pickup", false, 0.75f); // SE再生（取得）
		partsCountUI_->AddParts();
		gameHUD_->GetValueChangeUI()->RegistDigitValue(object_->transform_.translate_, 1, { 1,0,1,1 });
		if (onPickupPartCallback_) onPickupPartCallback_(PartType::High, object_->transform_.translate_);
	}

	/* ハート（回復アイテム）との衝突 */
	if (tag == "HeartItem") {
		Heal(1); // 1回復
		pickupAnimTimer_ = kPickupAnimDuration;
		Cygnus::SoundManager::GetInstance()->Play("se_pickup", false, 0.75f); // SE再生（取得）
		gameHUD_->GetValueChangeUI()->RegistDigitValue(object_->transform_.translate_, 1, { 1,0,0,1 }); // HP変動UIをつける
	}

	/* 爆弾アイテムとの衝突 */
	if (tag == "BombItem") {
		isTriggerBomb_ = true; // 爆弾取得フラグを立てる
		pickupAnimTimer_ = kPickupAnimDuration;
		Cygnus::SoundManager::GetInstance()->Play("se_pickup", false, 0.75f); // SE再生（取得）
	}

	ActionJudgment::GetInstance()->IsAction(other->GetTag() == "RepairPartLow", 2);// 説明 パーツを取る
	ActionJudgment::GetInstance()->IsAction(other->GetTag() == "RepairPartHigh", 3);// 説明 深いパーツを取る
}

void Player::ApplyDamage(int32_t damage) {
	if (isDead_) return;

	hp_ -= damage;

	// 被弾フラッシュ開始
	damageFlashTimer_ = kDamageFlashInDuration + kDamageFlashInDuration + kDamageFlashOutDuration;

	//ダメージ数を表記
	gameHUD_->GetValueChangeUI()->RegistDigitValue(object_->transform_.translate_, -damage, { 1,0,0,1 });

	// 被ダメージ時にシェイクのコールバックを呼ぶ
	if(onDamageCallback_) {
		onDamageCallback_(5.5f, 1.2f);
	}

	// HPが0になれば死亡フラグを立てる
	if (hp_ <= 0) {
		hp_ = 0;
		isDead_ = true;
	}
}

void Player::Heal(int32_t amount) {
	if (isDead_) return;
	hp_ = (std::min)(hp_ + amount, kMaxHP);
}

bool Player::IsTriggerBomb() {
	bool trigger = isTriggerBomb_;
	isTriggerBomb_ = false; // 消費してリセット
	return trigger;
}

void Player::Move() {
	auto input = Cygnus::Input::GetInstance();
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	/* 巻取りトリガー判定 */
	if (!isRewinding_) {
		// タイマー更新（宇宙船より下にいる間）
		if (object_->transform_.translate_.y < -5.0f) {
			autoRewindTimer_ += dt;
			// 自動巻き取りの限界時間（酸素ゲージ）に達したら自動巻き取り開始
			if (autoRewindTimer_ >= kDefaultAutoRewindTime) {
				StartRewind();
				Cygnus::SoundManager::GetInstance()->Play("se_return", false, 0.75f); // SE再生（帰還）
			}
		}
		else {
			autoRewindTimer_ = 0.0f; // 上部にいる間はリセット
		}

		// 巻取り入力判定（キーボード: SPACE / コントローラー: Aボタン or RBボタン）
		bool triggerKeyboard = input->TriggerKey(DIK_SPACE);
		bool triggerPad = input->IsTriggerButton(0, XINPUT_GAMEPAD_A) || input->IsTriggerButton(0, XINPUT_GAMEPAD_RIGHT_SHOULDER);
		if (triggerKeyboard || triggerPad) {
			StartRewind();
			Cygnus::SoundManager::GetInstance()->Play("se_return", false, 0.75f); // SE再生（帰還）
		}
	}

	/* 移動力の計算 */
	if (isRewinding_) {
		// 巻取り中の移動計算
		ProcessRewind();
	}
	else {
		// キーボードとゲームパッド双方の入力ベクトルを合算
		Cygnus::Float3 inputVec = { 0.0f, 0.0f, 0.0f };
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

	/* 慣性回転処理 */
	if (isRewinding_) {
		// 巻取り中に進行方向を向かせる
		if (Cygnus::Float3::Length(velocity_) > 0.01f) {
			float targetAngleZ = std::atan2f(-velocity_.x, velocity_.y);
			object_->transform_.rotate_.z = Cygnus::Easing::Lerp(object_->transform_.rotate_.z, targetAngleZ, kRotateLerpRate);
		}
	}
	else {
		// Z軸
		float ratioX = velocity_.x / kMaxSpeed;
		float targetRotateZ = -ratioX * kMaxTiltZ;

		// X軸
		float ratioY = velocity_.y / kMaxSpeed;
		float targetRotateX = ratioY * kMaxTiltX;

		// Y軸
		float targetRotateY = ratioX * 0.2f;

		// 実際に適用
		object_->transform_.rotate_ =
			Cygnus::Float3::Lerp(
				object_->transform_.rotate_,
				{ targetRotateX, targetRotateY, targetRotateZ },
				kRotateLerpRate
			);
	}

	// 移動時パーティクル生成
	/*Cygnus::ParticleEffectManager::GetInstance()->Emit("move_player", object_->transform_.translate_,
		1,
		Cygnus::Float3(0, 0, 0),
		0.0f
	);*/

}

Cygnus::Float3 Player::GetKeyInput() {
	auto input = Cygnus::Input::GetInstance();
	Cygnus::Float3 dir = { 0.0f, 0.0f, 0.0f };

	if (input->PushKey(DIK_W)) dir.y += 1.0f;
	if (input->PushKey(DIK_S)) dir.y -= 1.0f;
	if (input->PushKey(DIK_A)) dir.x -= 1.0f;
	if (input->PushKey(DIK_D)) dir.x += 1.0f;

	return dir;
}

Cygnus::Float3 Player::GetPadInput() {
	auto input = Cygnus::Input::GetInstance();
	XINPUT_STATE state;

	Cygnus::Float3 dir = { 0.0f, 0.0f, 0.0f };

	// コントローラー接続確認と状態取得
	if (input->GetJoystickState(0, state)) {
		// 左スティック入力
		dir.x = state.Gamepad.sThumbLX / 32767.0f;
		dir.y = state.Gamepad.sThumbLY / 32767.0f;
	}

	return dir;
}

bool Player::IsMoving() {
	auto input = Cygnus::Input::GetInstance();
	if (input->PushKey(DIK_W) || input->PushKey(DIK_A) || input->PushKey(DIK_S) || input->PushKey(DIK_D)) {
		return true;
	}

	XINPUT_STATE state;
	if (input->GetJoystickState(0, state)) {
		float x, y;
		x = state.Gamepad.sThumbLX / 32767.0f;
		y = state.Gamepad.sThumbLY / 32767.0f;
		// 左スティック入力
		if (x <= -0.1f || x >= 0.1f ||
			y <= -0.1f || y >= 0.1f) {
			return true;
		}
	}

	return false;
}

Cygnus::Float3 Player::Drift() {
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

	// 毎フレームタイマー加算
	driftTimer_ += dt * kDriftFrequency;

	// XとYでそれぞれ異なる周期にして不規則な漂い動作を行う
	float driftX = std::sinf(driftTimer_) * std::cosf(driftTimer_ * 0.7f) * kDriftAmplitude;
	float driftY = std::cosf(driftTimer_ * 1.3f) * std::sinf(driftTimer_ * 0.5f) * kDriftAmplitude;

	// 移動量を返す
	return { driftX, driftY, 0.0f };
}

void Player::ProcessRewind() {
	if (!spaceship_) return;

	// 宇宙船への方向ベクトルを算出
	Cygnus::Float3 targetPos = spaceship_->GetTranslate();
	Cygnus::Float3 diff = {
		targetPos - object_->transform_.translate_
	};
	float distance = std::sqrt(Cygnus::Float3::Length(diff));

	// 宇宙船に到達したら巻取り終了
	if (distance <= kRewindStopDistance) {
		// 宇宙船に所持パーツを渡して自動修理を実行
		spaceship_->Repair(repairPartLowCount_, repairPartMediumCount_, repairPartHighCount_);

		// 巻取り完了処理
		isRewinding_ = false;
		autoRewindTimer_ = 0.0f;
		velocity_ = { 0.0f, 0.0f, 0.0f };
		return;
	}

	// 宇宙船に向かう加速度を足す
	Cygnus::Float3 dir = { diff.x / distance, diff.y / distance, diff.z / distance };
	velocity_ += dir * kDefaultRewindAccel;

	// 巻取り中の最高速度制御
	float currentSpeed = std::sqrt(Cygnus::Float3::Length(velocity_));
	if (currentSpeed > kDefaultRewindMaxSpeed) {
		velocity_ = (velocity_ / currentSpeed) * kDefaultRewindMaxSpeed;
	}

	// 座標への適用
	object_->transform_.translate_ += velocity_;
}

void Player::DrawAreaLimit() {
	float z = object_->transform_.translate_.z;

	Cygnus::Float3 topLeft = { -kDefaultLimitX, kDefaultLimitMaxY, z };
	Cygnus::Float3 topRight = { kDefaultLimitX, kDefaultLimitMaxY, z };
	Cygnus::Float3 bottomLeft = { -kDefaultLimitX, kDefaultLimitMinY, z };
	Cygnus::Float3 bottomRight = { kDefaultLimitX, kDefaultLimitMinY, z };

	const Cygnus::Float4 lineColor = { 0.0f, 1.0f, 0.0f, 1.0f };
	const float thickness = 0.3f; // 点線の太さ

	// 点線を描画するヘルパー関数
	auto DrawDashedLine = [&](const Cygnus::Float3& start, const Cygnus::Float3& end, float dashLength, float gapLength) {
		Cygnus::Float3 dir = end - start;	
		float totalLength = std::sqrt(dir.x * dir.x + dir.y * dir.y + dir.z * dir.z);
		if (totalLength <= 0.0001f) return;

		// 方向の正規化
		Cygnus::Float3 normDir = {dir.x / totalLength, dir.y / totalLength, dir.z / totalLength};

		float currentDist = 0.0f;
		auto lineDrawer = Cygnus::LineDrawer::GetInstance();

		while(currentDist < totalLength) {
			float segStartDist = currentDist;
			float segEndDist = (std::min)(currentDist + dashLength, totalLength);

			 Cygnus::Float3 p1 = {
				start.x + normDir.x * segStartDist,
				start.y + normDir.y * segStartDist,
				start.z + normDir.z * segStartDist
			 };

			 Cygnus::Float3 p2 = {
				start.x + normDir.x * segEndDist,
				start.y + normDir.y * segEndDist,
				start.z + normDir.z * segEndDist
			 };
			 
			 // 線の登録
			 lineDrawer->RegisterTracer(p1, p2, thickness, lineColor, lineColor);

			 // 次の描画位置へ移動
			 currentDist += (dashLength + gapLength);
		}
	};

	// 点線の長さと間隔の設定
	const float kDashLength = 1.0f; // 描画する長さ
	const float kGapLength = 0.5f; // 空白の長さ

	// 外枠4辺の描画
	DrawDashedLine(topLeft, topRight, kDashLength, kGapLength); // 上
	DrawDashedLine(topRight, bottomRight, kDashLength, kGapLength); // 右
	DrawDashedLine(bottomRight, bottomLeft, kDashLength, kGapLength); // 下
	DrawDashedLine(bottomLeft, topLeft, kDashLength, kGapLength); // 左
}
