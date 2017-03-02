#include "BossLazerBody.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

#include "..\PlayerInfo\PlayerEntityBase.h"
#include "../PlayerInfo/PlayerShieldEntity.h"

BossLazerBody::BossLazerBody(Mesh* _modelMesh) :
ProjectileBase(_modelMesh),
growTime(1),
currentGrowTime(0),
lazerLength(1),
b_isCollided(false),
collidedPlayer(nullptr)
{
}

BossLazerBody::~BossLazerBody()
{
}

void BossLazerBody::Update(double dt)
{
    //ProjectileBase::Update(dt);
	EntityBase::Update(dt);
	ProjectileBase::UpdateLifeSpan(dt);
    ProjectileBase::UpdateAnimation(dt);

    if (!b_isCollided)
    {
        LazerUpdate(dt);

        if (collidedPlayer)
        {
            collidedPlayer->TakeDamage(1);
            collidedPlayer = nullptr;
        }
    }
	else
		b_isCollided = false;
}

void BossLazerBody::LazerUpdate(double dt)
{
    float growRatio = Math::Clamp(currentGrowTime / growTime, 0.f, 1.f);

    if (growRatio > Math::EPSILON)
    {
        if (growRatio > 1)
            growRatio = 1;

        if (front.LengthSquared() < Math::EPSILON)
            front = Vector3(0, 1, 0);

        front.Normalize();

        this->lazerScale = scale;
        this->lazerScale.x = lazerLength * growRatio;
        this->position = startPosition + (front * growRatio * lazerLength * 0.5);
        this->colliderSize = lazerScale;
    }
    else
    {
        this->position = startPosition;
        this->lazerScale = Vector3(0.1f, 0.1f, 0.1f);
        this->colliderSize = lazerScale;
    }

    currentGrowTime += dt;
}

void BossLazerBody::Render()
{
	float angle = Math::RadianToDegree(atan2(front.y, front.x));
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, 0.8f);
    modelStack.Rotate(angle, 0, 0, 1);
	modelStack.Scale(lazerScale.x, lazerScale.y, lazerScale.z);
    RenderHelper::RenderSprite(modelMesh, animationPlayer);
	modelStack.PopMatrix();
}

void BossLazerBody::HandleOutOfBounds(Vector3 min, Vector3 max, double dt)
{
	//NOTHING
}

void BossLazerBody::CollisionResponse(EntityBase* other, double dt)
{
    PlayerShieldEntity* shield = dynamic_cast<PlayerShieldEntity*>(other);

	if (shield)
	{
		this->b_isCollided = true;

		float distance = (other->GetPosition() - this->startPosition).Length();
		distance -= other->GetColliderSize().x;
		if (front.LengthSquared() < Math::EPSILON)
			front = Vector3(0, 1, 0);

		front.Normalize();

		this->lazerScale = scale;
		this->lazerScale.x = distance;
		this->position = startPosition + (front * distance * 0.5);
		this->colliderSize = lazerScale;
		this->collidedPlayer = NULL;

        return;
	}

	PlayerEntityBase* player = dynamic_cast<PlayerEntityBase*>(other);

	if (!player)
		return;

	//sound effect
    this->collidedPlayer = player;
}

BossLazerBody* Create::bossLazerBodyEntity(
    EntityManager* em,
    const std::string& _meshName,
    const std::string& _animationName,
    const Vector3& _position,
    const Vector3& _scale,
    const Vector3& _startPosition,
    const Vector3& _direction,
    const float& _lifetime,
    const float& _lazerLength,
    const float& _growTime
    )
{
    if (em == NULL)
        return NULL;

    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

    if (modelMesh == nullptr)
        return nullptr;

    BossLazerBody* result = new BossLazerBody(modelMesh);

    result->SetPosition(_position);
    result->SetFront(_direction);
    result->SetScale(_scale);

	result->SetColliderType(Collider::COLLIDER_BOX);
	result->SetColliderSize(_scale);

    result->SetLifespan(_lifetime);
    result->SetGrowTime(_growTime);
    result->SetLazerLength(_lazerLength);

    result->SetAnimation(_meshName, _animationName);

    em->AddEntity(result, true);
    return result;
}

BossLazerBody* Create::bossLazerBodyAsset(
    const std::string& _meshName,
    const std::string& _animationName,
    const Vector3& _position,
    const Vector3& _scale,
    const Vector3& _startPosition,
    const Vector3& _direction,
    const float& _lifetime,
    const float& _lazerLength,
    const float& _growTime
    )
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	BossLazerBody* result = new BossLazerBody(modelMesh);

	result->SetPosition(_position);
	result->SetScale(_scale);
    result->SetFront(_direction);

    result->SetColliderType(Collider::COLLIDER_BOX);
	result->SetColliderSize(_scale);
    
    result->SetLifespan(_lifetime);
    result->SetGrowTime(_growTime);
    result->SetLazerLength(_lazerLength);

    result->SetAnimation(_meshName, _animationName);

	return result;
}
