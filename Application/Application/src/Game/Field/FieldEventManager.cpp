#include "FieldEventManager.h"
#include "LotsOfOreFieldEvent.h"
#include "NormalField.h"
#include "SunGodRaFieldEvent.h"
#include "AnubisFieldEvent.h"
#include "SethFieldEvent.h"
#include "Input.h"

#include <random>
#include <src/Game/Sandstrom/SandstormManager.h>

void FieldEventManager::Initialize(Cygnus::SpriteCommon* spriteCommon,const EventRatio& eventRatio) {
	spriteCommon_ = spriteCommon;
	fieldEvent_ = std::make_unique<NormalField>();//最初はイベントなし
	fieldEvent_->Initialize(spriteCommon_);

	eventRatio_ = eventRatio;
}

void FieldEventManager::Update(float deltaTime) {
	//時間を進める
	fieldEvent_->EventTimeUpdate(deltaTime);
	//変更するフィールドイベント
	ChangeField();
	//
	fieldEvent_->Update();
}

void FieldEventManager::Draw() {
	fieldEvent_->Draw();
}

void FieldEventManager::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("fieldEvent");
	//現在のイベント名を出す
	fieldEvent_->Debug();
	//時間表示
	ImGui::Text("time : %02f", fieldEvent_->GetEventTime());
	ImGui::End();
#endif // USE_IMGUI
}

void FieldEventManager::ChangeField() {

	//一定の時間がたった場合
	if (fieldEvent_->EventEnd()) {
		//ランダムイベントが発動する処理

		if (number_ > 0) {
			number_ = 0;
		}
		else {
			//ランダム
			std::random_device seed;
			std::mt19937 random(seed());
			std::uniform_int_distribution<uint32_t> eventNum(1, 100);//fieldEventのmax - 1
			//ナンバーを変更
			number_ = eventNum(random);
		}
		fieldEvent_.reset();


		ratioCount_ = 0;
		if (number_ == ratioCount_) {
			FieldSelect(std::make_unique<NormalField>());
			SandstormManager::GetInstance()->Reset();
			return;
		}
		ratioCount_ += eventRatio_.LotOfOre;
		if (number_ < ratioCount_) {
			FieldSelect(std::make_unique<LotsOfOreFieldEvent>());
			return;
		}
		ratioCount_ += eventRatio_.Seth;
		if (number_ < ratioCount_) {
			FieldSelect(std::make_unique<SethFieldEvent>());
			return;
		}

		ratioCount_ += eventRatio_.Anubis;
		if (number_ < ratioCount_) {
			FieldSelect(std::make_unique<AnubisFieldEvent>());
			return;
		}

		ratioCount_ += eventRatio_.SunGodRa;
		if (number_ < ratioCount_) {
			FieldSelect(std::make_unique<SunGodRaFieldEvent>());
			return;
		};

		FieldSelect(std::make_unique<NormalField>());
		SandstormManager::GetInstance()->Reset();
	}
}

void FieldEventManager::FieldSelect(std::unique_ptr<BaseFieldEvent> fieldEvent) {
	fieldEvent_ = std::move(fieldEvent);
	fieldEvent_->Initialize(spriteCommon_);
	number_ = ratioCount_;
}

