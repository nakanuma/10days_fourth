#include "ParticleEditorSystem.h"

// c++
#include <filesystem>

// Engine
#include "ImguiWrapper.h"
#include "ParticleEffect/ParticleEffectManager.h"

// Application
#include <src/Game/Particles/Particle.h>
#include <src/Game/Particles/ParticleStrage.h>
#include <src/Util/JsonUtil.h>

#undef min
#undef max

namespace fs = std::filesystem;

void ParticleEditorSystem::Initialize() {
	//暗色背景用モデルの生成
	object_ = std::make_unique<Cygnus::Object3D>();
	object_->model_ = &Cygnus::ModelManager::GetInstance()->GetModel("BlackPlane");
	object_->transform_.scale_ = { 100.0f, 100.0f, 100.0f };
	object_->transform_.translate_ = { 0.0f, 0.0f, 100.0f };
	object_->transform_.rotate_ = { 0.0f, 3.14f, 0.0f };

}

void ParticleEditorSystem::Update() {
	object_->UpdateMatrix();
}

void ParticleEditorSystem::Draw() {
	if (isDrawObject_) {
		object_->Draw();
	}

}

void ParticleEditorSystem::Debug() {
#ifdef _DEBUG
	//ウィンドウ1(パラメータ)
	{
		ImGui::SetNextWindowPos(ImVec2(0, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(300, 640), ImGuiCond_Always);
		ImGui::Begin("Parameter", nullptr,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize);

		//定数データ
		ConstantsData constantsData = ParticleStrage::GetInstance()->GetEffects()[targetEffectName_]->GetConstantsData();

		//編集

		//初期トランスフォーム
		if (ImGui::CollapsingHeader("Initial Transform")) {
			ImGui::DragFloat3("Scale Min", &constantsData.minScale.x, 0.1f);

			//区切り
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::DragFloat3("Scale Max", &constantsData.maxScale.x, 0.1f);

			constantsData.minScale.x = std::min(constantsData.minScale.x, constantsData.maxScale.x);
			constantsData.minScale.y = std::min(constantsData.minScale.y, constantsData.maxScale.y);
			constantsData.minScale.z = std::min(constantsData.minScale.z, constantsData.maxScale.z);

			constantsData.maxScale.x = std::max(constantsData.maxScale.x, constantsData.minScale.x);
			constantsData.maxScale.y = std::max(constantsData.maxScale.y, constantsData.minScale.y);
			constantsData.maxScale.z = std::max(constantsData.maxScale.z, constantsData.minScale.z);
		}

		//更新トランスフォーム
		if (ImGui::CollapsingHeader("Update Transform")) {
			ImGui::DragFloat3("Velocity Min", &constantsData.minVelocity.x, 0.1f);
			ImGui::DragFloat3("Velocity Max", &constantsData.maxVelocity.x, 0.1f);

			constantsData.minVelocity.x = std::min(constantsData.minVelocity.x, constantsData.maxVelocity.x);
			constantsData.minVelocity.y = std::min(constantsData.minVelocity.y, constantsData.maxVelocity.y);
			constantsData.minVelocity.z = std::min(constantsData.minVelocity.z, constantsData.maxVelocity.z);

			constantsData.maxVelocity.x = std::max(constantsData.maxVelocity.x, constantsData.minVelocity.x);
			constantsData.maxVelocity.y = std::max(constantsData.maxVelocity.y, constantsData.minVelocity.y);
			constantsData.maxVelocity.z = std::max(constantsData.maxVelocity.z, constantsData.minVelocity.z);

			//区切り
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::DragFloat3("Rotation Speed Min", &constantsData.minRotationSpeed.x, 0.1f);
			ImGui::DragFloat3("Rotation Speed Max", &constantsData.maxRotationSpeed.x, 0.1f);

			constantsData.minRotationSpeed.x = std::min(constantsData.minRotationSpeed.x, constantsData.maxRotationSpeed.x);
			constantsData.minRotationSpeed.y = std::min(constantsData.minRotationSpeed.y, constantsData.maxRotationSpeed.y);
			constantsData.minRotationSpeed.z = std::min(constantsData.minRotationSpeed.z, constantsData.maxRotationSpeed.z);

			constantsData.maxRotationSpeed.x = std::max(constantsData.maxRotationSpeed.x, constantsData.minRotationSpeed.x);
			constantsData.maxRotationSpeed.y = std::max(constantsData.maxRotationSpeed.y, constantsData.minRotationSpeed.y);
			constantsData.maxRotationSpeed.z = std::max(constantsData.maxRotationSpeed.z, constantsData.minRotationSpeed.z);

			//区切り
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			ImGui::DragFloat3("Scale Speed Min", &constantsData.minScaleSpeed.x, 0.1f);
			ImGui::DragFloat3("Scale Speed Max", &constantsData.maxScaleSpeed.x, 0.1f);

			constantsData.minScaleSpeed.x = std::min(constantsData.minScaleSpeed.x, constantsData.maxScaleSpeed.x);
			constantsData.minScaleSpeed.y = std::min(constantsData.minScaleSpeed.y, constantsData.maxScaleSpeed.y);
			constantsData.minScaleSpeed.z = std::min(constantsData.minScaleSpeed.z, constantsData.maxScaleSpeed.z);

			constantsData.maxScaleSpeed.x = std::max(constantsData.maxScaleSpeed.x, constantsData.minScaleSpeed.x);
			constantsData.maxScaleSpeed.y = std::max(constantsData.maxScaleSpeed.y, constantsData.minScaleSpeed.y);
			constantsData.maxScaleSpeed.z = std::max(constantsData.maxScaleSpeed.z, constantsData.minScaleSpeed.z);
		}

		//加速度
		if (ImGui::CollapsingHeader("Acceleration")) {
			ImGui::DragFloat3("Acceleration Min", &constantsData.minAccerelation.x, 0.1f);
			ImGui::DragFloat3("Acceleration Max", &constantsData.maxAccerelation.x, 0.1f);

			constantsData.minAccerelation.x = std::min(constantsData.minAccerelation.x, constantsData.maxAccerelation.x);
			constantsData.minAccerelation.y = std::min(constantsData.minAccerelation.y, constantsData.maxAccerelation.y);
			constantsData.minAccerelation.z = std::min(constantsData.minAccerelation.z, constantsData.maxAccerelation.z);

			constantsData.maxAccerelation.x = std::max(constantsData.maxAccerelation.x, constantsData.minAccerelation.x);
			constantsData.maxAccerelation.y = std::max(constantsData.maxAccerelation.y, constantsData.minAccerelation.y);
			constantsData.maxAccerelation.z = std::max(constantsData.maxAccerelation.z, constantsData.minAccerelation.z);
		}

		//色
		if (ImGui::CollapsingHeader("Color")) {
			ImGui::ColorEdit4("Start Min Color", &constantsData.startMinColor.x);
			ImGui::ColorEdit4("Start Max Color", &constantsData.startMaxColor.x);
			ImGui::ColorEdit4("End Min Color", &constantsData.endMinColor.x);
			ImGui::ColorEdit4("End Max Color", &constantsData.endMaxColor.x);
		}

		//寿命
		if (ImGui::CollapsingHeader("LifeTime")) {
			ImGui::DragFloat("LifeTime Min", &constantsData.minLifeTime, 0.1f);
			ImGui::DragFloat("LifeTime Max", &constantsData.maxLifeTime, 0.1f);

			constantsData.minLifeTime = std::min(constantsData.minLifeTime, constantsData.maxLifeTime);
			constantsData.maxLifeTime = std::max(constantsData.maxLifeTime, constantsData.minLifeTime);
		}

		//その他
		if (ImGui::CollapsingHeader("Other")) {
			ImGui::Checkbox("Is Billboard", &constantsData.isBillboard);

			const char* blendModeItems[] = {
				"None",
				"Normal",
				"Add",
				"Subtract",
				"Multiply",
				"Screen",
				"Alpha"
			};

			int currentBlendMode = static_cast<int>(constantsData.blendMode);

			if (ImGui::Combo("Blend Mode", &currentBlendMode, blendModeItems, IM_ARRAYSIZE(blendModeItems))) {
				constantsData.blendMode = static_cast<Cygnus::BlendMode>(currentBlendMode);
			}
		}

		//定数データのセット
		ParticleStrage::GetInstance()->GetEffects()[targetEffectName_]->SetConstantsData(constantsData);

		ImGui::End();
	}
	//ウィンドウ2(制御)
	{
		ImGui::SetNextWindowPos(ImVec2(1280 - 300, 0), ImGuiCond_Always);
		ImGui::SetNextWindowSize(ImVec2(300, 640), ImGuiCond_Always);
		ImGui::Begin("Control", nullptr,
			ImGuiWindowFlags_NoMove |
			ImGuiWindowFlags_NoResize);

		//発生制御
		{
			//無限エミット
			ImGui::Checkbox("Infinite Emit", &isInfiniteEmit_);
			if (isInfiniteEmit_) {
				//無限エミットがONになったらエミット開始
				Cygnus::ParticleEffectManager::GetInstance()->Emit(targetEffectName_, Cygnus::Float3(0, 0, 0), 1, Cygnus::Float3(0, 0, 0), 0.0f);
			}

			//発生数
			ImGui::DragInt("Count", reinterpret_cast<int*>(&count_), 1, 1, 30);
			//エミット
			if (ImGui::Button("Emit")) {
				Cygnus::ParticleEffectManager::GetInstance()->Emit(targetEffectName_, Cygnus::Float3(0, 0, 0), count_);
			}
		}

		//読み込み
		{
			if (ImGui::Button("Load")) {
				// フォルダ読み直し
				files.clear();

				for (auto& p : fs::directory_iterator("resources/Particles")) {
					if (p.is_regular_file()) {
						files.push_back(p.path().filename().string());
					}
				}

				ImGui::OpenPopup("FileBrowser");
			}

			if (ImGui::BeginPopup("FileBrowser")) {
				for (auto& file : files) {
					if (ImGui::Selectable(file.c_str())) {
						//選択したファイルの読み込み
						std::string name = file;
						name = std::filesystem::path(name).stem().string();
						//制御対象の切り替え
						targetEffectName_ = name;
						//保存対象の切り替え
						saveFileName_ = name;
						//今のパーティクルにJSONデータを読み込ませる
						ParticleStrage::GetInstance()->GetEffects()[targetEffectName_]->LoadJsonData(name);

						ImGui::CloseCurrentPopup();
					}
				}

				ImGui::EndPopup();
			}
		}

		//保存
		{
			//保存対象の入力
			char buffer[256];
			strcpy_s(buffer, saveFileName_.c_str());
			if (ImGui::InputText("Save File Name", buffer, sizeof(buffer))) {
				saveFileName_ = buffer;
			}

			if (ImGui::Button("Save")) {
				//定数データの取得
				ConstantsData constantsData = ParticleStrage::GetInstance()->GetEffects()[targetEffectName_]->GetConstantsData();
				//jsonデータの作成
				json jsonData;
				jsonData["minScale"] = {
					constantsData.minScale.x,
					constantsData.minScale.y,
					constantsData.minScale.z
				};
				jsonData["maxScale"] = {
					constantsData.maxScale.x,
					constantsData.maxScale.y,
					constantsData.maxScale.z
				};
				jsonData["minVelocity"] = {
					constantsData.minVelocity.x,
					constantsData.minVelocity.y,
					constantsData.minVelocity.z
				};
				jsonData["maxVelocity"] = {
					constantsData.maxVelocity.x,
					constantsData.maxVelocity.y,
					constantsData.maxVelocity.z
				};
				jsonData["startMinColor"] = {
					constantsData.startMinColor.x,
					constantsData.startMinColor.y,
					constantsData.startMinColor.z,
					constantsData.startMinColor.w
				};
				jsonData["startMaxColor"] = {
					constantsData.startMaxColor.x,
					constantsData.startMaxColor.y,
					constantsData.startMaxColor.z,
					constantsData.startMaxColor.w
				};
				jsonData["endMinColor"] = {
					constantsData.endMinColor.x,
					constantsData.endMinColor.y,
					constantsData.endMinColor.z,
					constantsData.endMinColor.w
				};
				jsonData["endMaxColor"] = {
					constantsData.endMaxColor.x,
					constantsData.endMaxColor.y,
					constantsData.endMaxColor.z,
					constantsData.endMaxColor.w
				};
				jsonData["minLifeTime"] = constantsData.minLifeTime;
				jsonData["maxLifeTime"] = constantsData.maxLifeTime;
				jsonData["minRotationSpeed"] = {
					constantsData.minRotationSpeed.x,
					constantsData.minRotationSpeed.y,
					constantsData.minRotationSpeed.z
				};
				jsonData["maxRotationSpeed"] = {
					constantsData.maxRotationSpeed.x,
					constantsData.maxRotationSpeed.y,
					constantsData.maxRotationSpeed.z
				};
				jsonData["minAccerelation"] = {
					constantsData.minAccerelation.x,
					constantsData.minAccerelation.y,
					constantsData.minAccerelation.z
				};
				jsonData["maxAccerelation"] = {
					constantsData.maxAccerelation.x,
					constantsData.maxAccerelation.y,
					constantsData.maxAccerelation.z
				};
				jsonData["isBillboard"] = constantsData.isBillboard;
				jsonData["blendMode"] = static_cast<int>(constantsData.blendMode);


				if (JsonUtil::CheckJson(saveFileName_, "resources/Particles")) {
					//保存対象が存在している場合は上書き保存
					JsonUtil::EditJson("resources/Particles/" + saveFileName_, jsonData);

				}
				else {
					//保存対象が存在しない場合は新規作成
					JsonUtil::CreateJson(saveFileName_, "resources/Particles", jsonData);
				}

			}


		}

		//背景の変更
		{
			ImGui::Checkbox("BackColorChange", &isDrawObject_);
		}


		ImGui::End();
	}
	//ウィンドウ3(環境)
	{

	}

#endif
}