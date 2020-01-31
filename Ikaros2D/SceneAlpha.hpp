#pragma once
#include "IkarosCore.h"

class SceneAlpha : public MonoBehavior {
public:

	std::vector <Sprite*> vBGSprite;
	Sprite* sprFloor;

	void Awake() {
		vBGSprite.push_back(new Sprite("clouds1Sprite", Texture::LoadTexture("clouds1Texture", "Assets/Textures/BG/clouds_1.png")));
		vBGSprite.push_back(new Sprite("clouds2Sprite", Texture::LoadTexture("clouds2Texture", "Assets/Textures/BG/clouds_2.png")));
		vBGSprite.push_back(new Sprite("clouds3Sprite", Texture::LoadTexture("clouds3Texture", "Assets/Textures/BG/clouds_3.png")));
		vBGSprite.push_back(new Sprite("clouds4Sprite", Texture::LoadTexture("clouds4Texture", "Assets/Textures/BG/clouds_4.png")));
		vBGSprite.push_back(new Sprite("rocks1Sprite", Texture::LoadTexture("rocks1Texture", "Assets/Textures/BG/rocks_1.png")));
		vBGSprite.push_back(new Sprite("rocks2Sprite", Texture::LoadTexture("rocks2Texture", "Assets/Textures/BG/rocks_2.png")));
		vBGSprite.push_back(new Sprite("skySprite", Texture::LoadTexture("skyTexture", "Assets/Textures/BG/sky.png")));
		vBGSprite.push_back(new Sprite("treeSprite", Texture::LoadTexture("treeTexture", "Assets/Textures/BG/tree.png")));
		vBGSprite.push_back(new Sprite("ground2Sprite", Texture::LoadTexture("ground2Texture", "Assets/Textures/BG/ground.png")));

		sprFloor = new Sprite("ground1Sprite", Texture::LoadTexture("floor", "Assets/Textures/floor.png"));
	}

