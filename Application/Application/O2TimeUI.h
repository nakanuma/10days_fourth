#pragma once
#include "Sprite.h"

/// <summary>
/// 
/// </summary>
class O2TimeUI {
public:
	/// <summary>
	/// ‰Šú‰»ˆ—
	/// </summary>
	/// <param name="spriteCommon">ƒXƒvƒ‰ƒCƒg•`‰æ</param>
	void Initialize(Cygnus::SpriteCommon* spriteCommon);

	/// <summary>
	/// XVˆ—
	/// </summary>
	/// <param name="nowTime">Œ»İ‚ÌŠÔ</param>
	/// <param name="maxTime">§ŒÀŠÔ</param>
	void Update(float nowTime, float maxTime);

	/// <summary>
	/// •`‰æˆ—
	/// </summary>
	void Draw();

private:

	static constexpr float kUIDefaultSize_ = 128.0f;

	std::unique_ptr<Cygnus::Sprite> o2Meter_;//_‘f‚ ‚èó‘Ô
	std::unique_ptr<Cygnus::Sprite> o2Null_;//_‘f‚ª‹óó‘Ô

};

