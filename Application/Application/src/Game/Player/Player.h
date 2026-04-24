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

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// 鉱石を拾えるか（所持最大数に達していないか）取得
	/// </summary>
	/// <returns></returns>
	bool CanPickUpOre() const { return oreCount_ < kMaxOreCount; }

	/// <summary>
	/// プレイヤーの所持鉱石を1増やす（外部から呼ぶ用）
	/// </summary>
	void AddOreCount() { oreCount_++; }

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

	const float kMiningOffset = 1.5f;	// 採掘時の前方オフセット
	const float kMiningRange = 1.2f;	// 採掘時のブレ許容値

	const uint32_t kMaxOreCount = 3;	// 持てる鉱石の最大数

	// =========================================================
	// Member Variables
	// =========================================================

	std::unique_ptr<Cygnus::Object3D> object_;	// オブジェクト
	std::unique_ptr<Cygnus::Collider> collider_;	// コライダー

	Cygnus::Float3 velocity_ = {0.0f, 0.0f, 0.0f};	// 速度ベクトル

	uint32_t oreCount_ = 0;	// 現在の所持鉱石数
};

