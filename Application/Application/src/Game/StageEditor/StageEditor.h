#pragma once
// ---------------------------------------------------------
// Engine Includes
// ---------------------------------------------------------
#include "MyMath.h"
#include "Float3.h"

// ---------------------------------------------------------
// Application Includes
// ---------------------------------------------------------
#include "src/Game/Ore/Ore.h"
#include "src/Game/Player/Player.h"
#include "src/Game/Carrier/Carrier.h"

/// <summary>
/// 構造体_ゲームオブジェクトの設置
/// </summary>
struct GameObjectPosition {
	std::string name;
	Cygnus::Float3 position;
	Cygnus::AABBCollider collider;
};

/// <summary>
/// ステージエディタ
/// </summary>
class StageEditor
{
public:

	/// <summary>
	/// ステージ配置jsonを読み込む
	/// </summary>
	/// <param name="stageName">読み込みたいステージ</param>
	void LoadJsonFile(const std::string& stageName);

	/// <summary>
	/// 更新処理
	/// </summary>
	void Update();

	/// <summary>
	/// 
	/// </summary>
	/// <param name="gameObjectPos"></param>
	void SpitObjects(std::unique_ptr<Player>& player);

private:

	/// <summary>
	/// 配置の保存
	/// </summary>
	void Save();

	void Create();


	void SettingStage();


	std::vector<GameObjectPosition> gameObjectPositions_;

	std::string objectName_ = "";

	std::string imGuiText_ = "";
	char textureFileName[1024 * 160] = "";

	int objectNum_ = 0;

	GameObjectPosition newObject_;
	bool isCreateNewObject_ = false;
	
	Cygnus::Float3 colliderSize;
};

