/// --------------------------
///
/// 敵の大量発生イベント
/// 
/// --------------------------
#include "AnubisFieldEvent.h"
#include "src/Game/Mummy/MummyManager.h"

#include <TextureManager.h>

#include <random>

void AnubisFieldEvent::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	//スプライト作成
	effect_ = std::make_unique<Cygnus::Sprite>();
	effect_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().GetSRVIndex("resources/Images/white.png"));
	effect_->SetPosition({ 0.0f,0.0f });
	effect_->SetSize({ 16.0f,16.0f });

	eventTimer_ = kEventTime_;//タイムを設定
}

void AnubisFieldEvent::Update() {
	effect_->Update();

	//ミイラを最大数出したら止める
	if (MummyManager::GetInstance()->MummyMax()) return;

	if (isSummon_) {
		//召喚処理
		Summon();
	}
	else {
		//ク－ルタイム
		SummonCoolTime();
	}
}

void AnubisFieldEvent::Draw() {
	effect_->Draw();
}

void AnubisFieldEvent::Debug() {
#ifdef USE_IMGUI
	ImGui::Text("Anubis");
#endif // USE_IMGUI 
}

void AnubisFieldEvent::Summon() {
	//場所をランダムに
	std::mt19937 seed;
	std::random_device random;

	std::uniform_real_distribution<float> summonPos(-30.0f, 30.0f);

	Cygnus::Float3 position = { summonPos(random),2.0f,summonPos(random) };

	//召喚
	MummyManager::GetInstance()->AddMummy(position);

	isSummon_ = false;
}

void AnubisFieldEvent::SummonCoolTime() {
	nextSummonTimer_ += 1.0f / 60.0f;
	if (nextSummonTimer_ >= kNextSummonTimeMax_) {
		isSummon_ = true;
		nextSummonTimer_ = 0.0f;
	}
}

