#pragma once
#include "IkarosCore.h"

class SelectionScene : public MonoBehavior {
public:
	SelectionScene() {
		type = typeid(*this).name();
		mb_init();
	}

	int PLAYERKAZU = 0;
	int Pick1 = 0, P1_Ope = 0, P1_Wea = 0;
	int Pick2 = 0, P2_Ope = 2, P2_Wea = 0;
	int Pick3 = 0, P3_Ope = 2, P3_Wea = 0;
	int Pick4 = 0, P4_Ope = 2, P4_Wea = 0;


	GameObject* SelectionCamera;//ƒJƒƒ‰
	GameObject* SelectionBackdrop;//”wŒi
	

	Renderer* SelectionBackdropRen;
	Sprite* SelectionBackdropSpr;
	Renderer* SelectionRim0Ren;
	Sprite* SelectionRim0Spr;


	GameObject* SelectionRim2;//‘I‚ÔUI‚R
	Renderer* SelectionRim2Ren;
	Sprite* SelectionRim2Spr;

	GameObject* SelectionNoPlayer;
	GameObject* SelectionPlayer1;//player‚ÌŠG
	GameObject* SelectionPlayer2;
	GameObject* SelectionPlayer3;
	GameObject* SelectionPlayer4;
	Renderer* SelectionNoplayerRen;
	Renderer* SelectionPlayer1Ren;
	Renderer* SelectionPlayer2Ren;
	Renderer* SelectionPlayer3Ren;
	Renderer* SelectionPlayer4Ren;
	Sprite* SelectionNoPlayerSpr;
	Sprite* SelectionPlayer1pSpr;
	Sprite* SelectionPlayer2pSpr;
	Sprite* SelectionPlayer3pSpr;
	Sprite* SelectionPlayer4pSpr;



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


	int Option_x=-300, Option_y=300;



	void Awake() {

		SelectionBackdrop = new GameObject("SelectionBackdrop");
		
		SelectionRim2 = new GameObject("SelectionRim2");
		SelectionNoPlayer = new GameObject("SelectionNoplayer");
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
		
		SelectionRim2Spr = new Sprite("SelectionRim2", Texture::LoadTexture("SelectionRim2", "assets/Selection/SelectionRim2.png"));
		SelectionNoPlayerSpr = new Sprite("SelectionNoPlayer", Texture::LoadTexture("SelectionNoPlayer", "assets/Selection/SelectionNoPlayer.png"));
		SelectionPlayer1pSpr = new Sprite("SelectionPlayer1p", Texture::LoadTexture("SelectionPlayer1p", "assets/Selection/SelectionPlayer1p.png"));
		SelectionPlayer2pSpr = new Sprite("SelectionPlayer2p", Texture::LoadTexture("SelectionPlayer2p", "assets/Selection/SelectionPlayer2p.png"));
		SelectionPlayer3pSpr = new Sprite("SelectionPlayer3p", Texture::LoadTexture("SelectionPlayer3p", "assets/Selection/SelectionPlayer3p.png"));
		SelectionPlayer4pSpr = new Sprite("SelectionPlayer4p", Texture::LoadTexture("SelectionPlayer4p", "assets/Selection/SelectionPlayer4p.png"));

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
		SelectionBackdrop->transform->Scale(177, 177, 1);


		SelectionRim2->AddComponent<Renderer>()->sprite = SelectionRim2Spr;
		SelectionRim2->transform->position = Vector3( Option_x, Option_y, 0.0013);//-350 300
		SelectionRim2->transform->Scale(50, 50, 1);

		SelectionPlayer1->AddComponent<Renderer>()->sprite = SelectionPlayer1pSpr;
		SelectionPlayer1->transform->position = Vector3(-650, 250, 0.0014);
		SelectionPlayer1->transform->Scale(75, 75, 1);

		SelectionPlayer2->AddComponent<Renderer>()->sprite = SelectionPlayer2pSpr;
		SelectionPlayer2->transform->position = Vector3(250, 250, 0.0014);
		SelectionPlayer2->transform->Scale(75, 75, 1);

		SelectionPlayer3->AddComponent<Renderer>()->sprite = SelectionNoPlayerSpr;
		SelectionPlayer3->transform->position = Vector3(-650, -300, 0.0014);
		SelectionPlayer3->transform->Scale(75, 75, 1);

		SelectionPlayer4->AddComponent<Renderer>()->sprite = SelectionNoPlayerSpr;
		SelectionPlayer4->transform->position = Vector3(250, -300, 0.0014);
		SelectionPlayer4->transform->Scale(75, 75, 1);


		SelectionPlayer1Oper->AddComponent<Renderer>()->sprite = SelectionKey;
		SelectionPlayer1Oper->transform->position = Vector3(-300, 300, 0.0013);
		SelectionPlayer1Oper->transform->Scale(75, 75, 1);

		SelectionPlayer2Oper->AddComponent<Renderer>()->sprite = SelectionKey;
		SelectionPlayer2Oper->transform->position = Vector3(600, 300, 0.0013);
		SelectionPlayer2Oper->transform->Scale(75, 75, 1);

		SelectionPlayer3Oper->AddComponent<Renderer>()->sprite = SelectionKey;
		SelectionPlayer3Oper->transform->position = Vector3(-300, -200, 0.0013);
		SelectionPlayer3Oper->transform->Scale(75, 75, 1);

		SelectionPlayer4Oper->AddComponent<Renderer>()->sprite = SelectionKey;
		SelectionPlayer4Oper->transform->position = Vector3(600, -200, 0.0013);
		SelectionPlayer4Oper->transform->Scale(75, 75, 1);

		SelectionPlayer1Weapon->AddComponent<Renderer>()->sprite = SelectionSword;
		SelectionPlayer1Weapon->transform->position = Vector3(-300, 150, 0.0012);
		SelectionPlayer1Weapon->transform->Scale(75, 75, 1);

		SelectionPlayer2Weapon->AddComponent<Renderer>()->sprite = SelectionSword;
		SelectionPlayer2Weapon->transform->position = Vector3(600, 150, 0.0012);
		SelectionPlayer2Weapon->transform->Scale(75, 75, 1);

		SelectionPlayer3Weapon->AddComponent<Renderer>()->sprite = SelectionSword;
		SelectionPlayer3Weapon->transform->position = Vector3(-300, -350, 0.0012);
		SelectionPlayer3Weapon->transform->Scale(75, 75, 1);

		SelectionPlayer4Weapon->AddComponent<Renderer>()->sprite = SelectionSword;
		SelectionPlayer4Weapon->transform->position = Vector3(600, -350, 0.0012);
		SelectionPlayer4Weapon->transform->Scale(75, 75, 1);

	}

	void Update() {
		
		if (GetKey(DIK_END)) {
			PLAYERKAZU ++;
		}
		if (GetKey(DIK_HOME)) {
			PLAYERKAZU --;
		}
		if (PLAYERKAZU < 0) {
			PLAYERKAZU = 5;
		}
		if (PLAYERKAZU == 1) {
			Option_x = 600;
			Option_y = 300;
		}
		if (PLAYERKAZU == 2) {
			Option_x = -300;
			Option_y = -350;
		}

		if (PLAYERKAZU == 3) {
			Option_x = 600;
			Option_y = -350;
		}
	}
}

;