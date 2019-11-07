#pragma once
#include "GameHeader.h"
#include "IkarosCore.h"


class MyScript : MonoBehavior {


	void Awake() {
		GameObject* test = new GameObject();
		test->AddComponent<MyScript>();
	}

	void Start() {

	}

	void Update() {

	}
};