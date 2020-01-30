#pragma once
#include <iostream>
#include <cmath>
#include <map>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Box2D\Box2D.h"
#include "input.h"
#include "imgui\imgui.h"
#include "imgui\imgui_impl_dx9.h"
#include "imgui\imgui_impl_win32.h"
//#include <tchar.h>
#include "SceneManager.h"

//NEED UPDATE: Change Resolution Settings needs to read from file!! Also set d3d parameters like fullscreen etc
#define SCREEN_WIDTH  (1920)
#define SCREEN_HEIGHT  (1080)

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;
typedef D3DXVECTOR4 Vector4;
typedef D3DXQUATERNION Quaternion; //NEED UPDATE Switch to quaternion rotation

//Vector3
/*
struct Vector3 {
	/// Constructors
	Vector3() {
		x = 0;
		y = 0;
		z = 0;
	}
	Vector3(float x, float y, float z) : x(x), y(y), z(z) { ; }
	Vector3(Vector3 const& p) { x = p.x; y = p.y; z = p.z; }

	float sqrMagnitude() {
		return (float)((double)x * (double)x + (double)y * (double)y + (double)z * (double)z);
	}

	/// Arithmetic operators for Vector3
	Vector3 operator + (Vector3 const& p)
	{
		return Vector3(x + p.x, y + p.y, z + p.z);
	}

	Vector3 operator - (Vector3 const& p)
	{
		return Vector3(x - p.x, y - p.y, z - p.z);
	}

	Vector3 operator * (Vector3 const& p)
	{
		return Vector3(x * p.x, y * p.y, z * p.z);
	}

	Vector3 operator / (Vector3 const& p)
	{
		return Vector3(x / p.x, y / p.y, z / p.z);
	}

	/// Assignation operators for Vector3
	Vector3 operator += (Vector3 const& p)
	{
		x += p.x; y += p.y; z += p.z;
		return *this;
	}

	Vector3 operator -= (Vector3 const& p)
	{
		x -= p.x; y -= p.y; z -= p.z;
		return *this;
	}

	Vector3 operator *= (Vector3 const& p)
	{
		x *= p.x; y *= p.y; z *= p.z;
		return *this;
	}

	Vector3 operator /= (Vector3 const& p)
	{
		x /= p.x; y /= p.y; z /= p.z;
		return *this;
	}

	/// Arithmetic operators for float
	Vector3 operator + (float const& p)
	{
		return Vector3(x + p, y + p, z + p);
	}

	Vector3 operator - (float const& p)
	{
		return Vector3(x - p, y - p, z - p);
	}

	Vector3 operator * (float const& p)
	{
		return Vector3(x * p, y * p, z * p);
	}

	Vector3 operator / (float const& p)
	{
		return Vector3(x / p, y / p, z / p);
	}

	/// Assignation operators for float
	Vector3 operator += (float const& p)
	{
		x += p; y += p; z += p;
		return *this;
	}

	Vector3 operator -= (float const& p)
	{
		x -= p; y -= p; z -= p;
		return *this;
	}

	Vector3 operator *= (float const& p)
	{
		x *= p; y *= p; z *= p;
		return *this;
	}

	Vector3 operator /= (float const& p)
	{
		x /= p; y /= p; z /= p;
		return *this;
	}

	//Logical operators for Vector3
	bool operator == (Vector3 const& p)
	{
		Vector3 p2(*this - p);
		return (double)p2.sqrMagnitude() < 9.99999943962493E-11;
	}

	bool operator !=(Vector3 const& p)
	{
		Vector3 p2(*this - p);
		return (double)p2.sqrMagnitude() >= 9.99999943962493E-11;
	}

	//Convert to D3DXVECTOR3 for D3DX functions
	D3DXVECTOR3 ToD3DXVECTOR3() {
		return D3DXVECTOR3(x, y, z);
	}

	float x;
	float y;
	float z;
};
*/

class iTime;
class iScreen;
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
extern iScreen Screen;

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
	virtual Component* AddComponent(GameObject* parent) = 0;
};

//Creates factory function for custom component and maps pointer with its string name
#define REGISTER_COMPONENT(component) \
    class component##Factory : public ComponentFactory { \
    public: \
        component##Factory() \
        { \
            Component::registerType(#component, this); \
        } \
        virtual Component* AddComponent(GameObject* parent) { \
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
		timeScale = 1000000000;
	}

	void SetTimeScale(float t) {
		timeScale = 1000000000 / t;
	}

	void Start() {
		t1 = std::chrono::steady_clock::now();
	}
	void End() {
		t2 = std::chrono::steady_clock::now();
		DeltaTime = std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1).count();
		DeltaTime /= timeScale;
	}
