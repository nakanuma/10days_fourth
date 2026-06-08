#include "FieldEventManager.h"
#include "LotsOfOreFieldEvent.h"
#include "NormalField.h"
#include "SunGodRaFieldEvent.h"
#include "AnubisFieldEvent.h"
#include "SethFieldEvent.h"
#include "Input.h"

#include <random>
#include <src/Game/Sandstrom/SandstormManager.h>

void FieldEventManager::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	spriteCommon_ = spriteCommon;
	fieldEvent_ = std::make_unique<NormalField>();//最初はイベントなし
	fieldEvent_->Initialize(spriteCommon_);
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
		//通常(イベントなし)の場合
		if (number_ == 0) {
			//ランダム
			std::random_device seed;
			std::mt19937 random(seed());
			std::uniform_int_distribution<uint32_t> eventNum (1, max - 1);//fieldEventのmax - 1
			//ナンバーを変更
			number_ = eventNum(random);
		}
		else {
			number_ = 0;//通常(イベントなし)のフィールドに戻る
			SandstormManager::GetInstance()->Reset();
		}
	}

	//変更された時
	if (prevNum_ != number_) {
		fieldEvent_.reset();
		switch (number_)
		{
		case FieldEventManager::normal:
			fieldEvent_ = std::make_unique<NormalField>();
			break;
		case FieldEventManager::lotsOfOre:
			fieldEvent_ = std::make_unique<LotsOfOreFieldEvent>();
			break;
		case FieldEventManager::SunGodRa:
			fieldEvent_ = std::make_unique<SunGodRaFieldEvent>();
			break;
		case FieldEventManager::Anubis:
			fieldEvent_ = std::make_unique<AnubisFieldEvent>();
			break;
		case FieldEventManager::Seth:
			fieldEvent_ = std::make_unique<SethFieldEvent>();
			break;
		default:
			fieldEvent_ = std::make_unique<NormalField>();
			number_ = 0; 
			break;
		}
		fieldEvent_->Initialize(spriteCommon_);
		prevNum_ = number_;//現在 -> 前回に
	}
}


