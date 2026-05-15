#pragma once
// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include <memory>
#include "Object3D.h"

// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include "BaseFieldEvent.h"

/// <summary>
/// フィールドイベントマネージャ
/// </summary>
class FieldEventManager {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="spriteCommon">スプライト描画でいる</param>
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
	/// デバッグ
	/// </summary>
	void Debug();

private:

	/// <summary>
	/// フィールドを変更する
	/// </summary>
	void ChangeField();

	/// <summary>
	/// フィールドイベントたち
	/// </summary>
	enum FieldEvents {
		normal = 0,
		lotsOfOre,
		SunGodRa,
		max,
	};

	//番号
	uint32_t number_ = 0; //現在のフィールド番号
	uint32_t prevNum_ = 0;//前回のフィールド番号


	std::unique_ptr<BaseFieldEvent> fieldEvent_ = nullptr;

	Cygnus::SpriteCommon* spriteCommon_;

};

