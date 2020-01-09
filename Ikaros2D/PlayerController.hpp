#pragma once
#include "IkarosCore.h"
#include "Player1.hpp"

class PlayerController : public MonoBehavior {
public:
	PlayerController() {
		type = typeid(*this).name();
		mb_init();
	}

	GameObject* player1;
	GameObject* player2;

	void Awake() {
		for (int i = 0; i < 9; i++)
		{
			std::string textureName = "player_running" + std::to_string(i + 1);
			std::string path = "Assets/Textures/Player/" + textureName + ".png";

			Texture::LoadTexture(textureName, path);
		}
	}

	void Start() {
		player1 = new GameObject("Player1");
		player1->AddComponent<Player1>();

		player2 = new GameObject("Player2");
		player2->AddComponent<Player2>();
	}

	void Update() {

	}
};