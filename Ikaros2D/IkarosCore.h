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
#define SCREEN_WIDTH  (1920)
#define SCREEN_HEIGHT  (1080)

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;

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

enum ForceMode {
	Force,
	Impulse,
	VelocityChange
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

	iTime() {
		DeltaTime = 1;
	}

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
	friend class Debug;
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
	Vector2 size;

	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
	LPDIRECT3DINDEXBUFFER9 IndexBuffer;

	Sprite(Texture* pTexture = nullptr, std::string Name = "Sprite");
	~Sprite();

	void SetColor(D3DCOLOR rgba);
	void GenereteSprite(std::string Name = "null");

private:
	VertexBufferData* vertices;
	WORD* indices;
	D3DCOLOR color;
};

class Renderer : public Component {
public:
	Sprite* sprite;
	Vector2 size;
	int sortingOrder;

	Renderer(GameObject* Parent, std::string Name = "Renderer");
	~Renderer();

	static std::vector<Renderer*> RendererList;
};

//Holds transform data and matrice calculations. 
//THIS COMPONENT IS HANDLED BY THE ENGINE. DO NOT ADD THIS !!
class Transform : public Component {
public:
	//If there is a rigidbody attached these will be overwritten. Use RigidBody component for transformation instead.
	Vector3 position;
	//If there is a rigidbody attached these will be overwritten. Use RigidBody component for transformation instead.
	Vector3 rotation;
	//If there is a rigidbody attached these will be overwritten. Use RigidBody component for transformation instead.
	Vector3 scale;
	D3DXMATRIX localToWorldMatrix;

	Transform(Vector3 Position = Vector3(0, 0, 0), Vector3 Rotation = Vector3(0, 0, 0), Vector3 Scale = Vector3(1, 1, 1));
	~Transform();

	//Moves the tranform by entered values Additive.
	//If there is a rigidbody attached these will be overwritten. Use RigidBody component for transformation instead.
	void Translate(float x, float y, float z);

	//Additive rotation.
	//If there is a rigidbody attached these will be overwritten. Use RigidBody component for transformation instead.
	void Rotate(float x, float y, float z);

	//Multiplies scale value
	void Scale(float x, float y, float z);

	static void UpdateTransform();

private:
	Vector3 oldposition;
	Vector3 oldrotation;
	Vector3 oldscale;

	static std::vector<Transform*> TransformList;

	void SetMatrix();
};

//Adds physics to a GameObject
class RigidBody : public Component {
public:
	b2Body* rigidbody;
	Vector2 velocity;

	RigidBody(GameObject* Parent, std::string Name = "RigidBody");

	~RigidBody();

	static void UpdateRigidBody();

	void SetType(b2BodyType type) {
		rigidbody->SetType(type);
	}

	void SetContinuousDynamic(bool flag) {
		rigidbody->SetBullet(flag);
	}

	void AddForce(Vector2 force, ForceMode mode);
	void AddTorque(float torque, ForceMode mode);

	void Translate(float x, float y, float z) {
		rigidbody->SetTransform(b2Vec2(x, y), transform->rotation.z);
		transform->position.z = z;
	}
	void Rotate(float Ex, float Ey, float Ez) {
		rigidbody->SetTransform(b2Vec2(transform->position.x, transform->position.y), D3DXToRadian(Ez));
		transform->rotation.x = Ex;
		transform->rotation.y = Ey;
	}

	void AddCircleCollider(float radius = 1, Vector2 center = Vector2(0, 0), float density = 1.0f, float friction = 0.2f, float bounciness = 0, uint16 maskBits = 0x0001, bool isTrigger = false);
	void AddBoxCollider(Vector2 size = Vector2(1, 1), Vector2 center = Vector2(0, 0), float angle = 0, float density = 1.0f, float friction = 0.2f, float bounciness = 0, uint16 maskBits = 0x0001, bool isTrigger = false);

	std::vector<b2Fixture*> ColliderList;
private:
	static std::vector<RigidBody*> RigidBodyList;
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

	Camera(GameObject* Parent, std::string Name = "Camera");
	~Camera();

	///Calculates projection matrix
	void SetProjection();

	///Do NOT CALL THIS METHOD. It's meant for main loop
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
		p->transform = transform;
		ComponentList.push_back((Component*)p);

		return p;
	}

	template <> Camera* AddComponent<Camera>() {
		Camera* p = new Camera(this);
		ComponentList.push_back((Component*)p);
		return p;
	}

	template <> RigidBody* AddComponent<RigidBody>() {
		RigidBody* p = new RigidBody(this);
		ComponentList.push_back((Component*)p);
		return p;
	}

	template <> Renderer* AddComponent<Renderer>() {
		Renderer* p = new Renderer(this);
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