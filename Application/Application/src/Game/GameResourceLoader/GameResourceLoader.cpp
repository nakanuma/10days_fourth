#include "GameResourceLoader.h"

// Engine
#include <ModelManager.h>
#include <Engine/ParticleEffect/ParticleEffectManager.h>
#include <SkyBoxManager.h>
#include <SoundManager.h>

// Application
#include <src/Game/Particles/Particle.h>

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
}

void GameResourceLoader::LoadAllModelData() {
	auto modelManager = Cygnus::ModelManager::GetInstance();

	// ---------------------------------------------------------
	// プリミティブ
	// ---------------------------------------------------------

	// 板
	modelManager->LoadAndRegisterModel(
		"Plane",
		"Primitive/Plane/plane.obj",
		"white.png"
	);

	// スフィア
	modelManager->LoadAndRegisterModel(
		"Sphere",
		"Primitive/Sphere/sphere.obj",
		"white.png"
	);

	// キューブ
	modelManager->LoadAndRegisterModel(
		"Cube",
		"Primitive/Cube/cube.obj",
		"white.png"
	);

	// ---------------------------------------------------------
	// キャラクター
	// ---------------------------------------------------------

	// プレイヤー
	modelManager->LoadAndRegisterModel(
		"Player",
		"Character/Player/player.obj",
		"Character/Player/player.png"
	);

	// スフィンクス
	modelManager->LoadAndRegisterModel(
		"Sphinx",
		"Character/Sphinx/sphinx.obj",
		"Character/Sphinx/sphinx.png"
	);

	// ---------------------------------------------------------
	// オブジェクト
	// ---------------------------------------------------------

	// 経路に沿って動くオブジェクト
	modelManager->LoadAndRegisterModel(
		"Carrier",
		"Object/Carrier/carrier.obj",
		"Object/Carrier/carrier.png"
	);

	// 鉱石オブジェクト
	modelManager->LoadAndRegisterModel(
		"Ore",
		"Object/Ore/ore.obj",
		"Object/Ore/ore.png"
	);

	// 工作台オブジェクト
	modelManager->LoadAndRegisterModel(
		"WorkBench", 
		"Object/WorkBench/workBench.obj", 
		"Object/WorkBench/workBench.png"
	);

	// 歯車オブジェクト
	modelManager->LoadAndRegisterModel(
		"Gear", 
		"Object/Gear/gear.obj", 
		"Object/Gear/gear.png"
	);

	// 攻撃予兆オブジェクト
	modelManager->LoadAndRegisterModel(
		"AttackPlane",
		"Object/AttackPlane/attackPlane.obj",
		"white.png"
	);

	// 攻撃予兆オブジェクト
	modelManager->LoadAndRegisterModel(
		"AttachFrame",
		"Object/AttachFrame/attackFrame.obj",
		"white.png"
	);
}

void GameResourceLoader::RegisterAllParticleEffect() {
	RegisterParticle("template", "Cube");
}

void GameResourceLoader::LoadAllSoundData() {
	auto soundManager = Cygnus::SoundManager::GetInstance();
}