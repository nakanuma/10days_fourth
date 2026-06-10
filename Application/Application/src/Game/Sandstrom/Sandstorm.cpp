/// ------
///
/// 砂嵐
/// 
/// ------
#include "Sandstorm.h"
#include <Collider/CollisionManager.h>
#include <random>
#include "ImguiWrapper.h"

void Sandstorm::Initialize(const Cygnus::Float3& translate) {
	//開始位置
	startPoint_ = translate;
	

	std::random_device seed;
	std::mt19937 random(seed());

	std::uniform_real_distribution<float> distCarveZ(-40, 40);


	carvePoint_ = Cygnus::Float3{ 0,0,distCarveZ(random) };

	endPoint_ = translate;
	endPoint_.x = -translate.x;

	// コライダー生成 + 登録
	auto aabb = std::make_unique<Cygnus::AABBCollider>();
	aabb->SetTag("Sandstorm");
	aabb->SetFollowTarget(&transform_.translate_);
	aabb->SetSize(kColliderSize_);
	aabb->SetOwner(this);

	collider_ = std::move(aabb);
	Cygnus::CollisionManager::GetInstance()->Register(&*collider_);//コライダー削除
}

void Sandstorm::Update() {

	transform_.rotate_.y += kMoveSpeed_;

	Time_ += 1.0f / 180.0f;
	Cygnus::Float3 a = Cygnus::Float3::Lerp(startPoint_, carvePoint_, Time_);
	Cygnus::Float3 b = Cygnus::Float3::Lerp(carvePoint_, endPoint_ ,Time_);

	transform_.translate_ = Cygnus::Float3::Lerp(a,b,Time_);

	collider_->Update();
}

void Sandstorm::Draw() {
	
}

void Sandstorm::Debug() {
#ifdef USE_IMGUI

	ImGui::Text("CarvePoint: %f,%f,%f", carvePoint_.x, carvePoint_.y, carvePoint_.z);

#endif // USE_IMGUI
}

void Sandstorm::Finalize() {
	Cygnus::CollisionManager::GetInstance()->Unregister(&*collider_);//コライダー削除
}
