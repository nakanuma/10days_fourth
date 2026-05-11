#pragma once

// Engine
#include <Object3D.h>
#include <Collider/CollisionManager.h>

// =========================================================
// 経路に沿って動くオブジェクトクラス
// =========================================================
class Carrier : public Cygnus::ICollisionCallback
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(float deltaTime);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Debug();

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="collider"></param>
	void OnCollision(Cygnus::Collider* collider) override;
	
	/// <summary>
	/// setter_���W�ʒu
	/// </summary>
	/// <param name="translate">���W��ݒ�</param>
	void SetTranslate(const Cygnus::Float3& translate) { object_->transform_.translate_ = translate; }

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// 経路に沿った移動処理
	/// </summary>
	/// <param name="deltaTime"></param>
	void MoveAlongPath(float deltaTime);

private:
	// =========================================================
	// Constants
	// =========================================================

	const float kMoveSpeed = 2.0f;	// 移動速度
	const Cygnus::Float3 kColliderSize = {1.0f, 1.0f, 1.0f};	// コライダーサイズ

	// =========================================================
	// Member Variables
	// =========================================================

	std::unique_ptr<Cygnus::Object3D> object_;	// オブジェクト
	std::unique_ptr<Cygnus::Collider> collider_;	// コライダー

	bool isGoal_ = false;	// ゴール到達フラグ

	size_t targetIndex_ = 1;	// 次に向かうポイントの番号

	bool isActive_ = false;	// 有効化フラグ
};