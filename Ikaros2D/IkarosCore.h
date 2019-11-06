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

//NEED UPDATE:
//class SceneLoader
//class Input
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

extern iTime Time;


struct VertexBufferData {
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DCOLOR color;
	D3DXVECTOR2 uv;
};

struct Rect {
	float X;
	float Y;
	float W;
	float H;
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
		ObjectList.erase(std::remove(ObjectList.begin(), ObjectList.end(), this), ObjectList.end());
	}
	//NEED UPDATE: UNLOAD function clear list
private:
	static std::vector<Object*> ObjectList;
	

};

class Component : public Object {
public:
	GameObject* parent;
	Transform* transform;

	Component(std::string Name = "EmptyComponent") : Object(Name) {
		parent = nullptr;
		transform = nullptr;
	}

	~Component();
};

class Behavior : public Component {
public:
	Behavior(std::string Name) : Component(Name) {
		//NEED UPDATE : what should be inside B constructor ???
	}
};

class MonoBehavior : public Behavior {
public:
	MonoBehavior() : Behavior("MonoBehavior") {
		MonoBehaviorList.push_back(this);
	}

	~MonoBehavior();


	virtual void Awake() {
		//Only once for this script instance
	}
	virtual void Start() {
		//Run only once for each object instance
	}
	virtual void Update() {
		//Run every frame
	}

	static void AwakeMonoBehaviorArray() {
		for (auto p : MonoBehaviorList) {
			p->Awake();
		}
	}

	static void StartMonoBehaviorArray() {
		for (auto p : MonoBehaviorList) {
			p->Start();
		}
	}

	static void UpdateMonoBehaviorArray() {
		for (auto p : MonoBehaviorList) {
			p->Update();
		}
	}
	//NEED UPDATE: clear list
private:
	static std::vector<MonoBehavior*> MonoBehaviorList;
};

class Texture : public Object {
public:
	LPDIRECT3DTEXTURE9* texturedata;
	UINT Width;
	UINT Height;
	D3DSURFACE_DESC graphicsFormat;

	Texture(std::string Name) : Object(Name){
		texturedata = nullptr;
		graphicsFormat = D3DSURFACE_DESC();
		Width = 0;
		Height = 0;
	}

	//File formats : .bmp, .dds, .dib, .hdr, .jpg, .pfm, .png, .ppm, and .tga
	static Texture* LoadTexture(std::string TextureName, LPCTSTR FilePath) {
		LPDIRECT3DTEXTURE9* tex = new LPDIRECT3DTEXTURE9;
		if (CreateTexture(FilePath, tex)) {
			Texture* texture = new Texture("TextureName");
			texture->texturedata = tex;
			D3DSURFACE_DESC format;

			(*tex)->GetLevelDesc(0, &format);

			texture->graphicsFormat;
			texture->Width = format.Width;
			texture->Height = format.Height;

			TextureList.push_back(texture);

			return texture;
		}
		else delete tex;
	}

	static void ReleaseTextures() {
		for (auto p : TextureList) {
			(*(p->texturedata))->Release();
			delete p;
		}
		TextureList.clear();
	}

	static Texture* FindTexturebyName(std::string Name) {
		for (std::vector<Texture*>::iterator it = TextureList.begin(); it != TextureList.end(); ++it)
		{
			if (Name == (*it)->name)return *it;
			else return nullptr;
		}
	}

private:
	static std::vector<Texture*> TextureList;
	static bool CreateTexture(LPCTSTR FilePath, LPDIRECT3DTEXTURE9* texturedata);
};

class Sprite : public Object {
public:
	Object* renderer;
	Texture* texture;
	VertexBufferData* vertices;

	LPDIRECT3DVERTEXBUFFER9 VertexBuffer;

	Sprite() : Object("Sprite"){
		VertexBuffer = nullptr;
		renderer = nullptr;
		texture = nullptr;
		vertices = nullptr;
	}

	virtual ~Sprite() {
		VertexBuffer->Release();
	}

	void SetTexture(std::string Name);
};

class Renderer : public Component {
public:
	Vector3 color;
	Sprite* sprite;
	Vector2 size;
	int sortingOrder;

	Renderer();
	~Renderer();

