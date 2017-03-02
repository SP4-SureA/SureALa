#include "SkyWhale.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "../Physics/Physics.h"
#include "../Weapon/WeaponBase.h"
#include "../Weapon/BossLazerWeapon.h"
#include "../Weapon/BossRapidFireWeapon.h"
#include "../PlayerInfo/PlayerEntityBase.h"

SkyWhale::SkyWhale(Mesh* _modelMesh) :
BossEntity(_modelMesh), 
minionSpawnTime(0),
secondWeapon(nullptr)
{
	phase1StateMachine.AttachEntity(this);
	currentStateManager = &phase1StateMachine;
	SetWeight(200.f);
}

SkyWhale::~SkyWhale()
{
}

void SkyWhale::Init()
{
    phase1StateMachine.AttachEntity(this);
    phase2StateMachine.AttachEntity(this);
    currentStateManager = &phase1StateMachine;
    currentStateManager->Init();
}

void SkyWhale::Update(double dt)
{
    EntityBase::Update(dt);
    BossEntity::HandleControlledEntities();
    animationPlayer.Update(dt);

    {//Waypoints management
        if (waypoints.size() > 0 && waypoints.size() > waypointIndex)
        {
            if (ReachedWaypont())
            {
                waypointIndex = (waypointIndex + 1) % waypoints.size();
            }
            else
            {
                Vector3 dir = waypoints[waypointIndex] - this->position;
                if (!dir.IsZero())
                {
                    dir.Normalize();
                    velocity += dir * moveSpeed * dt;
                }
            }
        }
    }
    {//Max speed
        if (!velocity.IsZero())//if zero speed, skip checks
        {
            if (velocity.LengthSquared() > maxSpeed * maxSpeed)
            {
                velocity = velocity.Normalized() * maxSpeed;
            }
        }
    }

    position += velocity * dt;

    if (currentStateManager)
        currentStateManager->Update(dt);

    if (currentStateManager == &phase1StateMachine)
	{
        if (currentStateManager->GetShouldChange())
            SetCurrentStateManager(&phase2StateMachine);
	}
    else
    {
        isAngry = true;
    }

    UpdateWeapons(dt);
}

void SkyWhale::CollisionResponse(EntityBase* other, double dt)
{
	PlayerEntityBase* player = dynamic_cast<PlayerEntityBase*>(other);

	if (player)
	{
		Vector3 dir = other->GetPosition() - this->position;

		if (dir.LengthSquared() > Math::EPSILON)
		{
			dir.Normalize();
			player->AddVelocity(dir * player->GetMoveSpeed() * 0.5f);
			player->TakeDamage(1);
		}

		return;
	}
}

void SkyWhale::UpdateWeapons(double dt)
{
	if (currentStateManager == &phase2StateMachine)
    {
        if (weapon)
        {
            Vector3 weaponPos = this->position + Vector3(0, -this->scale.y * 1.f, 0);
            weapon->SetPosition(weaponPos);
            weapon->Update(dt);

			BossLazerWeapon* temp = dynamic_cast<BossLazerWeapon*>(weapon);
			
			if (temp)
			{
				if (!temp->GetNumLazers())
				{
					SetAnimation("blubby", "idle");
				}
			}
        }
        if (secondWeapon)
        {
            secondWeapon->SetPosition(this->position);
            secondWeapon->Update(dt);
        } 
    }
}

void SkyWhale::DischargeLazer()
{
	SetAnimation("blubby", "charging");
    weapon->Discharge(Vector3(0, -1, 0));
}

void SkyWhale::DischargeRapidWeapon()
{
    secondWeapon->Discharge(Vector3(0, -1, 0));
}

void SkyWhale::Render()
{
    MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y, position.z);
    modelStack.Scale(scale.x, scale.y, scale.z);
    RenderHelper::RenderSprite(modelMesh, animationPlayer);
    modelStack.PopMatrix();
}

void SkyWhale::SetSwitchPhase()
{
    if (currentStateManager == &phase1StateMachine)
    {
        SetCurrentStateManager(&phase2StateMachine);
    }
    else if (currentStateManager == &phase2StateMachine)
    {
        SetCurrentStateManager(&phase1StateMachine);
    }
}

