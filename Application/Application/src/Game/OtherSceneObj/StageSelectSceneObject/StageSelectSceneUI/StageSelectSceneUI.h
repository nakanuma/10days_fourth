#pragma once

// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include <Engine/Util/TimeManager.h>
#include <Input.h>
#include <SoundManager.h>
#include <Sprite.h>
#include <SpriteCommon.h>
#include <TextureManager.h>

// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------

class StageSelectSceneUI
{
public:
	/// <summary>初期化処理。スプライトの生成など。</summary>
	void Init(Cygnus::SpriteCommon* spriteCommon);
	/// <summary>更新処理。</summary>
	void Update();
	/// <summary>
	/// 描画処理（通常描画）。
	/// </summary>
	void Draw();

	bool IsStart() const { return isStart_; }

private:
	void StartButtonUpdate();

private:
	Cygnus::SpriteCommon* spriteCommon_ = nullptr;

	/// <summary>入力管理クラス。</summary>
	Cygnus::Input* input_ = nullptr;

	/// <summary>スプライト配列のインデックス定義。</summary>
	enum SelectUISprites
	{
		//SELECT_IMG,
		//SELECT_TEX,
		START_TEX,
		SpriteNum
	};

	struct UIState
	{
		std::unique_ptr<Cygnus::Sprite> sprite = nullptr;
		Cygnus::Float2 pos{ 0.0f, 0.0f };
		Cygnus::Float2 size{ 0.0f, 0.0f };
		float scale = 1.0f;
	};

	/// <summary>UIスプライト群。</summary>
	std::array<UIState, SpriteNum> sprites_;

	float alpha_ = 1.0f;

	float floating_ = 0.0f;

	bool isStart_ = false;

};