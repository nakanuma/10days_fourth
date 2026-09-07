#include "ItemPopupUI.h"

// Engine
#include <TextureManager.h>
#include <Easing.h>
#include <TimeManager.h>
#include <ImguiWrapper.h>

// Application
#include <src/Game/Util/Utility.h>

void ItemPopupUI::Initialize(Cygnus::SpriteCommon* spriteCommon, PartType type, std::function<Cygnus::Float3()> getWorldPosFunc) { 
	getWorldPosFunc_ = getWorldPosFunc;

	// アイコンスプライト生成
	std::string texIconName = "icon_partLow.png";
	if (type == PartType::Medium) texIconName = "icon_partMedium.png";
	if (type == PartType::High) texIconName = "icon_partHigh.png";
	uint32_t texIcon = Cygnus::TextureManager::Load(texIconName);

	spriteIcon_ = std::make_unique<Cygnus::Sprite>();
	spriteIcon_->Initialize(spriteCommon, texIcon);
	spriteIcon_->SetAnchorPoint({0.5f, 0.5f});
	spriteIcon_->SetSize({20.0f, 20.0f});

	// テキストスプライト生成
	uint32_t texText = Cygnus::TextureManager::Load("plusOne.png");
	spriteText_ = std::make_unique<Cygnus::Sprite>();
	spriteText_->Initialize(spriteCommon, texText);
	spriteText_->SetAnchorPoint({0.5f, 0.5f});
	spriteText_->SetSize({20.0f, 20.0f});
}

void ItemPopupUI::Update() { 
	float dt = Cygnus::TimeManager::GetInstance()->GetDeltaTime();
	timer_ += dt;

	float totalDuration = kFadeInTime + kDisplayTime + kFadeOutTime;

	// アニメーション時間終了で死亡
	if (timer_ >= totalDuration) {
		isDead_ = true;
		return;
	}

	// アルファ値計算
	if (timer_ < kFadeInTime) {
		// 徐々に濃くなる
		alpha_ = Cygnus::Easing::Lerp(0.0f, 1.0f, timer_ / kFadeInTime);
	} else if (timer_ < kFadeInTime + kDisplayTime) {
		alpha_ = 1.0f;
	} else {
		// 徐々に消える
		float fadeOutProgress = (timer_ - (kFadeInTime + kDisplayTime)) / kFadeOutTime;
		alpha_ = Cygnus::Easing::Lerp(1.0f, 0.0f, fadeOutProgress);
	}

	// 追従対象の最新ワールド座標から現在のスクリーン座標を取得
	Cygnus::Float3 targetWorldPos = {0.0f, 0.0f, 0.0f};
	if (getWorldPosFunc_) {
		targetWorldPos = getWorldPosFunc_();
	}
	Cygnus::Float3 screenPos3D = Utility::WorldToScreen(targetWorldPos);

	// 上昇移動
	float progress = timer_ / totalDuration;
	float easedProgress = Cygnus::Easing::EaseOutQuad(progress);
	float offsetY = easedProgress * kSlideDistance;

	currentPosition_.x = screenPos3D.x;
	currentPosition_.y = (screenPos3D.y - 45.0f) - offsetY;

	// スプライト座標とアルファ設定
	spriteIcon_->SetPosition(currentPosition_);
	spriteIcon_->SetColor({1.0f, 1.0f, 1.0f, alpha_});

	// テキストはアイコンの右隣に配置
	spriteText_->SetPosition({currentPosition_.x + 24.0f, currentPosition_.y});
	spriteText_->SetColor({1.0f, 1.0f, 1.0f, alpha_});

	// スプライト更新
	spriteIcon_->Update();
	spriteText_->Update();
}

void ItemPopupUI::Draw() {
	if (isDead_) return;
	spriteIcon_->Draw();
	spriteText_->Draw();
}

void ItemPopupUI::DebugUI(int index) {
#ifdef USE_IMGUI
	std::string label = "Popup #" + std::to_string(index);
	if (ImGui::TreeNode(label.c_str())) {
		ImGui::Text("Timer: %.2f / %.2f", timer_, kFadeInTime + kDisplayTime + kFadeOutTime);
		ImGui::Text("Alpha: %.2f", alpha_);
		ImGui::Text("Current Pos: (%.1f, %.1f)", currentPosition_.x, currentPosition_.y);

		if (spriteIcon_) {
			ImGui::Separator();
			ImGui::Text("Icon Sprite Exists: Yes");
		} else {
			ImGui::TextColored(ImVec4(1, 0, 0, 1), "Icon Sprite is NULL!");
		}

		ImGui::TreePop();
	}
#endif
}
