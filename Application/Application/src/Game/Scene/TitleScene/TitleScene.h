#pragma once
// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include <src/Game/OtherSceneObj/TitleSceneObject/TitleSceneUI/TitleSceneUI.h>
#include "../OtherSceneBase.h"

// =========================================================
// タイトルシーンクラス
// =========================================================
class TitleScene : public OtherSceneBase
{
public:
	// =========================================================
	// Public Methods
	// =========================================================
	void Initialize() override;
	void Finalize() override;
	void Update() override;

protected:
	// =========================================================
	// Draw Hooks (OtherSceneBaseのDrawから呼ばれる)
	// =========================================================
	void DrawUI() override { titleSceneUI_->Draw(); }
	void CheckSceneTransition() override;
	void Debug() override;

private:
	// =========================================================
	// Internal Methods
	// =========================================================
	void TransitionTitle();

private:
	// =========================================================
	// Member Variables
	// =========================================================
	// ※System変数やobjectGround_等はOtherSceneBaseへ移動したため削除

	// ----- Objects -----
	std::unique_ptr<TitleSceneUI> titleSceneUI_ = nullptr;
};