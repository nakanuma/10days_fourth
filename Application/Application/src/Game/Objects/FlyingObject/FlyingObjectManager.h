#pragma once

// Application
#include <src/Game/Objects/FlyingObject/Base/FlyingObject.h>

// =========================================================
// 飛翔物の管理クラス
// =========================================================
class FlyingObjectManager {
public:
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
	/// デバッグ表示
	/// </summary>
	void Debug();

	/// <summary>
	/// 飛翔物のスポーン処理
	/// </summary>
	/// <typeparam name="T"></typeparam>
	/// <param name="position"></param>
	/// <param name="speed"></param>
	template<typename T> void Spawn(const Cygnus::Float3& position) { 
		auto newObj = std::make_unique<T>();
		newObj->Initialize(position);
		objects_.push_back(std::move(newObj));
	}

private:
	// 飛翔物の配列
	std::vector<std::unique_ptr<FlyingObject>> objects_;
};
