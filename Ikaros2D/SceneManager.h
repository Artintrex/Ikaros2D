#pragma once
#include <string>
#include <vector>

struct Scene {
	std::string name;
	std::string path;
};

class MonoBehavior;

class SceneManager{
public:
	static Scene GetActiveScene();

	static void LoadScene(std::string Path);
	static void LoadScene(int SceneNumber);

	static void UnloadScene();

	static std::vector<MonoBehavior*>ActiveAwakerList;
	static std::vector<MonoBehavior*>ActiveStarterList;

	static void RunActiveInitilizer();

	static bool isLoaded;
private:
	static Scene scene;
};

