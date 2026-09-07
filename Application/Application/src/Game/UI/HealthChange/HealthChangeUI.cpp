#include "HealthChangeUI.h"
#define NOMINMAX

// Application
#include <src/Game/Util/Utility.h>

// C++
#include <algorithm>

// Engine
#include <TimeManager.h>
#include <TextureManager.h>
#include <Easing.h>
#include <Math/MyMath.h>


void HealthChangeUI::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	//スプライトコモンの設定
	spriteCommon_ = spriteCommon;

	//テクスチャ読み込み
	texNumbers_ = Cygnus::TextureManager::Load("numbers.png");
	texPlus_ = Cygnus::TextureManager::Load("plus.png");
	texMinus_ = Cygnus::TextureManager::Load("minus.png");
}

void HealthChangeUI::Update() {
	//各数字UIの更新
	for (auto& digitUI : digitUIs_) {
		//時間の更新
		digitUI.timer += Cygnus::TimeManager::GetInstance()->GetDeltaTime();

		//時間を超えたら削除
		if (digitUI.timer >= displayDuration) {
			digitUI.spriteNumber_.reset();
			digitUI.spritePlus_.reset();
			digitUI.spriteMinus_.reset();
			digitUIs_.erase(std::remove(digitUIs_.begin(), digitUIs_.end(), digitUI), digitUIs_.end());
			continue;
		}

		//上昇処理
		float snHeight = digitUI.spriteNumber_->GetPosition().y + upwardSpeed * Cygnus::TimeManager::GetInstance()->GetDeltaTime();
		float spHeight = digitUI.spritePlus_->GetPosition().y + upwardSpeed * Cygnus::TimeManager::GetInstance()->GetDeltaTime();
		float smHeight = digitUI.spriteMinus_->GetPosition().y + upwardSpeed * Cygnus::TimeManager::GetInstance()->GetDeltaTime();

		digitUI.spriteNumber_->SetPosition({ digitUI.spriteNumber_->GetPosition().x, snHeight });
		digitUI.spritePlus_->SetPosition({ digitUI.spritePlus_->GetPosition().x, spHeight });
		digitUI.spriteMinus_->SetPosition({ digitUI.spriteMinus_->GetPosition().x, smHeight });

		//透明化
		float alpha = 1.0f - (digitUI.timer / displayDuration);

		digitUI.spriteNumber_->SetColor({ 1.0f, 0.0f, 0.0f, alpha });
		digitUI.spritePlus_->SetColor({ 1.0f, 0.0f, 0.0f, alpha });
		digitUI.spriteMinus_->SetColor({ 1.0f, 0.0f, 0.0f, alpha });

		//行列の更新
		digitUI.spriteNumber_->Update();
		digitUI.spritePlus_->Update();
		digitUI.spriteMinus_->Update();
	}
}

void HealthChangeUI::Draw() {
	//各数字UIの描画
	for (auto& digitUI : digitUIs_) {
		digitUI.spriteNumber_->Draw();
		digitUI.spritePlus_->Draw();
		digitUI.spriteMinus_->Draw();
	}
}

void HealthChangeUI::RegistDigitValue(Cygnus::Float3 subjectPos, int value) {
	//0は登録しない
	if (value == 0) {
		return;
	}

	//新しい数字UIを作成
	DigitUI newDigitUI;
	newDigitUI.spriteNumber_ = std::make_unique<Cygnus::Sprite>();
	newDigitUI.spritePlus_ = std::make_unique<Cygnus::Sprite>();
	newDigitUI.spriteMinus_ = std::make_unique<Cygnus::Sprite>();

	//初期化
	newDigitUI.spriteNumber_->Initialize(spriteCommon_, texNumbers_);
	newDigitUI.spritePlus_->Initialize(spriteCommon_, texPlus_);
	newDigitUI.spriteMinus_->Initialize(spriteCommon_, texMinus_);

	//アンカーポイントの設定
	newDigitUI.spriteNumber_->SetAnchorPoint({ 0.5f, 0.5f });
	newDigitUI.spritePlus_->SetAnchorPoint({ 0.5f, 0.5f });
	newDigitUI.spriteMinus_->SetAnchorPoint({ 0.5f, 0.5f });

	//サイズの設定
	newDigitUI.spriteNumber_->SetSize({ 64.0f, 64.0f });
	newDigitUI.spritePlus_->SetSize({ 64.0f, 64.0f });
	newDigitUI.spriteMinus_->SetSize({ 64.0f, 64.0f });

	//テクスチャサイズの設定
	newDigitUI.spriteNumber_->SetTextureSize({ 64.0f, 64.0f });
	newDigitUI.spritePlus_->SetTextureSize({ 64.0f, 64.0f });
	newDigitUI.spriteMinus_->SetTextureSize({ 64.0f, 64.0f });

	//値の反映
	{
		newDigitUI.spriteNumber_->SetTextureLeftTop({ std::abs(value) * 64.0f, 0.0f });
	}

	//座標の算出
	{
		//ワールド座標->スクリーン座標変換
		Cygnus::Float3 screenPos3D = Utility::WorldToScreen(subjectPos);
		//スクリーン座標をFloat2に変換
		Cygnus::Float2 screenPos = { screenPos3D.x, screenPos3D.y };

		//数字の座標を設定
		newDigitUI.spriteNumber_->SetPosition({ screenPos.x + 32.0f,screenPos.y });
		//符号の座標を設定
		if (value > 0) {
			newDigitUI.spritePlus_->SetPosition({ screenPos.x - 32.0f,screenPos.y });
			newDigitUI.spriteMinus_->SetColor({ 1,1,1,0 });
		}
		else {
			newDigitUI.spriteMinus_->SetPosition({ screenPos.x - 32.0f,screenPos.y });
			newDigitUI.spritePlus_->SetColor({ 1,1,1,0 });
		}
	}

	//タイマーの初期化
	newDigitUI.timer = 0.0f;

	//リストに追加
	digitUIs_.push_back(std::move(newDigitUI));

}
