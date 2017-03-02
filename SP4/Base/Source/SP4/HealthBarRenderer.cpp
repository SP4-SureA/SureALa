#include "HealthBarRenderer.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"
#include "GameInfo.h"

HealthBarRenderer::HealthBarRenderer(Mesh* _frontMesh, Mesh* _backMesh) :
frontMesh(_frontMesh),
backMesh(_backMesh),
maxHealth(1),
currentHealth(0),
rotateAngle(0)
{
}

HealthBarRenderer::~HealthBarRenderer()
{
}

void HealthBarRenderer::RenderUI()
{
	if (!b_DoRender || maxHealth == 0)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();

    float bossHealthPercentage = currentHealth / maxHealth;
    Vector3 bossHealthScale(bossHealthPercentage * scale.x, scale.y, scale.z);

    modelStack.PushMatrix();
    modelStack.Translate(position.x, position.y, position.z);
    modelStack.Rotate(rotateAngle, 0, 0, 1);
    modelStack.Translate(-scale.x, 0, 0);

    if (bossHealthPercentage > 0)
    {
        modelStack.PushMatrix();
        modelStack.Scale(bossHealthScale.x, bossHealthScale.y, bossHealthScale.z);
        modelStack.Translate(0.5f, 0, 0.1f);
        RenderHelper::RenderMesh(frontMesh);
        modelStack.PopMatrix();
    }

    modelStack.PushMatrix();
    modelStack.Scale(scale.x, scale.y, scale.z);
    modelStack.Translate(0.5f, 0, 0.1f);
    RenderHelper::RenderMesh(backMesh);
    modelStack.PopMatrix();

    modelStack.PopMatrix();
}

HealthBarRenderer* Create::HealthBarRendererEntity(
    EntityManager* em,
    const std::string& _backMesh,
    const std::string& _frontMesh,
    const Vector3& _position,
    const Vector3& _scale,
    const float& _maxHealth
    )
{
	if (em == NULL)
		return NULL;

    Mesh* back_mesh = MeshBuilder::GetInstance()->GetMesh(_backMesh);
    Mesh* front_mesh = MeshBuilder::GetInstance()->GetMesh(_frontMesh);

    if (back_mesh == nullptr || front_mesh == nullptr)
        return nullptr;

    HealthBarRenderer* result = new HealthBarRenderer(front_mesh, back_mesh);

    result->SetMaxHealth(_maxHealth);

    result->SetPosition(_position);
    result->SetScale(_scale);

	em->AddEntity(result, true);
	return result;
}

HealthBarRenderer* Create::HealthBarRendererAsset(
    const std::string& _backMesh,
    const std::string& _frontMesh,
    const Vector3& _position,
    const Vector3& _scale,
    const float& _maxHealth
	)
{
    Mesh* back_mesh = MeshBuilder::GetInstance()->GetMesh(_backMesh);
    Mesh* front_mesh = MeshBuilder::GetInstance()->GetMesh(_frontMesh);

    if (back_mesh == nullptr || front_mesh == nullptr)
		return nullptr;

    HealthBarRenderer* result = new HealthBarRenderer(front_mesh, back_mesh);

    result->SetMaxHealth(_maxHealth);

    result->SetPosition(_position);
    result->SetScale(_scale);

	return result;
}
