#pragma once

#include <Engine/Util/TimeManager.h>
#include <Input.h>
#include <SoundManager.h>
#include <Sprite.h>
#include <SpriteCommon.h>
#include <TextureManager.h>
#include <vector>
#include <memory>

// すべてのシーンUIで共通使用するステータス構造体
struct UIState
{
	std::unique_ptr<Cygnus::Sprite> sprite = nullptr;
	Cygnus::Float2 pos{ 0.0f, 0.0f };
	Cygnus::Float2 size{ 0.0f, 0.0f };
	float scale = 1.0f;
};

class OtherSceneUIBase
{
public:
	virtual ~OtherSceneUIBase() = default;

	/// <summary>初期化処理。各種ポインタの取得など。</summary>
	virtual void Init(Cygnus::SpriteCommon* spriteCommon);

	/// <summary>更新処理。スプライト自体のUpdateを回します。</summary>
	virtual void Update();

	/// <summary>描画処理。スプライトのDrawを回します。</summary>
	virtual void Draw();

	/// <summary>遷移ボタンが押されたかどうか</summary>
	bool IsTriggered() const { return isTriggered_; }

protected:
	/// <summary>スタートボタン等のクリック・ホバー（拡大）処理を共通化</summary>
	/// <param name="buttonIndex">配列内のボタンのインデックス</param>
	void UpdateButtonLogic(size_t buttonIndex);

	void ResetTrigger() { isTriggered_ = false; }

protected:
	Cygnus::SpriteCommon* spriteCommon_ = nullptr;
	Cygnus::Input* input_ = nullptr;

	// 各シーンで必要な数だけ確保できるよう vector に変更
	std::vector<UIState> sprites_;

	float alpha_ = 1.0f;
	float floating_ = 0.0f;

	// isStart_ や isNext_ をこの変数に統合
	bool isTriggered_ = false;
};