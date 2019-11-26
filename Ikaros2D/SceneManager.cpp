#include "SceneManager.h"
#include "IkarosCore.h"
#include "GameHeader.h"

//IMPORTANT!
//
//
//Add your classes here so scene loader can recognize them
REGISTER_COMPONENT(MyScript)
REGISTER_COMPONENT(Player1)

//
//



Scene SceneManager::scene; //Create a default scene file then set it to active scene first.

Scene SceneManager::GetActiveScene() {
	return scene;
}

void SceneManager::LoadScene(std::string Path) {
	//read scene file and call factory functions to instantiate objects
}

void SceneManager::UnloadScene() {
	Object::ReleaseObjects();
}