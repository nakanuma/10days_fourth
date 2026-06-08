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
		START_TEX,
		SpriteNum
	};
};