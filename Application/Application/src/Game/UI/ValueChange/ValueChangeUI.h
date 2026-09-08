#pragma once

// C++
#include <vector>

// Engine
#include <Float2.h>
#include <Float4.h>
#include <Sprite.h>
#include <SpriteCommon.h>

// 数字UI一塊の構造体
struct DigitUI {
	std::unique_ptr<Cygnus::Sprite> spriteNumber_One_;
	std::unique_ptr<Cygnus::Sprite> spriteNumber_Ten_;
	std::unique_ptr<Cygnus::Sprite> spritePlus_;
	std::unique_ptr<Cygnus::Sprite> spriteMinus_;
	float timer;
	Cygnus::Float4 color;
};

// =========================================================
// 体力変動数字UI
// =========================================================
class ValueChangeUI {
public:
	// =========================================================
	// Public Methods
	// =========================================================

	/// <summary>
	/// 更新処理
	/// </summary>
	void Initialize(Cygnus::SpriteCommon* spriteCommon);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

	/// <summary>
	/// 数字UIの登録
	/// </summary>
	/// <param name="subjectPos">対象の座標</param>
	/// <param name="value">出力したい値</param>
	void RegistDigitValue(Cygnus::Float3 subjectPos,int value,Cygnus::Float4 color);

private:
	// =========================================================
	// Internal Methods
	// =========================================================


	



private:
	// =========================================================
	// Constants
	// =========================================================

	static constexpr float displayDuration = 1.0f; // 表示時間（秒）
	static constexpr float upwardSpeed = -75.0f; // 上昇速度（ピクセル/秒）



private:
	// =========================================================
	// Member Variables
	// =========================================================

	//スプライトコモン
	Cygnus::SpriteCommon* spriteCommon_ = nullptr;

	//テクスチャハンドル
	uint32_t texNumbers_ = 0;
	uint32_t texPlus_ = 0;
	uint32_t texMinus_ = 0;

	// 数字UIのリスト
	std::vector<DigitUI> digitUIs_;


};

