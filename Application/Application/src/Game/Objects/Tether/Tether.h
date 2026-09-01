#pragma once

// Engine
#include <MyMath.h>

// 前方宣言
class Spaceship;
class Player;

/// <summary>
/// 命綱のノード構造体
/// </summary>
struct TetherNode
{
	Cygnus::Float3 position;
	Cygnus::Float3 oldPosition;
	bool isLocked = false;
};

// =========================================================
// 命綱クラス
// =========================================================
class Tether
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="spaceship"></param>
	/// <param name="player"></param>
	void Initialize(Spaceship* spaceship, Player* player);

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

	// =========================================================
	// Accessor
	// =========================================================

	/// <summary>
	/// ノード一覧の取得
	/// </summary>
	/// <returns></returns>
	const std::vector<TetherNode>& GetNodes() const { return nodes_; }
	 
private:
	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// 距離拘束の計算
	/// </summary>
	void ApplyConstraints();

private:
	// =========================================================
	// Constants
	// =========================================================

	static constexpr size_t kNodeCount = 20; // ノードの個数
	static constexpr float kSegmentLength = 0.8f; // 各節の自然長
	static constexpr int kConstraintIterations = 5; // 拘束計算のループ回数（精度）
	static constexpr float kDamping = 0.98f; // 減衰率（空気抵抗）

	// =========================================================
	// Member Variables
	// =========================================================

	Spaceship* spaceship_ = nullptr;
	Player* player_ = nullptr;

	// ノード配列
	std::vector<TetherNode> nodes_;
};

