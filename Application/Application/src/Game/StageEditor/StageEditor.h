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
#include <src/Game/Sphinx/Sphinx.h>

/// <summary>
/// 構造体_ゲームオブジェクトの設置
/// </summary>
struct GameObjectPosition {
	std::string name;
	Cygnus::Float3 position;
	Cygnus::AABBCollider collider;
};

struct EventRatio {
	int Anubis;
	int Seth;
	int LotOfOre;
	int SunGodRa;
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
	/// オブジェクトの設置
	/// </summary>
	/// <param name="gameObjectPos">プレイヤーをもらう</param>
	void SpitObjects(std::unique_ptr<Player>& player,std::unique_ptr<Sphinx>& sphinx);

	const EventRatio& GetEventRatio() { return eventRatio_; }

private:

	/// <summary>
	/// 配置の保存
	/// </summary>
	void Save();

	void Create();


	void SettingStage();


	std::list<GameObjectPosition> gameObjectPositions_;

	std::string objectName_ = "";

	std::string imGuiText_ = "";
	char textureFileName[1024 * 160] = "";

	GameObjectPosition newObject_;
	bool isCreateNewObject_ = false;
	
	Cygnus::Float3 colliderSize;


	EventRatio eventRatio_;
};

