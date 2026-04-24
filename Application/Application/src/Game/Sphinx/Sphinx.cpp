#include "Sphinx.h"

// Engine
#include <Input/Input.h>
#include <ImguiWrapper.h>
#include <TimeManager.h>
#include <RandomGenerator.h>

void Sphinx::Initialize()
{
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Player");
	object_->transform_.translate_ = { -10.0f, 2.0f, 0.0f };
	object_->transform_.scale_ = { kColliderSize.x, 1.0f, kColliderSize.z };

	// コライダー生成 + 登録
	auto obb = std::make_unique<Cygnus::OBBCollider>();
	obb->SetTag("Player");
	obb->SetFollowTarget(&object_->transform_.translate_);
	obb->SetFollowRotation(&object_->transform_.rotate_);
	obb->SetSize(kColliderSize);
	obb->SetOwner(this);

	collider_ = std::move(obb);
	Cygnus::CollisionManager::GetInstance()->Register(collider_.get());

	// ランダムウォーククラス生成
	randomWalk_ = std::make_unique<RandomWalk>();
}

void Sphinx::Update(float deltaTime)
{
#pragma region 入力による移動処理
	auto input = Cygnus::Input::GetInstance();
	if (input->PushKey(DIK_Q)&& !isAttack_)
	{
		isAttack_ = true;
		attackTimer_ = kAttackTime_;
		attackDir_ = randomWalk_->GetRandomWalkDir();
		object_->transform_.rotate_.y = std::atan2(attackDir_.x, attackDir_.z);
	}

	if(!isAttack_)
	{
		randomWalk_->Update(deltaTime, 1.0f);
		Move(deltaTime);
	}
	else
	{
		Attack(deltaTime);
	}
	// 入力がある場合のみ回転と移動を行う
	//if (Cygnus::Float3::Length(moveDir) > 0.01f)
	//{
	//	// 回転処理
	//	float angle = std::atan2f(moveDir.x, moveDir.z);	// 入力ベクトルから角度を計算
	//	const float kStep = Cygnus::PIf / 4.0f;	// 8方向に限定するため45度
	//	object_->transform_.rotate_.y = std::round(angle / kStep) * kStep;	// オブジェクト回転に反映

	//	// 移動処理
	//	if (Cygnus::Float3::Length(moveDir) > 1.0f)
	//	{
	//		// 正規化して一定の速度を保つように
	//		moveDir = Cygnus::Float3::Normalize(moveDir);
	//	}
	//}

	// オブジェクト位置に反映
	//object_->transform_.translate_ += moveDir * kMoveSpeed * deltaTime;
#pragma endregion
	// コライダー更新
	collider_->Update();
	// オブジェクト更新
	object_->UpdateMatrix();
}

void Sphinx::Move(float deltaTime)
{
	moveDir_ = randomWalk_->GetRandomWalkDir();

	object_->transform_.rotate_.y = std::atan2(moveDir_.x, moveDir_.z);

	object_->transform_.translate_ += moveDir_ * kMoveSpeed * deltaTime;
}

void Sphinx::Attack(float deltaTime)
{
	attackTimer_ -= deltaTime;
	if(attackTimer_ > 0.0f)
	{
		object_->transform_.translate_ += attackDir_ * kAttackMoveSpeed_ * deltaTime;
	}
	else
	{
		Stop();
	}
}

void Sphinx::Stop()
{
	isAttack_ = false;
	attackTimer_ = 0.0f;
}

void Sphinx::Draw()
{
	// オブジェクト描画
	object_->Draw();
}

void Sphinx::Debug()
{
#ifdef USE_IMGUI
	ImGui::Begin("Sphinx");

	ImGui::DragFloat3("Translate", &object_->transform_.translate_.x, 0.01f);

	ImGui::End();
#endif
}

void Sphinx::OnCollision(Cygnus::Collider* other)
{
	// 線路に沿って動くオブジェクトとの衝突
	if (other->GetTag() == "Carrier")
	{
		Cygnus::AABBCollider* myAABB = dynamic_cast<Cygnus::AABBCollider*>(collider_.get());
		Cygnus::AABBCollider* otherAABB = dynamic_cast<Cygnus::AABBCollider*>(other);

		// 押し戻し処理
		if (myAABB && otherAABB)
		{
			// 押し戻しベクトル取得
			Cygnus::Float3 pushVec = myAABB->GetPushBackVector(*otherAABB);
			// プレイヤー位置を補正
			object_->transform_.translate_ += pushVec;
			object_->UpdateMatrix();

			// コライダーも更新
			Cygnus::Float3 currentMin = myAABB->GetMin();
			Cygnus::Float3 currentMax = myAABB->GetMax();
			myAABB->SetMin(currentMin + pushVec);
			myAABB->SetMax(currentMax + pushVec);
		}
		Stop();
	}

	// 鉱石オブジェクトとの衝突
	if (other->GetTag() == "Ore")
	{
		Cygnus::AABBCollider* myAABB = dynamic_cast<Cygnus::AABBCollider*>(collider_.get());
		Cygnus::AABBCollider* otherAABB = dynamic_cast<Cygnus::AABBCollider*>(other);

		// 押し戻し処理
		if (myAABB && otherAABB)
		{
			// 押し戻しベクトル取得
			Cygnus::Float3 pushVec = myAABB->GetPushBackVector(*otherAABB);
			// プレイヤー位置を補正
			object_->transform_.translate_ += pushVec;
			object_->UpdateMatrix();

			// コライダーも更新
			Cygnus::Float3 currentMin = myAABB->GetMin();
			Cygnus::Float3 currentMax = myAABB->GetMax();
			myAABB->SetMin(currentMin + pushVec);
			myAABB->SetMax(currentMax + pushVec);
		}
		Stop();
	}

	// プレイヤーオブジェクトとの衝突
	if (other->GetTag() == "Player")
	{
		Cygnus::AABBCollider* myAABB = dynamic_cast<Cygnus::AABBCollider*>(collider_.get());
		Cygnus::AABBCollider* otherAABB = dynamic_cast<Cygnus::AABBCollider*>(other);

		// 押し戻し処理
		if (myAABB && otherAABB)
		{
			// 押し戻しベクトル取得
			Cygnus::Float3 pushVec = myAABB->GetPushBackVector(*otherAABB);
			// プレイヤー位置を補正
			object_->transform_.translate_ += pushVec;
			object_->UpdateMatrix();

			// コライダーも更新
			Cygnus::Float3 currentMin = myAABB->GetMin();
			Cygnus::Float3 currentMax = myAABB->GetMax();
			myAABB->SetMin(currentMin + pushVec);
			myAABB->SetMax(currentMax + pushVec);
		}
		Stop();
	}
}
