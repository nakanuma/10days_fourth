#include "PauseMenu.h"

// Engine
#include <Input/Input.h>

void PauseMenu::Initialize() { 
	isPaused_ = false; 
}

void PauseMenu::Update() { 
	if (Cygnus::Input::GetInstance()->TriggerKey(DIK_ESCAPE)) {
		// ƒ|[ƒYó‘ÔØ‘Ö
		isPaused_ = !isPaused_;
	}
}

void PauseMenu::Draw() {

}