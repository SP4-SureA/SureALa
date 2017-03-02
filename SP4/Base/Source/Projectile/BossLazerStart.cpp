#include "BossLazerStart.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Projectile/ProjectileBase.h"
#include "..\SoundManager.h"

#include "..\PlayerInfo\PlayerEntityBase.h"

BossLazerStart::BossLazerStart(Mesh* _modelMesh) :
ProjectileBase(_modelMesh),
chargeTime(0),
lazerBody(nullptr),
firedLazer(false),
lazerLength(1)
{
}

BossLazerStart::~BossLazerStart()
{
}

void BossLazerStart::Update(double dt)
{
	ProjectileBase::Update(dt);
    ProjectileBase::UpdateAnimation(dt);
    UpdateLazer(dt);
}

void BossLazerStart::UpdateLazer(double dt)
{
    if (chargeTime < 0 && !firedLazer)
    {
        firedLazer = true;
		SoundManager::GetInstance()->PlaySound2D("skywhale_fireLaser");
        lazerBody = Create::bossLazerBodyEntity(EntityManager::GetInstance(),
            "boss_lazer",
            "attack",
            this->position,
            Vector3(10, 10, 5),
            this->position,
            this->front,
            this->lifespan - this->currentLifetime,
            this->lazerLength,
            1.f);
    }

    if (lazerBody)
    {
        lazerBody->SetStartPosition(this->position);

        if (lazerBody->GetIsDead())
            lazerBody = nullptr;
     
    }

    chargeTime -= dt;
}

void BossLazerStart::Render()
{
	float angle = Math::RadianToDegree(atan2(front.y, front.x));
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, 0.9f);
	modelStack.Rotate(angle, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);
    RenderHelper::RenderSprite(modelMesh, animationPlayer);
	modelStack.PopMatrix();
}

BossLazerStart* Create::bossLazerStartEntity(
    EntityManager* em,
    const std::string& _meshName,
    const std::string& _animationName,
    const Vector3& _position,
    const Vector3& _scale,
    const Vector3& _direction,
    const float& _lifespan,
    const float& _ChargeTime,
    const float& _LazerLength
    )
{
    if (em == NULL)
        return NULL;

    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

    if (modelMesh == nullptr)
        return nullptr;

    BossLazerStart* result = new BossLazerStart(modelMesh);

    result->SetPosition(_position);
    result->SetScale(_scale);
    result->SetFront(_direction);
    result->SetAnimation(_meshName, _animationName);

    result->SetColliderType(Collider::COLLIDER_BOX);
    result->SetColliderSize(_scale);

    result->SetLifespan(_lifespan);
	result->SetChargeTime(_ChargeTime);
    result->SetLazerLength(_LazerLength);

    em->AddEntity(result, true);

    return result;
}

BossLazerStart* Create::bossLazerStartAsset(
    const std::string& _meshName,
    const std::string& _animationName,
    const Vector3& _position,
    const Vector3& _scale,
    const Vector3& _direction,
    const float& _lifespan,
    const float& _ChargeTime,
    const float& _LazerLength
    )
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	BossLazerStart* result = new BossLazerStart(modelMesh);

	result->SetPosition(_position);
	result->SetScale(_scale);
    result->SetFront(_direction);
    result->SetAnimation(_meshName, _animationName);

    result->SetColliderType(Collider::COLLIDER_BOX);
	result->SetColliderSize(_scale);

    result->SetLifespan(_lifespan);
	result->SetChargeTime(_ChargeTime);
    result->SetLazerLength(_LazerLength);
    
	return result;
}
