#include "RandomWalk.h"
#include <numbers>

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

float RandomWalk::SmoothTurn(float currentAngle, float targetAngle, float turnSpeed, float deltaTime)
{
	float diff = targetAngle - currentAngle;
	const float PI = std::numbers::pi_v<float>;

	// -PI ? PI ‚Ì”ÍˆÍ‚É³‹K‰»iÅ’Z‹——£‚Å‰ñ‚é‚æ‚¤‚É‚·‚éj
	while (diff > PI)  diff -= 2.0f * PI;
	while (diff < -PI) diff += 2.0f * PI;

	return currentAngle + diff * turnSpeed * deltaTime;
}
