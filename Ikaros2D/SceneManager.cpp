#include "SceneManager.h"
#include "IkarosCore.h"
#include "GameHeader.h"

//Add your classes here so scene loader can recognize them
REGISTER_COMPONENT(MyScript)
REGISTER_COMPONENT(HuJian)

Scene SceneManager::scene; //Create a default scene file then set it to active scene first.

Scene SceneManager::GetActiveScene() {
	return scene;
}

void SceneManager::LoadScene(std::string Path) {

}

void SceneManager::UnloadScene() {
	Object::ReleaseObjects();
}