#pragma once

// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include <Object3D.h>
#include <Collider/CollisionManager.h>

// =========================================================
// プレイヤークラス
// =========================================================
class Player : public Cygnus::ICollisionCallback
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
	/// <param name="other"></param>
	void OnCollision(Cygnus::Collider* other) override;

	/// <summary>
	/// setter_座標位置
	/// </summary>
	/// <param name="translate">座標を設定</param>
	void SetTranslate(const Cygnus::Float3& translate) { object_->transform_.translate_ = translate; }

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// キーボード用入力取得
	/// </summary>
	/// <returns></returns>
	Cygnus::Float3 GetKeyInput();

	/// <summary>
	/// ゲームパッド用入力取得
	/// </summary>
	/// <returns></returns>
	Cygnus::Float3 GetPadInput();

private:
	// =========================================================
	// Constants
	// =========================================================

	const float kMoveSpeed = 20.0f;	// 移動速度
	const Cygnus::Float3 kColliderSize = {1.0f, 2.0f, 1.0f};	// コライダーサイズ

	// =========================================================
	// Member Variables
	// =========================================================

	std::unique_ptr<Cygnus::Object3D> object_;	// オブジェクト
	std::unique_ptr<Cygnus::Collider> collider_;	// コライダー

	Cygnus::Float3 velocity_ = {0.0f, 0.0f, 0.0f};	// 速度ベクトル
};

