#pragma once
#include "IkarosCore.h"

class SelectionScene : public MonoBehavior {
public:
	SelectionScene() {
		type = typeid(*this).name();
		mb_init();
	}




	GameObject* SelectionCamera;//ƒJƒƒ‰
	GameObject* SelectionBackdrop;//”wŒi
	GameObject* SelectionRim0;//‘I‚ÔUI‚P

	Renderer* SelectionBackdropRen;
	Sprite* SelectionBackdropSpr;
	Renderer* SelectionRim0Ren;
	Sprite* SelectionRim0Spr;

	GameObject* SelectionRim1;//‘I‚ÔUI‚Q
	GameObject* SelectionRim2;//‘I‚ÔUI‚R
	Renderer* SelectionRim1Ren;
	Renderer* SelectionRim2Ren;
	Sprite* SelectionRim1Spr;
	Sprite* SelectionRim2Spr;


	GameObject* SelectionPlayer1;//player‚ÌŠG
	GameObject* SelectionPlayer2;
	GameObject* SelectionPlayer3;
	GameObject* SelectionPlayer4;
	Renderer* SelectionPlayer1Ren;
	Renderer* SelectionPlayer2Ren;
	Renderer* SelectionPlayer3Ren;
	Renderer* SelectionPlayer4Ren;
	Sprite* SelectionPlayer1Red;
	Sprite* SelectionPlayer2Orange;
	Sprite* SelectionPlayer3Yellow;
	Sprite* SelectionPlayer4Green;
	Sprite* SelectionPlayer5Blue;
	Sprite* SelectionPlayer6Pink;
	Sprite* SelectionPlayer7Brown;


	GameObject* SelectionPlayer1Oper; //‘€ì•û–@‚ÌŠG
	GameObject* SelectionPlayer2Oper;
	GameObject* SelectionPlayer3Oper;
	GameObject* SelectionPlayer4Oper;
	Renderer* SelectionPlayer1OperRen;
	Renderer* SelectionPlayer2OperRen;
	Renderer* SelectionPlayer3OperRen;
	Renderer* SelectionPlayer4OperRen;
	Sprite* SelectionKey;
	Sprite* SelectionFeat;


	GameObject* SelectionPlayer1Weapon; //Žn‚ß‚Ì•Ší
	GameObject* SelectionPlayer2Weapon;
	GameObject* SelectionPlayer3Weapon;
	GameObject* SelectionPlayer4Weapon;
	Renderer* SelectionPlayer1WeaponRen;
	Renderer* SelectionPlayer2WeaponRen;
	Renderer* SelectionPlayer3WeaponRen;
	Renderer* SelectionPlayer4WeaponRen;
	Sprite* SelectionSword;
	Sprite* SelectionShield;
	Sprite* SelectionJavelin; 
	Sprite* SelectionBow;






