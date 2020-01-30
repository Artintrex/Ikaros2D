#pragma once
#include "IkarosCore.h"
#include "Javelin.hpp"

class Player : public MonoBehavior {
public:
	Player() {
		type = typeid(*this).name();
		mb_init();

		Left = DIK_A;
		Right = DIK_D;
		Jump = DIK_SPACE;
		Throw = DIK_LCONTROL;
		Attack = DIK_LSHIFT;
		Activate = DIK_F;
	}

	//Used for loading textures
	std::string PlayerPath = "1P";

	//Sprites
	std::vector <Sprite*> s_ShieldThrow; //5
	std::vector <Sprite*> s_SpearAttack; //4
	std::vector <Sprite*> s_SwordAttack; //5
	std::vector <Sprite*> s_BowDraw; //3

	std::vector <Sprite*> s_Idle; //4
	std::vector <Sprite*> s_BowIdle; //4
	std::vector <Sprite*> s_ShieldIdle; //4
	std::vector <Sprite*> s_SwordIdle; //4
	std::vector <Sprite*> s_SpearIdle; //4

	std::vector <Sprite*> s_Running; //12
	std::vector <Sprite*> s_BowRunning; //12
	std::vector <Sprite*> s_ShieldRunning; //12
	std::vector <Sprite*> s_SwordRunning; //12
	std::vector <Sprite*> s_SpearRunning; //12

	std::vector <Sprite*> s_Jump; //6
	std::vector <Sprite*> s_BowJump; //6
	std::vector <Sprite*> s_ShieldJump; //6
	std::vector <Sprite*> s_SwordJump; //6
	std::vector <Sprite*> s_SpearJump; //6

	RigidBody* rigidbody;
	RigidBody* sword;
	std::string swordName;
	Renderer* renderer;

	//Controls
	int Left,Right,Jump,Throw,Attack,Activate;

	Vector3 scale = Vector3(0.8, 0.8, 1);

	ImGuiWindowFlags UIFlags;

	void LoadSprites(int FrameCount, std::string AnimName, std::vector <Sprite*> &vector) {
		std::string folder = "Assets/Textures/Player/" + PlayerPath + "/" + AnimName + "/";
		for (int i = 0; i < FrameCount; i++)
		{
			std::string textureName = "AnimFrame" + std::to_string(i + 1);
			std::string path = folder + textureName + ".png";

			std::string Name = PlayerPath + AnimName + textureName;
			vector.push_back(new Sprite(Name + "_Sprite", Texture::LoadTexture(Name, path)));
			vector[i]->doubleSided = true;
			vector[i]->GenereteSprite(textureName);
		}
	}

	void Awake() {

	}

	void Start() {
		LoadSprites(5, "ShieldThrow", s_ShieldThrow);
		LoadSprites(4, "Pierce", s_SpearAttack);
		LoadSprites(5, "Slash", s_SwordAttack);
		LoadSprites(3, "BowDraw", s_BowDraw);

		LoadSprites(4, "Idle", s_Idle);
		LoadSprites(4, "IdleBow", s_BowIdle);
		LoadSprites(4, "IdleShield", s_ShieldIdle);
		LoadSprites(4, "IdleSword", s_SwordIdle);
		LoadSprites(4, "IdleSpear", s_SpearIdle);

		LoadSprites(12, "Run", s_Running);
		LoadSprites(12, "RunBow", s_BowRunning);
		LoadSprites(12, "RunShield", s_ShieldRunning);
		LoadSprites(12, "RunSword", s_SwordRunning);
		LoadSprites(12, "RunSpear", s_SpearRunning);

		LoadSprites(6, "Jump", s_Jump);
		LoadSprites(6, "JumpBow", s_BowJump);
		LoadSprites(6, "JumpShield", s_ShieldJump);
		LoadSprites(6, "JumpSword", s_SwordJump);
		LoadSprites(6, "JumpSpear", s_SpearJump);

		swordName = PlayerPath + "Sword";

		UIFlags = 0;
		UIFlags |= ImGuiWindowFlags_NoTitleBar;
		UIFlags |= ImGuiWindowFlags_NoScrollbar;

		UIFlags |= ImGuiWindowFlags_NoMove;
		UIFlags |= ImGuiWindowFlags_NoResize;
		UIFlags |= ImGuiWindowFlags_NoCollapse;
		UIFlags |= ImGuiWindowFlags_NoNav;
		UIFlags |= ImGuiWindowFlags_NoBackground;
		UIFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		// UIFlags |= ImGuiWindowFlags_MenuBar;

		renderer = parent->AddComponent<Renderer>();

		parent->transform->scale = scale;
		renderer->sprite = s_Running[0];

		rigidbody = parent->AddComponent<RigidBody>();
		rigidbody->SetType(b2_dynamicBody);
		rigidbody->rigidbody->SetGravityScale(5.0f);
		rigidbody->rigidbody->SetFixedRotation(true);
		rigidbody->AddBoxCollider(Vector2(s_Running[0]->size.x * parent->transform->scale.x * 0.5f,
			s_Running[0]->size.y * parent->transform->scale.y * 0.8));

		//Sword
		GameObject* GOsword = new GameObject(swordName);
		GOsword->tag = "Sword";
		GOsword->transform->position = Vector3(0, 1000, 0);
		sword = GOsword->AddComponent<RigidBody>();
		sword->SetType(b2_dynamicBody);
		sword->rigidbody->SetGravityScale(0.0f);
		sword->rigidbody->SetFixedRotation(true);
		sword->AddBoxCollider(Vector2(0.5, 0.5), Vector2(0, 0), 0, 500);
	}

