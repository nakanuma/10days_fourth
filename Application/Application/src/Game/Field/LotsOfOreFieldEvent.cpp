#include "LotsOfOreFieldEvent.h"
#include "TextureManager.h"
#include <src/Game/Ore/OreManager.h>
#include <random>

void LotsOfOreFieldEvent::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	//スプライト作成
	effect_ = std::make_unique<Cygnus::Sprite>();
	effect_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().GetSRVIndex("resources/Images/white.png"));
	effect_->SetPosition({ 0.0f,0.0f });
	effect_->SetSize({ 32.0f,32.0f });

	eventTimer_ = kEventTime_;//タイムを設定
}

void LotsOfOreFieldEvent::Update() {
	effect_->Update();
	//鉱石を生み出すイベント
	BornOres();
}

void LotsOfOreFieldEvent::Draw() {
	effect_->Draw();
}

void LotsOfOreFieldEvent::Debug() {
#ifdef USE_IMGUI
	ImGui::Text("LotsOfOre");
#endif // USE_IMGUI 
}

void LotsOfOreFieldEvent::BornOres() {
	//時間を進める
	addOreTimer_ += 1.0f / 60.0f;
	//一定時間になったら
	if (addOreTimer_ >= kAddOreTime_) {
		//ランダム
		std::random_device seed;
		std::mt19937 random(seed());
		//位置(x,z)
		std::uniform_real_distribution<float> positionX(-kBornPositionX_, kBornPositionX_);
		std::uniform_real_distribution<float> positionZ(-kBornPositionZ_, kBornPositionZ_);

		//位置(x,z)
		std::uniform_int_distribution<uint32_t> randomSize(1, kMaxSize_);
		float oreSize = randomSize(random); // x,zを同じに(正方形にする)

		//鉱石を追加
		OreManager::GetInstance()->AddPoint(
			Cygnus::Float3{ positionX(random), positionY_, positionZ(random) }, 
			Cygnus::Float3{ oreSize, oreSizeY_, oreSize }
		);
		addOreTimer_ = 0.0f;//時間リセット
	}
}

