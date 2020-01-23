#include "SceneManager.h"
#include "IkarosCore.h"
#include "GameHeader.h"
#include <fstream>
#include <sstream>
#include <iostream>

//IMPORTANT!
//
//
//Add your classes here so scene loader can recognize them
REGISTER_COMPONENT(Debug)
REGISTER_COMPONENT(Player1)
REGISTER_COMPONENT(Player2)
REGISTER_COMPONENT(PlayerController)
REGISTER_COMPONENT(GameManager)
REGISTER_COMPONENT(SceneAlpha)
REGISTER_COMPONENT(Title)
REGISTER_COMPONENT(Result)
REGISTER_COMPONENT(SelectionScene)
//
//

Scene SceneList[4] = { 
{"Title", "Assets/SceneData/Scene0.scene"},
{"DebugLevel", "Assets/SceneData/Scene1.scene"},
{"ResultScreen", "Assets/SceneData/Scene2.scene"},
{"SelectionScreen", "Assets/SceneData/Scene3.scene"}
};

void ReadSceneFile(std::string FilePath);

Scene SceneManager::scene;
bool SceneManager::isLoaded = false;

std::vector<MonoBehavior*> SceneManager::ActiveStarterList{};
std::vector<MonoBehavior*> SceneManager::ActiveAwakerList{};

void SceneManager::RunActiveInitilizer() {
	for (auto p : ActiveAwakerList) {
		p->Awake();
	}
	ActiveAwakerList.clear();
	for (auto p : ActiveStarterList) {
		p->Start();
	}
	ActiveStarterList.clear();
}

Scene SceneManager::GetActiveScene() {
	return scene;
}

void SceneManager::LoadScene(std::string Path) {
	if (isLoaded == false) {
		ReadSceneFile(Path);
		MonoBehavior::AwakeMonoBehaviorArray();
		MonoBehavior::StartMonoBehaviorArray();
		scene.name = Path;
		scene.path = Path;
		isLoaded = true;
	}
	else {
		SceneManager::UnloadScene();
		ReadSceneFile(Path);
		MonoBehavior::AwakeMonoBehaviorArray();
		MonoBehavior::StartMonoBehaviorArray();
		scene.name = Path;
		scene.path = Path;
		isLoaded = true;
	}
	Time.Start();
	std::cout << "Scene " << scene.name << " is loaded" << std::endl;
	std::cout << "Number of objects created: " << Object::ObjectList.size() << std::endl;
}

void SceneManager::LoadScene(int SceneNumber) {
	if (isLoaded == false) {
		ReadSceneFile(SceneList[SceneNumber].path);
		MonoBehavior::AwakeMonoBehaviorArray();
		MonoBehavior::StartMonoBehaviorArray();
		scene.name = SceneList[SceneNumber].name;
		scene.path = SceneList[SceneNumber].path;
		isLoaded = true;
	}
	else {
		SceneManager::UnloadScene();
		ReadSceneFile(SceneList[SceneNumber].path);
		MonoBehavior::AwakeMonoBehaviorArray();
		MonoBehavior::StartMonoBehaviorArray();
		scene.name = SceneList[SceneNumber].name;
		scene.path = SceneList[SceneNumber].path;
		isLoaded = true;
	}
	Time.Start();
	std::cout << "Scene" << SceneNumber << " " << scene.name << " is loaded" << std::endl;
	std::cout << "Number of objects created: " << Object::ObjectList.size() << std::endl;
}

void SceneManager::UnloadScene() {
	Object::ReleaseObjects();
	MonoBehavior::isAwake.clear();

	isLoaded = false;
}

std::vector<std::string> words;
void ParseLine(std::string line) {
	std::istringstream sline(line);
	std::string word;
	while (sline >> word) {
		sline.ignore(line.length(), ':');
		words.push_back(word);
	}
}

GameObject* ActiveGameObject = nullptr;
Object* ActiveObject = nullptr;
std::string ActiveObjectType;
bool newObject = true;

void ReadSceneFile(std::string FilePath) {
	std::ifstream file;
	file.open(FilePath);
	if (!file.is_open()) std::cout << "Failed to open file: "<< FilePath << std::endl;
	else{
		std::string line, activeObject;
		GameObject* activeParent = nullptr;
		while (std::getline(file, line))
		{
			if (!line.empty() && line.at(0) != '#') {

				ParseLine(line);
				if (words[0] == "---")newObject = true;

				//Seek next object type
				if (newObject == true) {
					if (words[0] == "GameObject:") {
						ActiveGameObject = new GameObject();
						ActiveObject = ActiveGameObject;
						ActiveObjectType = "GameObject";
						newObject == false;
					}
					else if (words[0] == "Transform:") {
						ActiveObject = ActiveGameObject->transform;
						ActiveObjectType = "Transform";
						newObject == false;
					}
					else {
						words[0].pop_back();
						ComponentFactory* factory = Component::factories[words[0]];
						if (factory != nullptr && ActiveGameObject != nullptr) {
							ActiveObject = factory->AddComponent(ActiveGameObject);
							ActiveObjectType = words[0];
							newObject == false;
						}
					}
				}
				//Check for object properties
				else {
					if (words[0] == "name:") {
						ActiveObject->name = words[1];
					}
					if (ActiveObjectType == "Transform" && words[0] == "position:") {
						ActiveGameObject->transform->position.x = std::stof(words[1]);
						ActiveGameObject->transform->position.y = std::stof(words[2]);
						ActiveGameObject->transform->position.z = std::stof(words[3]);
					}

					if (ActiveObjectType == "Transform" && words[0] == "rotation:") {
						ActiveGameObject->transform->rotation.x = std::stof(words[1]);
						ActiveGameObject->transform->rotation.y = std::stof(words[2]);
						ActiveGameObject->transform->rotation.z = std::stof(words[3]);
					}

					if (ActiveObjectType == "Transform" && words[0] == "scale:") {
						ActiveGameObject->transform->scale.x = std::stof(words[1]);
						ActiveGameObject->transform->scale.y = std::stof(words[2]);
						ActiveGameObject->transform->scale.z = std::stof(words[3]);
					}
				}
				words.clear();
			}
		}
		if (file.bad())std::cout << "Error while reading file: " << FilePath << std::endl;
	}
}