#pragma once

// C++
#include <vector>

// Engine
#include <Math/MyMath.h>

// =========================================================
// 移動オブジェクト用の経路管理クラス
// =========================================================
class PathManager
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns></returns>
	static PathManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// 指定したポイントの座標を取得
	/// </summary>
	/// <param name="index">ポイントの番号</param>
	/// <returns></returns>
	const Cygnus::Float3& GetPoint(size_t index) const { return points_[index]; }

	/// <summary>
	/// 登録されたポイントの総数を取得
	/// </summary>
	/// <returns></returns>
	size_t GetPointCount() const { return points_.size(); }

	/// <summary>
	/// �|�C���g���W��ǉ�
	/// </summary>
	/// <param name="point">�ǉ�������W</param>
	void AddPoint(const Cygnus::Float3& point) { points_.push_back(point); }

private:
	// =========================================================
	// Constants
	// =========================================================

	const Cygnus::Float4 kPathColor = {1.0f, 0.0f, 0.0f, 1.0f};	// 線の色

	// =========================================================
	// Member Variables
	// =========================================================

	std::vector<Cygnus::Float3> points_;	// 経路の頂点
};

