#pragma once

#include "Sphinx.h"
#include "State/StateMachine.h"
#include "State/State.h"

//Engine
#include "ParticleEffect/ParticleEffectManager.h"

// =========================================================
// 各ステートクラスの実装
// =========================================================

// --- 徘徊ステート ---
class WanderState : public State<SphinxState, Sphinx>
{
public:
	using State::State;

	void Update(Sphinx* owner, float deltaTime) override
	{
		// もし Sphinx 自体が一時停止中なら何もしない
		if (!owner->GetIsMoving()) return;

		auto* rw = owner->GetRandomWalk();
		auto* obj = owner->GetObject();

		// プレイヤーとの距離判定
		Cygnus::Float3 toTarget = owner->GetTargetPos() - obj->transform_.translate_;
		if (Cygnus::Float3::Length(toTarget) <= owner->GetSearchRange())
		{
			GetSM()->ChangeState(SphinxState::Charge);
			return;
		}

		// ランダム移動
		rw->Update(deltaTime, 2.0f); // kMoveChangeTime_
		Cygnus::Float3 moveDir = rw->GetRandomWalkDir();
		float targetAngle = std::atan2(moveDir.x, moveDir.z);
		obj->transform_.rotate_.y = rw->SmoothTurn(obj->transform_.rotate_.y, targetAngle, owner->GetWanderTurnSpeed(), deltaTime);

		owner->MoveForward(1.0f, deltaTime); // kMoveSpeed
	}
};

// --- チャージ（予備動作）ステート ---
class ChargeState : public State<SphinxState, Sphinx>
{
public:
	using State::State;

	void Update(Sphinx* owner, float deltaTime) override
	{
		// もし Sphinx 自体が一時停止中なら何もしない
		if (!owner->GetIsMoving()) return;

		auto* obj = owner->GetObject();
		float elapsed = GetElapsed();
		float chargeTime = owner->GetChargeTime();
		float homingLimit = owner->GetHomingLimitTime();

		if (elapsed < (chargeTime - homingLimit))
		{
			// 追従フェーズ
			Cygnus::Float3 toTarget = owner->GetTargetPos() - obj->transform_.translate_;
			if (Cygnus::Float3::Length(toTarget) > 0.1f)
			{
				Cygnus::Float3 targetDir = Cygnus::Float3::Normalize(toTarget);
				float targetAngle = std::atan2(targetDir.x, targetDir.z);
				obj->transform_.rotate_.y = owner->GetRandomWalk()->SmoothTurn(
					obj->transform_.rotate_.y, targetAngle, owner->GetChargeTurnSpeed(), deltaTime);

				// 突進方向を更新しておく
				float currentAngle = obj->transform_.rotate_.y;
				owner->SetAttackDir({ std::sin(currentAngle), 0.0f, std::cos(currentAngle) });
			}
			obj->transform_.translate_.y = owner->GetBaseY();
		}
		else
		{
			// ぴょんぴょんフェーズ
			float bounceTime = homingLimit - (chargeTime - elapsed);
			float jumpY = std::abs(std::sin(bounceTime * owner->GetBounceSpeed())) * owner->GetBounceHeight();
			obj->transform_.translate_.y = owner->GetBaseY() + jumpY;
		}

		owner->UpdateAttackSign(elapsed / chargeTime);

		if (elapsed >= chargeTime)
		{
			GetSM()->ChangeState(SphinxState::Attack);
		}
	}
};

// --- 突進攻撃ステート ---
class AttackState : public State<SphinxState, Sphinx>
{
public:
	using State::State;

	void Init(Sphinx* owner) override
	{
		owner->SetIsMining(false);
		owner->GetObject()->transform_.translate_.y = owner->GetBaseY();

		//攻撃ので始めにエフェクトを出す
		Cygnus::ParticleEffectManager::GetInstance()->Emit("pyonpyon", owner->GetObject()->transform_.translate_, 20);

	}

	void Update(Sphinx* owner, float deltaTime) override
	{
		// もし Sphinx 自体が一時停止中なら何もしない
		if (!owner->GetIsMoving()) return;

		owner->MoveForward(owner->GetAttackMoveSpeed(), deltaTime);
		owner->OreMining();

		if (GetElapsed() >= owner->GetAttackTime())
		{
			GetSM()->ChangeState(SphinxState::CoolDown);
		}
	}
};

// --- 気絶ステート ---
class FaintState : public State<SphinxState, Sphinx>
{
public:
	using State::State;

	void Update(Sphinx* owner, float deltaTime) override
	{
		// もし Sphinx 自体が一時停止中なら何もしない
		if (!owner->GetIsMoving()) return;

		if (GetElapsed() >= owner->GetFaintTime())
		{
			GetSM()->ChangeState(SphinxState::Wander);
		}
	}
};

// --- クールダウンステート ---
class CoolDownState : public State<SphinxState, Sphinx>
{
public:
	using State::State;

	void Update(Sphinx* owner, float deltaTime) override
	{
		// もし Sphinx 自体が一時停止中なら何もしない
		if (!owner->GetIsMoving()) return;

		auto* rw = owner->GetRandomWalk();
		auto* obj = owner->GetObject();


		// ランダム移動
		rw->Update(deltaTime, 2.0f); // kMoveChangeTime_
		Cygnus::Float3 moveDir = rw->GetRandomWalkDir();
		float targetAngle = std::atan2(moveDir.x, moveDir.z);
		obj->transform_.rotate_.y = rw->SmoothTurn(obj->transform_.rotate_.y, targetAngle, owner->GetWanderTurnSpeed(), deltaTime);

		owner->MoveForward(1.0f, deltaTime); // kMoveSpeed

		if (GetElapsed() >= owner->GetAttackCoolTime())
		{
			GetSM()->ChangeState(SphinxState::Wander);
		}
	}
};
