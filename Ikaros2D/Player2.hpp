#pragma once
#include "IkarosCore.h"
#include "Javelin.hpp"

class Player2 : public MonoBehavior {
public:
	GameObject* player2;
	RigidBody* rb_player2;
	Texture* PlayerTex[9];
	Sprite* PlayerSpr[9];
	Renderer* p2rend;

	Texture* texSpear;
	Sprite* sprSpear;

	void Awake() {



	}

	void Start() {

		player2 = new GameObject("Player2");

		PlayerTex[0] = Texture::LoadTexture("player1", "Assets/Textures/player2t.png");
		PlayerTex[1] = Texture::LoadTexture("player2", "Assets/Textures/player2t.png");
		PlayerTex[2] = Texture::LoadTexture("player3", "Assets/Textures/player3t.png");
		PlayerTex[3] = Texture::LoadTexture("player4", "Assets/Textures/player4t.png");
		PlayerTex[4] = Texture::LoadTexture("player5", "Assets/Textures/player5t.png");
		PlayerTex[5] = Texture::LoadTexture("player6", "Assets/Textures/player6t.png");
		PlayerTex[6] = Texture::LoadTexture("player7", "Assets/Textures/player7t.png");
		PlayerTex[7] = Texture::LoadTexture("player8", "Assets/Textures/player8t.png");
		PlayerTex[8] = Texture::LoadTexture("player9", "Assets/Textures/player9t.png");


		p2rend = player2->AddComponent<Renderer>();

		texSpear = Texture::LoadTexture("SpearTexture", "Assets/Textures/javelin.png");
		sprSpear = new Sprite();
		sprSpear->texture = texSpear;
		sprSpear->GenereteSprite();

		for (int player_shin = 0; player_shin < 9; player_shin++)
		{

			PlayerSpr[player_shin] = new Sprite();

			PlayerSpr[player_shin]->texture = PlayerTex[player_shin];


			PlayerSpr[player_shin]->GenereteSprite();
		}

		player2->transform->Scale(0.6, 0.6, 0.6);
		p2rend->sprite = PlayerSpr[0];

		rb_player2 = player2->AddComponent<RigidBody>();
		rb_player2->SetType(b2_dynamicBody);
		rb_player2->rigidbody->SetFixedRotation(true);
		rb_player2->AddBoxCollider(PlayerSpr[0]->size * 0.6f);
		rb_player2->transform->Translate(1.0f, 1.0f, 1.0f);
	}
	int cnt = 0;
	float Timer = 0.5;
	int direction = 1;
	void Update() {
		p2rend->sprite = PlayerSpr[cnt];
		if (Timer < 0) {
			cnt++;
			Timer = 0.5;
		}
		Timer -= Time.DeltaTime * abs(rb_player2->velocity.x);
		if (cnt > 8) cnt = 0;

		if (GetKey(DIK_LEFT)) {
			rb_player2->AddForce(Vector2(-200, 0), Force);
			player2->transform->scale = Vector3(-0.6, 0.6, 0.6);
			direction = -1;
		}

		if (GetKey(DIK_RIGHT)) {
			rb_player2->AddForce(Vector2(200, 0), Force);
			player2->transform->scale = Vector3(0.6, 0.6, 0.6);
			direction = 1;
		}
		if (GetKeyDown(DIK_UP)) {
			rb_player2->AddForce(Vector2(0, 300), Impulse);
		}

		if (GetKeyDown(DIK_RSHIFT)) {
			new Javelin(Vector2(player2->transform->position.x, player2->transform->position.y), direction, sprSpear);
		}
	}

};