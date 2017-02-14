#include "TextEntity.h"
#include "MeshBuilder.h"
#include "EntityManager.h"
#include "GraphicsManager.h"
#include "RenderHelper.h"

TextEntity::TextEntity(Mesh* _modelMesh, const std::string& _text, const Color& _color) :
modelMesh(_modelMesh),
text(_text),
mode(MODE_2D),
color(_color)
{
	position.Set(0.0f, 0.0f, 0.0f);
	scale.Set(1.0f, 1.0f, 1.0f);
}

TextEntity::~TextEntity()
{
}

void TextEntity::Update(double dt)
{
	// Does nothing here, can inherit & override or create your own version of this class :D
}

void TextEntity::Render()
{
	if (mode == MODE_2D)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderText(modelMesh, text, color);
	modelStack.PopMatrix();
}

void TextEntity::RenderUI()
{
	if (mode == MODE_3D)
		return;

	MS& modelStack = GraphicsManager::GetInstance()->GetModelStack();
	modelStack.PushMatrix();
	modelStack.Translate(position.x, position.y, position.z);
	modelStack.Scale(scale.x, scale.y, scale.z);
	RenderHelper::RenderText(modelMesh, text, color);
	modelStack.PopMatrix();
}

TextEntity* Create::Text2DObject(EntityManager* em, const std::string& _meshName, const Vector3& _position, const std::string& _text, const Vector3& _scale, const Color& _color)
{
	if (em == NULL)
		return NULL;
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	TextEntity* result = new TextEntity(modelMesh, _text, _color);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetTextRenderMode(TextEntity::MODE_2D);
	em->AddEntity(result);
	return result;
}

TextEntity* Create::Text3DObject(EntityManager* em, const std::string& _meshName, const Vector3& _position, const std::string& _text, const Vector3& _scale, const Color& _color)
{
	if (em == NULL)
		return NULL;
	Mesh* modelMesh = MeshBuilder::GetInstance()->GetMesh(_meshName);
	if (modelMesh == nullptr)
		return nullptr;

	TextEntity* result = new TextEntity(modelMesh, _text, _color);
	result->SetPosition(_position);
	result->SetScale(_scale);
	result->SetTextRenderMode(TextEntity::MODE_3D);
	em->AddEntity(result);
	return result;
}