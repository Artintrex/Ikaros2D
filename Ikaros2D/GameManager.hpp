#pragma once
#include "IkarosCore.h"

class GameManager : public MonoBehavior {
public:
	enum Scene {
		sTitle,
		sAlpha
	};
	int ActiveScene;

	GameObject* Alpha;
	GameObject* mTitle;

	void Awake() {

	}

	void Start() {
		this->parent->AddComponent<Debug>();

		//LoadAlpha();
		GameObject* test = new GameObject();
		//test->AddComponent<Title>();
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
		Alpha->AddComponent<SceneAlpha>();
	}

	void LoadTitle() {
		ActiveScene = sTitle;
	}

	void UnLoadAlpha() {
	}

	void UnLoadTitle() {
	}
};