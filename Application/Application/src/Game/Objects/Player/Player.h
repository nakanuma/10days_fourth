#pragma once

// Engine
#include <Object3D.h>
#include <Collider/Collider.h>

// 前方宣言
class Spaceship;

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
	void Initialize(Spaceship* spaceship);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ表示
	/// </summary>
	void Debug();

	/// <summary>
	/// 命綱巻取りの実行
	/// </summary>
	void StartRewind();

	/// <summary>
	/// 衝突時コールバック
	/// </summary>
	/// <param name="other"></param>
	void OnCollision(Cygnus::Collider* other) override;

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// 現在位置の取得
	/// </summary>
	/// <returns></returns>
	const Cygnus::Float3& GetTranslate() { return object_->transform_.translate_; }

	/// <summary>
	/// ダメージ処理
	/// </summary>
	/// <param name="damage"></param>
	void ApplyDamage(int32_t damage = 1);

	/// <summary>
	/// 残りHPの取得
	/// </summary>
	/// <returns></returns>
	int32_t GetHP() const { return hp_; }

	/// <summary>
	/// 死亡フラグ取得
	/// </summary>
	/// <returns></returns>
	bool IsDead() const { return isDead_; }

	/// <summary>
	/// 巻取り状態の取得
	/// </summary>
	/// <returns></returns>
	bool IsRewinding() const { return isRewinding_; }

	/// <summary>
	/// 各修理パーツの取得数を取得
	/// </summary>
	/// <returns></returns>
	int32_t GetRepairPartLowCount() const { return repairPartLowCount_; }
	int32_t GetRepairPartMediumCount() const { return repairPartMediumCount_; }
	int32_t GetRepairPartHighCount() const { return repairPartHighCount_; }

private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// キー入力による移動処理
	/// </summary>
	void Move();

	/// <summary>
	/// 無入力時の漂うオフセットを取得
	/// </summary>
	Cygnus::Float3 Drift();

	/// <summary>
	/// 巻取り時処理
	/// </summary>
	void ProcessRewind();

	/// <summary>
	/// 移動制限エリアの描画
	/// </summary>
	void DrawAreaLimit();

private:
	// =========================================================
	// Constants
	// =========================================================

	// コライダーの大きさ
	static constexpr Cygnus::Float3 kColliderSize = { 1.0f, 2.0f, 1.0f };

	// 宇宙空間移動用のパラメーター
	static constexpr float kAcceleration = 0.01f; // 加速度（入力時の増分）
	static constexpr float kMaxSpeed = 0.25f; // 最高速度
	static constexpr float kDamping = 0.985f; // 減衰率（1.0に近いほど止まりにくくなる）

	// 漂いパラメーター
	static constexpr float kDriftFrequency = 3.0f; // 漂う周期の速さ
	static constexpr float kDriftAmplitude = 0.01f; // 漂う力の強さ
	
	// 移動範囲の初期定数
	static constexpr float kDefaultLimitMinY = -60.0f;
	static constexpr float kDefaultLimitMaxY = 0.0f;
	static constexpr float kDefaultLimitX = 25.0f;

	// 巻取り用パラメーター
	static constexpr float kDefaultAutoRewindTime = 10.0f; // 自動巻き取りまでの限界時間（秒）
	static constexpr float kDefaultRewindAccel = 0.08f; // 巻取り時の加速度
	static constexpr float kDefaultRewindMaxSpeed = 0.75f; // 巻取り時の最高速度
	static constexpr float kRewindStopDistance = 2.0f; // 宇宙船にこの距離まで近づいたら終了

	// その他パラメーター
	static constexpr int32_t kMaxHP = 5; // 最大HP

	// =========================================================
	// Member Variables
	// =========================================================

	// オブジェクト
	std::unique_ptr<Cygnus::Object3D> object_;

	// コライダー
	std::unique_ptr<Cygnus::Collider> collider_;

	// 宇宙船へのポインタ
	Spaceship* spaceship_ = nullptr; 

	// 速度ベクトル
	Cygnus::Float3 velocity_ = {0.0f, 0.0f, 0.0f};

	// 漂い計算用タイマー
	float driftTimer_ = 0.0f;

	// 巻取り管理用
	bool isRewinding_ = false; // 巻取り中フラグ
	float autoRewindTimer_ = 0.0f; // 滞在時間カウント用タイマー

	// 修理パーツ管理用
	int32_t repairPartLowCount_ = 0;
	int32_t repairPartMediumCount_ = 0;
	int32_t repairPartHighCount_ = 0;

	// 残りHP
	int32_t hp_ = kMaxHP;

	// 死亡フラグ
	bool isDead_ = false;
};