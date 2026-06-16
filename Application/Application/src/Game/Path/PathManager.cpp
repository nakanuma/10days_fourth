#include "PathManager.h"

// Engine
#include <LineDrawer.h>

PathManager* PathManager::GetInstance() {
	static PathManager instance;
	return &instance;
}

void PathManager::Initialize() {
	points_.clear();
	// デバッグ用にベタ打ちで座標を追加（Todo : エディタで追加できるように変更する）
	points_.push_back({ 0.0f, 1.0f, 0.0f });
	points_.push_back({ 10.0f, 1.0f, 0.0f });
	points_.push_back({ 10.0f, 1.0f, -10.0f });
	points_.push_back({ 20.0f, 1.0f, -10.0f });
	points_.push_back({ 20.0f, 1.0f, -20.0f });
	points_.push_back({ 30.0f, 1.0f, -20.0f });
}

void PathManager::Draw() {
	auto lineDrawer = Cygnus::LineDrawer::GetInstance();

	// 全てのポイントを線で繋ぐ
	for (size_t i = 0; i < points_.size() - 1; ++i) {
		lineDrawer->RegisterLine(points_[i], points_[i + 1], kPathColor);
	}
}