#include "Carrier.h"

// Engine
#include <ImguiWrapper.h>

// Application
#include <src/Game/Path/PathManager.h>

void Carrier::Initialize() {
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Carrier");
	object_->transform_.translate_ = PathManager::GetInstance()->GetPoint(0);	// 経路の始点座標をセット

	// コライダー生成 + 登録
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("Carrier");
	aabb->SetFollowTarget(&object_->transform_.translate_);
	aabb->SetSize(kColliderSize);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());
}

void Carrier::Update(float deltaTime) {
	// 経路に沿った移動処理
	MoveAlongPath(deltaTime);

	// コライダー更新
	collider_->Update();
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
	ImGui::Separator();
	ImGui::DragFloat3("Translate", &object_->transform_.translate_.x, 0.01f);
	ImGui::Checkbox("IsGoal", &isGoal_);

	ImGui::End();
#endif
}

void Carrier::OnCollision(Cygnus::Collider* collider)
{

}

void Carrier::MoveAlongPath(float deltaTime)
{
	// 無効化状態なら終了
	if(!isActive_) return;

	// ゴール済みなら終了
	if(isGoal_) return;

	auto pathManager = PathManager::GetInstance();

	// 全てのポイントを通過したらゴール済みへ
	if(targetIndex_ >= pathManager->GetPointCount()) {
		isGoal_ = true;
		return;
	}

	// 現在地->目標地点への距離を計算
	Cygnus::Float3& currentPos = object_->transform_.translate_;
	Cygnus::Float3 targetPos = pathManager->GetPoint(targetIndex_);

	Cygnus::Float3 diff = targetPos - currentPos;
	float distance = Cygnus::Float3::Length(diff);

	// ポイント到着判定 + オブジェクト移動処理
	if(distance < kMoveSpeed * deltaTime) {
		currentPos = targetPos;
		targetIndex_++;	// 次のポイントへ
	} else {
		currentPos += (diff / distance) * kMoveSpeed * deltaTime;
	}
}
