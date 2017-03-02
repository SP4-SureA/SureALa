#include "Collider.h"

Collider::Collider() :
colliderType(COLLIDER_NONE),
isActive(true),
isTrigger(false),
isStatic(false),
colliderOffset(Vector3()),
colliderSize(Vector3())
{
}

Collider::~Collider()
{
}