	void Start() {
		GameObject* sky = new GameObject("sky1");
		sky->AddComponent<Renderer>()->sprite = vBGSprite[6];
		sky->transform->position = Vector3(0, 0, 1000);
		sky->transform->scale = Vector3(150, 150, 1);

		GameObject* sky2 = new GameObject("sky2");
		sky2->AddComponent<Renderer>()->sprite = vBGSprite[6];
		sky2->transform->position = Vector3(vBGSprite[6]->size.x * 150 - 5, 0, 999.9);
		sky2->transform->scale = Vector3(150, 150, 1);

		GameObject* sky3 = new GameObject("sky3");
		sky3->AddComponent<Renderer>()->sprite = vBGSprite[6];
		sky3->transform->position = Vector3(vBGSprite[6]->size.x * -150 + 5, 0, 999.9);
		sky3->transform->scale = Vector3(150, 150, 1);

		GameObject* Brocks1 = new GameObject("Brocks1");
		Brocks1->AddComponent<Renderer>()->sprite = vBGSprite[4];
		Brocks1->transform->position = Vector3(0, 0, 600);
		Brocks1->transform->scale = Vector3(75, 75, 1);

		GameObject* Brocks2 = new GameObject("Brocks2");
		Brocks2->AddComponent<Renderer>()->sprite = vBGSprite[4];
		Brocks2->transform->position = Vector3(vBGSprite[4]->size.x * 75 - 0.1, 0, 599.9);
		Brocks2->transform->scale = Vector3(75, 75, 1);

		GameObject* Brocks3 = new GameObject("Brocks3");
		Brocks3->AddComponent<Renderer>()->sprite = vBGSprite[4];
		Brocks3->transform->position = Vector3(vBGSprite[4]->size.x * -75 + 0.1, 0, 599.9);
		Brocks3->transform->scale = Vector3(75, 75, 1);

		GameObject* Frocks1 = new GameObject("Frocks1");
		Frocks1->AddComponent<Renderer>()->sprite = vBGSprite[5];
		Frocks1->transform->position = Vector3(0, -25, 200);
		Frocks1->transform->scale = Vector3(30, 30, 1);

		GameObject* Frocks2 = new GameObject("Frocks2");
		Frocks2->AddComponent<Renderer>()->sprite = vBGSprite[5];
		Frocks2->transform->position = Vector3(vBGSprite[4]->size.x * 30 - 0.1, -25, 199.9);
		Frocks2->transform->scale = Vector3(30, 30, 1);

		GameObject* Frocks3 = new GameObject("Frocks3");
		Frocks3->AddComponent<Renderer>()->sprite = vBGSprite[5];
		Frocks3->transform->position = Vector3(vBGSprite[4]->size.x * -30 + 0.1, -25, 199.9);
		Frocks3->transform->scale = Vector3(30, 30, 1);

		GameObject* Bclouds1 = new GameObject("Bclouds1");
		Bclouds1->AddComponent<Renderer>()->sprite = vBGSprite[0];
		Bclouds1->transform->position = Vector3(0, 0, 350);
		Bclouds1->transform->scale = Vector3(40, 40, 1);

		GameObject* Bclouds2 = new GameObject("Bclouds2");
		Bclouds2->AddComponent<Renderer>()->sprite = vBGSprite[0];
		Bclouds2->transform->position = Vector3(vBGSprite[0]->size.x * 40 - 0.1, 0, 349.9);
		Bclouds2->transform->scale = Vector3(40, 40, 1);

		GameObject* Bclouds3 = new GameObject("Bclouds3");
		Bclouds3->AddComponent<Renderer>()->sprite = vBGSprite[0];
		Bclouds3->transform->position = Vector3(vBGSprite[0]->size.x * -40 + 0.1, 0, 349.9);
		Bclouds3->transform->scale = Vector3(40, 40, 1);

		GameObject* Fclouds1 = new GameObject("Fclouds1");
		Fclouds1->AddComponent<Renderer>()->sprite = vBGSprite[1];
		Fclouds1->transform->position = Vector3(200, -10, 250);
		Fclouds1->transform->scale = Vector3(15, 15, 1);

		GameObject* Fclouds2 = new GameObject("Fclouds2");
		Fclouds2->AddComponent<Renderer>()->sprite = vBGSprite[2];
		Fclouds2->transform->position = Vector3(-200, -10, 150);
		Fclouds2->transform->scale = Vector3(15, 15, 1);

		GameObject* Fclouds3 = new GameObject("Fclouds3");
		Fclouds3->AddComponent<Renderer>()->sprite = vBGSprite[3];
		Fclouds3->transform->position = Vector3(-400, -50, 150);
		Fclouds3->transform->scale = Vector3(15, 15, 1);

		GameObject* Trees1 = new GameObject("Trees1");
		Trees1->AddComponent<Renderer>()->sprite = vBGSprite[7];
		Trees1->transform->scale = Vector3(5, 5, 1);
		Trees1->transform->position = Vector3(-vBGSprite[7]->size.x * Trees1->transform->scale.x, -47.5, 10);
		
		for (float i = Trees1->transform->position.y - ((vBGSprite[7]->size.y * 5 + vBGSprite[8]->size.y * 5) / 2); i > -150; i -= vBGSprite[8]->size.y * 5) {
			CreateUnderground(Trees1->transform->position.x, i);
		}
		GameObject* Trees2 = new GameObject("Trees2");
		Trees2->AddComponent<Renderer>()->sprite = vBGSprite[7];
		Trees2->transform->scale = Vector3(5, 5, 1);
		Trees2->transform->position = Vector3(0, -47.5, 10);

		for (float i = Trees2->transform->position.y - ((vBGSprite[7]->size.y * 5 + vBGSprite[8]->size.y * 5) / 2); i > -150; i -= vBGSprite[8]->size.y * 5) {
			CreateUnderground(Trees2->transform->position.x, i);
		}

		GameObject* Trees3 = new GameObject("Trees3");
		Trees3->AddComponent<Renderer>()->sprite = vBGSprite[7];
		Trees3->transform->scale = Vector3(5, 5, 1);
		Trees3->transform->position = Vector3(vBGSprite[7]->size.x * Trees3->transform->scale.x, -47.5, 10);

		for (float i = Trees3->transform->position.y - ((vBGSprite[7]->size.y * 5 + vBGSprite[8]->size.y * 5) / 2); i > -150; i -= vBGSprite[8]->size.y * 5) {
			CreateUnderground(Trees3->transform->position.x, i);
		}

		//GroundCollider
		/*
		for (float i = 150; i > -150; i -= 3.6) {
			CreateFloor(i, -50);
		}
		*/
		GameObject* GroundCollider;
		GroundCollider = new GameObject("GroundCollider");
		GroundCollider->tag = "Ground";
		GroundCollider->transform->position = Vector3(0, -50, 0);
		RigidBody* ground = GroundCollider->AddComponent<RigidBody>();
		ground->AddBoxCollider(Vector2(250, 1));

		//PlatformEast
		for (float i = -25; i > -50; i -= 3.6) {
			CreateFloor(i, -42);
		}

		GroundCollider = new GameObject("PlatformEast");
		GroundCollider->tag = "Ground";
		GroundCollider->transform->position = Vector3(-36.5f, -42, 0);
		ground = GroundCollider->AddComponent<RigidBody>();
		ground->AddBoxCollider(Vector2(25, 1));

		//PlatformWest
		for (float i = 50; i > 25; i -= 3.6) {
			CreateFloor(i, -42);
		}

		GroundCollider = new GameObject("PlatformWest");
		GroundCollider->tag = "Ground";
		GroundCollider->transform->position = Vector3(38.5f, -42, 0);
		ground = GroundCollider->AddComponent<RigidBody>();
		ground->AddBoxCollider(Vector2(25, 1));

		//PlatformMiddle
		for (float i = 25; i > -25; i -= 3.6) {
			CreateFloor(i, -34);
		}

		GroundCollider = new GameObject("PlatformMiddle");
		GroundCollider->tag = "Ground";
		GroundCollider->transform->position = Vector3(2, -34, 0);
		ground = GroundCollider->AddComponent<RigidBody>();
		ground->AddBoxCollider(Vector2(50, 1));
		
		//Collision box at the edges of the scene
		GameObject* WallE = new GameObject("EastWall");
		WallE->tag = "Wall";
		WallE->transform->position = Vector3(125, -50, 0);
		RigidBody* rbWE = WallE->AddComponent<RigidBody>();
		rbWE->AddBoxCollider(Vector2(1, 500));

		GameObject* WallW = new GameObject("WestWalls");
		WallW->tag = "Wall";
		WallW->transform->position = Vector3(-125, -50, 0);
		RigidBody* rbWW = WallW->AddComponent<RigidBody>();
		rbWW->AddBoxCollider(Vector2(1, 500));
	}

	void Update() {

	}

	int numberofFloor = 0;
	void CreateFloor(float x, float y) {
		GameObject* floorblock = new GameObject((std::string)"floor" + std::to_string(numberofFloor));
		floorblock->AddComponent<Renderer>()->sprite = sprFloor;
		floorblock->transform->position = Vector3(x, y, 0);

		numberofFloor++;
	}
	int numberofUG = 0;
	void CreateUnderground(float x, float y) {
		GameObject* floorblock = new GameObject((std::string)"Underground" + std::to_string(numberofUG));
		floorblock->AddComponent<Renderer>()->sprite = vBGSprite[8];
		floorblock->transform->position = Vector3(x, y, 10.1);
		floorblock->transform->scale = Vector3(5, 5, 1);

		numberofUG++;
	}
};