private:
	std::chrono::time_point<std::chrono::steady_clock> t1, t2;
	int timeScale;
};

struct Resolution {
	Resolution() {
		height = NULL;
		width = NULL;
		refreshRate = NULL;
	}
	int height;
	int width;
	int refreshRate;
};

class iScreen {
public:
	iScreen() {
		width = SCREEN_WIDTH;
		height = SCREEN_HEIGHT;
		fullScreen = false;
	}

	//Current window width
	int width;
	//Current window height
	int height;
	//Current screen resolution
	Resolution currentResolution;

	bool fullScreen;

	//fullScreenMode

	//float brightness
	//void SetResolution();
};


class Object {
public:
	std::string name;
	bool isHidden;

	Object(std::string Name);
	virtual ~Object();

	static void Destroy(Object* obj);
	
private:
	static std::vector<Object*> ObjectList;
	static std::unordered_set<Object*> DestroyList;

	static void ReleaseObjects();

	friend class SceneManager;
	friend class Debug;
	friend void GameLoop();
};

struct Collision {
	GameObject* parent;
	RigidBody* rigidbody;
	b2Fixture* b2fixture;
};

class Component : public Object {
public:
	GameObject* parent;
	Transform* transform;
	std::string type;

	Component(std::string Name = "EmptyComponent");
	virtual ~Component();

	virtual void OnCollisionEnter(Collision collider) {

	}

	virtual void OnCollisionExit(Collision collider) {

	}

	virtual void OnCollision(Collision collider) {

	}

	static void registerType(const std::string& name, ComponentFactory* factory) {
		factories[name] = factory;
	}

	static std::unordered_map<std::string, ComponentFactory*> factories;
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

private:
	friend class SceneManager;
	friend class Debug;
	friend void GameLoop();

	static void AwakeMonoBehaviorArray();
	static void StartMonoBehaviorArray();
	static void UpdateMonoBehaviorArray();

protected:
	static std::unordered_map<std::string, bool> isAwake;

	//Handles Awake and Start initilization and adds reflection to MB
	void mb_init() {
		if (isAwake[type] == false && SceneManager::isLoaded) {
			SceneManager::ActiveAwakerList.push_back(this); isAwake[type] = true;
		}
		if (SceneManager::isLoaded) {
			SceneManager::ActiveStarterList.push_back(this);
		}
	}
private:
	static std::vector<MonoBehavior*> MonoBehaviorList;

	friend class SceneManager;
};

class Texture : public Object {
public:
	LPDIRECT3DTEXTURE9* texturedata;
	int Width;
	int Height;

	Texture(std::string Name);
	~Texture();

	///File formats : .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga
	static Texture* LoadTexture(std::string TextureName, std::string FilePath);

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

	Vector2 uvMatrix[4] = {Vector2(0,0), Vector2(1,0), Vector2(0,1), Vector2(1,1)};

	//Enables generating double-sided polygon for this spite
	bool doubleSided;

	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;
	LPDIRECT3DINDEXBUFFER9 IndexBuffer;

	Sprite(std::string Name = "Sprite", Texture* pTexture = nullptr);
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

//Holds transform data and matrix calculations. 
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

class CollisionCallback : public b2ContactListener {
	void BeginContact(b2Contact* contact);

	void EndContact(b2Contact* contact);

	void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);
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
	static Camera* main;
	Transform* transform;
	float fieldOfView;
	float aspect;
	float farClipPlane;
	float nearClipPlane;
	bool orthographic;
	float orthographicSize;
	int pixelWidth;
	int pixelHeight;
	Rect rect;
	
	//NEED UPDATE: Add Layer systems to filter objects for cameras
	//frustum culling needed too

	Camera(GameObject* Parent, std::string Name = "Camera");
	~Camera();

	//Calculates world position from 2D screen position using Z value
	Vector3 ScreenToWorldPoint(Vector3 position);

	//Calculates screen position of a point from world coordinates
	Vector3 WorldToScreenPoint(Vector3 position);

	///Calculates projection matrix
	void SetProjection();
private:
	friend class SceneManager;
	friend class Debug;
	friend void GameLoop();

	static std::vector<Camera*> CameraList;

	D3DVIEWPORT9 viewport;

	D3DXMATRIX mCameraRot;
	D3DXMATRIX View;
	D3DXMATRIX Projection;

	//Draws all cameras
	static void Draw();

	//Draws this camera
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
	std::string tag;

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
		Type = GetTypeName<T>();
		for (std::vector<Component*>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it)
		{
			if (Type == (*it)->type)return static_cast<T*>(*it);
		}
		return nullptr;
	}

	std::vector<Component*> ComponentList;
private:
	static std::vector<GameObject*> GameObjectList;
};

void Shutdown();