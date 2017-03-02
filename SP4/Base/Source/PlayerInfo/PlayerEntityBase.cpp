#include "PlayerEntityBase.h"
#include "KeyboardController.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "AnimationManager.h"
#include "RenderHelper.h"

#include "..\FloorTiles\FloorTile.h"
#include "..\FloorTiles\FloorTilesManager.h"

#include "..\SP4\GameInfo.h"

#include <vector>
using std::vector;

PlayerEntityBase::PlayerEntityBase(Mesh* _modelMesh) :
GenericEntity(_modelMesh),
moveSpeed(0),
moveDirection(Vector3(0, 0, 0)),
bufferTime(0.0f),
bufferBlinkShowTimer(0.0f),
bufferBlinkHideTimer(0.0f),
bufferBlinkTimer(0.0f),
blinking(false),
hasDropped(false)
{
	bufferBlinkShowTimer = 0.15f;
	bufferBlinkHideTimer = 0.1f;
}

PlayerEntityBase::~PlayerEntityBase()
{
}

void PlayerEntityBase::UpdateInputs(double dt)
{
	//moveDirection.SetZero();

 //   if (KeyboardController::GetInstance()->IsKeyDown('W'))
 //   {
 //       AddMoveDir(Vector3(0, 1, 0));
 //   }
 //   if (KeyboardController::GetInstance()->IsKeyDown('S'))
 //   {
	//	AddMoveDir(Vector3(0, -1, 0));
 //   }
 //   if (KeyboardController::GetInstance()->IsKeyDown('A'))
 //   {
	//	AddMoveDir(Vector3(-1, 0, 0));
 //   }
 //   if (KeyboardController::GetInstance()->IsKeyDown('D'))
 //   {
 //       AddMoveDir(Vector3(1, 0, 0));
 //   }	
}

void PlayerEntityBase::Update(double dt)
{
	this->position += this->velocity * dt;
	if (hasDropped)
	{
		//scale down
		scale.x -= 2.0f * dt;
		scale.y -= 2.0f * dt;

		if (scale.x <= 0.1f)
			Respawn();

		return;
	}
	HandleFloorCollision(dt);

	if (bufferTime > 0.0f)
	{
		{//Do blink effect when in buffer
			if (bufferBlinkTimer <= 0.0f)
			{
				if (blinking)
				{
					bufferBlinkTimer = bufferBlinkShowTimer;
					blinking = false;
				}
				else
				{
					bufferBlinkTimer = bufferBlinkHideTimer;
					blinking = true;
				}
			}
			else
				bufferBlinkTimer -= dt;
		}
		
		bufferTime -= dt;
	}
	else
	{
		bufferTime = 0.0f;
		blinking = false;
		this->SetColliderIsActive(true);
	}
}

void PlayerEntityBase::HandleFloorCollision(double dt)
{
	Vector3 colliderPos = position + colliderOffset;
	bool isOnFloor = false;

	for (auto q : FloorTilesManager::GetInstance()->GetList())
	{
		if (colliderPos.x < q->GetPosition().x + (q->GetScale().x * 0.5f) &&
			colliderPos.x > q->GetPosition().x - (q->GetScale().x * 0.5f) &&
			colliderPos.y < q->GetPosition().y + (q->GetScale().y * 0.5f) &&
			colliderPos.y > q->GetPosition().y - (q->GetScale().y * 0.5f)
			)
		{
			isOnFloor = true;
			break;
		}
	}
	if (!isOnFloor)
	{
		Drop();
	}

	//Vector3 colliderPos = position + colliderOffset;
	//std::vector<FloorTile*> standingOnFloorList;

	//{//stores all floorTiles the player is standing on into a list
	//	for (auto q : FloorTilesManager::GetInstance()->GetList())
	//	{
	//		if (colliderPos.x < q->GetPosition().x + (q->GetScale().x * 0.5f) &&
	//			colliderPos.x > q->GetPosition().x - (q->GetScale().x * 0.5f) &&
	//			colliderPos.y < q->GetPosition().y + (q->GetScale().y * 0.5f) &&
	//			colliderPos.y > q->GetPosition().y - (q->GetScale().y * 0.5f)
	//			)
	//		{
	//			standingOnFloorList.push_back(q);
	//		}
	//	}
	//}
	//{//if exiting a tile, check if any nearby reachable tile exists
	//	for (auto q : standingOnFloorList)
	//	{
	//		Vector3 exitPoint = colliderPos;// determine which direction it will exit from
	//		if (colliderPos.x + colliderSize.x > q->GetPosition().x + (q->GetScale().x * 0.5f))
	//			exitPoint.x = colliderPos.x + colliderSize.x;
	//		else if (colliderPos.x - colliderSize.x < q->GetPosition().x - (q->GetScale().x * 0.5f))
	//			exitPoint.x = colliderPos.x - colliderSize.x;
	//		if (colliderPos.y + colliderSize.y > q->GetPosition().y + (q->GetScale().y * 0.5f))
	//			exitPoint.y = colliderPos.y + colliderSize.y;
	//		else if (colliderPos.y - colliderSize.y < q->GetPosition().y - (q->GetScale().y * 0.5f))
	//			exitPoint.y = colliderPos.y - colliderSize.y;

	//		if (exitPoint != colliderPos)//will be exiting current tile
	//		{
	//			bool anotherTileReachable = false;
	//			for (auto r : FloorTilesManager::GetInstance()->GetList())
	//			{
	//				if (r == q)
	//					continue;

	//				if (exitPoint.x < r->GetPosition().x + (r->GetScale().x * 0.5f) &&
	//					exitPoint.x > r->GetPosition().x - (r->GetScale().x * 0.5f) &&
	//					exitPoint.y < r->GetPosition().y + (r->GetScale().y * 0.5f) &&
	//					exitPoint.y > r->GetPosition().y - (r->GetScale().y * 0.5f)
	//					)
	//				{//able to reach another tile
	//					anotherTileReachable = true;
	//					break;
	//				}
	//			}
	//			if (anotherTileReachable == false)
	//			{
	//				Vector3 min = q->GetPosition() - q->GetScale() * 0.5f;
	//				Vector3 max = q->GetPosition() + q->GetScale() * 0.5f;

	//				if (colliderPos.x - colliderSize.x < min.x)
	//				{
	//					float offset = (min.x - (colliderPos.x - colliderSize.x));
	//					position.x += offset;
	//				}
	//				else if (colliderPos.x + colliderSize.x > max.x)
	//				{
	//					float offset = ((colliderPos.x + colliderSize.x) - max.x);
	//					position.x -= offset;
	//				}

	//				if (colliderPos.y - colliderSize.y < min.y)
	//				{
	//					float offset = (min.y - (colliderPos.y - colliderSize.y));
	//					position.y += offset;
	//				}
	//				else if (colliderPos.y + colliderSize.y > max.y)
	//				{
	//					float offset = ((colliderPos.y + colliderSize.y) - max.y);
	//					position.y -= offset;
	//				}
	//			}

	//			break;
	//		}
	//	}
	//}
}

