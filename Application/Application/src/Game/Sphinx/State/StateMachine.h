#pragma once
#include <unordered_map>
#include <optional>
#include <memory>
#include <vector>
#include <string>
#include <cassert>
#include "State.h"

// Externals
#include <ImguiWrapper.h>

/// <summary>
/// 汎用ステートマシンクラス（テンプレート）。
/// 各ステートをクラス(State継承クラス)として定義し、インスタンスで管理する。
/// </summary>
/// <typeparam name="StateEnum">ステートを定義するEnum型。</typeparam>
/// <typeparam name="Owner">ステートマシンを所有するクラス(Player, Enemy等)。</typeparam>
template<typename StateEnum, typename Owner>
class StateMachine
{
public:
	using StateType = State<StateEnum, Owner>;

public: // メンバ関数

	/// <summary>
	/// ステートの登録。
	/// unique_ptrとして所有権を受け取り、内部のマップで管理する。
	/// </summary>
	/// <param name="key">ステートの識別子(Enum)</param>
	/// <param name="stateInstance">ステートクラスのインスタンス(unique_ptr)</param>
	/// <param name="name">ステート名（ImGui表示用。省略時は数値が入る）</param>
	void RegisterState(StateEnum key, std::unique_ptr<StateType> stateInstance, std::string name = "")
	{
		if (name.empty())
		{
			name = "State_" + std::to_string(static_cast<int>(key));
		}
		stateNames_[key] = name;
		stateMap_[key] = std::move(stateInstance);
		stateList_.push_back(key);
	}

	/// <summary>
	/// ステート変更リクエスト。
	/// 実際の切り替えは次回のUpdateState内で行われる。
	/// </summary>
	void ChangeState(StateEnum next)
	{
		if (stateRequest_ != next)
		{
			stateRequest_ = next;
		}
	}

	/// <summary>
	/// ステートの更新処理。
	/// リクエストがある場合は、Exit -> Init の順で切り替えを行う。
	/// </summary>
	/// <param name="owner">操作対象のインスタンス</param>
	/// <param name="deltaTime">経過時間</param>
	void UpdateState(Owner* owner, float deltaTime)
	{
		// ステート切り替え判定
		if (stateRequest_ && allowExit_)
		{
			StateEnum next = *stateRequest_;

			// 現在のステートの終了処理
			if (currStatePtr_)
			{
				currStatePtr_->Exit(owner);
			}

			prevState_ = currState_;
			currState_ = next;

			// 新しいステートへポインタを更新
			assert(stateMap_.contains(currState_) && "Registered state not found.");
			currStatePtr_ = stateMap_[currState_].get();

			stateRequest_ = std::nullopt;

			// 新しいステートの初期化
			if (currStatePtr_)
			{
				currStatePtr_->stateTimer_ = 0.0f; // タイマーリセット
				currStatePtr_->Init(owner);
			}
		}

		// カレントステートの更新処理
		if (currStatePtr_)
		{
			currStatePtr_->TimerUpdate(deltaTime);
			currStatePtr_->Update(owner, deltaTime);
		}
	}

	/// <summary>デバッグ用のImGui表示</summary>
	void DebugImGui(const char* labelPrefix = "StateMachine")
	{
#ifdef USE_IMGUI
		std::string currentName = stateNames_.contains(currState_) ? stateNames_[currState_] : "Unknown";
		ImGui::Text("%s: %s", labelPrefix, currentName.c_str());
		ImGui::Text("Elapsed: %.2f sec", GetStateElapsedTime());

		// ステート切り替え用 Combo
		if (!stateList_.empty())
		{
			if (ImGui::BeginCombo("Change State", currentName.c_str()))
			{
				for (auto key : stateList_)
				{
					bool isSelected = (key == currState_);
					if (ImGui::Selectable(stateNames_[key].c_str(), isSelected))
					{
						ChangeState(key);
					}
				}
				ImGui::EndCombo();
			}
		}
		ImGui::Text("AllowExit: %s", allowExit_ ? "true" : "false");
#endif
	}

	// 各種ゲッター
	StateEnum GetCurrentState() const { return currState_; }
	StateEnum GetPreviousState() const { return prevState_; }
	float GetStateElapsedTime() const { return currStatePtr_ ? currStatePtr_->GetElapsed() : 0.0f; }

	// ロック制御
	void LockState() { allowExit_ = false; }
	void UnlockState() { allowExit_ = true; }

private:
	/// <summary>現在のステートキー。</summary>
	StateEnum currState_{};
	/// <summary>前回のステートキー。</summary>
	StateEnum prevState_{};
	/// <summary>現在アクティブなステートクラスへのポインタ。</summary>
	StateType* currStatePtr_ = nullptr;

	/// <summary>変更リクエスト（次のステート）。</summary>
	std::optional<StateEnum> stateRequest_;
	/// <summary>ステート脱出許可フラグ。</summary>
	bool allowExit_ = true;

	/// <summary>ステート実体の管理マップ。</summary>
	std::unordered_map<StateEnum, std::unique_ptr<StateType>> stateMap_;
	/// <summary>登録されたステート一覧（ImGui用）。</summary>
	std::vector<StateEnum> stateList_;
	/// <summary>ステート名マップ（ImGui用）。</summary>
	std::unordered_map<StateEnum, std::string> stateNames_;
};