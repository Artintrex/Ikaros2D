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

	std::vector <Sprite*> RunningSprite;
	std::vector <Sprite*> RunningSpriteWithSword;
	std::vector <Sprite*> AttackSprite;
	std::vector <Sprite*> IdleSprite;

	void Awake() {
		//Load running animation without weapon
		for (int i = 0; i < 6; i++)
		{
			std::string textureName = "player_running_test_" + std::to_string(i + 1);
			std::string path = "Assets/Textures/Player/" + textureName + ".png";

			RunningSprite.push_back(new Sprite(textureName + "Sprite", Texture::LoadTexture(textureName, path)));
			RunningSprite[i]->doubleSided = true;
			RunningSprite[i]->GenereteSprite(textureName);
		}

		//Load Running animation with weapon
		for (int i = 0; i < 6; i++)
		{
			std::string textureName = "player_running_sword_test_" + std::to_string(i + 1);
			std::string path = "Assets/Textures/Player/" + textureName + ".png";

			RunningSpriteWithSword.push_back(new Sprite(textureName + "Sprite", Texture::LoadTexture(textureName, path)));
			RunningSpriteWithSword[i]->doubleSided = true;
			RunningSpriteWithSword[i]->GenereteSprite(textureName);
		}

		//Load Attack animation with weapon
		for (int i = 0; i < 3; i++)
		{
			std::string textureName = "player_attack_sword_test_" + std::to_string(i + 1);
			std::string path = "Assets/Textures/Player/" + textureName + ".png";

			AttackSprite.push_back(new Sprite(textureName + "Sprite", Texture::LoadTexture(textureName, path)));
			AttackSprite[i]->doubleSided = true;
			AttackSprite[i]->GenereteSprite(textureName);
		}

		//Load idle animation
		for (int i = 0; i < 1; i++)
		{
			std::string textureName = "player_idle_test_" + std::to_string(i + 1);
			std::string path = "Assets/Textures/Player/" + textureName + ".png";

			AttackSprite.push_back(new Sprite(textureName + "Sprite", Texture::LoadTexture(textureName, path)));
			AttackSprite[i]->doubleSided = true;
			AttackSprite[i]->GenereteSprite(textureName);
		}
	}

	void Start() {
		player1 = new GameObject("Player1");
		Player1* gP1 = player1->AddComponent<Player1>();
		gP1->RunningSprite = RunningSprite;
		gP1->RunningSpriteWithSword = RunningSpriteWithSword;
		gP1->AttackSprite = AttackSprite;
		gP1->IdleSprite = IdleSprite;

		player2 = new GameObject("Player2");
		Player2* gP2 = player2->AddComponent<Player2>();
		gP2->RunningSprite = RunningSprite;
		gP2->RunningSpriteWithSword = RunningSpriteWithSword;
		gP2->AttackSprite = AttackSprite;
		gP2->IdleSprite = IdleSprite;
	}

	void Update() {

	}
};