#pragma once
// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include "../OtherSceneBase.h"
#include <src/Game/OtherSceneObj/StageSelectSceneObject/StageSelectSceneUI/StageSelectSceneUI.h>
#include <src/Game/OtherSceneObj/StageSelectSceneObject/SelectObjectManager.h>
#include "src/Util/State/StateMachine.h"

enum class SelectSceneState
{
	FADE_IN,        /* フェードイン中 */
	SELECTING,      /* ステージ選択中 */
	MOVING,         /* ステージ移動中 */
	FADE_OUT,       /* フェードアウト中 */
};

// =========================================================
// ステージセレクトシーンクラス
// =========================================================
class StageSelectScene : public OtherSceneBase
{
public:
	// =========================================================
	// Public Methods
	// =========================================================
	void Initialize() override;
	void Finalize() override;
	void Update() override;

	bool IsStartSelected() const
	{
		return stageSelectSceneUI_->IsStart() && selectObjects_->GetCurrentStage() == 0;
	}

	Cygnus::Camera* GetCamera() const { return camera_.get(); }

	SelectObjectManager* GetSelectObjectManager() const { return selectObjects_.get(); }

protected:
	// =========================================================
	// Draw Hooks (OtherSceneBaseのDrawから呼ばれる)
	// =========================================================
	void DrawMainScene3D() override { selectObjects_->Draw(); }
	void DrawUI() override { stageSelectSceneUI_->Draw(); }
	void CheckSceneTransition() override;
	void Debug() override;

private:
	// =========================================================
	// Internal Methods
	// =========================================================
	void TransitionGamePlay();

private:
	// =========================================================
	// Member Variables
	// =========================================================

	// ----- Objects -----
	std::unique_ptr<SelectObjectManager> selectObjects_;

	// ----- Others -----
	std::unique_ptr<StageSelectSceneUI> stageSelectSceneUI_;       /* ステージセレクトシーンのUIクラス */

	// ----- State Machine -----
	// 追加：このシーンの状態を管理するステートマシン
	StateMachine<SelectSceneState, StageSelectScene> stateMachine_;

	static constexpr Cygnus::Float3 kCameraInitialPosition = { 0.0f, 80.0f, -45.0f }; /* カメラの初期位置 */
	Cygnus::Float3 cameraPosition_;    /* カメラの位置 */
};