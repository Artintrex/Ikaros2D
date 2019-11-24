#pragma once
#include <iostream>
#include <cmath>
#include <map>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Box2D\Box2D.h"
#include "input.h"

//NEED UPDATE: Change Resolution Settings needs to read from file!! Also set d3d parameters like fullscreen etc
#define SCREEN_WIDTH  (1024)
#define SCREEN_HEIGHT  (768)

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;

//NEED UPDATE:
//class Collider(Types)
class iTime;
class Object;
class GameObject;
class Component;
class Behavior;
class Texture;
class Sprite;
//Components
class Transform;
class MonoBehavior;
class Renderer;
class RigidBody;
class Camera;

//Have to have my Time.DeltaTime
extern iTime Time;

struct VertexBufferData {
	D3DXVECTOR3 position;
	D3DCOLOR color;
	D3DXVECTOR2 uv;
};

struct Rect {
	float X;
	float Y;
	float W;
	float H;
};

//RTTI type info from template instances by Stefan Reinalter. Only works on MSVS "class #classname"
namespace internal
{
	static const unsigned int FRONT_SIZE = sizeof("internal::GetTypeNameHelper<") - 1u;
	static const unsigned int BACK_SIZE = sizeof(">::GetTypeName") - 1u;

	template <typename T>
	struct GetTypeNameHelper
	{
		static const char* GetTypeName(void)
		{
			static const size_t size = sizeof(__FUNCTION__) - FRONT_SIZE - BACK_SIZE;
			static char typeName[size] = {};
			memcpy(typeName, __FUNCTION__ + FRONT_SIZE, size - 1u);

			return typeName;
		}
	};
}

class ComponentFactory {
public:
	virtual Component* AddComponent(GameObject* parent, std::string type) = 0;
};

//Creates factory function for custom component and maps pointer with its string name
#define REGISTER_COMPONENT(component) \
    class component##Factory : public ComponentFactory { \
    public: \
        component##Factory() \
        { \
            Component::registerType(#component, this); \
        } \
        virtual Component* AddComponent(GameObject* parent, std::string type) { \
			Component* p = new component();\
			p->parent = parent;\
			p->transform = parent->transform;\
			parent->ComponentList.push_back((Component*)p);\
            return p; \
        } \
    }; \
    static component##Factory global_##component##Factory;


template <typename T>
const char* GetTypeName(void)
{
	return internal::GetTypeNameHelper<T>::GetTypeName();
}

class iTime {
public:
	///Time it took to render previous frame
	float DeltaTime;

	void Start() {
		t1 = std::chrono::high_resolution_clock::now();
	}
	void End() {
		t2 = std::chrono::high_resolution_clock::now();
		DeltaTime = std::chrono::duration_cast<std::chrono::microseconds>(t2 - t1).count();
		DeltaTime /= 1000000;
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

};

class Object {
public:
	std::string name;
	bool isHidden;

	Object(std::string Name);
	virtual ~Object();
	
private:
	static std::vector<Object*> ObjectList;

	static void ReleaseObjects();

	friend class SceneManager;
};

class Component : public Object {
public:
	GameObject* parent;
	Transform* transform;

	Component(std::string Name = "EmptyComponent");
	virtual ~Component();

	static void registerType(const std::string& name, ComponentFactory* factory) {
		factories[name] = factory;
	}

	static std::map<std::string, ComponentFactory*> factories;
private:
	
};

class Behavior : public Component {
public:
	bool enabled;
	Behavior(std::string Name) : Component(Name) {
		enabled = true;
	}
};

class MonoBehavior : public Behavior {
public:
	MonoBehavior(std::string Name = "MonoBehavior");
	virtual ~MonoBehavior();


	virtual void Awake() = 0;
	virtual void Start() = 0;
	virtual void Update() = 0;

