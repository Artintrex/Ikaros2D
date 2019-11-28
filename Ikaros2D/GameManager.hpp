#pragma once
#include "IkarosCore.h"
#include "Title.hpp"

class GameManager : public MonoBehavior {
public:
	enum Scene {
		sTitle,
		sAlpha
	};
	int ActiveScene;
	bool SceneisLoaded;

	GameObject* Alpha;
	GameObject* objTitle;
	Title* mTitle;

	void Awake() {

	}

	void Start() {
		this->parent->AddComponent<Debug>();

		LoadAlpha();
	}
	void Update() {

	}

	void ChangeScene(Scene scene) {
		if (ActiveScene == sAlpha)UnLoadAlpha();
		else if (ActiveScene == sTitle)UnLoadTitle();
	
		if (scene == sAlpha) LoadAlpha();
		else if (scene == sTitle) LoadTitle();
	}

private:
	void LoadAlpha() {
		ActiveScene = sAlpha;

		Alpha = new GameObject();
		Alpha->AddComponent<SceneAlpha>(); //run start here for now
	}

	void LoadTitle() {
		ActiveScene = sTitle;
	}

	void UnLoadAlpha() {
	}

	void UnLoadTitle() {
	}
};