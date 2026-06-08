#pragma once
// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include "../../OtherSceneUIBase.h"

class TitleSceneUI : public OtherSceneUIBase
{
public:
	void Init(Cygnus::SpriteCommon* spriteCommon) override;

	void Update() override;

	bool IsStart() const { return IsTriggered(); }

private:
	enum PlayUISprites
	{
		TITLE_IMG,
		TITLE_TEX,
		START_TEX,
		SpriteNum
	};
};
