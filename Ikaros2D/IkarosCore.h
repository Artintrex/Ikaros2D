#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <chrono>
#include <windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Box2D\Box2D.h"

//NEED UPDATE: Change Resolution Settings needs to read from file!! Also set d3d parameters like fullscreen etc
#define SCREEN_WIDTH  (1024)
#define SCREEN_HEIGHT  (768)

typedef D3DXVECTOR2 Vector2;
typedef D3DXVECTOR3 Vector3;

struct VertexBufferData {
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DCOLOR color;
	D3DXVECTOR2 uv;
};

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
	virtual void Awake() {
		//Only once for this script instance
	}
	virtual void Start() {
		//Run only once for each object instance
	}
	virtual void Update() {
		//Run every frame
	}
	Behavior(std::string Name) : Component(Name) {
		//NEED UPDATE : what should be inside B constructor ???
	}

	//NEED UPDATE: MonoBehavior List
};

class MonoBehavior : public Behavior {
public:
	MonoBehavior() : Behavior("MonoBehavior") {
		//NEED UPDATE : what should be inside MB constructor ???
	}
};

struct TextureIndexData {
	LPDIRECT3DTEXTURE9* texturedata;
	std::string name;
	UINT Width;
	UINT Height;
};

class Sprite : public Object {
public:
	TextureIndexData texture;
	Object* parent;
	VertexBufferData* vertices;

	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;

	Sprite() : Object("Sprite"){
		texture = {nullptr, "NoTextureSet", 0, 0};
		VertexBuffer = nullptr;
		parent = nullptr;
	}

	//File formats : .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga
	static TextureIndexData LoadTexture(std::string TextureName ,LPCTSTR FilePath) {
		LPDIRECT3DTEXTURE9* tex = new LPDIRECT3DTEXTURE9;
		if (CreateTexture(FilePath, tex)) {
			TextureIndexData NewEntry;
			NewEntry.texturedata = tex;
			NewEntry.name = TextureName;
			
			D3DSURFACE_DESC format;

			(*tex)->GetLevelDesc(0, &format);
			NewEntry.Width = format.Width;
			NewEntry.Height = format.Height;

			TextureList.push_back(NewEntry);

			return NewEntry;
		}
		else delete tex;
	}
	//NEED UPDATE: Call Release() method for Buffers in destructor and set to NULL
	//NEED UPDATE: Move texture serialization and format to a new texture class

	void SetTexture(std::string Name);

	static void ReleaseTextures() {
		for (auto p : TextureList) {
			delete p.texturedata;
		}
		TextureList.clear();
	}

private:
	static std::vector<TextureIndexData> TextureList;

	static bool CreateTexture(LPCTSTR FilePath, LPDIRECT3DTEXTURE9* texturedata);

	static TextureIndexData FindTexturebyName(std::string Name) {
		for (std::vector<TextureIndexData>::iterator it = TextureList.begin(); it != TextureList.end(); it++)
		{
			if (Name == (*it).name)return *it;
			else return { nullptr, "NoTextureSet", 0, 0 };
		}
	}
};

class Renderer : public Component {
public:
	Vector3 color;
	Sprite* sprite;
	Vector2 size;
	int sortingOrder;

	Renderer();

	//NEED UPDATE::delete renderer clear list
	//Set Sprite
	static std::vector<Renderer*> RendererList;
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
	Vector3 scale;
	D3DXMATRIX localToWorldMatrix;

	Transform(Vector3 Position = Vector3(0, 0, 0), Vector3 Rotation = Vector3(0, 0, 0), Vector3 Scale = Vector3(1, 1, 1)) : Component("Transform") {
		position = Position;
		rotation = Rotation;
		scale = Scale;

		D3DXMatrixIdentity(&localToWorldMatrix);
		SetMatrix();
	}

	void Translate(Vector3 translation) {
		position = translation;
		SetMatrix();
	}

	void Rotate(Vector3 eulers) {
		rotation = eulers;
		SetMatrix();
	}

	void Scale(Vector3 scales) {
		scale = scales;
		SetMatrix();
	}
	//NEED UPDATE: forward
private:
	void SetMatrix();
};

struct Rect {
	float X;
	float Y;
	float W;
	float H;
};

class Camera : public Behavior {
public:
	static LPDIRECT3DDEVICE9 pD3DDevice;
	Transform transform;
	float fieldOfView;
	float aspect;
	float farClipPlane;
	float nearClipPlane;
	bool orthographic;
	float orthographicSize;
	Rect rect;


	void Draw() {
		for (auto p : CameraList) {
			p->Draw();
		}
	}

	//NEED UPDATE:maybe move all objects relative to camera here
	//Add Layers to game object

	Camera(std::string Name) : Behavior(Name) {
		fieldOfView = 60.0f;
		nearClipPlane = 0.3f;
		farClipPlane = 1000.0f;
		rect.X = 0;
		rect.Y = 0;
		rect.W = 1;
		rect.H = 1;
		aspect = SCREEN_WIDTH / SCREEN_HEIGHT;

		orthographic = false;
		orthographicSize = 10.0f;

		SetD3DDevice();

		D3DXMatrixIdentity(&mCameraWorld);
		D3DXMatrixIdentity(&Projection);
		D3DXMatrixIdentity(&View);
	}

private:
	static std::vector<Camera*> CameraList;

	D3DXMATRIX mCameraRot;
	D3DXMATRIX mCameraWorld;
	D3DXMATRIX View;
	D3DXMATRIX Projection;

	void draw();
	
	//Sets D3DDevice settings and maybe shaders in the future
	void SetD3DDevice();

	void SetProjection(){
		D3DXMatrixPerspectiveFovLH(&Projection,
				fieldOfView,
				aspect,
				nearClipPlane,
				farClipPlane);
		if (orthographic) {
			D3DXMatrixIdentity(&Projection);

			D3DXMatrixOrthoLH(&Projection,
				SCREEN_WIDTH * rect.W * orthographicSize,
				SCREEN_HEIGHT * rect.H * orthographicSize,
				nearClipPlane,
				farClipPlane);
		}
	}

	void SetCamera() {

		D3DXMatrixRotationYawPitchRoll(&mCameraRot, transform.rotation.y, transform.rotation.x, transform.rotation.z);

		Vector3 vWorldUp, vWorldAhead;
		Vector3 vLocalUp(0, 1, 0);
		Vector3 vLocalAhead(0, 0, 1);

		D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &mCameraRot);
		D3DXVec3TransformCoord(&vWorldAhead, &vLocalAhead, &mCameraRot);

		//Local to World Coord for relative camera translation
		//D3DXVec3TransformCoord(&vPosWorld, &vDelta, &mCameraRot);
		//add postworld to transform for movement

		//NEED UPDATE: Move transformation matrices to transform

		Vector3 LookAt = transform.position + vWorldAhead;

		D3DXMatrixLookAtLH(&View, &transform.position, &LookAt, &vWorldUp);
		D3DXMatrixInverse(&mCameraWorld, NULL, &View);
	}
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