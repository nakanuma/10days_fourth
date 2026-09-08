#include "ValueChangeUI.h"
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


void ValueChangeUI::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	//スプライトコモンの設定
	spriteCommon_ = spriteCommon;

	//テクスチャ読み込み
	texNumbers_ = Cygnus::TextureManager::Load("numbers.png");
	texPlus_ = Cygnus::TextureManager::Load("plus.png");
	texMinus_ = Cygnus::TextureManager::Load("minus.png");
}

void ValueChangeUI::Update() {
	// 各数字UIの更新
	for (auto it = digitUIs_.begin(); it != digitUIs_.end();) {
		auto& digitUI = *it;

		// 時間の更新
		digitUI.timer += Cygnus::TimeManager::GetInstance()->GetDeltaTime();

		// 時間を超えたら削除
		if (digitUI.timer >= displayDuration) {
			digitUI.spriteNumber_One_.reset();
			digitUI.spriteNumber_Ten_.reset();
			digitUI.spritePlus_.reset();
			digitUI.spriteMinus_.reset();

			it = digitUIs_.erase(it);
			continue;
		}

		// 上昇処理
		float deltaTime = Cygnus::TimeManager::GetInstance()->GetDeltaTime();

		float snoHeight = digitUI.spriteNumber_One_->GetPosition().y + upwardSpeed * deltaTime;
		float sntHeight = digitUI.spriteNumber_Ten_->GetPosition().y + upwardSpeed * deltaTime;
		float spHeight = digitUI.spritePlus_->GetPosition().y + upwardSpeed * deltaTime;
		float smHeight = digitUI.spriteMinus_->GetPosition().y + upwardSpeed * deltaTime;

		digitUI.spriteNumber_One_->SetPosition({
			digitUI.spriteNumber_One_->GetPosition().x,
			snoHeight
			});

		digitUI.spriteNumber_Ten_->SetPosition({
			digitUI.spriteNumber_Ten_->GetPosition().x,
			sntHeight
			});

		digitUI.spritePlus_->SetPosition({
			digitUI.spritePlus_->GetPosition().x,
			spHeight
			});

		digitUI.spriteMinus_->SetPosition({
			digitUI.spriteMinus_->GetPosition().x,
			smHeight
			});

		// 透明化
		float alpha = 1.0f - (digitUI.timer / displayDuration);

		digitUI.spriteNumber_One_->SetColor({ digitUI.color.x, digitUI.color.y, digitUI.color.z, alpha });
		digitUI.spriteNumber_Ten_->SetColor({ digitUI.color.x, digitUI.color.y, digitUI.color.z, alpha });
		digitUI.spritePlus_->SetColor({ digitUI.color.x, digitUI.color.y, digitUI.color.z, alpha });
		digitUI.spriteMinus_->SetColor({ digitUI.color.x, digitUI.color.y, digitUI.color.z, alpha });

		// 行列の更新
		digitUI.spriteNumber_One_->Update();
		digitUI.spriteNumber_Ten_->Update();
		digitUI.spritePlus_->Update();
		digitUI.spriteMinus_->Update();

		++it;
	}
}

void ValueChangeUI::Draw() {
	//各数字UIの描画
	for (auto& digitUI : digitUIs_) {
		digitUI.spriteNumber_One_->Draw();
		digitUI.spriteNumber_Ten_->Draw();
		digitUI.spritePlus_->Draw();
		digitUI.spriteMinus_->Draw();
	}
}

