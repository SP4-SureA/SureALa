#include "GraveStoneEntity.h"
#include "RenderHelper.h"
#include "GraphicsManager.h"
#include "MeshBuilder.h"
#include "../../Projectile/PlayerMeleeHitboxEntity.h"
#include "../../Projectile/PlayerProjectileEntity.h"
#include "EntityManager.h"
#include "AnimationManager.h"
#include "../../Physics/Physics.h"
#include "../../PlayerInfo/PlayerEntityBase.h"
#include "..\..\Particle\ParticleScale.h"


GraveStoneEntity::GraveStoneEntity(Mesh* _modelMesh) :
EnemyBase(_modelMesh),
b_isHitByRange(false),
b_isHitByMelee(false),
hitRefreshTime(1.0f),
currentHitRefreshTime(0.0f)
{
}

GraveStoneEntity::~GraveStoneEntity()
{
}

void GraveStoneEntity::Update(double dt)
{
	EnemyBase::Update(dt);
	animationPlayer.Update(dt);

	switch (grave_type)
	{
	case GraveStoneEntity::GRAVE_RANGE:
	{
		if (b_isHitByRange)
			SetIsDead(true);
	}
		break;
	case GraveStoneEntity::GRAVE_MELEE:
	{
		if (b_isHitByMelee)
			SetIsDead(true);
	}
		break;
	case GraveStoneEntity::GRAVE_MIX:
	{
		if (b_isHitByMelee && b_isHitByRange)
		{
			SetIsDead(true);
			break;
		}

		if (currentHitRefreshTime > hitRefreshTime)
		{
			b_isHitByMelee = false;
			b_isHitByRange = false;
		}
		else
		{
			currentHitRefreshTime += dt;
			if (currentHitRefreshTime > hitRefreshTime)
			{
				animationPlayer.m_anim = AnimationManager::GetInstance("bossroomobjects")->GetAnimation("grave_mix");
				if (b_isHitByMelee)
					Create::particleScale(EntityManager::GetInstance(), "grave_melee_refresh_effect", this->position, this->scale, 0.2f, 0.05f, 0.2f);
				else
					Create::particleScale(EntityManager::GetInstance(), "grave_range_refresh_effect", this->position, this->scale, 0.2f, 0.05f, 0.2f);
			}
		}
	}
	break;

	default:
		break;
	}
}

void GraveStoneEntity::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderSprite(modelMesh, animationPlayer);
	modelStack.PopMatrix();
}

bool GraveStoneEntity::TakeDamage(float amount, EntityBase* other)
{
	PlayerProjectileEntity* rangeProjectile = dynamic_cast<PlayerProjectileEntity*>(other);
	PlayerMeleeHitboxEntity* meleeProjectile = dynamic_cast<PlayerMeleeHitboxEntity*>(other);

	if (rangeProjectile)
	{
		b_isHitByRange = true;
	}

	if (meleeProjectile)
	{
		b_isHitByMelee = true;
	}
	if (this->grave_type == GRAVE_MIX)
	{
		if (meleeProjectile)
			animationPlayer.m_anim = AnimationManager::GetInstance("bossroomobjects")->GetAnimation("grave_range");
		else if (rangeProjectile)
				animationPlayer.m_anim = AnimationManager::GetInstance("bossroomobjects")->GetAnimation("grave_melee");
	}

	currentHitRefreshTime = 0;

	return false;
}

void GraveStoneEntity::CollisionResponse(EntityBase* other, double dt)
{
	PlayerEntityBase* player = dynamic_cast<PlayerEntityBase*>(other);

	if (player)
	{
		Vector3 dir = other->GetPosition() - this->position;

		if (dir.LengthSquared() > Math::EPSILON)
		{
			dir.Normalize();
			player->AddVelocity((dir * player->GetMoveSpeed() * dt * 2));
		}

		return;
	}
}

GraveStoneEntity* Create::gravestoneEntity(
	EntityManager* em,
	const std::string& _meshName,
	const std::string& _animationName,
	GraveStoneEntity::GRAVE_TYPE _gravetype,
	const Vector3& _position,
	const Vector3& _scale,
	const float& _hitRefreshTime
	)
{
	if (em == NULL)
		return NULL;

	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

	if (modelMesh == nullptr)
		return nullptr;

	GraveStoneEntity* newGrave = new GraveStoneEntity(modelMesh);

	newGrave->SetPosition(_position);
	newGrave->SetScale(_scale);

	newGrave->SetColliderSize(_scale * 0.7);
	newGrave->SetColliderOffset(Vector3(0, -_scale.y * 0.25, 0));
	newGrave->SetColliderType(Collider::COLLIDER_SPHERE);
	
	newGrave->SetGraveType(_gravetype);
	newGrave->SetHitRefreshTime(_hitRefreshTime);
	newGrave->SetHitCurrentRefreshTime(_hitRefreshTime + 1);
	newGrave->SetAnimation(_meshName, _animationName);

	em->AddEntity(newGrave, true);

	return newGrave;
}

GraveStoneEntity* Create::gravestoneAsset(
	const std::string& _meshName,
	const std::string& _animationName,
	GraveStoneEntity::GRAVE_TYPE _gravetype,
	const Vector3& _position,
	const Vector3& _scale,
	const float& _hitRefreshTime
	)
{
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

	if (modelMesh == nullptr)
		return nullptr;

	GraveStoneEntity* newGrave = new GraveStoneEntity(modelMesh);

	newGrave->SetPosition(_position);
	newGrave->SetScale(_scale);

	newGrave->SetColliderSize(_scale);
	newGrave->SetColliderType(Collider::COLLIDER_SPHERE);

	newGrave->SetGraveType(_gravetype);
	newGrave->SetHitRefreshTime(_hitRefreshTime);
	newGrave->SetHitCurrentRefreshTime(_hitRefreshTime + 1);
	newGrave->SetAnimation(_meshName, _animationName);

	return newGrave;
}