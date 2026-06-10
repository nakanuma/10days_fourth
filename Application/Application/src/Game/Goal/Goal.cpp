#include "Goal.h"

// Application
#include <src/Game/Path/PathManager.h>

void Goal::Initialize() {
	// ゴール位置を取得
	auto pathManager = PathManager::GetInstance();
	const Cygnus::Float3& goalPosition = pathManager->GetPoint(pathManager->GetPointCount() - 1); // 最後のポイントの座標を取得

	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Goal");
	object_->transform_.translate_ = { goalPosition.x, kCoordinateY, goalPosition.z };
}

void Goal::Update() {
	// オブジェクト更新
	object_->UpdateMatrix();
}

void Goal::Draw() {
	// オブジェクト描画
	object_->Draw();
}