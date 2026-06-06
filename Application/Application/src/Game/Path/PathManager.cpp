#include "PathManager.h"

// Engine
#include <LineDrawer.h>

PathManager* PathManager::GetInstance() {
	static PathManager instance;
	return &instance;
}

void PathManager::Initialize() {
	points_.clear();
}

void PathManager::Draw() {
	auto lineDrawer = Cygnus::LineDrawer::GetInstance();

	// 全てのポイントを線で繋ぐ
	for (size_t i = 0; i < points_.size() - 1; ++i) {
		lineDrawer->RegisterLine(points_[i], points_[i + 1], kPathColor);
	}
}