void PlayerEntityBase::Drop()
{
	hasDropped = true;
	position.z = -3.0f;
	this->SetColliderIsActive(false);
	this->blinking = false;
}

void PlayerEntityBase::Respawn()
{
	GameInfo::GetInstance()->TakeDamage(1);
	this->scale = defaultScale;
	hasDropped = false;
	this->bufferTime = 2.0f;

	{//Find spawn point as avrgPos between all players
		Vector3 avrgPos;
		short counter = 0;
		for (auto q : GameInfo::GetInstance()->GetPlayersList())
		{
			if (this == q->GetCharacter() || q->GetCharacter()->GetHasDropped())
				continue;

			avrgPos += q->GetCharacter()->GetPosition();
			counter++;
		}
		
		if (counter <= 0)
			this->position = spawnPoint;
		else
			this->position = avrgPos / counter;
	}
}

void PlayerEntityBase::TakeDamage(int amount)
{
	if (bufferTime <= 0.0f)
	{
		bufferTime = 1.0f;
		this->SetColliderIsActive(false);
		GameInfo::GetInstance()->TakeDamage(amount);
	}
}

void PlayerEntityBase::HandleOutOfBounds(Vector3 min, Vector3 max, double dt)
{
	if ((position.x + colliderOffset.x) - colliderSize.x < min.x)
	{
		float offset = (min.x - ((position.x + colliderOffset.x) - colliderSize.x));
		position.x += offset;
	}
	else if ((position.x + colliderOffset.x) + colliderSize.x > max.x)
	{
		float offset = (((position.x + colliderOffset.x) + colliderSize.x) - max.x);
		position.x -= offset;
	}

	if ((position.y + colliderOffset.y) - colliderSize.y < min.y)
	{
		float offset = (min.y - ((position.y + colliderOffset.y) - colliderSize.y));
		position.y += offset;
	}
	else if ((position.y + colliderOffset.y) + colliderSize.y > max.y)
	{
		float offset = (((position.y + colliderOffset.y) + colliderSize.y) - max.y);
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
}

void PlayerEntityBase::Render()
{
	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

	if (!hasDropped)
	{
		modelStack.PushMatrix();
		modelStack.Translate(position.x, position.y, position.z);
		modelStack.Translate(colliderOffset.x, colliderOffset.y, colliderOffset.z);
		modelStack.Scale(colliderSize.x, colliderSize.x, 1);
		RenderHelper::RenderMesh(MeshBuilder::GetInstance()->GetMesh("shadow"));
		modelStack.PopMatrix();
	}

	if (!blinking)
	{
		modelStack.PushMatrix();
		modelStack.Translate(position.x, position.y, position.z + 0.1f);
		modelStack.Scale(scale.x, scale.y, scale.z);
		RenderHelper::RenderSprite(modelMesh, animationPlayer);
		modelStack.PopMatrix();
	}
}