	///Do not call this function, meant for main loop
	static void AwakeMonoBehaviorArray();
	///Do not call this function, meant for main loop
	static void StartMonoBehaviorArray();
	///Do not call this function, meant for main loop
	static void UpdateMonoBehaviorArray();

private:
	static std::vector<MonoBehavior*> MonoBehaviorList;
};

class Texture : public Object {
public:
	LPDIRECT3DTEXTURE9* texturedata;
	int Width;
	int Height;

	Texture(std::string Name);
	~Texture();

	///File formats : .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga
	static Texture* LoadTexture(std::string TextureName, LPCTSTR FilePath);

	static Texture* FindTexturebyName(std::string Name);

private:
	static std::vector<Texture*> TextureList;
	static bool CreateTexture(LPCTSTR FilePath, LPDIRECT3DTEXTURE9* texturedata);
};

class Sprite : public Object {
public:
	Object* renderer;
	Texture* texture;

	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
	LPDIRECT3DINDEXBUFFER9 IndexBuffer;

	Sprite(std::string Name = "Sprite");
	~Sprite();

	void GenereteSprite(std::string Name = "");

private:
	VertexBufferData* vertices;
	WORD* indices;
};

class Renderer : public Component {
public:
	Vector3 color;
	Sprite* sprite;
	Vector2 size;
	int sortingOrder;

	Renderer(std::string Name = "Renderer");
	~Renderer();

	static std::vector<Renderer*> RendererList;
};

class RigidBody : public Component {
public:
	RigidBody(std::string Name = "RigidBody") : Component(Name) {
		//NEED UPDATE: actually add physics instad of husk
	}

	~RigidBody() {
		//NEED UPDATE: Release fixtures etc
	}
};

class Transform : public Component {
public:
	Vector3 position;
	Vector3 rotation;
	Vector3 scale;
	D3DXMATRIX localToWorldMatrix;

	Transform(Vector3 Position = Vector3(0, 0, 0), Vector3 Rotation = Vector3(0, 0, 0), Vector3 Scale = Vector3(1, 1, 1));
	~Transform();

	//Moves the tranform 
	void Translate(float x, float y, float z);

	void Rotate(float x, float y, float z);

	void Scale(float x, float y, float z);

	static void UpdateTransform();

private:
	Vector3 oldposition;
	Vector3 oldrotation;
	Vector3 oldscale;

	static std::vector<Transform*> TransformList;

	void SetMatrix();
};

class Camera : public Behavior {
public:
	Transform* transform;
	float fieldOfView;
	float aspect;
	float farClipPlane;
	float nearClipPlane;
	bool orthographic;
	float orthographicSize;
	Rect rect;
	
	//NEED UPDATE: Add Layer systems to filter objects for cameras and clear camera list

	Camera(std::string Name = "Camera");
	~Camera();

	///Calculates projection matrix
	void SetProjection();

	///Do not call this method. It's meant for main loop
	static void Draw();
private:
	static std::vector<Camera*> CameraList;

	D3DVIEWPORT9 viewport;

	D3DXMATRIX mCameraRot;
	D3DXMATRIX mCameraWorld;
	D3DXMATRIX View;
	D3DXMATRIX Projection;

	void draw();

	//Calculates viewport based on rect and screen resolution
	void SetViewport();
	
	///Sets D3DDevice RenderStates
	void SetD3DDevice();

	//Calculates view matrix
	void SetCamera();
};

class GameObject : public Object {
public:
	Transform *transform;

	GameObject(std::string Name = "GameObject");
	~GameObject();

	static GameObject* Find(std::string Name);

	template <class T> T* AddComponent() {
		T* p = new T();
		p->parent = this;
		p->transform = this->transform;
		ComponentList.push_back((Component*)p);

		return p;
	}

	template <class T> T* GetComponent() {
		std::string Type;
		Type += GetTypeName<T>();
		for (std::vector<Component*>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it)
		{
			if (Type == "class " + (*it)->name)return static_cast<T*>(*it);
		}
		return nullptr;
	}

	std::vector<Component*> ComponentList;
private:
	static std::vector<GameObject*> GameObjectList;
};