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

#include <src/Game/StageEditor/StageEditor.h>

/// <summary>
/// フィールドイベントマネージャ
/// </summary>
class FieldEventManager {
public:
	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="spriteCommon">スプライト描画でいる</param>
	void Initialize(Cygnus::SpriteCommon* spriteCommon, const EventRatio& eventRatio);
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update(float deltaTime);
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

	void FieldSelect(std::unique_ptr<BaseFieldEvent> fieldEvent);

	//番号
	uint32_t number_ = 0; //現在のフィールド番号
	

	uint32_t ratioCount_ = 0;

	EventRatio eventRatio_;

	std::unique_ptr<BaseFieldEvent> fieldEvent_ = nullptr;

	Cygnus::SpriteCommon* spriteCommon_;

};

