#pragma once

// Engine
#include <Object3D.h>

// =========================================================
// 経路に沿って動くオブジェクトクラス（Todo : 設定が決まったら名前変更）
// =========================================================
class Carrier
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

	const float kMoveSpeed = 4.0f;	// 移動速度

	// =========================================================
	// Member Variables
	// =========================================================

	std::unique_ptr<Cygnus::Object3D> object_;	// オブジェクト

	bool isGoal_ = false;	// ゴール到達フラグ

	size_t targetIndex_ = 1;	// 次に向かうポイントの番号

	bool isActive_ = false;	// 有効化フラグ
};