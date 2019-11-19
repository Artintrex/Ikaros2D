#pragma once
#include <string>

struct Scene {
	bool isLoaded = false;
	std::string name;
	std::string path;
};

class SceneManager{
public:
	static Scene GetActiveScene();
	static void LoadScene(std::string Path);
	static void UnloadScene();

private:
	static Scene scene;
};

