#ifndef ENVIRONMENTOBJECT_H
#define ENVIRONMENTOBJECT_H

#include "EntityBase.h"
#include "Animation.h"
#include "AnimationManager.h"

class EntityManager;

class EnvironmentObject : public EntityBase
{
public:
	EnvironmentObject(Mesh* _modelMesh);
	~EnvironmentObject();

	virtual void Update(double dt);
	virtual void Render();

	inline void SetAnimation(std::string _meshName, std::string _animationName){ this->animationPlayer.m_anim = AnimationManager::GetInstance(_meshName)->GetAnimation(_animationName); animationPlayer.m_pause = false; }
	inline AnimationPlayer* GetAnimationPlayer(){ return &this->animationPlayer; };

private:
	Mesh* modelMesh;
	AnimationPlayer animationPlayer;
};

namespace Create
{
	EnvironmentObject* environmentObject(
		EntityManager* em,
		const std::string& _meshName,
		const std::string& animation,
		const Vector3& _position = Vector3(0, 0, 0),
		const Vector3& _scale = Vector3(1.0f, 1.0f, 1.0f)
		);
}

#endif