#include "GameResourceLoader.h"

// Engine
#include <ModelManager.h>
#include <Engine/ParticleEffect/ParticleEffectManager.h>
#include <SkyBoxManager.h>
#include <SoundManager.h>

// Application
#include <src/Game/Util/Transition/FadeTransition.h>

GameResourceLoader* GameResourceLoader::GetInstance() {
	static GameResourceLoader instance;
	return &instance;
}

void GameResourceLoader::Initialize() {
	// モデルデータの読み込み
	LoadAllModelData();
	// パーティクルの生成と登録
	RegisterAllParticleEffect();
	// サウンドデータの読み込み
	LoadAllSoundData();

	// SkyBoxの初期化
	Cygnus::SkyBoxManager::GetInstance()->Initialize("skybox.dds");

	// SpriteCommonの生成 + 初期化
	spriteCommon_ = std::make_unique<Cygnus::SpriteCommon>();
	spriteCommon_->Initialize(Cygnus::DirectXBase::GetInstance());
	// シーントランジション用フェードの初期化
	FadeTransition::GetInstance()->Initialize(spriteCommon_.get());
}

void GameResourceLoader::LoadAllModelData() {
	// ---------------------------------------------------------
	// プリミティブ
	// ---------------------------------------------------------

	// 板
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"Plane",
		"Primitive/Plane/plane.obj",
		"white.png"
	);

	// スフィア
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"Sphere",
		"Primitive/Sphere/sphere.obj",
		"white.png"
	);

	// キューブ
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"Cube",
		"Primitive/Cube/cube.obj",
		"white.png"
	);

	// ---------------------------------------------------------
	// ゲームオブジェクト
	// ---------------------------------------------------------

	// プレイヤー
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"Player",
		"Player/player.obj",
		"player.png"
	);

	// 宇宙船
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"Spaceship",
		"Spaceship/spaceship.obj",
		"spaceship.png"
	);

	// 隕石（小）
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"MeteorSmall", 
		"MeteorSmall/meteorSmall.obj", 
		"meteorSmall.png"
	);

	// 隕石（大）
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"MeteorLarge",
		"MeteorLarge/meteorLarge.obj",
		"meteorLarge.png"
	);

	// 修理パーツ（低品質）
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"RepairPartLow", 
		"RepairPartLow/repairpartLow.obj", 
		"repairpartLow.png"
	);

	// 修理パーツ（中品質）
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"RepairPartMedium",
		"RepairPartMedium/repairpartMedium.obj",
		"repairpartMedium.png"
	);

	// 修理パーツ（高品質）
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"RepairPartHigh",
		"RepairPartHigh/repairpartHigh.obj",
		"repairpartHigh.png"
	);
}

void GameResourceLoader::RegisterAllParticleEffect() {

}

void GameResourceLoader::LoadAllSoundData() {
	auto soundManager = Cygnus::SoundManager::GetInstance();
}