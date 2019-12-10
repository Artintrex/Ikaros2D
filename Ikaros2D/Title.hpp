#pragma once
#include "IkarosCore.h"

class Title : public MonoBehavior {
public:
	Title() {
		type = typeid(*this).name(); if (isAwake[type] == false)Awake(); isAwake[type] = true; if (SceneManager::isLoaded == true)Start();
	}

	GameObject* TitleBack;
	GameObject* Option;
	GameObject* Exit;
	GameObject* Editor;
	GameObject* ButtonBackdrop;
	GameObject* MainCamera;
	int BUTTON = 0, BACKDROPPOS = 0;
	int BUTTONS[3] = { -790,-755,-830 };

	ImGuiWindowFlags window_flags;

	void Awake() {

	}

	void Start() {
		//this->parent->AddComponent<Debug>();

		MainCamera = new GameObject("MainCamera");
		Camera* cam = MainCamera->AddComponent<Camera>();
		MainCamera->transform->Translate(0, 0, -40);
		cam->orthographic = true;
		cam->orthographicSize = 1;
		cam->SetProjection();


	TitleBack = new GameObject("TiTleBack");
	Option = new GameObject("Option");
	Exit = new GameObject("Exit");
	Editor = new GameObject("Editor");
	ButtonBackdrop = new GameObject("ButtonBackdrop");


	Texture* TitleBackTex = Texture::LoadTexture("TitleBackTex", "assets/Textures/title.jpg");
	Texture* OptionTex = Texture::LoadTexture("OptionTex", "assets/Textures/option.png");
	Texture* ExitTex = Texture::LoadTexture("ExitTex", "assets/Textures/exit.png");
	Texture* EditorTex = Texture::LoadTexture("EditorTex", "assets/Textures/editor.png");
	Texture* ButtonBackdropTex = Texture::LoadTexture("ButtonBackdropTex", "assets/Textures/ButtonBackdrop.png");

	Sprite* TitleBackSpr = new Sprite();
	Sprite* OptionSpr = new Sprite();
	Sprite* ExitSpr = new Sprite();
	Sprite* EditorSpr = new Sprite();
	Sprite* ButtonBackdropSpr = new Sprite();
	TitleBackSpr->texture = TitleBackTex;
	OptionSpr->texture = OptionTex;
	ExitSpr->texture = ExitTex;
	EditorSpr->texture = EditorTex;
	ButtonBackdropSpr->texture = ButtonBackdropTex;

	TitleBackSpr->GenereteSprite();
	OptionSpr->GenereteSprite();
	ExitSpr->GenereteSprite();
	EditorSpr->GenereteSprite();
	ButtonBackdropSpr->GenereteSprite();

	TitleBack->AddComponent<Renderer>()->sprite = TitleBackSpr;
	TitleBack->transform->position = Vector3(0, 0, 0.0010);
	TitleBack->transform->Scale(100, 100, 1);

	ButtonBackdrop->AddComponent<Renderer>()->sprite = ButtonBackdropSpr;
	ButtonBackdrop->transform->Scale(100, 100, 1);
	ButtonBackdrop->transform->position = Vector3(-700, -BACKDROPPOS, 0011);

	Option->AddComponent<Renderer>()->sprite = OptionSpr;
	Option->transform->position = Vector3(BUTTONS[0], 0, 0.0012);
	Option->transform->Scale(100, 100, 1);


	Editor->AddComponent<Renderer>()->sprite = EditorSpr;
	Editor->transform->Scale(100, 100, 1);
	Editor->transform->position = Vector3(BUTTONS[1], -100, 0.0012);


	Exit->AddComponent<Renderer>()->sprite = ExitSpr;
	Exit->transform->Scale(100, 100, 1);
	Exit->transform->position = Vector3(BUTTONS[2], -200, 0.0012);

	window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoScrollbar;

	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoNav;
	window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	}
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	void Update() {
		{
			int size[6] = { 720,1080,1920,2000,3000,4000 };
			static int  f = 0;
			static float  R = 720.0f;
			static int counter = 0;
			static int SOINZU = 0;
			ImGui::Begin("asd, world!", 0, window_flags);
			ImGui::SetWindowPos(ImVec2(0, 50));
			// Create a window called "Hello, world!" and append into it.

			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
			//ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
			//ImGui::Checkbox("Another Window", &show_another_window);



			ImGui::SliderInt("float", &f, 0, 1000);            // Edit 1 float using a slider from 0.0f to 1.0f
			ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

			if (ImGui::Button("REDUCE"))SOINZU--; if (SOINZU < 0)SOINZU = 5; ImGui::SameLine();
			ImGui::Text("Resolution  %d", size[SOINZU]); ImGui::SameLine();
			if (ImGui::Button("UP")) SOINZU++; if (SOINZU > 5)SOINZU = 0;

			ImGui::SliderInt("Resolution", &size[SOINZU], 720.0f, 4000.0f);

			enum Element { Element_720, Element_1080, Element_1K, Element_2K, Element_3K, Element_4k };
			const char* element_names[Element_4k] = { "720P", "1080P", "2K", "3K","4k" };
			static int current_element = Element_720;
			const char* current_element_name = (current_element >= 0 && current_element < Element_4k) ? element_names[current_element] : "Unknown";
			ImGui::SliderInt("slider Resolution", &current_element, 0, Element_4k - 1, current_element_name);





			if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
				counter++;
			ImGui::SameLine();//‚Â‚«‚ÍŒã‚ë‘‚­A‰üs‚µ‚È‚¢
			ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
		}

		if (GetKeyDown(DIK_S))
		{
			BUTTON++;
			if (BUTTON > 2)BUTTON = 0;
			BACKDROPPOS = BUTTON * 100;
		}
		if (GetKeyDown(DIK_W))
		{
			BUTTON--;
			if (BUTTON < 0)BUTTON = 2;
			BACKDROPPOS = BUTTON * 100;
		}
		ButtonBackdrop->transform->position = Vector3(-700, -BACKDROPPOS, 0.0011);

		if (GetKeyDown(DIK_RETURN) && BUTTON == 0) {
			SceneManager::LoadScene(1);
		}


	}
};