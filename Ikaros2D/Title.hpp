#pragma once
#include "IkarosCore.h"

class Title : public MonoBehavior {
public:
	Title() {
		type = typeid(*this).name();
		mb_init();
	}

	GameObject* TitleBack;

	GameObject* OptionButton;
	GameObject* ExitButton;
	GameObject* EditorButton;
	GameObject* StartButton;

	GameObject* MainCamera;

	int BUTTON = 0;
	int BUTTONPOS[4] = {-780,-780 ,-780 ,-780 };
	ImGuiWindowFlags window_flags;

	Renderer* OptionRenderer;
	Renderer* ExitRenderer;
	Renderer* StartRenderer;
	Renderer* EditorRenderer;

	Sprite* OptionSpr;
	Sprite* ExitSpr;
	Sprite* EditorSpr;
	Sprite* StartSpr;

	Sprite* OptionSpr1;
	Sprite* ExitSpr1;
	Sprite* EditorSpr1;
	Sprite* StartSpr1;
	int size[6] = { 720,1080,1920,2000,3000,4000 };
	int  f = 0;
	float  R = 720.0f;
	int counter = 0;
	int SOINZU = 0;
	bool GUI_MENU = 0;
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
	OptionButton = new GameObject("Option");
	ExitButton = new GameObject("Exit");
	EditorButton = new GameObject("Editor");
	StartButton = new GameObject("Start2");

	Sprite* TitleBackSpr = new Sprite("TitleBackground", Texture::LoadTexture("TitleBackTex", "assets/Textures/title.jpg"));
	 OptionSpr = new Sprite("OptionButton", Texture::LoadTexture("OptionTex", "assets/Textures/TitleButtons/tOptions2.png"));
	 ExitSpr = new Sprite("ExitButton", Texture::LoadTexture("ExitTex", "assets/Textures/TitleButtons/tExit2.png"));
	 EditorSpr = new Sprite("EditorButton", Texture::LoadTexture("EditorTex", "assets/Textures/TitleButtons/tEditor2.png"));
	 StartSpr = new Sprite("StartButton", Texture::LoadTexture("StartTex", "assets/Textures/TitleButtons/tStart2.png"));

	 OptionSpr1 = new Sprite("OptionButton", Texture::LoadTexture("OptionTex", "assets/Textures/TitleButtons/tOptions2HL.png"));
	 ExitSpr1 = new Sprite("ExitButton", Texture::LoadTexture("ExitTex", "assets/Textures/TitleButtons/tExit2HL.png"));
	 EditorSpr1 = new Sprite("EditorButton", Texture::LoadTexture("EditorTex", "assets/Textures/TitleButtons/tEditor2HL.png"));
	 StartSpr1 = new Sprite("StartButton", Texture::LoadTexture("StartTex", "assets/Textures/TitleButtons/tStart2HL.png"));




	TitleBack->AddComponent<Renderer>()->sprite = TitleBackSpr;
	TitleBack->transform->position = Vector3(0, 0, 0.0010);
	TitleBack->transform->Scale(100, 100, 1);

	StartRenderer = StartButton->AddComponent<Renderer>();
	StartRenderer->sprite = StartSpr;
	StartButton->transform->Scale(70, 70, 1);
	StartButton->transform->position = Vector3(750, 0, 0.0012);

	OptionRenderer = OptionButton->AddComponent<Renderer>();
	OptionRenderer->sprite = StartSpr;
	OptionButton->transform->position = Vector3(750, -100, 0.0012);
	OptionButton->transform->Scale(70, 70, 1);

	EditorRenderer = EditorButton->AddComponent<Renderer>();
	EditorRenderer->sprite = EditorSpr;
	EditorButton->transform->Scale(70, 70, 1);
	EditorButton->transform->position = Vector3(750, -200, 0.0012);


	ExitRenderer = ExitButton->AddComponent<Renderer>();
	ExitRenderer->sprite = ExitSpr;
	ExitButton->transform->Scale(70, 70, 1);
	ExitButton->transform->position = Vector3(750, -300, 0.0012);

	window_flags = 0;
	window_flags |= ImGuiWindowFlags_NoTitleBar;
	window_flags |= ImGuiWindowFlags_NoScrollbar;

	window_flags |= ImGuiWindowFlags_NoMove;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_NoNav;
	//window_flags |= ImGuiWindowFlags_NoBackground;
	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

	}
	ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);
	void Update() {
		{
			

			StartButton->transform->position = Vector3(BUTTONPOS[0], 0, 0.0012);
			OptionButton->transform->position = Vector3(BUTTONPOS[1], -100, 0.0012);
			EditorButton->transform->position = Vector3(BUTTONPOS[2], -200, 0.0012);
			ExitButton->transform->position = Vector3(BUTTONPOS[3], -300, 0.0012);
			
			
		}

		if(BUTTON==0)//start
		{
			BUTTONPOS[0] = -650;
			StartRenderer->sprite = StartSpr1;

			if (GetKeyDown(DIK_RETURN) ) {
				SceneManager::LoadScene(3);
				return;
			}
		}
		else {
			BUTTONPOS[0] = -780;
			StartRenderer->sprite = StartSpr;
		}
		if (BUTTON == 1) //options
		{
			BUTTONPOS[1] = -650;
			OptionRenderer->sprite = OptionSpr1;
			if (GetKeyDown(DIK_RETURN) )
			{
				GUI_MENU = 1;
			}
		}
		else {
			BUTTONPOS[1] = -780;
			OptionRenderer->sprite = OptionSpr;
		}
		if (BUTTON == 2)//editor
		{
			BUTTONPOS[2] = -650;
			EditorRenderer->sprite = EditorSpr1;
			
		}
		else
		{
			BUTTONPOS[2] = -780;
			EditorRenderer->sprite = EditorSpr;

		}

		if (BUTTON == 3)//exit
		{
			BUTTONPOS[3] = -650;
			ExitRenderer->sprite = ExitSpr1;
			if (GetKeyDown(DIK_RETURN)) {
				Shutdown();
			}
		
		}
		else
		{
			BUTTONPOS[3] = -780;
			ExitRenderer->sprite = ExitSpr;

		}
		//‘I‚Ô
		if (GetKeyDown(DIK_S)&& GUI_MENU == 0)
		{
			BUTTON++;
			if (BUTTON > 3)BUTTON = 0;
			
		}
		if (GetKeyDown(DIK_W)&& GUI_MENU == 0)
		{
			BUTTON--;
			if (BUTTON < 0)BUTTON = 3;
			
		}
		

		
		if (GUI_MENU == 1)//GUI_MENU‚É‚æ‚Á‚ÄGui‚ðŒÄ‚Ô
		{
			Gui_Options();
		}
		if (GetKeyDown(DIK_ESCAPE))
		{
			GUI_MENU = 0;
		}

		
	}
	void Gui_Options()
	{
		ImGui::SetNextWindowPos(ImVec2(50, 50));
		ImGui::SetNextWindowSize(ImVec2(Camera::main->pixelWidth-100, Camera::main->pixelHeight-100));
		ImGui::Begin("Option Menu", 0, window_flags);
		ImGui::SetWindowFontScale(2.0f);

		ImGui::AlignTextToFramePadding();
		ImGui::Text("OPTIONS");
		ImGui::SameLine(Camera::main->pixelWidth - 800);
		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

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


		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();
		ImGui::NewLine();

		if (ImGui::Button("SAVE")) {

		}

		ImGui::End();
	}
};