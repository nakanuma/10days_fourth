#pragma once
// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include "../../OtherSceneUIBase.h"

class StageSelectSceneUI : public OtherSceneUIBase
{
public:
	void Init(Cygnus::SpriteCommon* spriteCommon) override;

	void Update() override;

	bool IsStart() const { return IsTriggered(); }

private:
	enum SelectUISprites
	{
		STAGE_1,
		STAGE_2,
		STAGE_3,
		STAGE_4,
		STAGE_5,
		START_TEX,
		SpriteNum
	};
};