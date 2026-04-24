#include "Sphinx.h"

// Engine
#include <Input/Input.h>
#include <ImguiWrapper.h>
#include <TimeManager.h>
#include <RandomGenerator.h>

// Application
#include <src/Game/Ore/OreManager.h>

void Sphinx::Initialize()
{
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Player");
	object_->transform_.translate_ = { -10.0f, 2.0f, 0.0f };
	object_->transform_.scale_ = { kColliderSize.x, 1.0f, kColliderSize.z };

	// コライダー生成 + 登録
	auto obb = std::make_unique<Cygnus::OBBCollider>();
	obb->SetTag("Sphinx");
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
#pragma region 移動処理
	auto input = Cygnus::Input::GetInstance();
	if (input->PushKey(DIK_Q)&& !isAttack_)
	{
		StartAttack(randomWalk_->GetRandomWalkDir());
	}

	// 気絶中は攻撃・移動処理を行わない
	if(!Faint(deltaTime))
	{
		if (!isAttack_)
		{
			randomWalk_->Update(deltaTime, kMoveChangeTime_);
			Move(deltaTime);
		}
		else
		{
			Attack(deltaTime);
		}
	}
	MoveClamp();
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

void Sphinx::StartAttack(const Cygnus::Float3& targetDir)
{
	isAttack_ = true;
	attackTimer_ = kAttackTime_;
	attackDir_ = targetDir;
	object_->transform_.rotate_.y = std::atan2(attackDir_.x, attackDir_.z);
}

void Sphinx::Attack(float deltaTime)
{
	attackTimer_ -= deltaTime;
	if(attackTimer_ > 0.0f)
	{
		object_->transform_.translate_ += attackDir_ * kAttackMoveSpeed_ * deltaTime;
		OreMining();
	}
	else
	{
		StopAttack();
	}
}

bool Sphinx::Faint(float deltaTime)
{
	if (faintTimer_ > 0.0f)
	{
		faintTimer_ -= deltaTime;
		return true;
	}
	return false;
}

void Sphinx::StartFaint()
{
	faintTimer_ = kFaintTime_;
}

void Sphinx::OreMining()
{
	// 向きから前方のベクトルを作成する
	float angleY = object_->transform_.rotate_.y;
	Cygnus::Float3 frontVec = { std::sinf(angleY), 0.0f, std::cosf(angleY) };

	// スフィンクスの少し前方を判定の中心にする
	Cygnus::Float3 targetPos = {
		object_->transform_.translate_.x + frontVec.x * kMiningOffset,
		object_->transform_.translate_.y,
		object_->transform_.translate_.z + frontVec.z * kMiningOffset
	};

	// 鉱石採掘判定
	if (OreManager::GetInstance()->TryBreakAt(targetPos, kMiningRange))
	{
		// 鉱石採掘時の処理

	}
}

void Sphinx::MoveClamp()
{
	object_->transform_.translate_.x = std::clamp(object_->transform_.translate_.x, kMoveMin.x, kMoveMax.x);
	object_->transform_.translate_.z = std::clamp(object_->transform_.translate_.z, kMoveMin.z, kMoveMax.z);
}

void Sphinx::StopAttack()
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


	ImGui::Text("Status : ");
	ImGui::SameLine();
	if (faintTimer_ > 0.0f)
	{
		ImGui::Text("Faint");
		ImGui::Text("Timer : %.02f", faintTimer_);
	}
	else if (isAttack_)
	{
		ImGui::Text("Attack");
		ImGui::Text("Timer : %.02f", attackTimer_);
	}
	else
	{
		ImGui::Text("Move");
	}


	ImGui::End();
#endif
}

void Sphinx::OnCollision(Cygnus::Collider* other)
{
	// 線路に沿って動くオブジェクトとの衝突
	if (other->GetTag() == "Carrier")
	{
		Cygnus::OBBCollider* myOBB = dynamic_cast<Cygnus::OBBCollider*>(collider_.get());
		Cygnus::AABBCollider* otherAABB = dynamic_cast<Cygnus::AABBCollider*>(other);

		// 押し戻し処理
		if (isAttack_ && faintTimer_ <= 0.0f)
		{
			StopAttack();
		}
	}

	// 鉱石オブジェクトとの衝突
	if (other->GetTag() == "Ore")
	{
		Cygnus::OBBCollider* myOBB = dynamic_cast<Cygnus::OBBCollider*>(collider_.get());
		Cygnus::AABBCollider* otherAABB = dynamic_cast<Cygnus::AABBCollider*>(other);

		// 押し戻し処理
		if (isAttack_ && faintTimer_ <= 0.0f)
		{
			StopAttack();
			StartFaint();
		}
	}

	// プレイヤーオブジェクトとの衝突
	if (other->GetTag() == "Player")
	{
		Cygnus::OBBCollider* myOBB = dynamic_cast<Cygnus::OBBCollider*>(collider_.get());
		Cygnus::AABBCollider* otherAABB = dynamic_cast<Cygnus::AABBCollider*>(other);

		// 押し戻し処理
		if (isAttack_ && faintTimer_ <= 0.0f)
		{
			StopAttack();
		}
	}
}
