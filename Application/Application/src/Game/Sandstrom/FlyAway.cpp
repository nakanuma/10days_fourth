#include "FlyAway.h"
#include <algorithm>

void FlyAway::Update(Cygnus::Float3& translate) {

	//飛ばされるフラグがたったら
	if (isFlyAway_) {
		flyAwayY_ = 0.5f;//飛び上がる
	}
	else {
		flyAwayY_ = -1.0f;//落ちる
	}

	if (translate.y >= 100.0f) {
		isFlyAway_ = false;
	}	

	velocityY_ += flyAwayY_;
	velocityY_ = std::clamp(velocityY_, -1.0f, 10.0f);

	translate.y += velocityY_;
	translate.y = std::clamp(translate.y, 1.0f, 100.0f);
}

void FlyAway::InSandstorm() {
	isFlyAway_ = true;
}
