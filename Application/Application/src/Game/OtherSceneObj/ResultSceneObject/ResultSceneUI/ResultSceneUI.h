#pragma once
// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include "../../OtherSceneUIBase.h"

class ResultSceneUI : public OtherSceneUIBase
{
public:
	void Init(Cygnus::SpriteCommon* spriteCommon) override;

	void Update() override;

	bool IsNext() const { return IsTriggered(); }

private:
	enum PlayUISprites
	{
		RESULT_IMG,
		RESULT_TEX,
		NEXT_TEX,
		SpriteNum
	};
};