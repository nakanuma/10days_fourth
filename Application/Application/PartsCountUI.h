#pragma once
#include "2D/Sprite.h"

/// <summary>
/// パーツを数えるUI
/// </summary>
class PartsCountUI {
public:
	/// <summary>
	/// 初期化処理
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
	/// パーツの加算
	/// </summary>
	void AddParts();

	/// <summary>
	/// パーツ解放処理
	/// </summary>
	void ReleaseParts();

private:

	static constexpr float kUISize_ = 96.0f;
	static constexpr float kUISpace_ = 20.0f;

	static constexpr uint32_t kTenPlace_ = 10;

	uint32_t partsCounter_ = 0;
	uint32_t prevPartsCount_ = 0;

	std::unique_ptr<Cygnus::Sprite> spriteCountTenUI_;//カウント十の位
	std::unique_ptr<Cygnus::Sprite> spriteCountOneUI_;//カウント一の位

};