void ValueChangeUI::RegistDigitValue(Cygnus::Float3 subjectPos, int value, Cygnus::Float4 color) {
	//0は登録しない
	if (value == 0) {
		return;
	}

	//新しい数字UIを作成
	DigitUI newDigitUI;
	newDigitUI.spriteNumber_One_ = std::make_unique<Cygnus::Sprite>();
	newDigitUI.spriteNumber_Ten_ = std::make_unique<Cygnus::Sprite>();
	newDigitUI.spritePlus_ = std::make_unique<Cygnus::Sprite>();
	newDigitUI.spriteMinus_ = std::make_unique<Cygnus::Sprite>();

	//初期化
	newDigitUI.spriteNumber_One_->Initialize(spriteCommon_, texNumbers_);
	newDigitUI.spriteNumber_Ten_->Initialize(spriteCommon_, texNumbers_);
	newDigitUI.spritePlus_->Initialize(spriteCommon_, texPlus_);
	newDigitUI.spriteMinus_->Initialize(spriteCommon_, texMinus_);

	//アンカーポイントの設定
	newDigitUI.spriteNumber_One_->SetAnchorPoint({ 0.5f, 0.5f });
	newDigitUI.spriteNumber_Ten_->SetAnchorPoint({ 0.5f, 0.5f });
	newDigitUI.spritePlus_->SetAnchorPoint({ 0.5f, 0.5f });
	newDigitUI.spriteMinus_->SetAnchorPoint({ 0.5f, 0.5f });

	//サイズの設定
	newDigitUI.spriteNumber_One_->SetSize({ 32.0f, 32.0f });
	newDigitUI.spriteNumber_Ten_->SetSize({ 32.0f, 32.0f });
	newDigitUI.spritePlus_->SetSize({ 32.0f, 32.0f });
	newDigitUI.spriteMinus_->SetSize({ 32.0f, 32.0f });

	//テクスチャサイズの設定
	newDigitUI.spriteNumber_One_->SetTextureSize({ 64.0f, 64.0f });
	newDigitUI.spriteNumber_Ten_->SetTextureSize({ 64.0f, 64.0f });
	newDigitUI.spritePlus_->SetTextureSize({ 64.0f, 64.0f });
	newDigitUI.spriteMinus_->SetTextureSize({ 64.0f, 64.0f });

	//色の設定
	newDigitUI.color = color;
	newDigitUI.spriteNumber_One_->SetColor(color);
	newDigitUI.spriteNumber_Ten_->SetColor(color);
	newDigitUI.spritePlus_->SetColor(color);
	newDigitUI.spriteMinus_->SetColor(color);

	//値が十の位か
	bool isTenDigit = std::abs(value) >= 10;

	//値の反映
	{
		if(isTenDigit) {
			int tenDigit = std::abs(value) / 10;
			int oneDigit = std::abs(value) % 10;
			newDigitUI.spriteNumber_Ten_->SetTextureLeftTop({ tenDigit * 64.0f, 0.0f });
			newDigitUI.spriteNumber_One_->SetTextureLeftTop({ oneDigit * 64.0f, 0.0f });
		}
		else {
			int oneDigit = std::abs(value) % 10;
			newDigitUI.spriteNumber_One_->SetTextureLeftTop({ oneDigit * 64.0f, 0.0f });

			//一の位のみ表示
			newDigitUI.spriteNumber_Ten_->SetColor({ 1,1,1,0 });
		}
	}

	//座標の算出
	{
		//ワールド座標->スクリーン座標変換
		Cygnus::Float3 screenPos3D = Utility::WorldToScreen(subjectPos);
		//スクリーン座標をFloat2に変換
		Cygnus::Float2 screenPos = { screenPos3D.x, screenPos3D.y };

		//数字の座標を設定
		if (isTenDigit) {
			newDigitUI.spriteNumber_One_->SetPosition({ screenPos.x + 24.0f,screenPos.y });
			newDigitUI.spriteNumber_Ten_->SetPosition({ screenPos.x + 8.0f,screenPos.y });
			//符号の座標を設定
			if (value > 0) {
				newDigitUI.spritePlus_->SetPosition({ screenPos.x - 8.0f,screenPos.y });
				newDigitUI.spriteMinus_->SetColor({ 1,1,1,0 });
			}
			else {
				newDigitUI.spriteMinus_->SetPosition({ screenPos.x - 8.0f,screenPos.y });
				newDigitUI.spritePlus_->SetColor({ 1,1,1,0 });
			}
		}
		else {
			newDigitUI.spriteNumber_One_->SetPosition({ screenPos.x + 8.0f,screenPos.y });
			//符号の座標を設定
			if (value > 0) {
				newDigitUI.spritePlus_->SetPosition({ screenPos.x - 8.0f,screenPos.y });
				newDigitUI.spriteMinus_->SetColor({ 1,1,1,0 });
			}
			else {
				newDigitUI.spriteMinus_->SetPosition({ screenPos.x - 8.0f,screenPos.y });
				newDigitUI.spritePlus_->SetColor({ 1,1,1,0 });
			}
		}
	}

	//タイマーの初期化
	newDigitUI.timer = 0.0f;

	//リストに追加
	digitUIs_.push_back(std::move(newDigitUI));

}
