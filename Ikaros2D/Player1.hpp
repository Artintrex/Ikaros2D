#pragma once
#include "IkarosCore.h"

class Player1 : public MonoBehavior {
public:
	GameObject* player1;
	Texture* PlayerTex[10];
	Sprite* PlayerSpr[10];
	Renderer* p1rend;

	GameObject* floor;

	void Awake() {

		

	}

	void Start() {

		player1 = new GameObject("Player1");

		PlayerTex[0] = Texture::LoadTexture("player1", "Assets/Textures/player1t.png");
		PlayerTex[1] = Texture::LoadTexture("player2", "Assets/Textures/player2t.png");
		PlayerTex[2] = Texture::LoadTexture("player3", "Assets/Textures/player3t.png");
		PlayerTex[3] = Texture::LoadTexture("player4", "Assets/Textures/player4t.png");
		PlayerTex[4] = Texture::LoadTexture("player5", "Assets/Textures/player5t.png");
		PlayerTex[5] = Texture::LoadTexture("player6", "Assets/Textures/player6t.png");
		PlayerTex[6] = Texture::LoadTexture("player7", "Assets/Textures/player7t.png");
		PlayerTex[7] = Texture::LoadTexture("player8", "Assets/Textures/player8t.png");
		PlayerTex[8] = Texture::LoadTexture("player9", "Assets/Textures/player9t.png");


		p1rend = player1->AddComponent<Renderer>();


		for (int player_shin = 0; player_shin < 9; player_shin++)
		{

			PlayerSpr[player_shin] = new Sprite();

			PlayerSpr[player_shin]->texture = PlayerTex[player_shin];


			PlayerSpr[player_shin]->GenereteSprite();
		}

		player1->transform->Scale(0.3, 0.3, 0.3);
		p1rend->sprite = PlayerSpr[0];
		floor = new GameObject("floor");
		Texture* floorT = Texture::LoadTexture("TestImage", "Assets/Textures/floor.png");
		Sprite* flsprite = new Sprite();

		flsprite->texture = floorT;
		Renderer* flrend = floor->AddComponent<Renderer>();
		flrend->sprite = flsprite;
		flsprite->GenereteSprite();

		floor->transform->Scale(1, 1, 1);
		floor->transform->position = Vector3(-500, 59, 0);
	}
	int cnt = 0;
	float Timer = 0.5;
	void Update() {
		p1rend->sprite = PlayerSpr[cnt];
		if (Timer < 0) {
			cnt++;
			Timer = 0.18;
		}
		Timer -= Time.DeltaTime;
		if (cnt > 8) cnt = 0;

	}

};