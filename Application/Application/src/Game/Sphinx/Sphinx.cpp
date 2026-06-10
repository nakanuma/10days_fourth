#include "Sphinx.h"
#include "SphinxState.h"

// Engine
#include <Input/Input.h>
#include <ImguiWrapper.h>
#include <TimeManager.h>
#include <RandomGenerator.h>
#include <Collider/CollisionMath.h>
#include <ParticleEffect/ParticleEffectManager.h>

// Application
#include <src/Game/Ore/OreManager.h>

// =========================================================
// Sphinx クラスのメンバ関数
// =========================================================

void Sphinx::Initialize()
{
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Sphinx");
	object_->transform_.translate_ = { -10.0f, 2.0f, 0.0f };
	object_->transform_.scale_ = { 1.0f, 1.0f, 1.0f };

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

	// ステートの登録
	stateMachine_.RegisterState<WanderState>(SphinxState::Wander, "Wander");
	stateMachine_.RegisterState<ChargeState>(SphinxState::Charge, "Charge");
	stateMachine_.RegisterState<AttackState>(SphinxState::Attack, "Attack");
	stateMachine_.RegisterState<FaintState>(SphinxState::Faint, "Faint");
	stateMachine_.RegisterState<CoolDownState>(SphinxState::CoolDown, "CoolDown");

	stateMachine_.ChangeState(SphinxState::Wander);


	// オブジェクト生成
	attackPlane_ = std::make_unique<Cygnus::Object3D>();
	attackPlane_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("AttackPlane");
	attackPlane_->materialCB_.data_->color = kPlaneColor;

	// オブジェクト生成
	attackFrame_ = std::make_unique<Cygnus::Object3D>();
	attackFrame_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("AttachFrame");
	attackFrame_->transform_.scale_ = { 1.0f, 1.0f, kSearchRange_ / 2.0f };
	attackFrame_->materialCB_.data_->color = kFrameColor;
}

void Sphinx::Update(float deltaTime, const Cygnus::Float3& targetPos)
{
	// 共有変数の更新
	SetTargetPos(targetPos);

	// ステートマシンの更新
	stateMachine_.UpdateState(*this, deltaTime);

	MoveClamp();
	collider_->Update();
	object_->UpdateMatrix();
}

void Sphinx::UpdateAttackSign(float t)
{
	attackPlane_->transform_.rotate_ = object_->transform_.rotate_;
	attackPlane_->transform_.translate_ = { object_->transform_.translate_.x, 0.05f, object_->transform_.translate_.z };
	attackPlane_->transform_.scale_ = { 1.0f, 1.0f, (kSearchRange_ * t) / 2.0f };
	attackPlane_->UpdateMatrix();

	attackFrame_->transform_.rotate_ = object_->transform_.rotate_;
	attackFrame_->transform_.translate_ = { object_->transform_.translate_.x, 0.06f, object_->transform_.translate_.z };
	attackFrame_->UpdateMatrix();
}

void Sphinx::MoveForward(float speed, float deltaTime)
{
	float angleY = object_->transform_.rotate_.y;
	Cygnus::Float3 forward = { std::sin(angleY), 0.0f, std::cos(angleY) };
	uint32_t effectCount = 0;

	// 攻撃中なら attackDir を使い、それ以外なら向きから計算
	if (stateMachine_.GetCurrentState() == SphinxState::Attack)
	{
		object_->transform_.translate_ += attackDir_ * speed * deltaTime;
		effectCount = 6; // 攻撃中は多めにパーティクルを生成
	}
	else
	{
		object_->transform_.translate_ += forward * speed * deltaTime;
	}

	// 移動時パーティクルの生成
	Cygnus::ParticleEffectManager::GetInstance()->Emit("move_dust", object_->transform_.translate_+Cygnus::Float3(0,-2.0f,0),effectCount);

}

void Sphinx::OreMining()
{
	float angleY = object_->transform_.rotate_.y;
	Cygnus::Float3 frontVec = { std::sinf(angleY), 0.0f, std::cosf(angleY) };
	Cygnus::Float3 miningPos = {
		object_->transform_.translate_.x + frontVec.x * kMiningOffset,
		object_->transform_.translate_.y,
		object_->transform_.translate_.z + frontVec.z * kMiningOffset
	};

	if (OreManager::GetInstance()->BreakAllAt(miningPos, kMiningRange))
	{
		SetIsMining(true);
	}
}

void Sphinx::MoveClamp()
{
	object_->transform_.translate_.x = std::clamp(object_->transform_.translate_.x, kMoveMin.x, kMoveMax.x);
	object_->transform_.translate_.z = std::clamp(object_->transform_.translate_.z, kMoveMin.z, kMoveMax.z);
}

void Sphinx::Draw()
{
	// オブジェクト描画

	if (stateMachine_.GetCurrentState() == SphinxState::Charge)
	{
		attackPlane_->Draw();
		attackFrame_->Draw();
	}

	object_->Draw();
}

void Sphinx::Debug()
{
#ifdef USE_IMGUI
	ImGui::Begin("Sphinx");

	// 座標調整
	ImGui::DragFloat3("Translate", &object_->transform_.translate_.x, 0.01f);

	// Pause/Resume 機能
	if (ImGui::Button("Pause/Resume"))
	{
		isMoving_ = !isMoving_; // フラグ反転
	}
	ImGui::Text("Current : %s", isMoving_ ? "Move" : "Pause");

	ImGui::Separator(); // 区切り線

	// ステートマシンのデバッグ表示を呼び出す
	// これにより、現在のステート名と経過時間(Timer)が自動で表示されます
	stateMachine_.DebugImGui("Status");

	ImGui::End();
#endif
}

void Sphinx::OnCollision(Cygnus::Collider* other)
{
	// 線路に沿って動くオブジェクトとプレイヤーオブジェクトとの衝突
	if (other->GetTag() == "Player" || other->GetTag() == "WorkBench")
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
	}
	// Carrierとの押し戻し（OBB）
	if(other->GetTag() == "Carrier") {
		Cygnus::OBBCollider* myOBB = myOBB = dynamic_cast<Cygnus::OBBCollider*>(collider_.get());
		Cygnus::OBBCollider* otherOBB = dynamic_cast<Cygnus::OBBCollider*>(other);

		if(myOBB && otherOBB) {
			Cygnus::Float3 pushVec = Cygnus::CollisionMath::CalculatePushBackOBBvsOBB(myOBB, otherOBB);

			object_->transform_.translate_ += pushVec;
			object_->UpdateMatrix();

			myOBB->Update();
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
	}

	// ステート遷移の判定
	std::string tag = other->GetTag();
	std::optional<SphinxState> currentState = stateMachine_.GetCurrentState();

	// 攻撃中かつ特定のタグにぶつかった場合
	if (currentState == SphinxState::Attack)
	{
		bool shouldFaint = false;

		bool shouldCool = false;

		if (tag == "Ore")
		{
			OreMining();
		}

		// 列車や鉱石にぶつかった場合
		if (tag == "Carrier" || tag == "Ore")
		{
			shouldFaint = true;
		}
		// 落ちている鉱石との衝突 (採掘中フラグがある場合)
		else if (tag == "DroppedOre" && isMining_)
		{
			shouldFaint = true;
		}

		if (tag == "Player")
		{
			shouldCool = true;
		}

		if (shouldFaint)
		{
			// ステートマシンに気絶への遷移を命じる
			stateMachine_.ChangeState(SphinxState::Faint);
		}
		else if (shouldCool)
		{
			// ステートマシンに気絶への遷移を命じる
			stateMachine_.ChangeState(SphinxState::CoolDown);
		}
	}
}
