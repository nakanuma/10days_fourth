#pragma once
#include "Sprite.h"

/// <summary>
/// 酸素ゲージのUI
/// </summary>
class O2TimeUI {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="spriteCommon">スプライト描画</param>
	void Initialize(Cygnus::SpriteCommon* spriteCommon);

	/// <summary>
	/// 更新処理
	/// </summary>
	/// <param name="nowTime">現在の時間</param>
	/// <param name="maxTime">強制まきが入る制限時間</param>
	void Update(float nowTime, float maxTime);

	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();

private:

	static constexpr float kUIDefaultSize_ = 128.0f;// o2テクスチャの元サイズ
	static constexpr Cygnus::Float2 kUIPosition_ = { 20.0f,500.0f };// テクスチャ場所

	std::unique_ptr<Cygnus::Sprite> o2Max_;//酸素あり状態
	std::unique_ptr<Cygnus::Sprite> o2Null_;//酸素が空っぽ状態

};

