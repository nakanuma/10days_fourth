#include "Player.h"

// Engine
#include <Collider/CollisionManager.h>
#include <Input/Input.h>
#include <ImguiWrapper.h>

void Player::Initialize() {
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Player");
	object_->transform_.translate_ = {0.0f, -10.0f, 0.0f}; 

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

	// 移動量の計算用
	Cygnus::Float3 move = { 0.0f, 0.0f, 0.0f };

	// キー入力移動
	if (input->PushKey(DIK_W)) {
		move.y += kMoveSpeed;
	}
	if (input->PushKey(DIK_S)) {
		move.y -= kMoveSpeed;
	}
	if (input->PushKey(DIK_A)) {
		move.x -= kMoveSpeed;
	}
	if (input->PushKey(DIK_D)) {
		move.x += kMoveSpeed;
	}

	// 座標に加算
	object_->transform_.translate_ += move;
}
