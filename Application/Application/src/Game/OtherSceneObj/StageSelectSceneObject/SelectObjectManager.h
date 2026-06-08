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
	

private:
	// =========================================================
	// Constants
	// =========================================================
	static constexpr int kStageNum = 5; // ステージの数

	static constexpr float kPointDistance = 15.0f;

	// =========================================================
	// Member Variables
	// =========================================================

	Cygnus::Input* input_ = nullptr;

	std::array<std::unique_ptr<Cygnus::Object3D>, kStageNum> selectObjects_;		// 選択オブジェクトの配列
	std::array<std::unique_ptr<Cygnus::Object3D>, kStageNum - 1> barObjects_;	// 中間オブジェクトの配列

	std::unique_ptr<Cygnus::Object3D> playerObject_;	// プレイヤーオブジェクト

	int currentStage_ = 0;
	int prevStage_ = 0;

	int dir_ = 0.0f;
};