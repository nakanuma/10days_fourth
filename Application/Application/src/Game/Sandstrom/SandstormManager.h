#pragma once
#include "src//Game/Sandstrom/Sandstorm.h"

class SandstormManager {
public:
	static SandstormManager* GetInstance();

	/// <summary>
	/// 初期化処理
	/// </summary>
	void Initialize();
	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();
	/// <summary>
	/// 描画処理
	/// </summary>
	void Draw();
	/// <summary>
	/// デバッグ処理
	/// </summary>
	void Debug();

	/// <summary>
	/// 砂嵐の追加
	/// </summary>
	/// <param name="translate"></param>
	void AddSandstrom(const Cygnus::Float3& translate);

	/// <summary>
	/// 砂嵐リセット
	/// </summary>
	void Reset();

private:

	std::list<std::unique_ptr<Sandstorm>> sandStorms_;

};

