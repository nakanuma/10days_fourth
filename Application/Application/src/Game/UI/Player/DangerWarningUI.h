#pragma once

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// 前方宣言
class Player;
class Tether;
class FlyingObjectManager;

// =========================================================
// 隕石衝突危険マークUI
// =========================================================
class DangerWarningUI {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="spriteCommon"></param>
	void Initialize(Cygnus::SpriteCommon* spriteCommon);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(Player* player, Tether* tether, FlyingObjectManager* flyingObjectManager);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:
	// =========================================================
	// Internal Methods
	// =========================================================

private:
	struct WarningItem {
		std::unique_ptr<Cygnus::Sprite> sprite;
		Cygnus::Float2 position = {0.0f, 0.0f};
		float alphaTimer = 0.0f; // アルファ用タイマー
		float blinkTimer = 0.0f; // 点滅用タイマー
		bool isTargeted = false; // 当該フレームで危険判定されたか
	};

	// =========================================================
	// Constants
	// =========================================================

	static constexpr size_t kMaxWarnings = 10; // スプライトの最大割り当て数
	static constexpr float kDangerDistance = 15.0f; // 危険判定を行う距離

	// アニメーション用定数
	static constexpr float kMinAlpha = 0.0f;
	static constexpr float kMaxAlpha = 0.4f;
	static constexpr float kFadeDuration = 0.3f; // フェードにかかる時間（秒）
	static constexpr float kBlinkSpeed = 5.0f; // 点滅速度（値が大きいほど高速）

	// =========================================================
	// Member Variables
	// =========================================================

	// スプライト
	std::vector<WarningItem> items_;
};
