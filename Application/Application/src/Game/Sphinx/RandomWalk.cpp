#include "RandomWalk.h"

// Engine
#include <RandomGenerator.h>

void RandomWalk::Update(float deltaTime, float changeTime)
{
	if (enable_)
	{
		if (walkTimer_ > 0.0f)
		{
			walkTimer_ -= deltaTime;
		}
		else
		{
			// XZ•½–Ê‚Å‚ÌˆÚ“®Žž•ûŒü‚ðƒ‰ƒ“ƒ_ƒ€‚ÉŒˆ’è
			randomDir_ = Cygnus::Float3::Normalize({ Cygnus::RandomGenerator::GetInstance()->RandomValue(-1.0f, 1.0f), 0.0f, Cygnus::RandomGenerator::GetInstance()->RandomValue(-1.0f, 1.0f) });
			walkTimer_ = changeTime;
		}
	}
}
