#include "SceneFactory.h"

#include <src/Game/Scene/GamePlayScene.h>
#include <src/Game/Scene/ParticleEditorScene.h>
#include <src/Game/Scene/TitleScene/TitleScene.h>
#include <src/Game/Scene/ResultScene/ResultScene.h>
#include <src/Game/Scene/StageSelectScene/StageSelectScene.h>

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
	else if (sceneName == "SELECT")
	{
		auto newScene = std::make_unique<StageSelectScene>();
		newScene->Initialize();
		return newScene;
	}
	else if (sceneName == "GAMEPLAY") {
		auto newScene = std::make_unique<GamePlayScene>();
		newScene->Initialize();
		return newScene;
	}
	else if (sceneName == "RESULT")
	{
		auto newScene = std::make_unique<ResultScene>();
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