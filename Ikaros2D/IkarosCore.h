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
public:
	std::string name;
	bool isHidden;

	Object(std::string Name) {
		name = Name;
		ObjectList.push_back(this);
	}

	virtual ~Object() {
		for (std::vector<Object*>::iterator it = ObjectList.begin(); it != ObjectList.end(); it++)
		{
			if (this == (*it)) {
				ObjectList.erase(it);
			}
		}
	}

private:
	static std::vector<Object*> ObjectList;

};

class Component : public Object {
public:
	Component(std::string Name = "EmptyComponent") : Object(Name) {
		parent = nullptr;
	}
	Object* parent;
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
	virtual void Awake() {
		//Only once for this script instance
	}
	virtual void Start() {
		//Run only once for each object instance
	}
	virtual void Update() {
		//Run every frame
	}

	MonoBehavior() : Behavior("MonoBehavior") {
		//NEED UPDATE : what should be inside MB constructor ???
	}

	//NEED UPDATE: MonoBehavior List
};

struct TextureIndexData {
	LPDIRECT3DTEXTURE9* texturedata;
	std::string name;
};

class Sprite : public Object {
public:
	//File formats : .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga
	static LPDIRECT3DTEXTURE9* LoadTexture(std::string TextureName ,LPCTSTR FilePath) {
		LPDIRECT3DTEXTURE9* tex = new LPDIRECT3DTEXTURE9;
		if (CreateTexture(FilePath, tex)) {
			TextureIndexData NewEntry;
			NewEntry.texturedata = tex;
			NewEntry.name = TextureName;
			TextureList.push_back(NewEntry);

			return tex;
		}
		else delete tex;
	}

	//NEED UPDATE: static find by name

	static void ReleaseTextures() {
		for (auto p : TextureList) {
			delete p.texturedata;
		}
		TextureList.clear();
	}

private:
	static std::vector<TextureIndexData> TextureList;

	static bool CreateTexture(LPCTSTR FilePath, LPDIRECT3DTEXTURE9* texturedata);
};

class Renderer : public Component {
public:
	Renderer() : Component("Renderer") {

	}
	//NEED UPDATE: assign sprite

	//NEED UPDATE: render stuff
};

class RigidBody : public Component {
public:
	RigidBody() : Component("RigidBody") {
		//NEED UPDATE: actually add physics instad of husk
	}

};

class Transform : public Component {
public:
	Vector3 position;
	Vector3 rotation;
	Vector2 scale;

	Transform(Vector3 Position = Vector3(0, 0, 0), Vector3 Rotation = Vector3(0, 0, 0), Vector2 Scale = Vector2(0, 0)) : Component("Transform") {
		position = Position;
		rotation = Rotation;
		scale = Scale;
	}

	//NEED UPDATE: Translate	
	//NEED UPDATE: Rotate
};

class Camera : public Behavior {
	/*
	typedef struct
	{
		D3DXVECTOR3 posV;			// 視点
		D3DXVECTOR3 posR;			// 注視点
		D3DXVECTOR3 vecU;			// 上方向ベクトル
		D3DXMATRIX mtxProjection;	// プロジェクションマトリックス
		D3DXMATRIX mtxView;			// ビューマトリックス
	} CAMERA;

	CAMERA g_camera;

	void Camera_Initialize(void)
	{
		g_camera.posV = D3DXVECTOR3(CAM_POS_V_X, CAM_POS_V_Y, CAM_POS_V_Z);
		g_camera.posR = D3DXVECTOR3(CAM_POS_R_X, CAM_POS_R_X, CAM_POS_R_X);
		g_camera.vecU = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	}

	void Camera_Finalize(void)
	{
	}

	void Camera_Update(void)
	{
	}

	void Camera_SetCamera(void)
	{
		
		LPDIRECT3DDEVICE9 pDevice = GetD3DDevice();

		// プロジェクションマトリックスの初期化
		D3DXMatrixIdentity(&g_camera.mtxProjection);

		// プロジェクションマトリックスの作成
		D3DXMatrixPerspectiveFovLH(&g_camera.mtxProjection,
			VIEW_ANGLE,				// ビュー平面の視野角
			VIEW_ASPECT,			// ビュー平面のアスペクト比
			VIEW_NEAR_Z,			// ビュー平面のNearZ値
			VIEW_FAR_Z);			// ビュー平面のFarZ値

// プロジェクションマトリックスの設定
		pDevice->SetTransform(D3DTS_PROJECTION, &g_camera.mtxProjection);

		// ビューマトリックスの初期化
		D3DXMatrixIdentity(&g_camera.mtxView);

		// ビューマトリックスの作成
		D3DXMatrixLookAtLH(&g_camera.mtxView,
			&g_camera.posV,		// カメラの視点
			&g_camera.posR,		// カメラの注視点
			&g_camera.vecU);	// カメラの上方向ベクトル

// ビューマトリックスの設定
		pDevice->SetTransform(D3DTS_VIEW, &g_camera.mtxView);
	}
	*/
	//NEED UPDATE:Camera calculations
	//NEED UPDATE:maybe move all objects relative to camera here
};

class Time {
public:
	float DeltaTime;

	void Start() {
		t1 = std::chrono::high_resolution_clock::now();
	}
	void End() {
		t2 = std::chrono::high_resolution_clock::now();
		DeltaTime = std::chrono::duration_cast<std::chrono::seconds>(t2 - t1).count();
	}
private:
	std::chrono::time_point<std::chrono::high_resolution_clock> t1, t2;

};


class GameObject : public Object {
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

	GameObject(std::string Name) : Object(Name){
		transform = Transform();
	}

	~GameObject() {
		for (std::vector<GameObject*>::iterator it = GameObjectList.begin(); it != GameObjectList.end(); it++)
		{
			if (this == (*it)) {
				GameObjectList.erase(it);
			}
		}

		for (auto p : ComponentList) {
			delete p;
		}
		ComponentList.clear();
	}
private:
	static std::vector<GameObject*> GameObjectList;
	std::vector<Object*> ComponentList;
};

//NEED UPDATE:camera and input class required next
//NEED UPDATE:colliderclasses aka fixtures
//NEED UPDATE:scene loader

double frand() {
	return (double)rand() / RAND_MAX;
}