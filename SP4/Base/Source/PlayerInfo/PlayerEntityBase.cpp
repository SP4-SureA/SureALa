#include "PlayerEntityBase.h"
#include "Bitstream.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "AnimationManager.h"
#include "RenderHelper.h"

#include "BitStream.h"

//testy
#include "PlayerInfo.h"

PlayerEntityBase::PlayerEntityBase(Mesh* _modelMesh) :
NetworkEntity(_modelMesh),
moveSpeed(0),
moveDirection(Vector3(0, 0, 0))
{
}

PlayerEntityBase::~PlayerEntityBase()
{
}

void PlayerEntityBase::UpdateInputs(double dt)
{
	moveDirection.SetZero();

    if (KeyboardController::GetInstance()->IsKeyDown('W'))
    {
        AddMoveDir(Vector3(0, 1, 0));
    }
    if (KeyboardController::GetInstance()->IsKeyDown('S'))
    {
		AddMoveDir(Vector3(0, -1, 0));
    }
    if (KeyboardController::GetInstance()->IsKeyDown('A'))
    {
		AddMoveDir(Vector3(-1, 0, 0));
    }
    if (KeyboardController::GetInstance()->IsKeyDown('D'))
    {
        AddMoveDir(Vector3(1, 0, 0));
    }	
}

void PlayerEntityBase::Update(double dt)
{
    if (moveDirection.IsZero())
    {
        this->velocity.Lerp(0, dt * 10);
    }
    else
    {
        SetVelocity(moveSpeed * moveDirection * dt);
    }
    ClampSpeed();
    this->position += this->velocity;
}

void PlayerEntityBase::HandleOutOfBounds(Vector3 min, Vector3 max, double dt)
{
	if (position.x - scale.x < min.x)
	{
		float offset = (min.x - (position.x - scale.x));
		position.x += offset;
	}
	else if (position.x + scale.x > max.x)
	{
		float offset = ((position.x + scale.x) - max.x);
		position.x -= offset;
	}

	if (position.y - scale.y < min.y)
	{
		float offset = (min.y - (position.y - scale.y));
		position.y += offset;
	}
	else if (position.y + scale.y > max.y)
	{
		float offset = ((position.y + scale.y) - max.y);
		position.y -= offset;
	}
}

void PlayerEntityBase::UpdateAnimation(double dt)
{
	if (moveDirection.IsZero())//no moving
	{
		animationPlayer.m_pause = true;
	}
	else//yes moving
	{
		animationPlayer.m_pause = false;
		if (abs(moveDirection.x) > abs(moveDirection.y))//horizontal direction
		{
			if (moveDirection.x > 0)
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("right");
			else
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("left");
		}
		else if (abs(moveDirection.y) > abs(moveDirection.x)) //vertical direction
		{
			if (moveDirection.y > 0)
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("up");
			else
				animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("down");
		}
		else//diagonal direction
		{
			if (moveDirection.y > 0)//top-diagonal
			{
				if (moveDirection.x > 0)
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("up_right");
				else
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("up_left");
			}
			else
			{
				if (moveDirection.x > 0)
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("down_right");
				else
					animationPlayer.m_anim = AnimationManager::GetInstance("player_shield")->GetAnimation("down_left");
			}
		}
	}

	animationPlayer.Update(dt);
}

void PlayerEntityBase::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderSprite(modelMesh, animationPlayer);
	modelStack.PopMatrix();
}

void PlayerEntityBase::Read(RakNet::BitStream &bs)
{
	float posX, posY;
	float velX, velY;
	float moveDirX, moveDirY;

	bs.Read(posX);
	bs.Read(posY);
	bs.Read(velX);
	bs.Read(velY);
	bs.Read(moveDirX);
	bs.Read(moveDirY);

	SetServerPos(posX, posY);
	SetServerVel(velX, velY);
	this->moveDirection.Set(moveDirX, moveDirY, 0);
}

void PlayerEntityBase::Write(RakNet::BitStream &bs)
{
	bs.Write(GetPosition().x);
	bs.Write(GetPosition().y);
	bs.Write(GetVelocity().x);
	bs.Write(GetVelocity().y);
	bs.Write(moveDirection.x);
	bs.Write(moveDirection.y);
}

void PlayerEntityBase::ReadInit(RakNet::BitStream &bs)
{

}

void PlayerEntityBase::WriteInit(RakNet::BitStream &bs)
{
}

PlayerEntityBase* Create::PlayerEntity(EntityManager* em,
    const std::string& _meshName,
    float _moveSpeed,
    float _maxSpeed,
    const Vector3& _position,
    const Vector3& _scale
    )
{
    if (em == NULL)
        return NULL;
	
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);

    if (modelMesh == nullptr)
        return nullptr;

    PlayerEntityBase* result = new PlayerEntityBase(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
    result->SetHasCollider(false);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
    em->AddEntity(result, true);
    return result;
}

PlayerEntityBase* Create::PlayerAsset(const std::string& _meshName,
    float _moveSpeed,
    float _maxSpeed,
    const Vector3& _position,
    const Vector3& _scale
    )
{
    Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
    if (modelMesh == nullptr)
        return nullptr;

    PlayerEntityBase* result = new PlayerEntityBase(modelMesh);
    result->SetPosition(_position);
    result->SetScale(_scale);
    result->SetMoveSpeed(_moveSpeed);
    result->SetMaxSpeed(_maxSpeed);
    result->SetHasCollider(false);
    return result;
}
