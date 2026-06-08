#pragma once

#include "StageSelectScene.h"
#include "src/Util/State/StateMachine.h"
#include "src/Util/State/State.h"

// --- フェードインステート ---
class FadeInState : public State<SelectSceneState, StageSelectScene>
{
public:
	using State::State;

	void Init(StageSelectScene& owner) override
	{
	}

	void Update(StageSelectScene& owner, float deltaTime) override
	{
		if (true)
		{
			GetSM()->ChangeState(SelectSceneState::SELECTING);
			return;
		}
	}
};

// --- セレクトステート ---
class SelectingState : public State<SelectSceneState, StageSelectScene>
{
public:
	using State::State;

	void Update(StageSelectScene& owner, float deltaTime) override
	{
		if(owner.GetSelectObjectManager()->SelectStage())
		{
			GetSM()->ChangeState(SelectSceneState::MOVING);
			return;
		}
		if (owner.IsStartSelected())
		{
			GetSM()->ChangeState(SelectSceneState::FADE_OUT);
		}
	}
};

// --- カメラ移動ステート ---
class MovingState : public State<SelectSceneState, StageSelectScene>
{
public:
	using State::State;

	void Init(StageSelectScene& owner) override
	{
		isMoving_ = true; // カメラ移動開始
		startX_ = owner.GetCamera()->transform_.translate_.x;
		goalX_ = startX_ + (owner.GetSelectObjectManager()->GetPointDistance() * owner.GetSelectObjectManager()->GetDir());
	}

	void Update(StageSelectScene& owner, float deltaTime) override
	{
		if(!isMoving_)
		{
			// カメラ移動が完全に終わったら、親クラスの遷移フラグを立てる
			GetSM()->ChangeState(SelectSceneState::SELECTING);
			return;
		}
		if(isMoving_)
		{
			owner.GetCamera()->transform_.translate_.x = 
				startX_ < goalX_ ? 
				std::clamp(std::lerp(startX_, goalX_, GetElapsed() / movingTime_), startX_, goalX_) : 
				std::clamp(std::lerp(startX_, goalX_, GetElapsed() / movingTime_), goalX_, startX_);
			

			if(GetElapsed() >= movingTime_)
			{
				isMoving_ = false; // カメラ移動完了
				owner.GetSelectObjectManager()->ResetDir();
				owner.GetSelectObjectManager()->Front();
			}
		}
	}

private:
	bool isMoving_ = false; // カメラが移動中かどうかのフラグ
	float movingTime_ = 0.5f;
	float startX_ = 0.0f;
	float goalX_ = 0.0f;
};

// --- フェードアウトステート ---
class FadeOutState : public State<SelectSceneState, StageSelectScene>
{
public:
	using State::State;

	void Update(StageSelectScene& owner, float deltaTime) override
	{
		if (true /* フェードアウト完了条件 */)
		{
			// フェードが完全に終わったら、親クラスの遷移フラグを立てる
			owner.SetTransition(true);
		}
	}
};