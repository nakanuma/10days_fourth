#pragma once
#include "Float3.h"

class FlyAway {
public:
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="translate">動かす座標</param>
	void Update(Cygnus::Float3& translate);

	/// <summary>
	/// 砂嵐に入ったら
	/// </summary>
	void InSandstorm();

private:

	bool isFlyAway_ = false;//飛ばされるフラグ
	float flyAwayY_ = 0.0f; //飛ばし方向Y
	float velocityY_ = 0.0f; //飛ばし方向Y


};

