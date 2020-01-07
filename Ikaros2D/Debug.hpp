#pragma once
#include "IkarosCore.h"
#include "SceneManager.h"

class Debug : public MonoBehavior {
public:
	Debug() {
		type = typeid(*this).name(); if (isAwake[type] == false)Awake(); isAwake[type] = true; if (SceneManager::isLoaded == true)Start();
	}

	void Awake() {

	}

	void Start() {
		/*
	
		std::cout << "Factory Register" << "\n";
		for (auto it = Component::factories.cbegin(); it != Component::factories.cend(); ++it)
		{
			std::cout << it->first << " " << it->second << "\n";
		}

		std::cout << SceneManager::GetActiveScene().path << "\n";
		*/

		Texture::LoadTexture("null", "Assets/Textures/null.png");

		/*
		camObj2 = new GameObject("Camera2");
		cam = camObj2->AddComponent<Camera>();
		camObj2->transform->Translate(0, 2, -5000);
		cam->rect.X = 0.5;
		cam->rect.W = 0.5;
		cam->rect.H = 0.5;
		cam->SetProjection();
		*/

	}

	float Timer = 0;
	void Update() {
		Timer -= Time.DeltaTime;

		if (Timer < 0) {
			//ListObject();
			Timer = 120;
		}
	}


	void ListObject() {
		std::cout << "\n" << "--------Object List--------" << "\n" << "\n" << "\n";
		for (auto p : Object::ObjectList)
		{
			std::cout << p->name << " " << p <<"\n";
		}
		std::cout << "\n" << "Number of Objects:  " << Object::ObjectList.size() << "\n";
	}
};