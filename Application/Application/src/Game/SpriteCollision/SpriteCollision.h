#pragma once
#include <MyMath.h>
#include <2D/Sprite.h>

class SpriteCollision
{
public:
	/// <summary>
	/// 指定した座標がスプライトの矩形内にあるか判定する（回転未考慮）。
	/// </summary>
	/// <param name="spr">判定したい画像</param>
	/// <param name="point">判定したい座標</param>
	/// <returns>矩形内にあれば true</returns>
	bool IsPointInside(const Cygnus::Sprite& spr, const Cygnus::Float2& point = { 0.0f, 0.0f }) const;

	/// <summary>
	/// 現在のマウスカーソルがスプライト上に重なっているか判定する。
	/// </summary>
	/// <returns>マウスが重なっていれば true</returns>
	bool IsMouseHover(const Cygnus::Sprite& spr) const;

	/// <summary>
	/// スプライト上でマウスボタンがクリック（トリガー）されたか判定する。
	/// </summary>
	/// <param name="buttonNumber">0:左クリック, 1:右クリック, 2:中クリック</param>
	/// <returns>クリックされた瞬間なら true</returns>
	bool IsMouseClicked(const Cygnus::Sprite& spr, int32_t buttonNumber = 0) const;
};