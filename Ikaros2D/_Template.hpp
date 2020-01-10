#pragma once
#include "IkarosCore.h"

class Template : public MonoBehavior {
public:
	Template() {
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