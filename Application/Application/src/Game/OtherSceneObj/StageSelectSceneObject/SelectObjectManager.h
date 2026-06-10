#pragma once

#include <Object3D.h>
#include <Input.h>
#include <numbers>

class SelectObjectManager
{
public:
	/// <summary>
	/// 初期化処理を行います。
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新処理を行います。
	/// </summary>
	/// <param name="deltaTime">経過時間</param>
	void Update(float deltaTime);

	/// <summary>
	/// 描画処理を行います。
	/// </summary>
	void Draw();

	/// <summary>
	/// デバッグ用表示処理を行います。
	/// </summary>
	void Debug();

	/// <summary>
	/// ステージへの飛び込み演出を開始します。
	/// 決定ボタンが押された瞬間に1度だけ呼び出してください。
	/// </summary>
	void StartJumpToStage();

	/// <summary>
	/// 飛び込み演出の更新を行います。
	/// </summary>
	/// <param name="deltaTime">経過時間</param>
	/// <returns>演出が完了したら true を返します</returns>
	bool UpdateJumpAnimation();

	/// <summary>
	/// 現在飛び込み演出中かどうかを取得します。
	/// </summary>
	bool IsJumping() const { return isJumping_; }

	/// <summary>
	/// ステージが踏まれたリアクション演出を開始します。
	/// </summary>
	void StartStageReaction();

	/// <summary>
	/// ステージリアクション演出の更新を行います。
	/// </summary>
	/// <param name="deltaTime">経過時間</param>
	/// <returns>演出が完了したら true を返します</returns>
	bool UpdateStageReaction();

	/// <summary>
	/// 現在ステージリアクション演出中かどうかを取得します。
	/// </summary>
	bool IsStageReacting() const { return isStageReacting_; }

	// --- Getter / Setter ---

	int GetCurrentStage() { return currentStage_; }

	void SetPlayerObjectPosX(const float posX) { playerObject_->transform_.translate_.x = posX; }

	bool SelectStage();

	int GetDir() { return dir_; }

	void ResetDir() { dir_ = 0.0f; }

	float GetPointDistance() { return kPointDistance; }

	void Right() { playerObject_->transform_.rotate_.y = -std::numbers::pi_v<float> / 2.0f; };
	void Left() { playerObject_->transform_.rotate_.y = std::numbers::pi_v<float> / 2.0f; };
	void Front() { playerObject_->transform_.rotate_.y = 0.0f; };

private:
	void SwapModel();

	void FloatingObj();
	

private:
	// =========================================================
	// Constants
	// =========================================================
	static constexpr int kStageNum = 5; // ステージの数

	static constexpr float kPointDistance = 15.0f;

	static constexpr float kFloatingTime_ = 3.0f;

	static constexpr float kJumpDuration_ = 2.0f; // 飛び込みにかかる時間（秒）
	static constexpr float kJumpHeight_ = 5.0f;   // 描く曲線の高さ（弧の高さ）

	static constexpr float kReactionDuration_ = 0.6f; // 演出全体の時間（秒）
	static constexpr float kSquashFactor_ = 0.5f;     // Y軸が最大でどれだけ潰れるか (0.5 = 50%潰れる)
	static constexpr float kStretchFactor_ = 0.3f;    // XZ平面が最大でどれだけ広がるか (0.3 = 30%広がる)

	// =========================================================
	// Member Variables
	// =========================================================

	Cygnus::Input* input_ = nullptr;

	float deltaTime_;

	std::array<std::unique_ptr<Cygnus::Object3D>, kStageNum> selectObjects_;		// 選択オブジェクトの配列
	std::array<std::unique_ptr<Cygnus::Object3D>, kStageNum - 1> barObjects_;	// 中間オブジェクトの配列

	std::array<std::unique_ptr<Cygnus::Object3D>, kStageNum> stageObjects_;		// ステージの見た目オブジェクトの配列

	std::unique_ptr<Cygnus::Object3D> playerObject_;	// プレイヤーオブジェクト

	int currentStage_ = 0;
	int prevStage_ = 0;

	int dir_ = 0.0f;

	float floatingTimer_ = 0.0f;


	bool isJumping_ = false;
	float jumpTimer_ = 0.0f;

	Cygnus::Float3 jumpStartPos_{ 0.0f, 0.0f, 0.0f };
	Cygnus::Float3 jumpEndPos_{ 0.0f, 0.0f, 0.0f };
	Cygnus::Float3 jumpScaleF3_{ 0.0f, 0.0f, 0.0f };


	bool isStageReacting_ = false;
	float stageReactTimer_ = 0.0f;

	// 各ステージの初期スケールを保存する配列
	std::array<Cygnus::Float3, kStageNum> stageInitialScales_;
};