/// ----------------------
///
/// ミイラ召喚マネージャ
/// 
/// ----------------------
#include "MummyManager.h"

MummyManager* MummyManager::GetInstance() {
	static MummyManager sInstance;
	return &sInstance;
}

void MummyManager::Initialize() {
	mummies_.clear();
}

void MummyManager::Update(const Cygnus::Float3& playerPos, float dt) {
	
	//消滅処理
	mummies_.remove_if([](std::unique_ptr<Mummy>& mummy) {
		if (mummy->IsDead()) {
			mummy.reset();
			return true;
		}
		return false;
	});

	//更新
	for (auto& mummy : mummies_) {
		mummy->Update(playerPos, dt);
	}
}

void MummyManager::Draw() {
	//描画
	for (auto& mummy : mummies_) {
		mummy->Draw();
	}
}

void MummyManager::Debug() {
	for (auto& mummy : mummies_) {
		mummy->Debug();
	}
}

void MummyManager::AddMummy(const Cygnus::Float3& position) {
	//新しいミイラを追加する
	std::unique_ptr<Mummy> newMummy = std::make_unique<Mummy>();
	newMummy->Initialize(position);//初期化・初期位置を設定
	//追加する
	mummies_.push_back(std::move(newMummy));
}
