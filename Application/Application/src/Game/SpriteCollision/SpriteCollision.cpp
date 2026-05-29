#include "SpriteCollision.h"
#include <Input/Input.h>

using namespace Cygnus;

bool SpriteCollision::IsPointInside(const Sprite& spr, const Cygnus::Float2& point) const
{
	Float2 anchorPoint = spr.GetAnchorPoint();

	// アンカーポイントを基準にしたローカルの端座標を計算
	float baseLeft = 0.0f - anchorPoint.x;
	float baseRight = 1.0f - anchorPoint.x;
	float baseTop = 0.0f - anchorPoint.y;
	float baseBottom = 1.0f - anchorPoint.y;

	// フリップ（反転）状態の反映
	if (spr.IsFlipX())
	{
		baseLeft = -baseLeft;
		baseRight = -baseRight;
	}
	if (spr.IsFlipY())
	{
		baseTop = -baseTop;
		baseBottom = -baseBottom;
	}

	Float2 size = spr.GetSize();

	// 実際の幅と高さ
	float actualWidth = size.x;
	float actualHeight = size.y;

	Float2 position = spr.GetPosition();

	// actualWidth / actualHeight とワールド座標を乗算・加算して実際の画面座標を算出
	float actualLeft = position.x + baseLeft * actualWidth;
	float actualRight = position.x + baseRight * actualWidth;
	float actualTop = position.y + baseTop * actualHeight;
	float actualBottom = position.y + baseBottom * actualHeight;

	// フリップによって左右・上下が入れ替わっていても正しく判定できるよう、最小値と最大値を取る
	float minX = std::min<float>(actualLeft, actualRight);
	float maxX = std::max<float>(actualLeft, actualRight);
	float minY = std::min<float>(actualTop, actualBottom);
	float maxY = std::max<float>(actualTop, actualBottom);

	// 指定座標が矩形領域内にあるか判定
	if (point.x >= minX && point.x <= maxX &&
		point.y >= minY && point.y <= maxY)
	{
		return true;
	}

	return false;
}

bool SpriteCollision::IsMouseHover(const Cygnus::Sprite& spr) const
{
	POINT point = Cygnus::Input::GetInstance()->GetMousePosition();
	Float2 pos = { point.x, point.y };
	return IsPointInside(spr, pos);
}

bool SpriteCollision::IsMouseClicked(const Cygnus::Sprite& spr, int32_t buttonNumber) const
{
	// マウスが乗っている ＆＆ 指定したボタンが押された瞬間か
	auto input = Cygnus::Input::GetInstance();
	return IsMouseHover(spr) && input->IsTriggerMouse(buttonNumber);
}
