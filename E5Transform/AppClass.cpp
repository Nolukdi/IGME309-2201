#include "AppClass.h"
void Application::InitVariables(void)
{
	//Define amount of meshes
	meshAmount = 46;

	//Loop through mesh list
	for (uint i = 0; i < meshAmount; ++i)
	{
		//Initialize meshes
		MyMesh* pMesh = new MyMesh();
		meshes.push_back(pMesh);
		meshes[i]->GenerateCube(1.0f, C_BLACK);
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the arcball active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//Calculate the model, view and projection matrix
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	
	//Starting translation value
	static float value = -5.0f;

	//Translation step
	value += 0.1f;

	//Initialize vector
	int i = 0;
	static float startingX = -10.0f;
	static vector3 vecPos(startingX, 0.0f, 0.0f);

	//Translate object across screen
	matrix4 m4Position = glm::translate(vector3(value, -3.0f, -15.0f)) * glm::translate(vecPos);

	//For everything in the first row
	for (int a = 3; a <= 7; a++)
	{
		//If in middle place
		if (a == 5)
		{
			//Skip
			a++;
		}

		//Draw cubes
		meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(a, 0.0f, 0.0f))); ++i;
	}

	//For everything in the second row, draw at these specific points
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, 1.0f, 0.0f))); ++i;
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, 1.0f, 0.0f))); ++i;
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8.0f, 1.0f, 0.0f))); ++i;
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(10.0f, 1.0f, 0.0f))); ++i;

	//For everything in the third row
	for (int a = 0; a < 10; a++)
	{
		//If it's in the second or second to last spot
		if (a == 1 || a == 9)
		{
			//Skip
			a++;
		}

		//Draw cubes
		meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(a, 2.0f, 0.0f))); ++i;
	}

	//For everything in the fourth row
	for (int a = 0; a < 11; a++)
	{
		//Draw cubes
		meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(a, 3.0f, 0.0f))); ++i;
	}

	//For everything in the fifth row
	for (int a = 1; a <= 9; a++)
	{
		//If it's in the 4th or 8th spot
		if (a == 3 || a == 7)
		{
			//Skip
			a++;
		}

		//Draw cubes
		meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(a, 4.0f, 0.0f))); ++i;
	}
	
	//For everything in the sixth row (starting from 2)
	for (int a = 2; a < 9; a++)
	{
		//Draw cubes
		meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(a, 5.0f, 0.0f))); ++i;
	}

	//For everything in the seventh row, draw at these specific points
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3, 6.0f, 0.0f))); ++i;
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7, 6.0f, 0.0f))); ++i;

	//For everything in the eighth row, draw at these specific points
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2, 7.0f, 0.0f))); ++i;
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8, 7.0f, 0.0f))); ++i;

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	std::vector<MyMesh*> m_pMeshList;
	uint uSize = m_pMeshList.size();
	for (uint i = 0; i < uSize; ++i)
	{
		if (m_pMeshList[i] != nullptr)
		{
			delete m_pMeshList[i];
			m_pMeshList[i] = nullptr;
		}
	}

	//release GUI
	ShutdownGUI();
}