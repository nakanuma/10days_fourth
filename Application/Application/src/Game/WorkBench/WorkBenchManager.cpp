#include "WorkBenchManager.h"

WorkBenchManager* WorkBenchManager::GetInstance() {
	static WorkBenchManager instance;
	return &instance;
}

void WorkBenchManager::Initialize() { 
	workBenches_.clear(); 
	// デバッグ用にベタ打ちで座標を追加（Todo : エディタで追加できるように変更する）
	auto workBench1 = std::make_unique<WorkBench>();
	workBench1->Initialize({10.0f, 1.0f, 0.0f});
	workBenches_.push_back(std::move(workBench1));

	auto workBench2 = std::make_unique<WorkBench>();
	workBench2->Initialize({0.0f, 1.0f, 10.0f});
	workBenches_.push_back(std::move(workBench2));
}

void WorkBenchManager::Update() {
	for (auto& workBench : workBenches_) {
		workBench->Update();
	}
}

void WorkBenchManager::Draw() {
	for (auto& workBench : workBenches_) {
		workBench->Draw();
	}
}
