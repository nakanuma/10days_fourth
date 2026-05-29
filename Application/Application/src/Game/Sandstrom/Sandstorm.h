#pragma once
#include <memory>
#include "Object3D.h"
#include "Collider/Collider.h"

class Sandstorm : public Cygnus::ICollisionCallback {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="translate">初期位置</param>
	void Initialize(const Cygnus::Float3& translate);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
	/// <summary>
	/// デバッグ処理
	/// </summary>
	void Debug();
	/// <summary>
	/// 解放処理
	/// </summary>
	void Finalize();

private:
	std::unique_ptr<Cygnus::Collider> collider_;//当たり判定(本体)
	Cygnus::Transform transform_;

	const Cygnus::Float3 kColliderSize_ = { 5,45,5 };//飛ばす範囲

	const float kMoveSpeed_ = 0.1f;

	//曲線
	Cygnus::Float3 startPoint_ = { 0,0,0 };
	Cygnus::Float3 carvePoint_ = { 0,0,0 };
	Cygnus::Float3 endPoint_ = { 0,0,0 };

	float Time_ = 0.0f;

};

