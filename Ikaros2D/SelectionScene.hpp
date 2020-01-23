#pragma once
#include "IkarosCore.h"

class SelectionScene : public MonoBehavior {
public:
	SelectionScene() {
		type = typeid(*this).name();
		mb_init();
	}

	void Awake() {

	}

	void Start() {

	}

	void Update() {

	}
};