	void Awake() {

		SelectionBackdrop = new GameObject("SelectionBackdrop");
		SelectionRim0 = new GameObject("SelectionRim0");
		SelectionRim1 = new GameObject("SelectionRim1");
		SelectionRim2 = new GameObject("SelectionRim2");
		SelectionPlayer1 = new GameObject("SelectionPlayer1");
		SelectionPlayer2 = new GameObject("SelectionPlayer2");
		SelectionPlayer3 = new GameObject("SelectionPlayer3");
		SelectionPlayer4 = new GameObject("SelectionPlayer4");
		SelectionPlayer1Oper = new GameObject("SelectionnPlayer1Oper");
		SelectionPlayer2Oper = new GameObject("SelectionnPlayer2Oper");
		SelectionPlayer3Oper = new GameObject("SelectionnPlayer3Oper");
		SelectionPlayer4Oper = new GameObject("SelectionnPlayer4Oper");
		SelectionPlayer1Weapon = new GameObject("SelectionPlayer1Weapon");
		SelectionPlayer2Weapon = new GameObject("SelectionPlayer2Weapon");
		SelectionPlayer3Weapon = new GameObject("SelectionPlayer3Weapon");
		SelectionPlayer4Weapon = new GameObject("SelectionPlayer4Weapon");

		SelectionBackdropSpr = new Sprite("SelectionBackdrop", Texture::LoadTexture("TitleBackTex", "assets/Selection/SelectionBackdrop.png"));
		SelectionRim0Spr = new Sprite("SelectionRim0", Texture::LoadTexture("SelectionRim0", "assets/Selection/SelectionRim0.png"));
		SelectionRim1Spr = new Sprite("SelectionRim1", Texture::LoadTexture("SelectionRim1", "assets/Selection/SelectionRim1.png"));
		SelectionRim2Spr = new Sprite("SelectionRim2", Texture::LoadTexture("SelectionRim2", "assets/Selection/SelectionRim2.png"));
		SelectionPlayer1Red = new Sprite("SelectionPlayer1Red", Texture::LoadTexture("SelectionPlayer1Red", "assets/Selection/SelectionPlayer1Red.png"));
		SelectionPlayer2Orange = new Sprite("SelectionPlayer2Orange", Texture::LoadTexture("SelectionPlayer2Orange", "assets/Selection/SelectionPlayer2Orange.png"));
		SelectionPlayer3Yellow = new Sprite("SelectionPlayer3Yellow", Texture::LoadTexture("SelectionPlayer3Yellow", "assets/Selection/SelectionPlayer3Yellow.png"));
		SelectionPlayer4Green = new Sprite("SelectionPlayer4Green", Texture::LoadTexture("SelectionPlayer4Green", "assets/Selection/SelectionPlayer4Green.png"));
		SelectionPlayer5Blue = new Sprite("SelectionPlayer5Blue", Texture::LoadTexture("SelectionPlayer5Blue", "assets/Selection/SelectionPlayer5Blue.png"));
		SelectionPlayer6Pink = new Sprite("SelectionPlayer6Pink", Texture::LoadTexture("SelectionPlayer6Pink", "assets/Selection/SelectionPlayer6Pink.png"));
		SelectionPlayer7Brown = new Sprite("SelectionPlayer7Brown", Texture::LoadTexture("SelectionPlayer7Brown", "assets/Selection/SelectionPlayer7Brown.png"));
		SelectionKey = new Sprite("SelectionKey", Texture::LoadTexture("SelectionKey", "assets/Selection/SelectionKey.png"));
		SelectionFeat = new Sprite("SelectionFeat", Texture::LoadTexture("SelectionFeat", "assets/Selection/SelectionFeat.png"));

		SelectionSword = new Sprite("SelectionSword", Texture::LoadTexture("SelectionSword", "assets/Selection/SelectionSword.png"));
		SelectionShield = new Sprite("SelectionShield", Texture::LoadTexture("SelectionShield", "assets/Selection/SelectionShield.png"));
		SelectionJavelin = new Sprite("SelectionJavelin", Texture::LoadTexture("SelectionJavelin", "assets/Selection/SelectionJavelin.png"));
		SelectionBow = new Sprite("SelectionBow", Texture::LoadTexture("SelectionBow", "assets/Selection/SelectionBow.png"));
	}

