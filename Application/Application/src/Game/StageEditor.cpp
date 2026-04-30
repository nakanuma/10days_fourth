#include "StageEditor.h"
#include "externals/nlohmann/json.hpp"
#include "ImguiWrapper.h"

#include <iostream>
#include <string>
#include <sstream>

using namespace Cygnus;

#ifndef 初期化系統

void StageEditor::LoadJsonFile(const std::string& stageName) {

	nlohmann::json json;
	std::ifstream file;

	//ステージの名前
	file.open(stageName);

	//ファイルが開けない、そもそもない場合
	if (file.fail()) {
		return;
		assert(0);
	}

	//解凍処理
	file >> json;
	
	assert(json.is_object());
	assert(json.contains("name"));
	assert(json["name"].is_string());

	std::string name = json["name"].get<std::string>();

	if (name != "StageEditor") {
		assert(0);
	}

	for (auto& object : json["object"]) {
		
		GameObjectPosition gameObjectPos;
		//名前の読み込み
		gameObjectPos.name = object["objectName"];
		//座標位置の読み込み
		nlohmann::json position = object["position"];
		gameObjectPos.position.x = position[0];
		gameObjectPos.position.y = position[1];
		gameObjectPos.position.z = position[2];

		//当たり判定の読み込み
		nlohmann::json collider = object["collider"];
		gameObjectPos.collider.SetSize({ collider["size"][0] , collider["size"][1] , collider["size"][2]});
		gameObjectPos.collider.SetTag(collider["tag"]);

		//追加
		SpitObjects(gameObjectPos);

		gameObjectPositions_.push_back(gameObjectPos);
	}
}

#endif // !初期化系統

#ifndef 更新(デバッグ)

void StageEditor::Update() {
#ifdef _DEBUG

	ImGui::Begin("StageEditor");

	SettingStage();
	Save();

	ImGui::End();

#endif // _DEBUG
}

void StageEditor::SpitObjects(const GameObjectPosition& gameObjectPos) {
	//名前から生成物を判断する

	if (gameObjectPos.name == "Player") {
		std::unique_ptr<Player> object = std::make_unique<Player>();
		object->Initialize();
		players_.push_back(std::move(object));
	}
	else if (gameObjectPos.name == "Carrier") {
		std::unique_ptr<Carrier> object = std::make_unique<Carrier>();
		object->Initialize();
		Carriers_.push_back(std::move(object));
	}
}

void StageEditor::SettingStage() {
#ifdef _DEBUG

	if (isCreateNewObject_) {
		ImGui::Text("parameter");
		ImGui::InputText("name", objectName_.data(), IM_ARRAYSIZE(textureFileName));//オブジェクトの名前
		ImGui::DragFloat3("position", &newObject_.position.x);//座標位置
		ImGui::DragFloat3("colliderSize", &colliderSize.x);//当たり判定サイズ

		newObject_.collider.SetSize(colliderSize);//当たり判定サイズを設定

		//パラメータを設定できたら
		//オブジェクトの追加
		if (ImGui::Button("Add_Object")) {
			newObject_.name = objectName_.c_str();//名前
			newObject_.collider.SetTag(objectName_);//オブジェクトタグを設定
			gameObjectPositions_.push_back(newObject_);//導入
			isCreateNewObject_ = !isCreateNewObject_;//オブジェクトの追加完了
		}

		//キャンセル
		if (ImGui::Button("cancel")) {
			isCreateNewObject_ = !isCreateNewObject_;//オブジェクトの追加キャンセル
		}
	}
	else if (ImGui::Button("Create_Object")) {
		isCreateNewObject_ = !isCreateNewObject_;//オブジェクトの追加開始
	}

	//現在のデータを表示
	if (ImGui::TreeNode("now ObjectData")) {
		ImGui::Separator();//行分け
		for (auto& object : gameObjectPositions_) {
			ImGui::Text("position : %f,%f,%f", object.position.x, object.position.y, object.position.z);//位置
			ImGui::Text("colliderTag : %s", object.name.c_str());//タグ(名前)
			ImGui::Text("colliderSize : %f,%f,%f", object.collider.GetSize().x, object.collider.GetSize().y, object.collider.GetSize().z);//当たり判定のサイズ
			ImGui::Separator();//行分け
		}
		ImGui::TreePop();
	}

	//現在のデータをリセットする
	if (ImGui::Button("Reset Objects")) {
		gameObjectPositions_.clear();
	}

#endif // DEBUG
}


void StageEditor::Save() {
#ifdef _DEBUG

	//ステージの名前を設定
	ImGui::InputText("Stage Json Name", imGuiText_.data(), IM_ARRAYSIZE(textureFileName));
	std::string fileName = imGuiText_.c_str();

	//jsonを作成
	if (ImGui::Button("Create Json")) {
		nlohmann::json jsonFile;
		jsonFile["name"] = "StageEditor";

		uint32_t num = 0;
		for (auto& gameObject : gameObjectPositions_) {
			//オブジェクトクラスの名前
			jsonFile["object"][num]["objectName"] = gameObject.name;
			//設置位置
			jsonFile["object"][num]["position"][0] = gameObject.position.x;
			jsonFile["object"][num]["position"][1] = gameObject.position.y;
			jsonFile["object"][num]["position"][2] = gameObject.position.z;
			//タグ名
			jsonFile["object"][num]["collider"]["tag"] = gameObject.name.c_str();
			//当たり判定のサイズ
			jsonFile["object"][num]["collider"]["size"][0] = gameObject.collider.GetSize().x;
			jsonFile["object"][num]["collider"]["size"][1] = gameObject.collider.GetSize().y;
			jsonFile["object"][num]["collider"]["size"][2] = gameObject.collider.GetSize().z;
			num++;
		}

		//保存場所の設定
		std::string outputFileName = "resources/stageEditor/" + fileName + ".json";
		std::ofstream file(outputFileName);

		//保存
		if (file.is_open()) {
			file << jsonFile.dump(4);
			file.close();
		}
	}

#endif // DEBUG

}

#endif // !更新(デバッグ)
