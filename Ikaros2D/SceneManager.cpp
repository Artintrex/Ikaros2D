#include "SceneManager.h"
#include "IkarosCore.h"
#include "GameHeader.h"

//IMPORTANT!
//
//
//Add your classes here so scene loader can recognize them
REGISTER_COMPONENT(Debug)
REGISTER_COMPONENT(Player1)
REGISTER_COMPONENT(Player2)
REGISTER_COMPONENT(GameManager)
REGISTER_COMPONENT(SceneAlpha)
REGISTER_COMPONENT(Title)

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