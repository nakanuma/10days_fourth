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

	auto InitStageObj = [&](int index, const std::string& modelKey, const Cygnus::Float3& offsetPos = { 0.0f,0.0f,0.0f }, float offsetScale = 1.0f)
		{
			stageObjects_[index] = std::make_unique<Cygnus::Object3D>();
			stageObjects_[index]->model_ = &Cygnus::ModelManager::GetInstance()->GetModel(modelKey);
			stageObjects_[index]->transform_.translate_ = selectObjects_[index]->transform_.translate_ + offsetPos;
			stageObjects_[index]->transform_.scale_ = stageObjects_[index]->transform_.scale_ * offsetScale;
			stageObjects_[index]->transform_.rotate_.y = std::numbers::pi_v<float>;
		};

	for (size_t i = 0; i < stageObjects_.size(); ++i)
	{
		InitStageObj(i, "Sphinx", { 0.0f, 4.0f, 9.0f }, 1.5f);
	}
}

void SelectObjectManager::Update(float deltaTime)
{
	deltaTime_ = deltaTime;

	FloatingObj();

	playerObject_->UpdateMatrix();

	// すべてのオブジェクトの行列を更新
	for (auto& obj : selectObjects_)
	{
		if (obj) // nullptr チェック
		{
			obj->UpdateMatrix();
		}
	}
	for (auto& obj : stageObjects_)
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
	for (auto& obj : stageObjects_)
	{
		if (obj) // nullptr チェック
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

void SelectObjectManager::FloatingObj()
{
	for (size_t i = 0; i < selectObjects_.size(); ++i)
	{
		if (i != currentStage_)
		{
			stageObjects_[i]->transform_.translate_.y = 4.0f;
			stageObjects_[i]->transform_.rotate_.y = std::numbers::pi_v<float>;
		}
	}
	static bool isUP = true;
	if(floatingTimer_ < kFloatingTime_ && isUP)
	{
		floatingTimer_ += deltaTime_;
		if (floatingTimer_ >= kFloatingTime_) 
		{
			isUP = false;
			floatingTimer_ = 0.0f;
		}
	}
	else
	{
		floatingTimer_ += deltaTime_;
		if (floatingTimer_ >= kFloatingTime_)
		{
			isUP = true;
			floatingTimer_ = 0.0f;
		}
	}
	if(isUP)
	{
		stageObjects_[currentStage_]->transform_.translate_.y = std::lerp(3.0f, 6.0f, floatingTimer_ / kFloatingTime_);
	}
	else
	{
		stageObjects_[currentStage_]->transform_.translate_.y = std::lerp(6.0f, 3.0f, floatingTimer_ / kFloatingTime_);
	}
	stageObjects_[currentStage_]->transform_.rotate_.y += (std::numbers::pi_v<float> / 6.0f) * deltaTime_;
}
