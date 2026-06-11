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

	// 黒い板
	modelManager->LoadAndRegisterModel(
		"BlackPlane",
		"Primitive/Plane/plane.obj",
		"black.png"
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

	modelManager->LoadAndRegisterModel(
		"Mummy",
		"Character/Mummy/Mummy.obj",
		"Character/Mummy/mummy.png"
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

	// ゴールオブジェクト
	modelManager->LoadAndRegisterModel(
		"Goal",
		"Object/Goal/goal.obj",
		"Object/Goal/goal.png"
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

	// 太陽レーザーオブジェクト
	modelManager->LoadAndRegisterModel(
		"SunLaserArea",
		"Object/SunLaserArea/SunLaserArea.obj",
    "white.png"
	);
  
	// ステージ選択用オブジェクト
	modelManager->LoadAndRegisterModel(
		"StagePointBlue",
		"Object/stagePoint/stagePoint.obj",
		"Object/stagePoint/stagePointBlue.png"
	);

	// ステージ選択用オブジェクト
	modelManager->LoadAndRegisterModel(
		"StagePointRed",
		"Object/stagePoint/stagePoint.obj",
		"Object/stagePoint/stagePointRed.png"
	);

	// ステージ選択用オブジェクト
	modelManager->LoadAndRegisterModel(
		"StageBar",
		"Object/stageBar/stageBar.obj",
		"white.png"
	);
}

void GameResourceLoader::RegisterAllParticleEffect() {
	RegisterParticle("template", "Cube");
	RegisterParticle("move_dust", "Cube");
	RegisterParticle("dush", "Cube");
	RegisterParticle("ore_break", "Cube");
	RegisterParticle("fireworks_diffusion", "Cube");
	RegisterParticle("sand_rain", "Cube");
	RegisterParticle("interact", "Plane");
	RegisterParticle("debuff", "Cube");
	RegisterParticle("get", "Cube");
	RegisterParticle("sand_grain", "Cube");
}

void GameResourceLoader::LoadAllSoundData() {
	auto soundManager = Cygnus::SoundManager::GetInstance();

	soundManager->Load("resources/Sounds/enter.wav", "enter");
	soundManager->Load("resources/Sounds/idou.wav", "idou");
	soundManager->Load("resources/Sounds/totunyuu.wav", "totunyuu");

	soundManager->Load("resources/Sounds/craft.wav", "craft");
	soundManager->Load("resources/Sounds/get.wav", "get");
	soundManager->Load("resources/Sounds/hameru.wav", "hameru");
	soundManager->Load("resources/Sounds/horu.wav", "horu");
	soundManager->Load("resources/Sounds/iwakudaki.wav", "iwakudaki");
	soundManager->Load("resources/Sounds/resshaugoku.wav", "resshaugoku");
	soundManager->Load("resources/Sounds/sandstorm.wav", "sandstorm");
	soundManager->Load("resources/Sounds/slow.wav", "slow");
	soundManager->Load("resources/Sounds/tossin.wav", "tossin");
	soundManager->Load("resources/Sounds/fanfare.wav", "fanfare");
	soundManager->Load("resources/Sounds/bgm.wav", "bgm");
}