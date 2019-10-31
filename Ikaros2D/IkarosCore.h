#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Box2D\Box2D.h"



class Object {
private:
	static std::vector<Object*> ObjectList;

public:
	std::string name;
	bool isHidden;

	Object(std::string Name) {
		name = Name;
		ObjectList.push_back(this);
	}

	static Object* FindbyName(std::string Name) {
		for (std::vector<Object*>::iterator it = ObjectList.begin(); it != ObjectList.end(); it++)
		{
			if (Name == (*it)->name)return *it;
			else return nullptr;
		}
	}

	static void Destroy(std::string Name) {
		for (std::vector<Object*>::iterator it = ObjectList.begin(); it != ObjectList.end(); it++)
		{
			if (Name == (*it)->name) {
				delete* it;
				ObjectList.erase(it);
			}
			//if component remove from component list
			
		}
	}
};

class GameObject : Object {
private:
	std::vector<Object*> ComponentList;

public:
	void AddComponent() {
		//add to list
	}
	Object* GetComponent(std::string Name) {
		Object* p;
		//search component list
		return p;
	}
};
class Component : Object {

};

class Behavior : Component {

};

class MonoBehavior : Behavior {
public:
	virtual void Start() {
		//Run only once
	}
	virtual void Update() {
		//Run every frame
	}
};

class Sprite : Object {

};

class Renderer : Component {

};

class RigidBody : Component {

};

class Tranform : Component {
	D3DXVECTOR3 position;
	D3DXVECTOR2 rotation;
	D3DXVECTOR2 scale;
};

class Camera : Behavior {

};

class Time {
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;
public:
	float DeltaTime;

	void Start() {
		t1 = std::chrono::high_resolution_clock::now();
	}
	void End() {
		t2 = std::chrono::high_resolution_clock::now();
		DeltaTime = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
	}
};