	int cnt = 0, direction = 1;
	float Timer = 0.5;
	bool JumpFlag = true;

	int ActiveItem = gNoItem;
	int JavelinCount = 0;

	int AtkCnt = 0;
	bool isAttacking = false;
	float AttackDelay = 0.2;
	float AttackTimer = AttackDelay;

	void Update() {
		if (Timer < 0) {
			cnt++;
			Timer = 0.5;
		}
		Timer -= Time.DeltaTime * abs(rigidbody->velocity.x);
		if (cnt >= s_Running.size()) cnt = 0;

		if (ActiveItem == 0) {
			renderer->sprite = s_Running[cnt];
		}
		else if (ActiveItem == gSword)renderer->sprite = s_SwordRunning[cnt];


		if (abs(rigidbody->velocity.y) > 0.1f)JumpFlag = false;

		if (GetKey(Left)) {
			if (JumpFlag) {
				rigidbody->AddForce(Vector2(-600, 0), Force);
			}
			else rigidbody->AddForce(Vector2(-100, 0), Force);

			transform->scale.x = -scale.x;
			direction = -1;
		}

		if (GetKey(Right)) {
			if (JumpFlag) {
				rigidbody->AddForce(Vector2(600, 0), Force);
			}
			else rigidbody->AddForce(Vector2(100, 0), Force);

			transform->scale.x = scale.x;
			direction = 1;
		}

		if (GetKeyDown(Jump) && JumpFlag) {
			rigidbody->AddForce(Vector2(0, 800), Impulse);
		}

		if (GetKeyDown(Throw) && JavelinCount > 0) {
			GameObject* jav = new GameObject("Javelin");
			jav->transform->position = Vector3(transform->position.x + (direction * 3),
				transform->position.y,
				transform->position.z);

			Javelin* gjav = jav->AddComponent<Javelin>();
			gjav->direction = direction;

			JavelinCount--;
		}

		AttackTimer -= Time.DeltaTime;

		if (GetKey(Attack) && ActiveItem == gSword) {
			if (isAttacking == false) {
				AttackTimer = AttackDelay; AtkCnt = 0;
				sword->Translate(transform->position.x + direction, transform->position.y - 2, 0);
				sword->AddForce(Vector2(5 * direction, 0), VelocityChange);
			}
			isAttacking = true;
		}

		if (AttackTimer < 0) {
			AtkCnt++;
			AttackTimer = AttackDelay;
		}

		if (AtkCnt >= s_SwordAttack.size()) {
			isAttacking = false;
			AtkCnt = 0;
		}

		if (isAttacking == true) {
			renderer->sprite = s_SwordAttack[AtkCnt];
		}
		else {
			sword->Translate(0, 1000, 0);
			sword->AddForce(Vector2(0, 0), VelocityChange);
		}

		UIUpdate();
	}

	void OnCollision(Collision collider) {
		if (rigidbody->velocity.y <= 0) {
			JumpFlag = true;
		}

		if (collider.parent->tag == "Box") {
			if (GetKeyDown(Activate)) {
				switch (collider.parent->GetComponent<Box>()->TypeOfBox) {
				case 0:
					ActiveItem = gSword;
				case 1:
					JavelinCount += 2;
				}

				Destroy(collider.parent);
			}
		}
	}

	void RecieveBlow() {
		//Do stuff aka reduce health gain score etc...
	}

	void OnCollisionEnter(Collision collider) {
		if (collider.parent->tag == "Sword" && collider.parent->name != swordName) {
			RecieveBlow();
		}
	}

	void UIUpdate() {
		Vector3 ScreenCoord = Camera::main->WorldToScreenPoint(transform->position);
		ImGui::SetNextWindowPos(ImVec2(ScreenCoord.x, ScreenCoord.y + Screen.height - 150));
		ImGui::SetNextWindowSize(ImVec2(100, 200));
		ImGui::Begin(PlayerPath.c_str(), 0, UIFlags);
		ImGui::Text("Javelin: %d", JavelinCount);
		ImGui::End();
	}

};