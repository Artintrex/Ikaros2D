#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Box2D\Box2D.h"

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;

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

	~Object() {
		for (std::vector<Object*>::iterator it = ObjectList.begin(); it != ObjectList.end(); it++)
		{
			if (this == (*it)) {
				ObjectList.erase(it);
			}
		}
	}
};

class Component : public Object {
public:
	Object* parent;

	Component(std::string Name) : Object(Name) {

	}

	void AssignParent(Object* p) {
		parent = p;
	}
};

class Behavior : public Component {
public:
	Behavior(std::string Name) : Component(Name) {

	}
};

class MonoBehavior : public Behavior {
public:
	virtual void Start() {
		//Run only once
	}
	virtual void Update() {
		//Run every frame
	}

	MonoBehavior() : Behavior("MonoBehavior") {
		//write stuff here
	}
};

class Sprite : public Object {
	//load texture
	//list of sprites
};

class Renderer : public Component {
public:
	Renderer() : Component("Renderer") {

	}
	//assign sprite

	//render stuff
};

class RigidBody : public Component {
public:
	RigidBody() : Component("RigidBody") {
		//write stuff here
	}
};

class Transform : public Component {
public:
	Vector3 position;
	Vector2 rotation;
	Vector2 scale;

	Transform(Vector3 Position = Vector3(0, 0, 0), Vector2 Rotation = Vector2(0, 0), Vector2 Scale = Vector2(0, 0)) : Component("Transform") {
		position = Position;
		rotation = Rotation;
		scale = Scale;
	}

	//Translate	
	//Rotate
};

class Camera : public Behavior {
	//Camera calculations
	//maybe move all objects relative to camera here
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


class GameObject : public Object {
private:
	static std::vector<GameObject*> GameObjectList;
	std::vector<Object*> ComponentList;

public:
	Transform transform;

	void AddComponent(std::string type) {
		if (type == "Renderer") {
			Renderer* p = new Renderer();
			p->AssignParent(static_cast<Object*>(this));
			ComponentList.push_back((Object*)p);
		}
		else if (type == "RigidBody") {
			RigidBody* p = new RigidBody();
			p->AssignParent(static_cast<Object*>(this));
			ComponentList.push_back((Object*)p);
		}
		else if (type == "MonoBehavior") {
			MonoBehavior* p = new MonoBehavior();
			p->AssignParent(static_cast<Object*>(this));
			ComponentList.push_back((Object*)p);
		}
	}

	void RemoveComponent(std::string type) {
		for (std::vector<Object*>::iterator it = ComponentList.begin(); it != ComponentList.end(); it++)
		{
			if (type == (*it)->name) {
				delete (*it);
				ComponentList.erase(it);
			}
		}
	}

	Component* GetComponent(std::string Name) {
		Object* p;
		for (std::vector<Object*>::iterator it = ComponentList.begin(); it != ComponentList.end(); it++)
		{
			if (Name == (*it)->name)return static_cast<Component*>(*it);
			else return nullptr;
		}
	}

	static GameObject* FindbyName(std::string Name) {
		for (std::vector<GameObject*>::iterator it = GameObjectList.begin(); it != GameObjectList.end(); it++)
		{
			if (Name == (*it)->name)return *it;
			else return nullptr;
		}
	}

	//Constructor with name parameter send to object and destructor to destroy all components
};