	//NEED UPDATE::Set Sprite
	//clear list
	static std::vector<Renderer*> RendererList;
};

class RigidBody : public Component {
public:
	RigidBody() : Component("RigidBody") {
		//NEED UPDATE: actually add physics instad of husk
	}

	~RigidBody();
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

	~Transform();

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

	void Draw() {
		for (auto p : CameraList) {
			p->Draw();
		}
	}

	//NEED UPDATE: Add Layer systems to filter objects for cameras and clear camera list

	Camera(std::string Name = "Camera") : Behavior(Name) {
		transform = nullptr;
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

		CameraList.push_back(this);
	}

	~Camera();

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

		D3DXMatrixRotationYawPitchRoll(&mCameraRot, transform->rotation.y, transform->rotation.x, transform->rotation.z);

		Vector3 vWorldUp, vWorldAhead;
		Vector3 vLocalUp(0, 1, 0);
		Vector3 vLocalAhead(0, 0, 1);

		D3DXVec3TransformCoord(&vWorldUp, &vLocalUp, &mCameraRot);
		D3DXVec3TransformCoord(&vWorldAhead, &vLocalAhead, &mCameraRot);

		//Local to World Coord for relative camera translation
		//D3DXVec3TransformCoord(&vPosWorld, &vDelta, &mCameraRot);
		//add postworld to transform for movement

		Vector3 LookAt = transform->position + vWorldAhead;

		D3DXMatrixLookAtLH(&View, &(transform->position), &LookAt, &vWorldUp);
		D3DXMatrixInverse(&mCameraWorld, NULL, &View);
	}
};

class iTime {
public:
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


class GameObject : public Object {
public:
	Transform *transform;

	template <typename T> T* AddComponent();

	template <> Renderer* AddComponent<Renderer>() {
		Renderer* p = new Renderer();
		p->parent = this;
		ComponentList.push_back((Component*)p);

		return p;
	}

	template <> RigidBody* AddComponent<RigidBody>() {
		RigidBody* p = new RigidBody();
		p->parent = this;
		ComponentList.push_back((Component*)p);

		return p;
	}

	template <> MonoBehavior* AddComponent<MonoBehavior>() {
		MonoBehavior* p = new MonoBehavior();
		p->parent = this;
		ComponentList.push_back((Component*)p);

		return p;
	}

	template <typename T> T* GetComponent();

	template <> Renderer* GetComponent<Renderer>() {
		for (std::vector<Component*>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it)
		{
			if ("Renderer" == (*it)->name)return static_cast<Renderer*>(*it);
		}
		return nullptr;
	}

	template <> RigidBody* GetComponent<RigidBody>() {
		for (std::vector<Component*>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it)
		{
			if ("RigidBody" == (*it)->name)return static_cast<RigidBody*>(*it);
		}
			return nullptr;
	}

	template <> MonoBehavior* GetComponent<MonoBehavior>() {
		for (std::vector<Component*>::iterator it = ComponentList.begin(); it != ComponentList.end(); ++it)
		{
			if ("MonoBehavior" == (*it)->name)return static_cast<MonoBehavior*>(*it);
		}
		return nullptr;
	}

	static GameObject* FindbyName(std::string Name) {
		for (std::vector<GameObject*>::iterator it = GameObjectList.begin(); it != GameObjectList.end(); ++it)
		{
			if (Name == (*it)->name)return *it;
		}
		return nullptr;
	}

	GameObject(std::string Name = "GameObject") : Object(Name){
		transform = new Transform();
		transform->parent = this;
		GameObjectList.push_back(this);
	}

	~GameObject() {
		delete transform;

		for (int j = 0, i = ComponentList.size(); j < i; j++) {
			delete ComponentList.at(0);
		}

		ComponentList.clear();

		for (std::vector<GameObject*>::iterator it = GameObjectList.begin(); it != GameObjectList.end(); ++it)
		{
			if (this == (*it)) {
				GameObjectList.erase(it);
				break;
			}
		}
	}

	std::vector<Component*> ComponentList;
private:
	static std::vector<GameObject*> GameObjectList;
};

double frand() {
	return (double)rand() / RAND_MAX;
}