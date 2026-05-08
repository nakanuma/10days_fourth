#include "Sphinx.h"

// Engine
#include <Input/Input.h>
#include <ImguiWrapper.h>
#include <TimeManager.h>
#include <RandomGenerator.h>
#include <Collider/CollisionMath.h>

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

void Sphinx::Update(float deltaTime, const Cygnus::Float3& targetPos)
{
#pragma region 移動処理

	// 気絶中は攻撃・移動処理を行わない
	if (!Faint(deltaTime))
	{
		if (attackCoolTimer_ > 0.0f)
		{
			attackCoolTimer_ -= deltaTime;
		}

		// チャージ中の処理
		if (isCharge_)
		{
			chargeTimer_ -= deltaTime;

			// 追従処理：残り時間が制限時間以上の間はプレイヤーを狙い続ける
			if (chargeTimer_ > kHomingLimitTime_)
			{
				// 常に最新のプレイヤー座標から方向を再計算
				Cygnus::Float3 toTarget = targetPos - object_->transform_.translate_;
				if (Cygnus::Float3::Length(toTarget) > 0.1f) // 念のためゼロ除算防止
				{
					attackDir_ = Cygnus::Float3::Normalize(toTarget);
					// 向き（回転）も更新
					object_->transform_.rotate_.y = std::atan2(attackDir_.x, attackDir_.z);
				}
			}

			if (chargeTimer_ <= 0.0f)
			{
				StartAttack();
			}
		}
		else if (!isAttack_)
		{
			// プレイヤーをサーチ
			Cygnus::Float3 toTarget = targetPos - object_->transform_.translate_;
			float distance = Cygnus::Float3::Length(toTarget);

			if (distance <= kSearchRange_ && attackCoolTimer_ <= 0.0f)
			{
				Cygnus::Float3 dir = Cygnus::Float3::Normalize(toTarget);
				// ▼ 変更：攻撃ではなくチャージを開始
				StartCharge(dir);
			}
			else
			{
				// 範囲外、またはクールダウン中ならランダムウォーク
				randomWalk_->Update(deltaTime, kMoveChangeTime_);
				Move(deltaTime);
			}
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

void Sphinx::StartCharge(const Cygnus::Float3& targetDir)
{
	isCharge_ = true;
	chargeTimer_ = kChargeTime_;
	attackDir_ = targetDir;

	randomWalk_->Reset();
}

void Sphinx::StartAttack()
{
	isCharge_ = false;
	isAttack_ = true;
	isMining_ = false;
	attackTimer_ = kAttackTime_;
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
		isMining_ = true;
	}
}

void Sphinx::MoveClamp()
{
	object_->transform_.translate_.x = std::clamp(object_->transform_.translate_.x, kMoveMin.x, kMoveMax.x);
	object_->transform_.translate_.z = std::clamp(object_->transform_.translate_.z, kMoveMin.z, kMoveMax.z);
}

void Sphinx::StopAttack()
{
	isCharge_ = false;
	isAttack_ = false;
	attackTimer_ = 0.0f;
	chargeTimer_ = 0.0f;
	attackCoolTimer_ = kAttackCoolTime_;
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
	else if (isCharge_) // ▼ 追加
	{
		ImGui::Text("Charging");
		ImGui::Text("Timer : %.02f", chargeTimer_);
	}
	else if (isAttack_)
	{
		ImGui::Text("Attack");
		ImGui::Text("Timer : %.02f", attackTimer_);
	}
	else if (attackCoolTimer_ > 0.0f)
	{
		ImGui::Text("Cooling Down");
		ImGui::Text("Timer : %.02f", attackCoolTimer_);
	}
	else
	{
		ImGui::Text("Move (Searching)");
	}

	ImGui::End();
#endif
}

void Sphinx::OnCollision(Cygnus::Collider* other)
{
	// 線路に沿って動くオブジェクトとプレイヤーオブジェクトとの衝突
	if (other->GetTag() == "Carrier" || other->GetTag() == "Player")
	{
		Cygnus::OBBCollider* myOBB = dynamic_cast<Cygnus::OBBCollider*>(collider_.get());
		Cygnus::AABBCollider* otherAABB = dynamic_cast<Cygnus::AABBCollider*>(other);

		// 押し戻し処理
		if (myOBB && other)
		{
			// 相手のAABBを一時的にOBBとして扱う
			Cygnus::OBBCollider otherAsOBB;
			otherAsOBB.SetCenter((otherAABB->GetMin() + otherAABB->GetMax()) * 0.5f);
			otherAsOBB.SetSize((otherAABB->GetMax() - otherAABB->GetMin()) * 0.5f);
			otherAsOBB.SetXAxis({ 1.0f, 0.0f, 0.0f });
			otherAsOBB.SetYAxis({ 0.0f, 1.0f, 0.0f });
			otherAsOBB.SetZAxis({ 0.0f, 0.0f, 1.0f });

			// 押し戻しベクトルを計算
			Cygnus::Float3 pushVec = Cygnus::CollisionMath::CalculatePushBackOBBvsOBB(myOBB, &otherAsOBB);

			// 位置を補正
			object_->transform_.translate_ += pushVec;
			object_->UpdateMatrix();

			// コライダーも更新
			myOBB->Update();
		}

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
		if (myOBB && other)
		{
			// 相手のAABBを一時的にOBBとして扱う
			Cygnus::OBBCollider otherAsOBB;
			otherAsOBB.SetCenter((otherAABB->GetMin() + otherAABB->GetMax()) * 0.5f);
			otherAsOBB.SetSize((otherAABB->GetMax() - otherAABB->GetMin()) * 0.5f);
			otherAsOBB.SetXAxis({ 1.0f, 0.0f, 0.0f });
			otherAsOBB.SetYAxis({ 0.0f, 1.0f, 0.0f });
			otherAsOBB.SetZAxis({ 0.0f, 0.0f, 1.0f });

			// 押し戻しベクトルを計算
			Cygnus::Float3 pushVec = Cygnus::CollisionMath::CalculatePushBackOBBvsOBB(myOBB, &otherAsOBB);

			// プレイヤーの位置を補正
			object_->transform_.translate_ += pushVec;
			object_->UpdateMatrix();

			// コライダーも更新
			myOBB->Update();
		}

		if (isAttack_ && faintTimer_ <= 0.0f)
		{
			StopAttack();
			StartFaint();
		}
	}

	// 落ちている鉱石（ドロップアイテム）と衝突した時も気絶判定
	if (other->GetTag() == "DroppedOre" && isMining_)
	{
		if (isAttack_ && faintTimer_ <= 0.0f)
		{
			StopAttack();
			StartFaint();
		}
	}
}
