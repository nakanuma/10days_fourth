#include "SelectObjectManager.h"
#include <ModelManager.h>

void SelectObjectManager::Initialize()
{
	input_ = Cygnus::Input::GetInstance();

	// オブジェクト生成
	playerObject_ = std::make_unique<Cygnus::Object3D>();
	playerObject_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("Player");
	playerObject_->transform_.translate_ = { 0.0f, 2.3f, 0.0f };

	// 選択オブジェクトの生成と初期化
	for (size_t i = 0; i < selectObjects_.size(); ++i)
	{
		// インスタンスを生成して unique_ptr 配列に格納
		selectObjects_[i] = std::make_unique<Cygnus::Object3D>();

		// Ore.cpp と同様に、生成したオブジェクトに対してモデルの紐付けや座標設定を行う
		selectObjects_[i]->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("StagePointBlue");

		// 配置の計算例（X軸方向に等間隔に並べる場合）
		selectObjects_[i]->transform_.translate_ = { static_cast<float>(i) * kPointDistance, 0.0f, 0.0f };
	}

	SwapModel();

	// 中間オブジェクトの生成と初期化
	for (size_t i = 0; i < barObjects_.size(); ++i)
	{
		// インスタンスを生成して unique_ptr 配列に格納
		barObjects_[i] = std::make_unique<Cygnus::Object3D>();

		// モデルの紐付け
		barObjects_[i]->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("StageBar");

		// 選択オブジェクトの間に配置
		barObjects_[i]->transform_.translate_ = { static_cast<float>(i) * kPointDistance, 0.0f, 0.0f };

		barObjects_[i]->transform_.scale_.x = kPointDistance;
	}
}

void SelectObjectManager::Update(float deltaTime)
{
	playerObject_->UpdateMatrix();

	// すべてのオブジェクトの行列を更新
	for (auto& obj : selectObjects_)
	{
		if (obj) // nullptr チェック
		{
			obj->UpdateMatrix();
		}
	}
	for (auto& bar : barObjects_)
	{
		if (bar)
		{
			bar->UpdateMatrix();
		}
	}
}

void SelectObjectManager::Draw()
{
	// すべてのオブジェクトを描画
	for (auto& obj : selectObjects_)
	{
		if (obj)
		{
			obj->Draw();
		}
	}
	for (auto& bar : barObjects_)
	{
		if (bar)
		{
			bar->Draw();
		}
	}
	playerObject_->Draw();
}

void SelectObjectManager::Debug()
{
	// ImGui などのデバッグ用表示処理を記述します
}

bool SelectObjectManager::SelectStage()
{
	if (input_->TriggerKey(DIK_A))
	{
		if (currentStage_ > 0)
		{
			prevStage_ = currentStage_;
			currentStage_--;
			dir_--;
			Right();
			SwapModel();
			return true; // ステージが変更されたことを示す
		}
	}
	if (input_->TriggerKey(DIK_D))
	{
		if (currentStage_ < kStageNum - 1)
		{
			prevStage_ = currentStage_;
			currentStage_++;
			dir_++;
			Left();
			SwapModel();
			return true; // ステージが変更されたことを示す
		}
	}
	return false; // ステージが変更されなかったことを示す
}

void SelectObjectManager::SwapModel()
{
	selectObjects_[prevStage_]->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("StagePointBlue");
	selectObjects_[currentStage_]->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("StagePointRed");
}
