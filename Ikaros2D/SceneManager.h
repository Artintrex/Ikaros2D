#pragma once
#include <string>

struct Scene {
	std::string name;
	std::string path;
};

class SceneManager{
public:
	static Scene GetActiveScene();

	static void LoadScene(std::string Path);
	static void LoadScene(int SceneNumber);

	static void UnloadScene();

	static bool isLoaded;
private:
	static Scene scene;
};

