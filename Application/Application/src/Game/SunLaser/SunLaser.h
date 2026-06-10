#pragma once
#include "src/Game/Player/Player.h"
#include "Collider/Collider.h"

/// <summary>
/// レーザー状態遷移(基盤クラス)
/// </summary>
class BaseLaserState {
public:
	/// <summary>
	/// 更新処理
	/// </summary>
	virtual void Update(float dt, Cygnus::Float3& position) = 0;

	/// <summary>
	/// ステート変更
	/// </summary>
	/// <param name="state_">現在のステート</param>
	std::unique_ptr<BaseLaserState> ChangeState();

	const Cygnus::Float4 GetColor() { return areaColor_; }

	/// <summary>
	/// ステートを変更するフラグ
	/// </summary>
	/// <returns>次のステートがnullでないならtrue</returns>
	bool IsChangeState() {
		if (nextState_) {
			return true;
		}
		return false;
	}

protected:
	//共通時間
	float laserTime_ = 0.0f;
	//次のステート
	std::unique_ptr<BaseLaserState> nextState_;
	
	Cygnus::Float4 areaColor_;
};

/// <summary>
/// プレイヤーに追尾する
/// </summary>
class LaserHomingState : public BaseLaserState {
public:
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(float dt, Cygnus::Float3& position) override;
private:
	//時間
	float kHomingTime_ = 3.0f;//追尾時間
	const Cygnus::Float4 kAreaColor_ = { 1,1,1,1 };//色
};

/// <summary>
/// レーザー発射準備
/// </summary>
class LaserReserveState : public BaseLaserState {
public:
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(float dt, Cygnus::Float3& position) override;
private:
	//時間
	float kReserveTime_ = 2.5f;//放出準備時間
	const Cygnus::Float4 kAreaColor_ = { 0.8f,0.8f,0.1f,1.0f };//色
};

/// <summary>
/// レーザー放出
/// </summary>
class LaserReleaseState : public BaseLaserState , public Cygnus::ICollisionCallback{
public:
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(float dt, Cygnus::Float3& position) override;
private:
	//時間
	float kReleaseLaserTime_ = 1.0f;//放出時間
	float kFinishLaserTime_ = 0.5f;//放出時間

	const Cygnus::Float4 kAreaColor_ = { 1.0f,0.1f,0.1f,1.0f };//色

	const float kRadius_ = 10.0f;

	//当たり判定(円)
	std::unique_ptr<Cygnus::SphereCollider> sphereCollider_;
};


/// <summary>
/// 太陽レーザー
/// </summary>
class SunLaser {
public:

	static SunLaser* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="player">プレイヤーを狙う</param>
	/// <param name="deltaTime">時間</param>
	void Update(const Cygnus::Float3& playerPosition, float deltaTime);
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	void IsEventStart() { isEventStart_ = true; }

private:
	/// <summary>
	/// ステートパターンの処理
	/// </summary>
	/// <param name="dt"></param>
	void StateUpdate(float dt);
	/// <summary>
	/// レーザーの場所ぎめ
	/// </summary>
	/// <param name="position">位置を設定</param>
	void SetPosition(const Cygnus::Float3& position) {
		object_->transform_.translate_ = { position.x, 1.0f, position.z };
	}

	std::unique_ptr<Cygnus::Object3D> object_;

	//状態ステート
	std::unique_ptr<BaseLaserState> state_;

	uint32_t changeCount_ = 0;
	const uint32_t kChange_ = 2;

	Cygnus::Float3 position_;//固定ポイント

	bool isEventStart_ = false;
};