void SkyWhale::HandleOutOfBounds(Vector3 min, Vector3 max, double dt)
{   
    Vector3 colliderPosiition = position + colliderOffset;
    Vector3 collidedPosition;

    static float bufferTime = 0;

    bool outofbounds = false;

    if (colliderPosiition.x - GetColliderSize().x < min.x )
    {
        outofbounds = true;
        collidedPosition = colliderPosiition - Vector3(GetColliderSize().x);
    }
    else if (colliderPosiition.x + GetColliderSize().x > max.x)
    {
        outofbounds = true;
        collidedPosition = colliderPosiition + Vector3(GetColliderSize().x);
    }
    else if (colliderPosiition.y - GetColliderSize().y < min.y)
    {
        outofbounds = true;
        collidedPosition = colliderPosiition - Vector3(0, GetColliderSize().y);
    }
    else if (colliderPosiition.y + GetColliderSize().y > max.y)
    {
        outofbounds = true;
        collidedPosition = colliderPosiition + Vector3(0, GetColliderSize().y);
    }

    if (outofbounds)
    {
        if (bufferTime > 0.5)
        {
            EntityBase* temp = Create::Asset(NULL, "circle", collidedPosition, Vector3(1, 1, 1));
            Physics::CollisionResponse(temp, this);

            ++waypointIndex;
            waypointIndex = (waypointIndex + 1) % waypoints.size();

            bufferTime = 0;
        }
    }

    bufferTime += dt;
}

bool SkyWhale::TakeDamage(float amount, EntityBase* other)
{
    if (currentStateManager == &phase1StateMachine)
        return false;

    else
    {
        EnemyBase::TakeDamage(amount, other);
    }

    return true;
}

SkyWhale* Create::skywhaleEntity(
	EntityManager* em,
	const std::string& _meshName,
	const std::string& _animationName,
	const Vector3& _position,
	const Vector3& _scale,
	const Vector3& _worldMin,
	const Vector3& _worldMax,
    const float& _speed,
    const float& _maxSpeed,
    const float& _health,
    const float& _minionSpawnRate
    )
{
	if (em == NULL)
		return NULL;

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

	if (modelMesh == nullptr)
		return nullptr;

	SkyWhale* newSkyWhale = new SkyWhale(modelMesh);

	newSkyWhale->SetPosition(_position);
	newSkyWhale->SetScale(_scale);
	newSkyWhale->SetColliderSize(_scale * 0.7);
    newSkyWhale->SetColliderOffset(Vector3(0.f, _scale.y * 0.1f, 0.f));
	newSkyWhale->SetColliderType(Collider::COLLIDER_SPHERE);

	newSkyWhale->SetHealth(_health);
	newSkyWhale->SetMaxHealth(_health);
    newSkyWhale->SetMaxSpeed(_maxSpeed);
	newSkyWhale->SetMoveSpeed(_speed);
    newSkyWhale->SetMinionSpawnTime(_minionSpawnRate);
	
	newSkyWhale->SetWorldMin(_worldMin);
	newSkyWhale->SetWorldMax(_worldMax);

    newSkyWhale->SetAnimation(_meshName, _animationName);

	em->AddEntity(newSkyWhale, true);
    newSkyWhale->Init();

    BossLazerWeapon* weapon = Create::bossLazerWeapon(12.f, 2.1f, 150.f, 5.f);
    newSkyWhale->SetWeapon(weapon);

    BossRapidFireWeapon* secondWeapon = Create::bossRapidFireWeapon(5, 14, 0.3f);
    newSkyWhale->SetSecondaryWeapon(secondWeapon);

	return newSkyWhale;
}

SkyWhale* Create::skywhaleAsset(
	const std::string& _meshName,
	const std::string& _animationName,
	const Vector3& _position,
	const Vector3& _scale,
	const Vector3& _worldMin,
	const Vector3& _worldMax,
    const float& _speed,
    const float& _maxSpeed,
    const float& _health,
    const float& _minionSpawnRate
	)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

	if (modelMesh == nullptr)
		return nullptr;

	SkyWhale* newSkyWhale = new SkyWhale(modelMesh);

	newSkyWhale->SetPosition(_position);
	newSkyWhale->SetScale(_scale);
	newSkyWhale->SetColliderSize(_scale * 0.8);
	newSkyWhale->SetColliderOffset(Vector3(0, -_scale.y * 0.1, 0));
	newSkyWhale->SetColliderType(Collider::COLLIDER_SPHERE);

	newSkyWhale->SetHealth(_health);
	newSkyWhale->SetMaxHealth(_health);
	newSkyWhale->SetMaxSpeed(_maxSpeed);
	newSkyWhale->SetMoveSpeed(_speed);
    newSkyWhale->SetMinionSpawnTime(_minionSpawnRate);

	newSkyWhale->SetWorldMin(_worldMin);
	newSkyWhale->SetWorldMax(_worldMax);

    newSkyWhale->SetAnimation(_meshName, _animationName);

    newSkyWhale->Init();

    BossLazerWeapon* weapon = Create::bossLazerWeapon(10.f, 1.f, 150.f, 5.f);
    newSkyWhale->SetWeapon(weapon);

    BossRapidFireWeapon* secondWeapon = Create::bossRapidFireWeapon(3, 12, 0.5f);
    newSkyWhale->SetSecondaryWeapon(secondWeapon);

	return newSkyWhale;
}