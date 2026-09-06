#pragma once

// C++
#include <array>

// Engine
#include <Sprite.h>
#include <SpriteCommon.h>

// 前方宣言
class Player;

// =========================================================
// プレイヤーのパーツ所持数UI
// =========================================================
class PartsInventoryUI
{
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 更新処理
	/// </summary>
	void Initialize(Cygnus::SpriteCommon* spriteCommon, Player* player);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// パーツの連続消費を開始
	/// </summary>
	void StartConsuming();

	/// <summary>
	/// 消費中かどうか
	/// </summary>
	/// <returns></returns>
	bool IsConsuming() const { return isConsuming_; }

private:
	// =========================================================
	// Structures
	// =========================================================

	// パーツごとのUIスロット
	struct PartSlot {
		std::unique_ptr<Cygnus::Sprite> icon; // パーツアイコン
		std::unique_ptr<Cygnus::Sprite> timesSymbol; // 「x」テクスチャ
		std::unique_ptr<Cygnus::Sprite> digitTens; // 十の位
		std::unique_ptr<Cygnus::Sprite> digitOnes; // 一の位

		// 元の描画サイズ保存用
		Cygnus::Float2 iconBaseSize = {0.0f, 0.0f};
		Cygnus::Float2 timesBaseSize = {0.0f, 0.0f};
		Cygnus::Float2 digitBaseSize = {0.0f, 0.0f};

		int32_t currentCount = -1; // 値変化検知用
		float popTimer = 0.0f; // ポップアップアニメーション用タイマー
	};

	enum PartType {
		Low = 0,
		Medium,
		High,
		Count
	};

	// =========================================================
	// Internal Methods
	// =========================================================

	/// <summary>
	/// スロットごとのスプライト更新処理
	/// </summary>
	/// <param name="slot"></param>
	/// <param name="count"></param>
	void UpdateSlot(PartSlot& slot, int32_t count);

	/// <summary>
	/// 数字スプライトのUV切り出し設定（0~9）
	/// </summary>
	/// <param name="sprite"></param>
	/// <param name="digit"></param>
	void SetDigitValue(Cygnus::Sprite* sprite, int32_t digit);

private:
	// =========================================================
	// Constants
	// =========================================================

	// 中心アンカーポイント
	static constexpr Cygnus::Float2 kAnchorPointCenter = { 0.5f, 0.5f };

	// 文字色
	static constexpr Cygnus::Float4 kNumberColor = {0.0f, 0.0f, 0.0f, 1.0f}; // 黒

	// スプライト初期位置
	static constexpr Cygnus::Float2 kBackgroundInitPos = { 640.0f, 650.0f }; // インベントリ背景初期位置

	// 各パーツスロットのX座標
	static constexpr std::array<float, PartType::Count> kSlotXPositions = {
		480.0f, // Low
		640.0f, // Medium
		800.0f // High
	};

	// スロット内部のオフセット（アイコン位置からの相対距離）
	static constexpr float kYPos = 650.0f;
	static constexpr float kOffsetX = -25.0f; // アイコンの位置
	static constexpr float kOffsetTimesX = 15.0f; // 「x」の位置
	static constexpr float kOffsetTensX = 32.5f; // 十の位の位置
	static constexpr float kOffsetOnesX = 50.0f; // 一の位の位置
	static constexpr float kOffsetNumbersY = 10.0f; // 文字郡のY軸オフセット

	// 数字テクスチャの設定
	static constexpr Cygnus::Float2 kDigitTextureSize = {64.0f, 64.0f}; // テクスチャ内の1文字のピクセルサイズ
	static constexpr Cygnus::Float2 kDigitDisplaySize = {32.0f, 32.0f}; // 画面に描画するサイズ

	// アニメーション用
	static constexpr float kPopDuration = 0.2f; // アニメーション時間
	static constexpr float kMaxScale = 1.3f; // 最大拡大倍率

	// 連続消費の間隔時間
	static constexpr float kConsumeInterval = 0.1f;

private:
	// =========================================================
	// Member Variables
	// =========================================================

	// プレイヤー参照
	Player* player_ = nullptr;

	// テクスチャハンドル
	uint32_t texNumbers_ = 0;

	// スプライト
	std::unique_ptr<Cygnus::Sprite> spriteBackground_; // インベントリ背景

	// 各品質のパーツスロット配列（低・中・高）
	std::array<PartSlot, PartType::Count> slots_;

	// 連続小費用パラメーター
	bool isConsuming_ = false;
	float consumeTimer_ = 0.0f;
	int32_t currentConsumeIndex_ = 0; // 現在減らしているパーツの種類
};

