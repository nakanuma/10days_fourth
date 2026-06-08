#pragma once
// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include "../OtherSceneBase.h"
#include <src/Game/OtherSceneObj/ResultSceneObject/ResultSceneUI/ResultSceneUI.h>

// =========================================================
// リザルトシーンクラス
// =========================================================
class ResultScene : public OtherSceneBase // 継承元を変更
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
	void DrawUI() override { resultSceneUI_->Draw(); }
	void CheckSceneTransition() override;
	void Debug() override;

private:
	// =========================================================
	// Internal Methods
	// =========================================================
	void TransitionResult();

private:
	// =========================================================
	// Member Variables
	// =========================================================
	// ※System変数やobjectGround_等はOtherSceneBaseへ移動したため削除

	// ----- Others -----
	std::unique_ptr<ResultSceneUI> resultSceneUI_;                 /* リザルトシーンのUIクラス */
};