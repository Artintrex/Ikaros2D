#pragma once
#include "IkarosCore.h"
#include "Javelin.hpp"

extern int fScore1, fScore2, fScore3, fScore4;

class Player : public MonoBehavior {
public:

	//Used for loading textures
	std::string PlayerPath = "1P";

	//Sprites
	std::vector <Sprite*> sv_ShieldThrow; //5
	std::vector <Sprite*> sv_SpearAttack; //4
	std::vector <Sprite*> sv_SwordAttack; //5
	std::vector <Sprite*> sv_BowDraw; //3

	std::vector <Sprite*> sv_Idle; //4
	std::vector <Sprite*> sv_BowIdle; //4
	std::vector <Sprite*> sv_ShieldIdle; //4
	std::vector <Sprite*> sv_SwordIdle; //4
	std::vector <Sprite*> sv_SpearIdle; //4

	std::vector <Sprite*> sv_Running; //12
	std::vector <Sprite*> sv_BowRunning; //12
	std::vector <Sprite*> sv_ShieldRunning; //12
	std::vector <Sprite*> sv_SwordRunning; //12
	std::vector <Sprite*> sv_SpearRunning; //12

	std::vector <Sprite*> sv_Jump; //6
	std::vector <Sprite*> sv_BowJump; //6
	std::vector <Sprite*> sv_ShieldJump; //6
	std::vector <Sprite*> sv_SwordJump; //6
	std::vector <Sprite*> sv_SpearJump; //6

	RigidBody* rigidbody;
	RigidBody* sword;
	std::string swordName;
	Renderer* renderer;

