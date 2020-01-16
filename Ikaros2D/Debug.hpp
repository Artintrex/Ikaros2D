#pragma once
#include "IkarosCore.h"

class Debug : public MonoBehavior {
public:
	Debug() {
		type = typeid(*this).name();
		mb_init();
	}
	ImGuiWindowFlags window_flags;
	void Awake() {
		window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoScrollbar;

		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
		window_flags |= ImGuiWindowFlags_NoNav;
		//window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		window_flags |= ImGuiWindowFlags_MenuBar;
	}

	void Start() {
		/*
	
		std::cout << "Factory Register" << "\n";
		for (auto it = Component::factories.cbegin(); it != Component::factories.cend(); ++it)
		{
			std::cout << it->first << " " << it->second << "\n";
		}

		std::cout << SceneManager::GetActiveScene().path << "\n";
		*/

		Texture::LoadTexture("null", "Assets/Textures/null.png");

		/*
		camObj2 = new GameObject("Camera2");
		cam = camObj2->AddComponent<Camera>();
		camObj2->transform->Translate(0, 2, -5000);
		cam->rect.X = 0.5;
		cam->rect.W = 0.5;
		cam->rect.H = 0.5;
		cam->SetProjection();
		*/
	}


	float Timer = 0;
	void Update() {
		Timer -= Time.DeltaTime;

		if (Timer < 0) {
			//ListObject();
			Timer = 120;
		}

		static float f = 0.0f;
		static int counter = 0;

		/*
		ImGui::Begin("Hello, world!", 0, window_flags);                          // Create a window called "Hello, world!" and append into it.
		ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

		ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

		if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
			counter++;
		ImGui::SameLine();
		ImGui::Text("counter = %d", counter);

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
		*/
		ImGui::ShowDemoWindow();
	}


	void ListObject() {
		std::cout << "\n" << "--------Object List--------" << "\n" << "\n" << "\n";
		for (auto p : Object::ObjectList)
		{
			std::cout << p->name << " " << p <<"\n";
		}
		std::cout << "\n" << "Number of Objects:  " << Object::ObjectList.size() << "\n";
	}
};