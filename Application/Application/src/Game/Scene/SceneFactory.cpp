#include "SceneFactory.h"

#include <src/Game/Scene/GamePlayScene.h>
#include <src/Game/Scene/ParticleEditorScene.h>
#include <src/Game/Scene/TitleScene.h>

SceneFactory* SceneFactory::GetInstance() {
	static SceneFactory instance;
	return &instance;
}

std::unique_ptr<Cygnus::BaseScene> SceneFactory::CreateScene(const std::string& sceneName) {
	if (sceneName == "TITLE")
	{
		auto newScene = std::make_unique<TitleScene>();
		newScene->Initialize();
		return newScene;
	}
	else if (sceneName == "GAMEPLAY") {
		auto newScene = std::make_unique<GamePlayScene>();
		newScene->Initialize();
		return newScene;
	}
	//パーティクルエディター
	else if(sceneName == "PARTICLEEDITOR") {
		auto newScene = std::make_unique<ParticleEditorScene>();
		newScene->Initialize();
		return newScene;
	}

	return nullptr;
}

std::string SceneFactory::GetInitialSceneName() {
	return initialSceneName;
}