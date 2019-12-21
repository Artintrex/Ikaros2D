#pragma once
#include "IkarosCore.h"
#include "Title.hpp"

class GameManager : public MonoBehavior {
public:
	GameManager() {
		type = typeid(*this).name(); if (isAwake[type] == false)Awake(); isAwake[type] = true; if (SceneManager::isLoaded == true)Start();
	}
	enum Scene {
		sTitle,
		sAlpha
	};
	int ActiveScene;
	bool SceneisLoaded;

	GameObject* Alpha;
	GameObject* objTitle;
	Title* mTitle;

	void Awake() {

	}
	ImGuiWindowFlags window_flags;
	void Start() {
		this->parent->AddComponent<Debug>();

		LoadAlpha();

		window_flags = 0;
		window_flags |= ImGuiWindowFlags_NoTitleBar;
		window_flags |= ImGuiWindowFlags_NoScrollbar;

		window_flags |= ImGuiWindowFlags_NoMove;
		window_flags |= ImGuiWindowFlags_NoResize;
		window_flags |= ImGuiWindowFlags_NoCollapse;
	    window_flags |= ImGuiWindowFlags_NoNav;
		window_flags |= ImGuiWindowFlags_NoBackground;
		window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
		// window_flags |= ImGuiWindowFlags_MenuBar;
	}
	void Update() {
		{
			static float f = 0.0f;
			static int counter = 0;

			ImGui::Begin("Hello, world!", 0, window_flags);                          // Create a window called "Hello, world!" and append into it.
			ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

			ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

		    if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
		        counter++;
		    ImGui::SameLine();
		    ImGui::Text("counter = %d", counter);

			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
			ImGui::End();
			//ImGui::ShowDemoWindow();
		}
	}

	void ChangeScene(Scene scene) {
		if (ActiveScene == sAlpha)UnLoadAlpha();
		else if (ActiveScene == sTitle)UnLoadTitle();
	
		if (scene == sAlpha) LoadAlpha();
		else if (scene == sTitle) LoadTitle();
	}

private:
	void LoadAlpha() {
		ActiveScene = sAlpha;

		Alpha = new GameObject();
		Alpha->AddComponent<SceneAlpha>(); //run start here for now
	}

	void LoadTitle() {
		ActiveScene = sTitle;
	}

	void UnLoadAlpha() {
	}

	void UnLoadTitle() {
	}
};