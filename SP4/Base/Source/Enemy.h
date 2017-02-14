//#ifndef ENEMY_H
//#define ENEMY_H
//
//#include "GenericEntity.h"
//#include "GroundEntity.h"
//
//class Mesh;
//
//class Enemy : public GenericEntity
//{
//public:
//	Enemy();
//	virtual ~Enemy();
//
//	virtual void Init();
//	virtual void Update(double dt);
//	virtual void Render();
//	void Constrain();
//	void Reset();
//
//	void SetPosition(const Vector3& pos);
//	void SetTarget(const Vector3& target);
//	void SetUp(const Vector3& up);
//	void SetBoundary(Vector3 max, Vector3 min);
//	void SetTerrain(GroundEntity* terrain);
//
//	Vector3 GetPosition() const;
//	Vector3 GetTarget() const;
//	Vector3 GetUp() const;
//	
//	GroundEntity* GetTerrain();
//
//protected:
//	Vector3 defaultPosition, defaultTarget, defaultUp;
//	Vector3 target, up;
//	Vector3 maxBoundary, minBoundary;
//	GroundEntity* terrain;
//
//	float speed;
//	float acceleration;
//};
//
//#endif