#pragma once
#include "IkarosCore.h"

class SelectionScene : public MonoBehavior {
public:

	int PLAYERKAZU = 0;
	int P1_Opn, P1_Wea;
	int P2_Opn, P2_Wea;
	int P3_Opn, P3_Wea;
	int P4_Opn, P4_Wea;
	
	GameObject* SelectionCamera;
	GameObject* SelectionBackdrop;
	
	Renderer* SelectionBackdropRen;
	Sprite* SelectionBackdropSpr;
	Renderer* SelectionRim0Ren;
	Sprite* SelectionRim0Spr;

	GameObject* SelectionRim2;
	Renderer* SelectionRim2Ren;
	Sprite* SelectionRim2Spr;
	GameObject* SelectionGO;
	Renderer* SelectionGORen;
	Sprite* SelectionGOSpr;

	GameObject* SelectionNoPlayer;
	GameObject* SelectionPlayer1;
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



	GameObject* SelectionPlayer1Oper; 
	GameObject* SelectionPlayer2Oper;
	GameObject* SelectionPlayer3Oper;
	GameObject* SelectionPlayer4Oper;
	Renderer* SelectionPlayer1OperRen;
	Renderer* SelectionPlayer2OperRen;
	Renderer* SelectionPlayer3OperRen;
	Renderer* SelectionPlayer4OperRen;
	Sprite* SelectionKey;
	Sprite* SelectionFeat;


	GameObject* SelectionPlayer1Weapon; 
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
		SelectionGO = new GameObject("SelectionGO");

		SelectionGOSpr = new Sprite("SelectionGO", Texture::LoadTexture("SelectionGO", "assets/Selection/SelectionGo.png"));


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

		SelectionGO->AddComponent<Renderer>()->sprite = SelectionGOSpr;
		SelectionGO->transform->position = Vector3(800, 0, 0.0012);
		SelectionGO->transform->Scale(75, 75, 1);


