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
	Cygnus::SkyBoxManager::GetInstance()->SetTranslate({0.0f, 0.0f, 1500.0f});
	Cygnus::SkyBoxManager::GetInstance()->SetRotate({0.37f, 1.29f, 0.26f});
	Cygnus::SkyBoxManager::GetInstance()->SetColor({0.5f, 0.3f, 1.0f, 1.0f});

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

	// 黒い板
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"BlackPlane",
		"Primitive/Plane/plane.obj",
		"black.png"
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

	// ハート（回復）
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"HeartItem", 
		"HeartItem/heartItem.obj", 
		"heartItem.png"
	);

	// 爆弾（隕石全破壊）
	Cygnus::ModelManager::GetInstance()->LoadAndRegisterModel(
		"BombItem", 
		"BombItem/bombItem.obj", 
		"bombItem.png"
	);
}

void GameResourceLoader::RegisterAllParticleEffect() {
	//テンプレ―ト
	RegisterParticle("template", "Cube");
	//プレイヤー移動時パーティクル
	RegisterParticle("move_player", "Cube");
	//大隕石移動時パーティクル
	RegisterParticle("move_large_meteor", "Cube");
	//小隕石移動時パーティクル
	RegisterParticle("move_small_meteor", "Cube");
	//修理パーツ移動時パーティクル
	RegisterParticle("move_repair", "Cube");
	//隕石衝突パーティクル1
	RegisterParticle("hit_meteor1", "Cube");
	//隕石衝突パーティクル2
	RegisterParticle("hit_meteor2", "Cube");
	//修理パーツ衝突パーティクル
	RegisterParticle("hit_repair", "Cube");
	//酸素ゲージ減少時の冷や汗パーティクル
	RegisterParticle("low_oxy", "Cube");
	//クリアしたときのロケット噴射パーティクル
	RegisterParticle("clear_rocket", "Cube");

}

void GameResourceLoader::LoadAllSoundData() {
	auto soundManager = Cygnus::SoundManager::GetInstance();

	// bgm
	soundManager->Load("resources/Sounds/bgm_title.wav", "bgm_title");
	soundManager->Load("resources/Sounds/bgm_gameplay.wav", "bgm_tutorial"); // ゲーム用をチュートリアル用としても扱う（BGM停止対策）
	soundManager->Load("resources/Sounds/bgm_gameplay.wav", "bgm_gameplay");
	soundManager->Load("resources/Sounds/bgm_gameclear.wav", "bgm_gameclear");
	soundManager->Load("resources/Sounds/bgm_gameover.wav", "bgm_gameover");
	soundManager->Load("resources/Sounds/bgm_tutorial.wav", "bgm_tutorial");

	// se
	soundManager->Load("resources/Sounds/se_switch.wav", "se_switch");
	soundManager->Load("resources/Sounds/se_decide.wav", "se_decide");
	soundManager->Load("resources/Sounds/se_pickup.wav", "se_pickup");
	soundManager->Load("resources/Sounds/se_consume.wav", "se_consume");
	soundManager->Load("resources/Sounds/se_collide.wav", "se_collide");
	soundManager->Load("resources/Sounds/se_return.wav", "se_return");
	soundManager->Load("resources/Sounds/se_pause.wav", "se_pause");
	soundManager->Load("resources/Sounds/se_explosion.wav", "se_explosion");
}