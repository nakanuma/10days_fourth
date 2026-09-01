#include "Tether.h"

// Engine
#include <ImguiWrapper.h>
#include <LineDrawer.h>
#include <Collider/CollisionMath.h>

// Application
#include <src/Game/Objects/Spaceship/Spaceship.h>
#include <src/Game/Objects/Player/Player.h>

void Tether::Initialize(Spaceship* spaceship, Player* player) {
	spaceship_ = spaceship;
	player_ = player;

	nodes_.clear();
	nodes_.resize(kNodeCount);

	Cygnus::Float3 startPos = spaceship_->GetTranslate();
	Cygnus::Float3 endPos = player_->GetTranslate();

	// 宇宙船からプレイヤーの位置まで等間隔にノードを配置
	for (size_t i = 0; i < kNodeCount; ++i) {
		float t = static_cast<float>(i) / static_cast<float>(kNodeCount - 1);

		Cygnus::Float3 pos = {
			startPos.x + (endPos.x - startPos.x) * t,
			startPos.y + (endPos.y - startPos.y) * t,
			startPos.z + (endPos.z - startPos.z) * t
		};

		nodes_[i].position = pos;
		nodes_[i].oldPosition = pos;
		nodes_[i].isLocked = false;
	}

	// 両端を固定
	nodes_.front().isLocked = true;
	nodes_.back().isLocked = true;
}

void Tether::Update() {
	if (!spaceship_ || !player_) return;

	// 両端の位置に同期
	nodes_.front().position = spaceship_->GetTranslate();
	nodes_.back().position = player_->GetTranslate();

	// 各ノードのベレ物理更新
	for (auto& node : nodes_) {
		if (node.isLocked) continue;

		// 過去座標との差分から慣性速度を計算
		Cygnus::Float3 velocity = {
			(node.position.x - node.oldPosition.x) * kDamping,
			(node.position.y - node.oldPosition.y) * kDamping,
			(node.position.z - node.oldPosition.z) * kDamping
		};

		node.oldPosition = node.position;

		// 外力（いまのところは保留）
		Cygnus::Float3 gravity = { 0.0f, 0.0f, 0.0f };

		node.position.x += velocity.x + gravity.x;
		node.position.y += velocity.y + gravity.y;
		node.position.z += velocity.z + gravity.z;
	}

	// 距離拘束の適用
	for (int i = 0; i < kConstraintIterations; ++i) {
		ApplyConstraints();
	}
}

void Tether::Draw() {
	// 隣り合うノード同士を線で描画
	for(size_t i = 0; i < nodes_.size() - 1; ++i) {
		Cygnus::LineDrawer::GetInstance()->RegisterLine(nodes_[i].position, nodes_[i + 1].position, {1.0f, 0.0f, 0.0f, 1.0f});
	}
}

void Tether::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("Tether");

	ImGui::Text("Node Count: %d", nodes_.size());

	ImGui::End();
#endif
}

void Tether::CheckCollisionWithFlyingObjects(FlyingObjectManager* flyingObjectManager) { 
	if (!flyingObjectManager) return; 

	const auto& objects = flyingObjectManager->GetObjects();
	
	for (const auto& obj : objects) {
		// 飛翔物の中心位置と判定半径を取得
		Cygnus::Float3 objPos = obj->GetTranslate();
		float hitRadius = obj->GetHitRadius();

		// テザーの各線分と飛翔物の衝突チェック
		for (size_t i = 0; i < nodes_.size() - 1; ++i) {
			const auto& nodeA = nodes_[i].position;
			const auto& nodeB = nodes_[i + 1].position;

			// 衝突時の処理
			if (Cygnus::CollisionMath::IsSegmentIntersectSphere(nodeA, nodeB, objPos, hitRadius)) {
				// 飛翔物を消滅させる
				obj->Destroy();

				// 命中時の処理
				if (obj->GetCategory() == ObjectCategory::Meteor) {
				
				}
				if (obj->GetCategory() == ObjectCategory::RepairPart) {
				
				}

				// 衝突したらこの飛翔物の判定は終了
				break;
			}
		}
	}
}

void Tether::ApplyConstraints() {
	for(size_t i = 0; i < nodes_.size() - 1; ++i) {
		TetherNode& nodeA = nodes_[i];
		TetherNode& nodeB = nodes_[i + 1];

		// 2点間のベクトルと距離を計算
		Cygnus::Float3 delta = {
			nodeB.position.x - nodeA.position.x,
			nodeB.position.y - nodeA.position.y,
			nodeB.position.z - nodeA.position.z
		};

		float distance = std::sqrt(delta.x * delta.x + delta.y * delta.y + delta.z * delta.z);
		if(distance < 0.0001f) continue;

		// 自然長からのズレ量を算出
		float difference = (distance - kSegmentLength) / distance;

		Cygnus::Float3 correction = {
			delta.x * 0.5f * difference,
			delta.y * 0.5f * difference,
			delta.z * 0.5f * difference
		};

		// 移動制限
		if(!nodeA.isLocked && !nodeB.isLocked) {
			nodeA.position += correction;
			nodeB.position -= correction;
		} else if (!nodeA.isLocked) {
			nodeA.position += correction * 2.0f;
		} else if (!nodeB.isLocked) {
			nodeB.position -= correction * 2.0f;
		}
	}
}