	void Start() {
		SelectionCamera = new GameObject("SelectionCamera");
		Camera* cam = SelectionCamera->AddComponent<Camera>();
		SelectionCamera->transform->Translate(0, 0, -40);
		cam->orthographic = true;
		cam->orthographicSize = 1;
		cam->SetProjection();


		

		SelectionBackdrop->AddComponent<Renderer>()->sprite = SelectionBackdropSpr;
		SelectionBackdrop->transform->position = Vector3(0, 0, 0.0010);
		SelectionBackdrop->transform->Scale(100, 100, 1);

		
	
		SelectionRim1->AddComponent<Renderer>()->sprite = SelectionRim1Spr;
		SelectionRim1->transform->position = Vector3(0, 0, 0.0015);
		SelectionRim1->transform->Scale(100, 100, 1);

		SelectionRim2->AddComponent<Renderer>()->sprite = SelectionRim2Spr;
		SelectionRim2->transform->position = Vector3(0, 0, 0.0015);
		SelectionRim2->transform->Scale(100, 100, 1);

		SelectionPlayer1->AddComponent<Renderer>()->sprite = SelectionPlayer1Red;
		SelectionPlayer1->transform->position = Vector3(-500, -500, 0.0011);
		SelectionPlayer1->transform->Scale(100, 100, 1);

		SelectionPlayer2->AddComponent<Renderer>()->sprite = SelectionPlayer2Orange;
		SelectionPlayer2->transform->position = Vector3(-100, -100, 0.0011);
		SelectionPlayer2->transform->Scale(100, 100, 1);

		SelectionPlayer3->AddComponent<Renderer>()->sprite = SelectionPlayer3Yellow;
		SelectionPlayer3->transform->position = Vector3(300, 300, 0.0011);
		SelectionPlayer3->transform->Scale(100, 100, 1);

		SelectionPlayer4->AddComponent<Renderer>()->sprite = SelectionPlayer4Green;
		SelectionPlayer4->transform->position = Vector3(700, 700, 0.0011);
		SelectionPlayer4->transform->Scale(100, 100, 1);

		SelectionPlayer1Oper->AddComponent<Renderer>()->sprite = SelectionKey;
		SelectionPlayer1Oper->transform->position = Vector3(200, 200, 0.0011);
		SelectionPlayer1Oper->transform->Scale(100, 100, 1);

		SelectionPlayer2Oper->AddComponent<Renderer>()->sprite = SelectionKey;
		SelectionPlayer2Oper->transform->position = Vector3(300, 200, 0.0011);
		SelectionPlayer2Oper->transform->Scale(100, 100, 1);

		SelectionPlayer3Oper->AddComponent<Renderer>()->sprite = SelectionKey;
		SelectionPlayer3Oper->transform->position = Vector3(300, 300, 0.0011);
		SelectionPlayer3Oper->transform->Scale(100, 100, 1);

		SelectionPlayer4Oper->AddComponent<Renderer>()->sprite = SelectionKey;
		SelectionPlayer4Oper->transform->position = Vector3(400, 300, 0.0011);
		SelectionPlayer4Oper->transform->Scale(100, 100, 1);

		SelectionPlayer1Weapon->AddComponent<Renderer>()->sprite = SelectionSword;
		SelectionPlayer1Weapon->transform->position = Vector3(400, 300, 0.0011);
		SelectionPlayer1Weapon->transform->Scale(100, 100, 1);

		SelectionPlayer2Weapon->AddComponent<Renderer>()->sprite = SelectionSword;
		SelectionPlayer2Weapon->transform->position = Vector3(400, 300, 0.0011);
		SelectionPlayer2Weapon->transform->Scale(100, 100, 1);

		SelectionPlayer3Weapon->AddComponent<Renderer>()->sprite = SelectionSword;
		SelectionPlayer3Weapon->transform->position = Vector3(400, 300, 0.0011);
		SelectionPlayer3Weapon->transform->Scale(100, 100, 1);

		SelectionPlayer4Weapon->AddComponent<Renderer>()->sprite = SelectionSword;
		SelectionPlayer4Weapon->transform->position = Vector3(400, 300, 0.0011);
		SelectionPlayer4Weapon->transform->Scale(100, 100, 1);

		SelectionRim0->AddComponent<Renderer>()->sprite = SelectionRim0Spr;
		SelectionRim0->transform->position = Vector3(0, 0, 0.0016);
		SelectionRim0->transform->Scale(150, 150, 1);

	}

	void Update() {

	}
};