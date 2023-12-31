#include "GameObject.h"


GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::InitComponents()
{
    tarmacTex.TextureLoader("..\\res\\TarmacDark_D.jpg");

	shader1.InitalizeShader("..\\res\\shader.vert", "..\\res\\shader.frag");
	mesh1.ModelLoader("..\\res\\cube.obj");
}

/*void GameObject::SetTransform(Transform transform, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	transform.SetPos(pos);
	transform.SetRot(rot);
	transform.SetScale(scale);
}*/

void GameObject::DrawAll(Transform transform, MainCamera mainCamera, glm::vec3 pos, glm::vec3 rot, glm::vec3 scale)
{
	transform.SetPos(pos);
	transform.SetRot(rot);
	transform.SetScale(scale);

	tarmacTex.BindTexture(0);

	shader1.Bind();
	shader1.UpdateShader(transform, mainCamera);

	mesh1.Draw();
}


/*
Transform transform;



void GameObject::CombineObject()
{
	transform.SetPos(glm::vec3(sinf(gameManager.counter), 0.5, 0.0));
	transform.SetRot(glm::vec3(0.0, 0.0, gameManager.counter * 5));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));


	TextureManager texture("..\\res\\bricks.jpg"); //load texture
	texture.BindTexture(0);

	shader.Bind();
	shader.Update(transform, mainCamera);

	mesh.updateSphereData(*transform.GetPos(), 0.6f);
}

void GameObject::CombineObject1()
{
	transform.SetPos(glm::vec3(-sinf(gameManager.counter), -0.5, -sinf(gameManager.counter) * 5));
	transform.SetRot(glm::vec3(0.0, 0.0, gameManager.counter * 5));
	transform.SetScale(glm::vec3(0.6, 0.6, 0.6));

	TextureManager texture1("..\\res\\water.jpg"); //load texture
	texture1.BindTexture(0);

	shader.Bind();
	shader.Update(transform, mainCamera);

	mesh.updateSphereData(*transform.GetPos(), 0.6f);
}

bool GameManager::collision(glm::vec3 m1Pos, float m1Rad, glm::vec3 m2Pos, float m2Rad)
{
	float distance = ((m2Pos.x - m1Pos.x) * (m2Pos.x - m1Pos.x) + (m2Pos.y - m1Pos.y) * (m2Pos.y - m1Pos.y) + (m2Pos.z - m1Pos.z) * (m2Pos.z - m1Pos.z));

	if (distance * distance < (m1Rad + m2Rad))
	{
		//audioDevice.setlistner(myViewport.getPos(), m1Pos); // add bool to mesh
		gameAudio.PlaySound(0);
		cout << distance << '\n';
		return true;
	}
	else
	{
		return false;
	}
}*/