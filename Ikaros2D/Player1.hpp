#pragma once
#include "IkarosCore.h"
class Player1 : public MonoBehavior {
public:
	Player1() {
		type = typeid(*this).name(); if (isAwake[type] == false)Awake(); isAwake[type] = true; if (SceneManager::isLoaded == true)Start();
	}
	GameObject* player1;
	RigidBody* rb_player1;
	Texture* PlayerTex[9];
	Sprite* PlayerSpr[9];
	Renderer* p1rend;

	Texture* texSpear;
	Sprite* sprSpear;

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

		texSpear = Texture::LoadTexture("SpearTexture", "Assets/Textures/javelin.png");
		sprSpear = new Sprite();
		sprSpear->texture = texSpear;
		sprSpear->GenereteSprite();

		p1rend = player1->AddComponent<Renderer>();



		for (int player_shin = 0; player_shin < 9; player_shin++)
		{

			PlayerSpr[player_shin] = new Sprite();

			PlayerSpr[player_shin]->texture = PlayerTex[player_shin];


			PlayerSpr[player_shin]->GenereteSprite();
		}

		player1->transform->Scale(0.6, 0.6, 0.6);
		p1rend->sprite = PlayerSpr[0];

		rb_player1 = player1->AddComponent<RigidBody>();
		rb_player1->SetType(b2_dynamicBody);
		rb_player1->rigidbody->SetFixedRotation(true);
		rb_player1->AddBoxCollider(Vector2(PlayerSpr[0]->size.x - 2, PlayerSpr[0]->size.y - 0.4) * 0.6f);

	}
	int cnt = 0;
	float Timer = 0.5;
	int direction = 1;
	void Update() {
		p1rend->sprite = PlayerSpr[cnt];
		if (Timer < 0) {
			cnt++;
			Timer = 0.5;
		}
		Timer -= Time.DeltaTime * abs(rb_player1->velocity.x);
		if (cnt > 8) cnt = 0;

		if (GetKey(DIK_A)) {
			rb_player1->AddForce(Vector2(-200,0), Force);
			player1->transform->scale = Vector3(-0.6, 0.6, 0.6);
			direction = -1;
		}
		if (GetKey(DIK_D)) {
			rb_player1->AddForce(Vector2(200, 0), Force);
			player1->transform->scale = Vector3(0.6, 0.6, 0.6);
			direction = 1;
		}
		if (GetKeyDown(DIK_SPACE)) {
			rb_player1->AddForce(Vector2(0, 300), Impulse);
		}
		if (GetKeyDown(DIK_B)) {
			new Javelin(Vector2(player1->transform->position.x, player1->transform->position.y), direction, sprSpear);
		}
	}
};