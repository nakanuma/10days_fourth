#pragma once

//c++
#include <string>
#include <cstdint>
#include <vector>

//Engine
#include <Object3D.h>

/// <summary>
/// パーティクルエディターシステムクラス
/// </summary>
class ParticleEditorSystem {
public:

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新
	/// </summary>
	void Update();
	/// <summary>
	/// オブジェクト描画
	/// </summary>
	void Draw();
	/// <summary>
	/// デバッグ
	/// </summary>
	void Debug();

private:
	//制御対象
	std::string targetEffectName_ = "template";
	//保存対象
	std::string saveFileName_ = "template";

	//エミットカウント
	uint32_t count_ = 10;

	//ファイルリスト
	std::vector<std::string> files;

	//無限エミットの有無
	bool isInfiniteEmit_ = false;

	//暗色背景用モデル
	std::unique_ptr<Cygnus::Object3D> object_;
	bool isDrawObject_ = false;

};

