#include "AppClass.h"
void Application::InitVariables(void)
{
	//Change this to your name and email
	m_sProgrammer = "Alex Di Filippo - ald1289@rit.edu";

	//Set the position and target of the camera
	//(I'm at [0,0,10], looking at [0,0,0] and up is the positive Y axis)
	m_pCameraMngr->SetPositionTargetAndUpward(AXIS_Z * 10.0f, ZERO_V3, AXIS_Y);

	//Allocate the memory for the Meshes
	meshAmount = 46;
	for (uint i = 0; i < meshAmount; ++i)
	{
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
	matrix4 m4Projection = m_pCameraMngr->GetProjectionMatrix();
	matrix4 m4View = m_pCameraMngr->GetViewMatrix();

	//Get a timer
	static float fTimer = 0;	//store the new timer
	static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
	fTimer += static_cast<float>(m_pSystem->GetDeltaTime(uClock)); //get the delta time for that timer

	//Draw Meshes
	uint i = 0;
	static float fHorizontal = -10.0f;
	static vector3 v3Position(fHorizontal, 0.0f, 0.0f);
	
	v3Position.x = fHorizontal + fTimer;
	v3Position.y = glm::cos(glm::radians(fTimer * 60.0f));
	matrix4 m4Position = glm::translate(vector3(-5.0f, -3.0f, -15.0f)) * glm::translate(v3Position);

	//first row
	for (int a = 3; a < 7; a++)
	{
		if (i == 5)
		{
			a++;
		}

		meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3.0f, 0.0f, 0.0f))); ++i;
	}

	//second row
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(0.0f, 1.0f, 0.0f))); ++i;
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(2.0f, 1.0f, 0.0f))); ++i;
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(8.0f, 1.0f, 0.0f))); ++i;
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(10.0f, 1.0f, 0.0f))); ++i;

	//third row
	for (int a = 0; a < 10; a++)
	{
		if (a == 1 || a == 9)
		{
			a++;
		}

		meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(a, 2.0f, 0.0f))); ++i;
	}

	//fourth row
	for (int a = 0; a < 11; a++)
	{
		meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(a, 3.0f, 0.0f))); ++i;
	}

	//fifth row
	for (int a = 0; a < 9; a++)
	{
		if (a == 3 || a == 7)
		{
			a++;
		}

		meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(a, 4.0f, 0.0f))); ++i;
	}

	//sixth row
	for (int a = 2; a < 9; a++)
	{
		meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(a, 5.0f, 0.0f))); ++i;
	}

	//seventh row
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(3, 6.0f, 0.0f))); ++i;
	meshes[i]->Render(m4Projection, m4View, glm::translate(m4Position, vector3(7, 6.0f, 0.0f))); ++i;

	//eight row
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