#pragma once
#include "IkarosCore.h"

class Template : public MonoBehavior {
public:
	Template() {
		type = typeid(*this).name(); if (isAwake[type] == false)Awake(); isAwake[type] = true; if(SceneManager::isLoaded == true)Start();
	}

	void Awake() {

	}

	void Start() {

	}

	void Update() {

	}
};