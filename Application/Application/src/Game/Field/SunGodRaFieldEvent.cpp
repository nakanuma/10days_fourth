#include "SunGodRaFieldEvent.h"
#include "TextureManager.h"

void SunGodRaFieldEvent::Initialize(Cygnus::SpriteCommon* spriteCommon) {
	effect_ = std::make_unique<Cygnus::Sprite>();
	effect_->Initialize(spriteCommon, Cygnus::TextureManager::GetInstance().GetSRVIndex("resources/Images/white.png"));
	effect_->SetPosition({ 100.0f,100.0f });
	effect_->SetSize({ 32.0f,32.0f });

}

void SunGodRaFieldEvent::Update() {
	effect_->Update();
}

void SunGodRaFieldEvent::Draw() {
	effect_->Draw();
}

void SunGodRaFieldEvent::Debug() {
#ifdef USE_IMGUI
	ImGui::Text("SunGod-Ra");
#endif // USE_IMGUI 
}