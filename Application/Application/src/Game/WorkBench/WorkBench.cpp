#include "WorkBench.h"

// Engine
#include <Input/Input.h>

// Application
#include <src/Game/Player/Player.h>
#include <src/Game/Gear/GearManager.h>
#include <src/Game/UI/UIManager.h>

void WorkBench::Initialize(const Cygnus::Float3 translate) {
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("WorkBench");
	object_->transform_.translate_ = translate;

	// コライダー生成 + 登録
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("WorkBench");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize(kColliderSize);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());

	auto sensor = std::make_unique<Cygnus::AABBCollider>();
	sensor->SetTag("WorkBenchSensor");
	sensor->SetFollowTarget(&object_->transform_.translate_);
	sensor->SetSize(kSensorSize);
	sensor->SetOwner(this);

	colliderSensor_ = std::move(sensor);
	Cygnus::CollisionManager::GetInstance()->Register(colliderSensor_.get());
}

void WorkBench::Update() {
	// コライダー更新
	collider_->Update();
	colliderSensor_->Update();
	// オブジェクト更新
	object_->UpdateMatrix();
}

void WorkBench::Draw() {
	// オブジェクト描画
	object_->Draw();
}

void WorkBench::OnCollision(Cygnus::Collider* other) { 
	auto input = Cygnus::Input::GetInstance();

	if (other->GetTag() == "Player") {
		Player* player = static_cast<Player*>(other->GetOwner());

		// プレイヤーが歯車の生成に必用な鉱石を所持しているか確認
		if (player->GetOreCount() >= kRequiredOreCount) {
			// インタラクトUIの表示を要求
			UIManager::GetInstance()->RequestInteract(InteractGuide::ActionType::Craft);

			// キー or ボタン入力操作
			if (input->TriggerKey(DIK_SPACE)) {
				// プレイヤーの鉱石を消費させる（生成に使用した分のみ）
				player->ConsumeOre(kRequiredOreCount);
				// 歯車を前方に生成
				GearManager::GetInstance()->Spawn(object_->transform_.translate_ + Cygnus::Float3{0.0f, 0.0f, kDropOffset});
			}
		}
	}
}