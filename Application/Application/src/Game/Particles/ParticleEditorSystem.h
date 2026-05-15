#pragma once

//c++
#include <string>
#include <cstdint>
#include <vector>

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
	/// 描画
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

};

