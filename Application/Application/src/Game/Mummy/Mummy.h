#pragma once
#include "Collider/Collider.h"
#include "Object3D.h"
#include "Float3.h"
#include <memory>

class Player;
/// <summary>
/// 敵[ミイラ]
/// プレイヤーを追っかける雑魚敵
/// </summary>
class Mummy : public Cygnus::ICollisionCallback {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize(const Cygnus::Float3& translate);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(const Cygnus::Float3& playerPos,float deltaTime);
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
	/// <summary>
	/// デバッグ
	/// </summary>
	void Debug();

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other"></param>
	void OnCollision(Cygnus::Collider* other) override;


	/// <summary>
	/// 死亡フラグ
	/// </summary>
	/// <returns>倒されているなら true</returns>
	bool IsDead() { return isDead_; }

private:

	const float kMoveSpeed_ = 2.0f; //移動速度

	std::unique_ptr<Cygnus::Object3D> object_;	// オブジェクト
	std::unique_ptr<Cygnus::Collider> collider_;	// コライダー

	Cygnus::Float3 velocity_ = { 0.0f, 0.0f, 0.0f };	// 速度ベクトル

	float lifeTime_ = 6.0f;//生存時間
	bool isDead_ = false;//死亡フラグ

	const Cygnus::Float3 kColliderSize_ = { 1,1,1 };
};

namespace MummyState {
	class BaseState {
	public:
		virtual void Update();
	};

	class SummonState : public BaseState {
	public:
		void Update() override;
	};

	class MoveState : public BaseState {
	public:
		void Update() override;
	};

}
