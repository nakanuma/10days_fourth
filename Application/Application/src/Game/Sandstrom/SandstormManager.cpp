#include "SandstormManager.h"
#include "ImguiWrapper.h"
#include <src/Game/Particles/storm/StormManager.h>

SandstormManager* SandstormManager::GetInstance() {
	static SandstormManager sInstance;
	return &sInstance;
}

void SandstormManager::Initialize() {
	sandStorms_.clear();
}

void SandstormManager::Update() {
	for (auto& sandstorm : sandStorms_) {
		sandstorm->Update();
	}
}

void SandstormManager::Draw() {
	for (auto& sandstorm : sandStorms_) {
		sandstorm->Draw();
	}
}

void SandstormManager::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("SandStorm");
	for (auto& sandstorm : sandStorms_) {
		sandstorm->Debug();
	}
	ImGui::End();
#endif //USE_IMGUI
}

void SandstormManager::AddSandstrom(const Cygnus::Float3& translate) {
	
	StormManager::GetInstance()->Delete("SandStorm");//削除

	std::unique_ptr<Sandstorm> newSandstorm = std::make_unique<Sandstorm>();
	newSandstorm->Initialize(translate);
	sandStorms_.push_back(std::move(newSandstorm));
}

void SandstormManager::Reset() {
	for (auto& sandstorm : sandStorms_) {
		sandstorm->Finalize();
	}
	StormManager::GetInstance()->Delete("SandStorm");//削除
	sandStorms_.clear();
}
