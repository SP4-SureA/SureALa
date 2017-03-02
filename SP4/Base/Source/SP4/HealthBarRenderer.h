#ifndef _HEALTHBAR_RENDERER_H
#define _HEALTHBAR_RENDERER_H

#include "EntityBase.h"

class Mesh;
class EntityManager;

class HealthBarRenderer : public EntityBase
{
public:
    HealthBarRenderer(Mesh* _frontMesh, Mesh* _backMesh);
	virtual ~HealthBarRenderer();

	virtual void RenderUI();

    inline void SetBackMesh(Mesh* _mesh){ if (backMesh){ this->backMesh = _mesh; } }
    inline void SetFrontMesh(Mesh* _mesh){ if (frontMesh){ this->frontMesh = _mesh; } }

    inline void SetCurrentHealth(float _health){ this->currentHealth = _health; };
    inline float GetCurrentHealth(){ return this->currentHealth; };

    inline void SetMaxHealth(float _health){ this->maxHealth = _health; };
    inline float GetMaxHealth(){ return this->maxHealth; };

    inline void SetRotateAngle(float _angle){ this->rotateAngle = _angle; };
    inline float GetRotateAngle(){ return this->rotateAngle; };

protected:
    Mesh* backMesh;
	Mesh* frontMesh;

    float maxHealth;
    float currentHealth;
    float rotateAngle;
};

namespace Create
{
	HealthBarRenderer* HealthBarRendererEntity(EntityManager* em,
		const std::string& _backMesh,
        const std::string& _frontMesh,
        const Vector3& _position,
		const Vector3& _scale,
        const float& _maxHealth = 1
		);

	HealthBarRenderer* HealthBarRendererAsset(
        const std::string& _backMesh,
        const std::string& _frontMesh,
        const Vector3& _position,
        const Vector3& _scale,
        const float& _maxHealth = 1
		);
};

#endif _HEALTHBAR_RENDERER_H