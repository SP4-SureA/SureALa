#ifndef _GRAVE_STONE_ENTITY_H
#define _GRAVE_STONE_ENTITY_H

#include "../../Enemy/EnemyBase.h"
#include "Animation.h"

class GraveStoneEntity : public EnemyBase
{
public:
	enum GRAVE_TYPE
	{
		GRAVE_RANGE = 0,
		GRAVE_MELEE,
		GRAVE_MIX,
		NUM_GRAVE_TYPE,
	};

	GraveStoneEntity(Mesh* _modelMesh);
	virtual ~GraveStoneEntity();

	virtual void Update(double dt);
	virtual void Render();

	inline void SetGraveType(GRAVE_TYPE _gravetype){ this->grave_type = _gravetype; };
	inline GRAVE_TYPE GetGraveType(){ return this->grave_type; };

	inline void SetIsHitByRange(bool _hit){ this->b_isHitByRange = _hit; };
	inline bool GetIsHitByRange(){ return this->b_isHitByRange; };

	inline void SetIsHitByMelee(bool _hit){ this->b_isHitByMelee = _hit; };
	inline bool GetIsHitByMelee(){ return this->b_isHitByMelee; };

	inline void SetHitRefreshTime(float _time){ this->hitRefreshTime = _time; };
	inline float GetHitRefreshTime(){ return this->hitRefreshTime; };

	inline void SetHitCurrentRefreshTime(float _time){ this->currentHitRefreshTime = _time; };
	inline float GetHitCurrentRefreshTime(){ return this->currentHitRefreshTime; };

    virtual bool TakeDamage(float amount, EntityBase* other = NULL);

	virtual void CollisionResponse(EntityBase* other, double dt);

protected :
	GRAVE_TYPE grave_type;

	bool b_isHitByRange;
	bool b_isHitByMelee;

	float hitRefreshTime;
	float currentHitRefreshTime;
};

namespace Create
{
	GraveStoneEntity* gravestoneEntity(
		EntityManager* em, 
		const std::string& _meshName,
		const std::string& _animationName,
		GraveStoneEntity::GRAVE_TYPE _gravetype = GraveStoneEntity::NUM_GRAVE_TYPE,
		const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		const float& _hitRefreshTime = 0.0f
		);

	GraveStoneEntity* gravestoneAsset(
		const std::string& _meshName,
		const std::string& _animationName,
		GraveStoneEntity::GRAVE_TYPE _gravetype = GraveStoneEntity::NUM_GRAVE_TYPE,
		const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f),
		const float& _hitRefreshTime = 0.0f
		);
};


#endif // _GRAVE_STONE_ENTITY_H