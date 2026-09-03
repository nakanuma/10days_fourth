#include "SceneFactory.h"

#include <src/Game/Scene/GamePlayScene.h>
#include <src/Game/Scene/ParticleEditorScene.h>

SceneFactory* SceneFactory::GetInstance() {
	static SceneFactory instance;
	return &instance;
}

std::unique_ptr<Cygnus::BaseScene> SceneFactory::CreateScene(const std::string& sceneName) {
	if (sceneName == "GAMEPLAY") {
		auto newScene = std::make_unique<GamePlayScene>();
		newScene->Initialize();
		return newScene;
	}
	// ※パーティクルエディターシーン
	else if(sceneName == "PARTICLEEDITOR") {
		// パーティクルエディターシーンの生成と初期化処理を行う
		auto newScene = std::make_unique<ParticleEditorScene>();
		newScene->Initialize();
		return newScene;
	}

	return nullptr;
}

std::string SceneFactory::GetInitialSceneName() {
	return initialSceneName;
}