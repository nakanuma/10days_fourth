#include "SunLaser.h"

SunLaser* SunLaser::GetInstance() {
	static SunLaser sInstance_;
	return &sInstance_;
}

void SunLaser::Initialize() {
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("SunLaserArea");
	object_->transform_.scale_ = { 10.0f,1.0f,10.0f };

	state_ = std::make_unique<LaserHomingState>();
	
	changeCount_ = 0;
}

void SunLaser::Update(const Cygnus::Float3& playerPosition,float deltaTime) {
	if (!isEventStart_) return;

	if (changeCount_ > 0) {
		SetPosition(position_);
	}
	else {
		//ホーミング中のみ
		SetPosition(playerPosition);
	}

	//ステート更新処理
	StateUpdate(deltaTime);

	object_->UpdateMatrix();

}

void SunLaser::Draw() {
	if (!isEventStart_) return;

	object_->Draw();
	isEventStart_ = false;
}

void SunLaser::StateUpdate(float dt) {

	object_->materialCB_.data_->color = state_->GetColor();//エリアの色変更
	state_->Update(dt, object_->transform_.translate_);

	//変更の処理
	if (state_->IsChangeState()) {
		state_ = std::move(state_->ChangeState());

		if (changeCount_ <= 0) {
			position_ = object_->transform_.translate_;//止める
		}

		changeCount_++;

		if (changeCount_ >= 3) {
			changeCount_ = 0;
		}
	}
}

#pragma region レーザーの状態ステート


std::unique_ptr<BaseLaserState> BaseLaserState::ChangeState() {
	return std::move(nextState_);
}


void LaserHomingState::Update(float dt, Cygnus::Float3& position) {
	laserTime_ += dt;
	areaColor_ = kAreaColor_;

	if (laserTime_ >= kHomingTime_) {
		nextState_ = std::make_unique<LaserReserveState>();
	}
}

void LaserReserveState::Update(float dt, Cygnus::Float3& position) {
	laserTime_ += dt;
	areaColor_ = kAreaColor_;

	if (laserTime_ >= kReserveTime_) {
		nextState_ = std::make_unique <LaserReleaseState>();
	}
}

void LaserReleaseState::Update(float dt, Cygnus::Float3& position) {

	if (laserTime_ <= 0.0f) {
		// コライダー生成 + 登録
		auto sphere = std::make_unique<Cygnus::SphereCollider>();
		sphere->SetTag("SunLaser");
		sphere->SetFollowTarget(&position);
		sphere->SetRadius(kRadius_);
		sphere->SetOwner(this);

		sphereCollider_ = std::move(sphere);
		Cygnus::CollisionManager::GetInstance()->Register(sphereCollider_.get());
	}

	laserTime_ += dt;
	areaColor_ = kAreaColor_;

	sphereCollider_->Update();

	if (laserTime_ >= kReleaseLaserTime_) {
		Cygnus::CollisionManager::GetInstance()->Unregister(sphereCollider_.get());//登録解除
	}

	if (laserTime_ >= kReleaseLaserTime_ + kFinishLaserTime_) {
		nextState_ = std::make_unique<LaserHomingState>();//ステートの変更
	}
}

#pragma endregion
