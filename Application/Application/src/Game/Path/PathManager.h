#pragma once

// C++
#include <vector>

// Engine
#include <Math/MyMath.h>

// =========================================================
// 遘ｻ蜍輔が繝悶ず繧ｧ繧ｯ繝育畑縺ｮ邨瑚ｷｯ邂｡逅・け繝ｩ繧ｹ
// =========================================================
class PathManager
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 繧､繝ｳ繧ｹ繧ｿ繝ｳ繧ｹ蜿門ｾ・
	/// </summary>
	/// <returns></returns>
	static PathManager* GetInstance();

	/// <summary>
	/// 蛻晄悄蛹門・逅・
	/// </summary>
	void Initialize();

	/// <summary>
	/// 謠冗判蜃ｦ逅・
	/// </summary>
	void Draw();

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// 謖・ｮ壹＠縺溘・繧､繝ｳ繝医・蠎ｧ讓吶ｒ蜿門ｾ・
	/// </summary>
	/// <param name="index">繝昴う繝ｳ繝医・逡ｪ蜿ｷ</param>
	/// <returns></returns>
	const Cygnus::Float3& GetPoint(size_t index) const { return points_[index]; }

	/// <summary>
	/// 逋ｻ骭ｲ縺輔ｌ縺溘・繧､繝ｳ繝医・邱乗焚繧貞叙蠕・
	/// </summary>
	/// <returns></returns>
	size_t GetPointCount() const { return points_.size(); }

	/// <summary>
	/// ポイント座標を追加
	/// </summary>
	/// <param name="point">追加する座標</param>
	void AddPoint(const Cygnus::Float3& point) { points_.push_back(point); }

private:
	// =========================================================
	// Constants
	// =========================================================

	const Cygnus::Float4 kPathColor = {1.0f, 0.0f, 0.0f, 1.0f};	// 邱壹・濶ｲ

	// =========================================================
	// Member Variables
	// =========================================================

	std::vector<Cygnus::Float3> points_;	// 邨瑚ｷｯ縺ｮ鬆らせ
};

