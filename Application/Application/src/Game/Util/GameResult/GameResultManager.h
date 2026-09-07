#pragma once

enum class GameResult {
	Clear,
	GameOver
};

// =========================================================
// ゲーム結果受け渡し用クラス
// =========================================================
class GameResultManager {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// ゲーム結果を設定
	/// </summary>
	/// <param name="result"></param>
	static void SetResult(GameResult result) { result_ = result; }

	/// <summary>
	/// ゲーム結果を返す
	/// </summary>
	/// <returns></returns>
	static GameResult GetResult() { return result_; }

	/// <summary>
	/// クリアしているかどうかを取得
	/// </summary>
	/// <returns></returns>
	static bool IsClear() { return result_ == GameResult::Clear; }

private:
	// =========================================================
	// Member Variables
	// =========================================================

	// ゲーム結果
	inline static GameResult result_ = GameResult::GameOver;
};
