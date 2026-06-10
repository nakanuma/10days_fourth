#include "SelectObjectManager.h"
#include <ModelManager.h>
#include <src/Game/GameData/GameDataManager.h>
#include <Easing.h>

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

			// 生成直後の初期スケールを記憶しておく
			stageInitialScales_[index] = stageObjects_[index]->transform_.scale_;
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

	UpdateJumpAnimation();
	UpdateStageReaction();

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
}

void SelectObjectManager::StartJumpToStage()
{
	isJumping_ = true;
	jumpTimer_ = 0.0f;

	// ジャンプの開始位置は「現在のプレイヤーの位置」
	jumpStartPos_ = playerObject_->transform_.translate_;

	// ジャンプの目標位置は「現在選択されているステージオブジェクトの位置」
	jumpEndPos_ = stageObjects_[currentStage_]->transform_.translate_;

	jumpScaleF3_ = playerObject_->transform_.scale_;
}

bool SelectObjectManager::UpdateJumpAnimation()
{
	if (!isJumping_)
	{
		return false; // ジャンプ中でなければ即座に終了
	}

	// タイマーを進める
	jumpTimer_ += deltaTime_;

	// 進行度を 0.0 ～ 1.0 の範囲で計算 (t)
	float t = std::clamp(jumpTimer_ / kJumpDuration_, 0.0f, 1.0f);

	// 飛び込み中にプレイヤーを縮小させる
	playerObject_->transform_.scale_ = jumpScaleF3_ * (1.0f - t);

	// 飛び込み中にプレイヤーを回転させる
	playerObject_->transform_.rotate_.y -= (std::numbers::pi_v<float> *2.0f) * deltaTime_;

	t = Cygnus::Easing::EaseOutQuad(t);

	// --- 座標の計算 ---
	// XとZは開始地点から目標地点まで直線的に移動 (Lerp)
	playerObject_->transform_.translate_.x = std::lerp(jumpStartPos_.x, jumpEndPos_.x, t);
	playerObject_->transform_.translate_.z = std::lerp(jumpStartPos_.z, jumpEndPos_.z, t);

	// Y軸は、直線的な移動に加えて、sin波を使って上に凸の弧を足し合わせる
	float baseY = std::lerp(jumpStartPos_.y, jumpEndPos_.y, t);
	float arcY = std::sin(t * std::numbers::pi_v<float>) * kJumpHeight_;
	playerObject_->transform_.translate_.y = baseY + arcY;

	// 行列を更新
	playerObject_->UpdateMatrix();

	// tが1.0に達したら演出完了
	if (t >= 1.0f)
	{
		isJumping_ = false;
		return true; // 完了を知らせる
	}

	return false;
}

void SelectObjectManager::StartStageReaction()
{
	isStageReacting_ = true;
	stageReactTimer_ = 0.0f;
}

bool SelectObjectManager::UpdateStageReaction()
{
	if (!isStageReacting_)
	{
		return false;
	}

	stageReactTimer_ += deltaTime_;
	float t = std::clamp(stageReactTimer_ / kReactionDuration_, 0.0f, 1.0f);

	// プルンとした弾力を出すための波形計算 (減衰振動)
	// 3.0f * PI で「潰れる -> 伸びる -> 軽く潰れる -> 戻る」という1往復半の動きになります
	// (1.0f - t) を掛けることで、時間が経つにつれて揺れが収束します
	float wave = std::sin(t * std::numbers::pi_v<float> *3.0f) * (1.0f - t);

	// 現在選択されているステージの参照と初期スケールを取得
	auto& targetObj = stageObjects_[currentStage_];
	const auto& baseScale = stageInitialScales_[currentStage_];

	// --- スケールの計算 ---
	// wave がプラスのとき：Y軸が縮み（潰れる）、XZ平面が広がる
	// wave がマイナスのとき：Y軸が伸び（反動）、XZ平面が縮む
	float scaleY = 1.0f - (wave * kSquashFactor_);
	float scaleXZ = 1.0f + (wave * kStretchFactor_);

	targetObj->transform_.scale_.x = baseScale.x * scaleXZ;
	targetObj->transform_.scale_.y = baseScale.y * scaleY;
	targetObj->transform_.scale_.z = baseScale.z * scaleXZ;

	// 行列の更新
	targetObj->UpdateMatrix();

	// 演出終了判定
	if (t >= 1.0f)
	{
		isStageReacting_ = false;

		// 念のため完全に初期スケールに戻す
		targetObj->transform_.scale_ = baseScale;
		targetObj->UpdateMatrix();

		return true; // 演出完了
	}

	return false;
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
			GameDataManager::GetInstance()->SetTargetStage(currentStage_);
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
			GameDataManager::GetInstance()->SetTargetStage(currentStage_);
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