	//Controls
	int iLeft = DIK_A,
		iRight = DIK_D,
		iJump = DIK_SPACE,
		iThrow = DIK_LCONTROL,
		iAttack = DIK_LSHIFT,
		iActivate = DIK_F;

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
			vector[i]->GenereteSprite();
		}
	}

	void Awake() {

	}

	void Start() {
		LoadSprites(5, "ShieldThrow", sv_ShieldThrow);
		LoadSprites(4, "Pierce", sv_SpearAttack);
		LoadSprites(5, "Slash", sv_SwordAttack);
		LoadSprites(3, "BowDraw", sv_BowDraw);

		LoadSprites(4, "Idle", sv_Idle);
		LoadSprites(4, "IdleBow", sv_BowIdle);
		LoadSprites(4, "IdleShield", sv_ShieldIdle);
		LoadSprites(4, "IdleSword", sv_SwordIdle);
		LoadSprites(4, "IdleSpear", sv_SpearIdle);

		LoadSprites(12, "Run", sv_Running);
		LoadSprites(12, "RunBow", sv_BowRunning);
		LoadSprites(12, "RunShield", sv_ShieldRunning);
		LoadSprites(12, "RunSword", sv_SwordRunning);
		LoadSprites(12, "RunSpear", sv_SpearRunning);

		LoadSprites(6, "Jump", sv_Jump);
		LoadSprites(6, "JumpBow", sv_BowJump);
		LoadSprites(6, "JumpShield", sv_ShieldJump);
		LoadSprites(6, "JumpSword", sv_SwordJump);
		LoadSprites(6, "JumpSpear", sv_SpearJump);

		swordName = PlayerPath + "Sword";

		UIFlags = 0;
		UIFlags |= ImGuiWindowFlags_NoTitleBar;
		UIFlags |= ImGuiWindowFlags_NoScrollbar;

		UIFlags |= ImGuiWindowFlags_NoMove;
		UIFlags |= ImGuiWindowFlags_NoResize;
		UIFlags |= ImGuiWindowFlags_NoCollapse;
		UIFlags |= ImGuiWindowFlags_NoNav;
		//UIFlags |= ImGuiWindowFlags_NoBackground;
		UIFlags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		// UIFlags |= ImGuiWindowFlags_MenuBar;

		renderer = parent->AddComponent<Renderer>();

		parent->transform->scale = scale;
		renderer->sprite = sv_Running[0];

		rigidbody = parent->AddComponent<RigidBody>();
		rigidbody->SetType(b2_dynamicBody);
		rigidbody->rigidbody->SetGravityScale(5.0f);
		rigidbody->rigidbody->SetFixedRotation(true);
		rigidbody->AddBoxCollider(Vector2(sv_Running[0]->size.x * parent->transform->scale.x * 0.5f,
			sv_Running[0]->size.y * parent->transform->scale.y * 0.8));

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

	int RunAnimCnt = 0, direction = 1;
	float RunAnimTimer = 0.5;
	bool JumpFlag = true;

	int ActiveItem = gNoItem;
	int JavelinCount = 0;

	enum Animation {
		ShieldThrow,
		SpearAttack,
		SwordAttack,
		BowDraw,
		Idle,
		Running,
		Jump,
	};


	float fSpeedMax = 9.0f;

	int AnimType = Idle;
	int AnimCnt = 0;
	float AnimTime = 0;
	float AnimDelay = 0.2;

	void Update() {
		if (abs(rigidbody->velocity.y) > 0.1f)JumpFlag = false;
		if (GetKey(iLeft) && rigidbody->velocity.x > -fSpeedMax) {
			if (JumpFlag) {
				rigidbody->AddForce(Vector2(-600, 0), Force);
			}
			else rigidbody->AddForce(Vector2(-100, 0), Force);

			transform->scale.x = -scale.x;
			direction = -1;
			if (AnimType == Idle) {
				AnimType = Running;
			}
		}

		if (GetKey(iRight) && rigidbody->velocity.x < fSpeedMax) {
			if (JumpFlag) {
				rigidbody->AddForce(Vector2(600, 0), Force);
			}
			else rigidbody->AddForce(Vector2(100, 0), Force);

			transform->scale.x = scale.x;
			direction = 1;
			if (AnimType == Idle) {
				AnimType = Running;
			}
		}

		if (GetKeyDown(iJump) && JumpFlag && (AnimType == Idle || AnimType == Running)) {
			rigidbody->AddForce(Vector2(0, 800), Impulse);
			AnimCnt = 0;
			AnimTime = AnimDelay;
			AnimType = Jump;
		}

		if (GetKeyDown(iThrow) && JavelinCount > 0) {
			GameObject* jav = new GameObject(PlayerPath + "Javelin");
			jav->tag = "Javelin";
			jav->transform->position = Vector3(transform->position.x + (direction * 3),
				transform->position.y,
				transform->position.z);

			Javelin* gjav = jav->AddComponent<Javelin>();
			gjav->direction = direction;

			JavelinCount--;
		}

		if (GetKey(iAttack) && ActiveItem == gSword) {
			if (AnimType != SwordAttack) {
				sword->Translate(transform->position.x + direction, transform->position.y - 2, 0);
				sword->AddForce(Vector2(5 * direction, 0), VelocityChange);
				AnimCnt = 0;
				AnimTime = AnimDelay;
			}
			AnimType = SwordAttack;
		}

		//Animation Timing
		AnimTime -= Time.DeltaTime;
		if (AnimTime < 0) {
			AnimCnt++;
			AnimTime = AnimDelay;
		}

		//Animation Flow
		if (AnimType == ShieldThrow) {
			if (AnimCnt >= sv_ShieldThrow.size()) {
				AnimCnt = 0;
			}
		}
		else if (AnimType == SpearAttack) {
			if (AnimCnt >= sv_SpearAttack.size()) {
				AnimCnt = 0;
			}
		}
		else if (AnimType == SwordAttack) {
			if (AnimCnt >= sv_SwordAttack.size()) {
				AnimCnt = 0;
				sword->Translate(0, 1000, 0);
				sword->AddForce(Vector2(0, 0), VelocityChange);

				AnimType = Idle;
			}
			renderer->sprite = sv_SwordAttack[AnimCnt];
		}
		else if (AnimType == BowDraw) {
			if (AnimCnt >= sv_BowDraw.size()) {
				AnimCnt = 0;
			}
		}
		else if (AnimType == Running) {
			if (RunAnimTimer < 0) {
				RunAnimCnt++;
				RunAnimTimer = 0.5;
			}
			RunAnimTimer -= Time.DeltaTime * abs(rigidbody->velocity.x);
			if (RunAnimCnt >= sv_Running.size()) RunAnimCnt = 0;

			if (ActiveItem == gNoItem)renderer->sprite = sv_Running[RunAnimCnt];
			else if (ActiveItem == gSword)renderer->sprite = sv_SwordRunning[RunAnimCnt];
			else if (ActiveItem == gSpear)renderer->sprite = sv_SpearRunning[RunAnimCnt];
			else if (ActiveItem == gShield)renderer->sprite = sv_ShieldRunning[RunAnimCnt];
			else if (ActiveItem == gBow)renderer->sprite = sv_BowRunning[RunAnimCnt];

			if (abs(rigidbody->velocity.x) < 2.0f) {
				AnimType = Idle;
			}
		}
		else if (AnimType == Jump) {
			if (AnimCnt >= sv_Jump.size()) {
				AnimCnt = sv_Jump.size()-1;
			}

			if (ActiveItem == gNoItem)renderer->sprite = sv_Jump[AnimCnt];
 			else if (ActiveItem == gSword)renderer->sprite = sv_SwordJump[AnimCnt];
			else if (ActiveItem == gSpear)renderer->sprite = sv_SpearJump[AnimCnt];
			else if (ActiveItem == gShield)renderer->sprite = sv_ShieldJump[AnimCnt];
			else if (ActiveItem == gBow)renderer->sprite = sv_BowJump[AnimCnt];

			if (JumpFlag && AnimCnt > 0)AnimType = Idle;
		}else if (AnimType == Idle) {
			if (AnimCnt >= sv_Idle.size()) {
				AnimCnt = 0;
			}

			if (ActiveItem == gNoItem)renderer->sprite = sv_Idle[AnimCnt];
			else if (ActiveItem == gSword)renderer->sprite = sv_SwordIdle[AnimCnt];
			else if (ActiveItem == gSpear)renderer->sprite = sv_SpearIdle[AnimCnt];
			else if (ActiveItem == gShield)renderer->sprite = sv_ShieldIdle[AnimCnt];
			else if (ActiveItem == gBow)renderer->sprite = sv_BowIdle[AnimCnt];
		}

		UIUpdate();
	}

	void OnCollision(Collision collider) {
		if (rigidbody->velocity.y <= 0) {
			JumpFlag = true;
		}

		if (collider.parent->tag == "Box") {
			if (GetKey(iActivate)) {
				switch (collider.parent->GetComponent<Box>()->TypeOfBox) {
				case 0:
					ActiveItem = gSword;
					break;
				case 1:
					JavelinCount += 2;
					break;
				}

				Destroy(collider.parent);
			}
		}
	}

	void OnCollisionEnter(Collision collider) {
		if (collider.parent->tag == "Sword" && collider.parent->name != swordName) {
			std::string op = collider.parent->name.substr(0, 2);
			if (op == "1P")fScore1 += 100;
			else if (op == "2P")fScore2 += 100;
			else if (op == "3P")fScore3 += 100;
			else if (op == "4P")fScore4 += 100;
		}
		else if (collider.parent->tag == "Javelin") {
			std::string op = collider.parent->name.substr(0, 2);
			if (op != PlayerPath) {
				if (op == "1P")fScore1 += 100;
				else if (op == "2P")fScore2 += 100;
				else if (op == "3P")fScore3 += 100;
				else if (op == "4P")fScore4 += 100;

				Destroy(collider.parent);
			}
		}
	}

	void UIUpdate() {
		Vector3 ScreenCoord = Camera::main->WorldToScreenPoint(transform->position);
		ImGui::SetNextWindowPos(ImVec2(ScreenCoord.x, ScreenCoord.y + Screen.height - 150));
		ImGui::SetNextWindowSize(ImVec2(100, 50));
		ImGui::Begin(PlayerPath.c_str(), 0, UIFlags);
		ImGui::Text("Javelin: %d", JavelinCount);
		if (parent->name == "Player1") {
			ImGui::Text("Score: %d", fScore1);
		}
		else if (parent->name == "Player2") {
			ImGui::Text("Score: %d", fScore2);
		}
		else if (parent->name == "Player3") {
			ImGui::Text("Score: %d", fScore3);
		}
		else if (parent->name == "Player4") {
			ImGui::Text("Score: %d", fScore4);
		}
		ImGui::End();
	}

};