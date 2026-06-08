#pragma once
#include "Collider/Collider.h"
#include "Object3D.h"
#include "Float3.h"
#include <memory>
#include "MummyBaseState.h"

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

	void OnDeadFlag() { isDead_ = true; }

	void Move(const Cygnus::Float3& move);

	void Rotate(const Cygnus::Float3 rotate);

	void CreateCollider();

	void DeleteCollider();

	Cygnus::Float3 GetPlayerMummyLength() { return playerAndMummyLength_; }

	void ChangeMummyState(std::unique_ptr<MummyState::BaseState> nextState);

private:

	std::unique_ptr<Cygnus::Object3D> object_;	// オブジェクト
	std::unique_ptr<Cygnus::Collider> collider_;	// コライダー

	std::unique_ptr<MummyState::BaseState> state_;

	Cygnus::Float3 playerAndMummyLength_;

	bool isDead_ = false;//死亡フラグ

	const Cygnus::Float3 kColliderSize_ = { 1,1,1 };
};