		SelectionRim2->AddComponent<Renderer>()->sprite = SelectionRim2Spr;
		SelectionRim2->transform->position = Vector3(Option_x, Option_y, 0.0013);//-350 300
		SelectionRim2->transform->Scale(50, 50, 1);

	}

	void Update() {
		SelectionRim2->transform->position = Vector3(Option_x, Option_y, 0.0013);

		
			if (GetKeyDown(DIK_UP)) {
				PLAYERKAZU++;
			}
			if (GetKeyDown(DIK_DOWN)) {
				PLAYERKAZU--;
			}
			if (PLAYERKAZU < 0) {
				PLAYERKAZU = 8;
			}
			if (PLAYERKAZU > 8)
			{
				PLAYERKAZU = 0;
			}
			if (PLAYERKAZU == 0) {
				Option_x = -300, Option_y = 300;
				if (GetKeyDown(DIK_A)) {
					P1_Opn--;
				}
				if (GetKeyDown(DIK_D)) {
					P1_Opn++;
				}
				if (P1_Opn == 0) {
					SelectionPlayer1Oper->GetComponent<Renderer>()->sprite = SelectionKey;

				}
				if (P1_Opn == 1) {
				SelectionPlayer1Oper->GetComponent<Renderer>()->sprite = SelectionFeat;

				}
				if (P1_Opn < 0) {
					P1_Opn = 1;
				}
				if (P1_Opn > 1) {
					P1_Opn = 0;
				}
			}
			if (PLAYERKAZU == 1) {
				Option_x = -300;
				Option_y = 150;
				if (GetKeyDown(DIK_A)) {
					P1_Wea--;
				}
				if (GetKeyDown(DIK_D)) {
					P1_Wea++;
				}
				if (P1_Wea == 0) {

					SelectionPlayer1Weapon->GetComponent<Renderer>()->sprite = SelectionSword;
				}
				if (P1_Wea == 1) {
					SelectionPlayer1Weapon->GetComponent<Renderer>()->sprite = SelectionShield;

				}
				if (P1_Wea == 2) {
					SelectionPlayer1Weapon->GetComponent<Renderer>()->sprite = SelectionJavelin;
				}
				if (P1_Wea == 3)
				{
					SelectionPlayer1Weapon->GetComponent<Renderer>()->sprite = SelectionBow;
				}
				if (P1_Wea < 0) {
					P1_Wea = 3;
				}
				if (P1_Wea > 3) {
					P1_Wea = 0;
				}
			}
			if (PLAYERKAZU == 2) {
				Option_x = 600;
				Option_y = 300;
				if (GetKeyDown(DIK_A)) {
					P2_Opn--;
				}
				if (GetKeyDown(DIK_D)) {
					P2_Opn++;
				}
				if (P2_Opn == 0) {
					SelectionPlayer2Oper->GetComponent<Renderer>()->sprite = SelectionKey;

				}
				if (P2_Opn == 1) {
					SelectionPlayer2Oper->GetComponent<Renderer>()->sprite = SelectionFeat;
				}
				if (P2_Opn < 0) {
					P2_Opn = 1;
				}
				if (P2_Opn > 1) {
					P2_Opn = 0;
				}
			}
				if (PLAYERKAZU == 3) {
					Option_x = 600;
					Option_y = 150;
					if (GetKeyDown(DIK_A)) {
						P2_Wea--;
					}
					if (GetKeyDown(DIK_D)) {
						P2_Wea++;
					}
					if (P2_Wea == 0) {

						SelectionPlayer2Weapon->GetComponent<Renderer>()->sprite = SelectionSword;
					}
					if (P2_Wea == 1) {
						SelectionPlayer2Weapon->GetComponent<Renderer>()->sprite = SelectionShield;

					}
					if (P2_Wea == 2) {
						SelectionPlayer2Weapon->GetComponent<Renderer>()->sprite = SelectionJavelin;
					}
					if (P2_Wea == 3)
					{
						SelectionPlayer2Weapon->GetComponent<Renderer>()->sprite = SelectionBow;
					}
					if (P2_Wea < 0) {
						P2_Wea = 3;
					}
					if (P2_Wea > 3) {
						P2_Wea = 0;
					}
				}
				if (PLAYERKAZU == 4) {
					Option_x = -300;
					Option_y = -200;
					SelectionPlayer3->GetComponent<Renderer>()->sprite = SelectionPlayer3pSpr;
					if (GetKeyDown(DIK_A)) {
						P3_Opn--;
					}
					if (GetKeyDown(DIK_D)) {
						P3_Opn++;
					}
					if (P3_Opn == 0) {
						SelectionPlayer3Oper->GetComponent<Renderer>()->sprite = SelectionKey;

					}
					if (P3_Opn == 1) {
						SelectionPlayer3Oper->GetComponent<Renderer>()->sprite = SelectionFeat;

					}
					if (P3_Opn < 0) {
						P3_Opn = 1;
					}
					if (P3_Opn > 1) {
						P3_Opn = 0;
					}
				}
				if (PLAYERKAZU == 5) {
					Option_x = -300;
					Option_y = -350;

					if (GetKeyDown(DIK_A)) {
						P3_Wea--;
					}
					if (GetKeyDown(DIK_D)) {
						P3_Wea++;
					}
					if (P3_Wea == 0) {

						SelectionPlayer3Weapon->GetComponent<Renderer>()->sprite = SelectionSword;
					}
					if (P3_Wea == 1) {
						SelectionPlayer3Weapon->GetComponent<Renderer>()->sprite = SelectionShield;

					}
					if (P3_Wea == 2) {
						SelectionPlayer3Weapon->GetComponent<Renderer>()->sprite = SelectionJavelin;
					}
					if (P3_Wea == 3)
					{
						SelectionPlayer3Weapon->GetComponent<Renderer>()->sprite = SelectionBow;
					}
					if (P3_Wea < 0) {
						P3_Wea = 3;
					}
					if (P3_Wea > 3) {
						P3_Wea = 0;
					}
				}
				if (PLAYERKAZU == 6) {
					Option_x = 600;
					Option_y = -200;
					SelectionPlayer4->GetComponent<Renderer>()->sprite = SelectionPlayer4pSpr;
					if (GetKeyDown(DIK_A)) {
						P4_Opn--;
					}
					if (GetKeyDown(DIK_D)) {
						P4_Opn++;
					}
					if (P4_Opn == 0) {
						SelectionPlayer4Oper->GetComponent<Renderer>()->sprite = SelectionKey;

					}
					if (P4_Opn == 1) {
						SelectionPlayer4Oper->GetComponent<Renderer>()->sprite = SelectionFeat;

					}
					if (P4_Opn < 0) {
						P4_Opn = 1;
					}
					if (P4_Opn > 1) {
						P4_Opn = 0;
					}
				}

				if (PLAYERKAZU == 7) {
					Option_x = 600;
					Option_y = -350;
					if (GetKeyDown(DIK_A)) {
						P4_Wea--;
					}
					if (GetKeyDown(DIK_D)) {
						P4_Wea++;
					}
					if (P4_Wea == 0) {

						SelectionPlayer4Weapon->GetComponent<Renderer>()->sprite = SelectionSword;
					}
					if (P4_Wea == 1) {
						SelectionPlayer4Weapon->GetComponent<Renderer>()->sprite = SelectionShield;

					}
					if (P4_Wea == 2) {
						SelectionPlayer4Weapon->GetComponent<Renderer>()->sprite = SelectionJavelin;
					}
					if (P4_Wea == 3)
					{
						SelectionPlayer4Weapon->GetComponent<Renderer>()->sprite = SelectionBow;
					}
					if (P4_Wea < 0) {
						P4_Wea = 3;
					}
					if (P4_Wea > 3) {
						P4_Wea = 0;
					}
				}
				if (PLAYERKAZU == 8) {
					Option_x = 800;
					Option_y = 0;
					if (GetKeyDown(DIK_RETURN))
					{
						SceneManager::LoadScene(1);
						return;
					}
				}
			
	}
};