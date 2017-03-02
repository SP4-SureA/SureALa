#include "ScrollingCamera.h"

#include "Application.h"
#include "EntityBase.h"

ScrollingCamera::ScrollingCamera()
{
}

ScrollingCamera::~ScrollingCamera()
{
}

Mtx44 ScrollingCamera::GetViewMatrix()
{
	Mtx44 result;
	result.SetToLookAt(position.x, position.y, position.z,
		target.x, target.y, target.z,
		up.x, up.y, up.z);
	return result;
}

void ScrollingCamera::Init(const Vector3& pos, const Vector3& target, const Vector3& up)
{
	this->position = defaultPosition = pos;
	this->target = defaultTarget = target;
	Vector3 view = Vector3(0, 0, -1);// (target - position).Normalized();
	Vector3 right = view.Cross(up);
	right.y = 0;
	right.Normalize();
	this->up = defaultUp = right.Cross(view).Normalized();

	watchList.clear();
}

void ScrollingCamera::AddToWatch(EntityBase* entity)
{
	if (!entity)
		return;

	watchList.push_back(entity);
}
bool ScrollingCamera::RemoveFromWatch(EntityBase* entity)
{
	// Find the entity's iterator
	std::vector<EntityBase*>::iterator findIter = std::find(watchList.begin(), watchList.end(), entity);

	// Remove the entity if found
	if (findIter != watchList.end())
	{
		findIter = watchList.erase(findIter);

		return true;
	}
	// Return false if not found
	return false;
}

void ScrollingCamera::Update(double dt)
{
	if (watchList.size() <= 0)
		return;

	orthoSize = minOrthoSize;

	Vector3 avrgPos;
	for (auto q : watchList)
	{
		avrgPos += q->GetPosition() + q->GetColliderOffset();
	}
	avrgPos /= watchList.size();

	float halfOrthoWidth = orthoSize.x * 0.5f;
	float halfOrthoHeight = orthoSize.y * 0.5f;

	if (avrgPos.x + halfOrthoWidth > worldMax.x)
		avrgPos.x += worldMax.x - (avrgPos.x + halfOrthoWidth);
	if (avrgPos.x - halfOrthoWidth < worldMin.x)
		avrgPos.x -= (avrgPos.x - halfOrthoWidth) - worldMin.x;
	if (avrgPos.y + halfOrthoHeight > worldMax.y)
		avrgPos.y += worldMax.y - (avrgPos.y + halfOrthoHeight);
	if (avrgPos.y - halfOrthoHeight < worldMin.y)
		avrgPos.y -= (avrgPos.y - halfOrthoHeight) - worldMin.y;
	
	position = avrgPos;
	position.z = 1;
	target = avrgPos;
	target.z = 0;
}

void ScrollingCamera::Reset()
{
	position = defaultPosition;
	target = defaultTarget;
	up = defaultUp;
}