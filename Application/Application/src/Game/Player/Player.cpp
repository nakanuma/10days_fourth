#include "Player.h"

// Engine
#include <Input/Input.h>
#include <ImguiWrapper.h>
#include <TimeManager.h>

void Player::Initialize() {
	// オブジェクト生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Player");
	object_->transform_.translate_ = {0.0f, 2.0f, -10.0f};
}

void Player::Update(float deltaTime) {
#pragma region 入力による移動処理
	// キーボードとゲームパッド両方の入力を加算
	Cygnus::Float3 moveDir = {0.0f, 0.0f, 0.0f};
	moveDir += GetKeyInput();
	moveDir += GetPadInput();

	// 正規化して一定の速度を保つ
	if(Cygnus::Float3::Length(moveDir) > 1.0f) {
		moveDir = Cygnus::Float3::Normalize(moveDir);
	}

	// オブジェクト位置に反映
	object_->transform_.translate_ += moveDir * kMoveSpeed * deltaTime;
#pragma endregion

	// オブジェクト更新
	object_->UpdateMatrix();
}

void Player::Draw() {
	// オブジェクト描画
	object_->Draw();
}

void Player::Debug() {
#ifdef USE_IMGUI
	ImGui::Begin("Player");

	ImGui::DragFloat3("Translate", &object_->transform_.translate_.x, 0.01f);

	ImGui::End();
#endif
}

Cygnus::Float3 Player::GetKeyInput()
{
	auto input = Cygnus::Input::GetInstance();
	Cygnus::Float3 dir = {0.0f, 0.0f, 0.0f};
	// 移動キー入力
	if(input->PushKey(DIK_W)) dir.z += 1.0f;
	if(input->PushKey(DIK_S)) dir.z -= 1.0f;
	if(input->PushKey(DIK_A)) dir.x -= 1.0f;
	if(input->PushKey(DIK_D)) dir.x += 1.0f;
	// キー入力結果を返す
	return dir;
}

Cygnus::Float3 Player::GetPadInput()
{
	auto input = Cygnus::Input::GetInstance();
	XINPUT_STATE state;

	// コントローラー取得
	if(input->GetJoystickState(0, state)) {
		// 左スティック入力結果を返す
		return {
			state.Gamepad.sThumbLX / 32767.0f,
			0.0f, 
			state.Gamepad.sThumbLY / 32767.0f
		};
	}
	return {0.0f, 0.0f, 0.0f};
}
