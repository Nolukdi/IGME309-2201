#include "AppClass.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

using namespace Simplex;
void Application::InitVariables(void)
{
	//Gets random world gen
	srand(time(NULL));

	//Set the position and target of the camera
	m_pCameraMngr->SetPositionTargetAndUpward(
		vector3(5.0f, 15.0f, 15.0f), //Position
		vector3(5.0f, 0.0f, 0.0f),	//Target
		AXIS_Y);					//Up

	m_pLightMngr->SetPosition(vector3(0.0f, 3.0f, 13.0f), 1); //set the position of first light (0 is reserved for ambient light)

	startList.push_back(vector3(6.0f, 1.0f, 8.0f));
	startList.push_back(vector3(2.0f, 1.0f, 2.0f));

	m_pEntityMngr->AddEntity("Minecraft\\Steve.obj", "Steve");
	m_pEntityMngr->SetPosition(vector3(5.0f, 1.0f, 3.0f), "Steve");
	m_stevePosition = vector3(5.0f, -1.1f, 3.0f);
	m_pEntityMngr->UsePhysicsSolver();

	m_pEntityMngr->AddEntity("Minecraft\\Cow.obj", "Cow");
	m_pEntityMngr->SetPosition(vector3(7.5f, 1.0f, 6.5f), "Cow");
	m_cowPosition = vector3(8.0f, -1.1f, 7.0f);
	m_pEntityMngr->UsePhysicsSolver();

	m_pEntityMngr->AddEntity("Minecraft\\Pig.obj", "Pig");
	m_pEntityMngr->SetPosition(vector3(0.5f, 1.0f, 4.5f), "Pig");
	m_pigPosition = vector3(1.0f, -1.1f, 5.0f);
	m_pEntityMngr->UsePhysicsSolver();

	//Sets up the start and the target
	startIndexI = m_stevePosition.z;
	startIndexJ = m_stevePosition.x;
	targetIndexI = m_cowPosition.z;
	targetIndexJ = m_cowPosition.x;

	//For 10 loops
	for (int i = 0; i < 10; i++)
	{
		//For 10 loops
		for (int j = 0; j < 10; j++)
		{
			//Set up square in position
			squares[i][j] = new AStar();
			squares[i][j]->setID(i, j);

			//Randomized holes
			if (rand() % 10 < 1)
			{
				squares[i][j]->setHole(true);
				continue;
			}

			//Draws out the floor
			m_pEntityMngr->AddEntity("Minecraft\\Cube.obj", "Cube_" + std::to_string(i));
			vector3 v3Position = vector3(i, -1, j);
			matrix4 m4Position = glm::translate(v3Position);
			m_pEntityMngr->SetModelMatrix(m4Position * glm::scale(vector3(1.0f)));

			//Sets up heuristic cost for square
			squares[i][j]->setHeuristicCost(HCost(i, j, m_cowPosition));

			//Sets regular cost for square
			squares[i][j]->setCost(1.0f);

			//If it has reached the target
			if (i == targetIndexI && j == targetIndexJ)
			{
				//Set heuristic to 0
				squares[i][j]->setHeuristicCost(0.0f);
			}

			//If it is at the start
			if (i == startIndexI && j == startIndexJ)
			{
				//Set cost to 0
				squares[i][j]->setCost(0.0f);
			}
		}
	}
}
void Application::Update(void)
{
	//Update the system so it knows how much time has passed since the last call
	m_pSystem->Update();

	//Is the ArcBall active?
	ArcBall();

	//Is the first person camera active?
	CameraRotation();

	//Update Entity Manager
	m_pEntityMngr->Update();

	//Add objects to render list
	m_pEntityMngr->AddEntityToRenderList(-1, true);

	//If still going
	if (cont)
	{
		//Run again
		aStar();
	}

	//For all closed points
	for (int i = 0; i < closed.size(); i++)
	{
		//Draws path with squares
		matrix4 m4Rotation = glm::rotate(IDENTITY_M4, glm::radians(-90.0f), vector3(1.0f, 0.0f, 0.0f));
		matrix4 planePosition = glm::translate(vector3(closed[i]->getID()[1] - 0.5f, -0.01f, closed[i]->getID()[0] - 0.5f)) * m4Rotation; //adjust by 0.5 to get in the cube perfectly
		m_pMeshMngr->AddPlaneToRenderList(planePosition, C_BLUE, 1);
	}

	//If they are colliding
	if (newPosition)
	{
		//If collision is 0
		if (collision == 0)
		{
			m_pEntityMngr->SetPosition(vector3(2.0f, 1.0f, 5.0f), "Steve");
			m_stevePosition = vector3(2.0f, -1.1f, 5.0f);
			m_pEntityMngr->SetPosition(vector3(8.0f, 1.0f, 9.0f), "Cow");
			m_cowPosition = vector3(8.0f, -1.1f, 9.0f);
			collision++;
		}
		//If collision is 1
		else if (collision == 1)
		{
			m_pEntityMngr->SetPosition(vector3(6.0f, 1.0f, 8.0f), "Steve");
			m_stevePosition = vector3(6.0f, -1.1f, 8.0f);
			m_pEntityMngr->SetPosition(vector3(3.0f, 1.0f, 2.0f), "Cow");
			m_cowPosition = vector3(3.0f, -1.1f, 2.0f);
			collision++;
		}
		//If collision is 2
		else if (collision == 2)
		{
			m_pEntityMngr->SetPosition(vector3(1.0f, 1.0f, 7.0f), "Steve");
			m_stevePosition = vector3(1.0f, -1.1f, 7.0f);
			m_pEntityMngr->SetPosition(vector3(5.0f, 1.0f, 6.0f), "Cow");
			m_cowPosition = vector3(5.0f, -1.1f, 6.0f);
			collision++;
		}
		//If collision is 3
		else if (collision == 3)
		{
			m_pEntityMngr->SetPosition(vector3(2.0f, 1.0f, 5.0f), "Steve");
			m_stevePosition = vector3(2.0f, -1.1f, 5.0f);
			m_pEntityMngr->SetPosition(vector3(3.0f, 1.0f, 6.0f), "Cow");
			m_cowPosition = vector3(3.0f, -1.1f, 6.0f);
			collision = 0;
		}

		//Set up start and target points
		startIndexI = m_stevePosition.z;
		startIndexJ = m_stevePosition.x;
		targetIndexI = m_cowPosition.z;
		targetIndexJ = m_cowPosition.x;

		//For all squares
		for (int i = 0; i < 10; i++)
		{
			//For all squares
			for (int j = 0; j < 10; j++)
			{
				//Gets holes
				if (squares[i][j]->getHole())
				{
					continue;
				}

				//Sets up heuristic cost
				squares[i][j]->setHeuristicCost(HCost(i, j, m_cowPosition));

				//Sets up regular cost
				squares[i][j]->setCost(1.0f);

				//Sets parent to null
				squares[i][j]->setParent(nullptr);

				//All are closed
				squares[i][j]->setClosed(false);

				//If at target
				if (i == targetIndexI && j == targetIndexJ)
				{
					//Reset heuristic
					squares[i][j]->setHeuristicCost(0.0f);
				}

				//If at start
				if (i == startIndexI && j == startIndexJ)
				{
					//Reset cost
					squares[i][j]->setCost(0.0f);
				}
			}
		}

		//Run AStar again
		cont = true;
		aStar();
		newPosition = false;
	}
}
void Application::Display(void)
{
	// Clear the screen
	ClearScreen();

	//If there are items on the closed list
	if (closed.size() > 0)
	{
		//get a timer
		static float fTimer = 0;	//store the new timer
		static uint uClock = m_pSystem->GenClock(); //generate a new clock for that timer
		fTimer += m_pSystem->GetDeltaTime(uClock); //get the delta time for that timer
		vector3 currentPosition;
		float anim = 2.0f;

		//Save start and end
		vector3 startPosition;
		vector3 endPosition;

		//Model's place
		static uint place = 0;

		//If their place is at the end of the list
		if (place < closed.size() - 1)
		{
			//Set positions
			startPosition = vector3(closed[place]->getID()[1] - 0.5f, 1.0, closed[place]->getID()[0]);
			endPosition = vector3(closed[place + 1]->getID()[1] - 0.5f, 1.0, closed[place + 1]->getID()[0]);
		}
		//If not
		else
		{
			//If there is a collision
			if (m_pEntityMngr->GetRigidBody("Steve")->IsColliding(m_pEntityMngr->GetRigidBody("Cow")))
			{
				//Clear the list
				closed.clear();
				open.clear();
				newPosition = true;
			}
		}
		float totalTime = MapValue(fTimer, 0.0f, anim, 0.0f, 1.0f);

		//Lerp model according to time value
		currentPosition = glm::lerp(startPosition, endPosition, totalTime);

		//If the time is greater than 1
		if (totalTime >= 1.0f)
		{
			//Reset the time
			place++;
			fTimer = 0;
			place %= closed.size();
		}

		//Translate the model to position
		matrix4 m4Model = glm::translate(currentPosition);

		//Set steve's position
		m_pEntityMngr->SetPosition(currentPosition, "Steve");
	}

	// draw a skybox
	m_pMeshMngr->AddSkyboxToRenderList();

	//render list call
	m_uRenderCallCount = m_pMeshMngr->Render();

	//clear the render list
	m_pMeshMngr->ClearRenderList();

	//draw gui,
	DrawGUI();

	//end the current frame (internally swaps the front and back buffers)
	m_pWindow->display();
}
void Application::Release(void)
{
	//Release MyEntityManager
	MyEntityManager::ReleaseInstance();

	//For all squares
	for (int i = 0; i < 10; i++)
	{
		//For all squares
		for (int j = 0; j < 10; j++)
		{
			//Delete them
			SafeDelete(squares[i][j]);
		}
	}

	//release GUI
	ShutdownGUI();
}

//Gets heuristic cost (distance formula)
float Application::HCost(int x, int z, vector3 target)
{
	//Finding x values
	float Xs = x - target.x;
	float Zs = z - target.z;

	//Multiplying
	Xs = Xs * Xs;
	Zs = Zs * Zs;

	//Take square root of sum
	float h = sqrt(Xs + Zs);

	return h;
}

//Method to complete AStar
void Application::aStar()
{
	//Add first cell to closed list
	closed.push_back(squares[startIndexI][startIndexJ]);

	//While the method is running
	while (cont)
	{
		//If first cell is not a hole
		if (isValid(startIndexI - 1, startIndexJ))
		{
			//If target
			if (startIndexI - 1 == targetIndexI && startIndexJ == targetIndexJ)
			{
				//Set current as target
				squares[startIndexI - 1][startIndexJ]->setParent(squares[startIndexI][startIndexJ]);

				//Get regular cost
				squares[startIndexI - 1][startIndexJ]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI - 1][startIndexJ]->getCost());

				//Get weight
				squares[startIndexI - 1][startIndexJ]->setWeight(squares[startIndexI - 1][startIndexJ]->getCost() + 
					squares[startIndexI - 1][startIndexJ]->getHeuristicCost());

				//Close position off
				closed.push_back(squares[startIndexI - 1][startIndexJ]);

				cont = false;
				open.clear();
				return;
			}
			
			//If there is no parent and it isn't a hole
			else if (squares[startIndexI - 1][startIndexJ]->getParent() == nullptr && 
				!squares[startIndexI - 1][startIndexJ]->getHole() && 
				!squares[startIndexI - 1][startIndexJ]->getClosed())
			{
				//Square is open
				open.push_back(squares[startIndexI - 1][startIndexJ]);

				//Set current as parent
				squares[startIndexI - 1][startIndexJ]->setParent(squares[startIndexI][startIndexJ]);

				//Get regular cost
				squares[startIndexI - 1][startIndexJ]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI - 1][startIndexJ]->getCost());

				//Get weight
				squares[startIndexI - 1][startIndexJ]->setWeight(squares[startIndexI - 1][startIndexJ]->getCost() + 
					squares[startIndexI - 1][startIndexJ]->getHeuristicCost());
			}
			
			//If there is a parent and isn't a hole
			else if (squares[startIndexI - 1][startIndexJ]->getParent() != nullptr && 
				!squares[startIndexI - 1][startIndexJ]->getHole() && 
				!squares[startIndexI - 1][startIndexJ]->getClosed())
			{
				//If the cost is lower than the parent
				if (squares[startIndexI - 1][startIndexJ]->getParent()->getCost() + 
					squares[startIndexI - 1][startIndexJ]->getCost() >
					squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI - 1][startIndexJ]->getCost())
				{
					//Set new parent
					squares[startIndexI - 1][startIndexJ]->setParent(squares[startIndexI][startIndexJ]);

					//Set regular cost
					squares[startIndexI - 1][startIndexJ]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
						squares[startIndexI - 1][startIndexJ]->getCost());

					//Get weight 
					squares[startIndexI - 1][startIndexJ]->setWeight(squares[startIndexI - 1][startIndexJ]->getCost() + 
						squares[startIndexI - 1][startIndexJ]->getHeuristicCost());
				}
			}
		}

		//If next cell is not a hole
		if (isValid(startIndexI - 1, startIndexJ + 1))
		{
			if (startIndexI - 1 == targetIndexI && startIndexJ + 1 == targetIndexJ)
			{
				squares[startIndexI - 1][startIndexJ + 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI - 1][startIndexJ + 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI - 1][startIndexJ + 1]->getCost());
				squares[startIndexI - 1][startIndexJ + 1]->setWeight(squares[startIndexI - 1][startIndexJ + 1]->getCost() + 
					squares[startIndexI - 1][startIndexJ + 1]->getHeuristicCost());
				closed.push_back(squares[startIndexI - 1][startIndexJ + 1]);

				cont = false;
				open.clear();
				return;
			}

			else if (squares[startIndexI - 1][startIndexJ + 1]->getParent() == nullptr && 
				!squares[startIndexI - 1][startIndexJ + 1]->getHole() && 
				!squares[startIndexI - 1][startIndexJ + 1]->getClosed())
			{
				open.push_back(squares[startIndexI - 1][startIndexJ + 1]);
				squares[startIndexI - 1][startIndexJ + 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI - 1][startIndexJ + 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + squares[startIndexI - 1][startIndexJ + 1]->getCost());
				squares[startIndexI - 1][startIndexJ + 1]->setWeight(squares[startIndexI - 1][startIndexJ + 1]->getCost() + squares[startIndexI - 1][startIndexJ + 1]->getHeuristicCost());
			}
			
			else if (squares[startIndexI - 1][startIndexJ + 1]->getParent() != nullptr && 
				!squares[startIndexI - 1][startIndexJ + 1]->getHole() && 
				!squares[startIndexI - 1][startIndexJ + 1]->getClosed())
			{
				if (squares[startIndexI - 1][startIndexJ + 1]->getParent()->getCost() + 
					squares[startIndexI - 1][startIndexJ + 1]->getCost() >
					squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI - 1][startIndexJ + 1]->getCost())
				{
					squares[startIndexI - 1][startIndexJ + 1]->setParent(squares[startIndexI][startIndexJ]);
					squares[startIndexI - 1][startIndexJ + 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
						squares[startIndexI - 1][startIndexJ + 1]->getCost());
					squares[startIndexI - 1][startIndexJ + 1]->setWeight(squares[startIndexI - 1][startIndexJ + 1]->getCost() + 
						squares[startIndexI - 1][startIndexJ + 1]->getHeuristicCost());
				}
			}
		}

		if (isValid(startIndexI - 1, startIndexJ - 1))
		{
			if (startIndexI - 1 == targetIndexI && startIndexJ - 1 == targetIndexJ)
			{
				squares[startIndexI - 1][startIndexJ - 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI - 1][startIndexJ - 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI - 1][startIndexJ - 1]->getCost());
				squares[startIndexI - 1][startIndexJ - 1]->setWeight(squares[startIndexI - 1][startIndexJ - 1]->getCost() + 
					squares[startIndexI - 1][startIndexJ - 1]->getHeuristicCost());
				closed.push_back(squares[startIndexI - 1][startIndexJ - 1]);

				cont = false;
				open.clear();
				return;
			}

			else if (squares[startIndexI - 1][startIndexJ - 1]->getParent() == nullptr && 
				!squares[startIndexI - 1][startIndexJ - 1]->getHole() && 
				!squares[startIndexI - 1][startIndexJ - 1]->getClosed())
			{
				open.push_back(squares[startIndexI - 1][startIndexJ - 1]);
				squares[startIndexI - 1][startIndexJ - 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI - 1][startIndexJ - 1]->setCost(squares[startIndexI][startIndexJ]->getCost() +
					squares[startIndexI - 1][startIndexJ - 1]->getCost());
				squares[startIndexI - 1][startIndexJ - 1]->setWeight(squares[startIndexI - 1][startIndexJ - 1]->getCost() + 
					squares[startIndexI - 1][startIndexJ - 1]->getHeuristicCost());
			}

			else if (squares[startIndexI - 1][startIndexJ - 1]->getParent() != nullptr && 
				!squares[startIndexI - 1][startIndexJ - 1]->getHole() && 
				!squares[startIndexI - 1][startIndexJ - 1]->getClosed())
			{
				if (squares[startIndexI - 1][startIndexJ - 1]->getParent()->getCost() + 
					squares[startIndexI - 1][startIndexJ + 1]->getCost() >
					squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI - 1][startIndexJ - 1]->getCost())
				{
					squares[startIndexI - 1][startIndexJ - 1]->setParent(squares[startIndexI][startIndexJ]);
					squares[startIndexI - 1][startIndexJ - 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
						squares[startIndexI - 1][startIndexJ - 1]->getCost());
					squares[startIndexI - 1][startIndexJ - 1]->setWeight(squares[startIndexI - 1][startIndexJ - 1]->getCost() + 
						squares[startIndexI - 1][startIndexJ - 1]->getHeuristicCost());
				}
			}
		}

		if (isValid(startIndexI + 1, startIndexJ))
		{
			if (startIndexI + 1 == targetIndexI && startIndexJ == targetIndexJ)
			{
				squares[startIndexI + 1][startIndexJ]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI + 1][startIndexJ]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI + 1][startIndexJ]->getCost());
				squares[startIndexI + 1][startIndexJ]->setWeight(squares[startIndexI + 1][startIndexJ]->getCost() +
					squares[startIndexI + 1][startIndexJ]->getHeuristicCost());
				closed.push_back(squares[startIndexI + 1][startIndexJ]);

				cont = false;
				open.clear();
				return;
			}

			else if (squares[startIndexI + 1][startIndexJ]->getParent() == nullptr && 
				!squares[startIndexI + 1][startIndexJ]->getHole() && 
				!squares[startIndexI + 1][startIndexJ]->getClosed())
			{
				open.push_back(squares[startIndexI + 1][startIndexJ]);
				squares[startIndexI + 1][startIndexJ]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI + 1][startIndexJ]->setCost(squares[startIndexI][startIndexJ]->getCost() + squares[startIndexI + 1][startIndexJ]->getCost());
				squares[startIndexI + 1][startIndexJ]->setWeight(squares[startIndexI + 1][startIndexJ]->getCost() + squares[startIndexI + 1][startIndexJ]->getHeuristicCost());
			}

			else if (squares[startIndexI + 1][startIndexJ]->getParent() != nullptr && 
				!squares[startIndexI + 1][startIndexJ]->getHole() && 
				!squares[startIndexI + 1][startIndexJ]->getClosed())
			{
				if (squares[startIndexI + 1][startIndexJ]->getParent()->getCost() + 
					squares[startIndexI + 1][startIndexJ]->getCost() >
					squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI + 1][startIndexJ]->getCost())
				{
					squares[startIndexI + 1][startIndexJ]->setParent(squares[startIndexI][startIndexJ]);
					squares[startIndexI + 1][startIndexJ]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
						squares[startIndexI + 1][startIndexJ]->getCost());
					squares[startIndexI + 1][startIndexJ]->setWeight(squares[startIndexI + 1][startIndexJ]->getCost() + 
						squares[startIndexI + 1][startIndexJ]->getHeuristicCost());
				}
			}
		}

		if (isValid(startIndexI + 1, startIndexJ + 1))
		{
			if (startIndexI + 1 == targetIndexI && startIndexJ + 1 == targetIndexJ)
			{
				squares[startIndexI + 1][startIndexJ + 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI + 1][startIndexJ + 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI + 1][startIndexJ + 1]->getCost());
				squares[startIndexI + 1][startIndexJ + 1]->setWeight(squares[startIndexI + 1][startIndexJ + 1]->getCost() + 
					squares[startIndexI + 1][startIndexJ + 1]->getHeuristicCost());
				closed.push_back(squares[startIndexI + 1][startIndexJ + 1]);

				cont = false;
				open.clear();
				return;
			}

			else if (squares[startIndexI + 1][startIndexJ + 1]->getParent() == nullptr && 
				!squares[startIndexI + 1][startIndexJ + 1]->getHole() && 
				!squares[startIndexI + 1][startIndexJ + 1]->getClosed())
			{
				open.push_back(squares[startIndexI + 1][startIndexJ + 1]);
				squares[startIndexI + 1][startIndexJ + 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI + 1][startIndexJ + 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI + 1][startIndexJ + 1]->getCost());
				squares[startIndexI + 1][startIndexJ + 1]->setWeight(squares[startIndexI + 1][startIndexJ + 1]->getCost() + 
					squares[startIndexI + 1][startIndexJ + 1]->getHeuristicCost());
			}

			else if (squares[startIndexI + 1][startIndexJ + 1]->getParent() != nullptr && 
				!squares[startIndexI + 1][startIndexJ + 1]->getHole() && 
				!squares[startIndexI + 1][startIndexJ + 1]->getClosed())
			{
				if (squares[startIndexI + 1][startIndexJ + 1]->getParent()->getCost() + squares[startIndexI + 1][startIndexJ + 1]->getCost() >
					squares[startIndexI][startIndexJ]->getCost() + squares[startIndexI + 1][startIndexJ + 1]->getCost())
				{
					squares[startIndexI + 1][startIndexJ + 1]->setParent(squares[startIndexI][startIndexJ]);
					squares[startIndexI + 1][startIndexJ + 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
						squares[startIndexI + 1][startIndexJ + 1]->getCost());
					squares[startIndexI + 1][startIndexJ + 1]->setWeight(squares[startIndexI + 1][startIndexJ + 1]->getCost() + 
						squares[startIndexI + 1][startIndexJ + 1]->getHeuristicCost());
				}
			}
		}

		if (isValid(startIndexI + 1, startIndexJ - 1))
		{
			if (startIndexI + 1 == targetIndexI && startIndexJ - 1 == targetIndexJ)
			{
				squares[startIndexI + 1][startIndexJ - 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI + 1][startIndexJ - 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI + 1][startIndexJ - 1]->getCost());
				squares[startIndexI + 1][startIndexJ - 1]->setWeight(squares[startIndexI + 1][startIndexJ - 1]->getCost() + 
					squares[startIndexI + 1][startIndexJ - 1]->getHeuristicCost());
				closed.push_back(squares[startIndexI + 1][startIndexJ - 1]);

				cont = false;
				open.clear();
				return;
			}

			else if (squares[startIndexI + 1][startIndexJ - 1]->getParent() == nullptr && 
				!squares[startIndexI + 1][startIndexJ - 1]->getHole() && 
				!squares[startIndexI + 1][startIndexJ - 1]->getClosed())
			{
				open.push_back(squares[startIndexI + 1][startIndexJ - 1]);
				squares[startIndexI + 1][startIndexJ - 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI + 1][startIndexJ - 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI + 1][startIndexJ - 1]->getCost());
				squares[startIndexI + 1][startIndexJ - 1]->setWeight(squares[startIndexI + 1][startIndexJ - 1]->getCost() + 
					squares[startIndexI + 1][startIndexJ - 1]->getHeuristicCost());
			}

			else if (squares[startIndexI + 1][startIndexJ - 1]->getParent() != nullptr && 
				!squares[startIndexI + 1][startIndexJ - 1]->getHole() && 
				!squares[startIndexI + 1][startIndexJ - 1]->getClosed())
			{
				if (squares[startIndexI + 1][startIndexJ - 1]->getParent()->getCost() + squares[startIndexI + 1][startIndexJ - 1]->getCost() >
					squares[startIndexI][startIndexJ]->getCost() + squares[startIndexI + 1][startIndexJ - 1]->getCost())
				{
					squares[startIndexI + 1][startIndexJ - 1]->setParent(squares[startIndexI][startIndexJ]);
					squares[startIndexI + 1][startIndexJ - 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
						squares[startIndexI + 1][startIndexJ - 1]->getCost());
					squares[startIndexI + 1][startIndexJ - 1]->setWeight(squares[startIndexI + 1][startIndexJ - 1]->getCost() + 
						squares[startIndexI + 1][startIndexJ - 1]->getHeuristicCost());
				}
			}
		}

		if (isValid(startIndexI, startIndexJ + 1))
		{
			if (startIndexI == targetIndexI && startIndexJ + 1 == targetIndexJ)
			{
				squares[startIndexI][startIndexJ + 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI][startIndexJ + 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI][startIndexJ + 1]->getCost());
				squares[startIndexI][startIndexJ + 1]->setWeight(squares[startIndexI][startIndexJ + 1]->getCost() + 
					squares[startIndexI][startIndexJ + 1]->getHeuristicCost());
				closed.push_back(squares[startIndexI][startIndexJ + 1]);

				cont = false;
				open.clear();
				return;
			}

			else if (squares[startIndexI][startIndexJ + 1]->getParent() == nullptr && 
				!squares[startIndexI][startIndexJ + 1]->getHole() && 
				!squares[startIndexI][startIndexJ + 1]->getClosed())
			{
				open.push_back(squares[startIndexI][startIndexJ + 1]);
				squares[startIndexI][startIndexJ + 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI][startIndexJ + 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI][startIndexJ + 1]->getCost());
				squares[startIndexI][startIndexJ + 1]->setWeight(squares[startIndexI][startIndexJ + 1]->getCost() + 
					squares[startIndexI][startIndexJ + 1]->getHeuristicCost());
			}

			else if (squares[startIndexI][startIndexJ + 1]->getParent() != nullptr && 
				!squares[startIndexI][startIndexJ + 1]->getHole() && 
				!squares[startIndexI][startIndexJ + 1]->getClosed())
			{
				if (squares[startIndexI][startIndexJ + 1]->getParent()->getCost() + squares[startIndexI][startIndexJ + 1]->getCost() >
					squares[startIndexI][startIndexJ]->getCost() + squares[startIndexI][startIndexJ + 1]->getCost())
				{
					squares[startIndexI][startIndexJ + 1]->setParent(squares[startIndexI][startIndexJ]);
					squares[startIndexI][startIndexJ + 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
						squares[startIndexI][startIndexJ + 1]->getCost());
					squares[startIndexI][startIndexJ + 1]->setWeight(squares[startIndexI][startIndexJ + 1]->getCost() + 
						squares[startIndexI][startIndexJ + 1]->getHeuristicCost());
				}
			}
		}

		if (isValid(startIndexI, startIndexJ - 1))
		{
			if (startIndexI == targetIndexI && startIndexJ - 1 == targetIndexJ)
			{
				squares[startIndexI][startIndexJ - 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI][startIndexJ - 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI][startIndexJ - 1]->getCost());
				squares[startIndexI][startIndexJ - 1]->setWeight(squares[startIndexI][startIndexJ - 1]->getCost() + 
					squares[startIndexI][startIndexJ - 1]->getHeuristicCost());
				closed.push_back(squares[startIndexI][startIndexJ - 1]);

				cont = false;
				open.clear();
				return;
			}

			else if (squares[startIndexI][startIndexJ - 1]->getParent() == nullptr && 
				!squares[startIndexI][startIndexJ - 1]->getHole() && 
				!squares[startIndexI][startIndexJ - 1]->getClosed())
			{
				open.push_back(squares[startIndexI][startIndexJ - 1]);
				squares[startIndexI][startIndexJ - 1]->setParent(squares[startIndexI][startIndexJ]);
				squares[startIndexI][startIndexJ - 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
					squares[startIndexI][startIndexJ - 1]->getCost());
				squares[startIndexI][startIndexJ - 1]->setWeight(squares[startIndexI][startIndexJ - 1]->getCost() + 
					squares[startIndexI][startIndexJ - 1]->getHeuristicCost());
			}

			else if (squares[startIndexI][startIndexJ - 1]->getParent() != nullptr && 
				!squares[startIndexI][startIndexJ - 1]->getHole() && 
				!squares[startIndexI][startIndexJ - 1]->getClosed())
			{
				if (squares[startIndexI][startIndexJ - 1]->getParent()->getCost() + squares[startIndexI][startIndexJ - 1]->getCost() >
					squares[startIndexI][startIndexJ]->getCost() + squares[startIndexI][startIndexJ - 1]->getCost())
				{
					squares[startIndexI][startIndexJ - 1]->setParent(squares[startIndexI][startIndexJ]);
					squares[startIndexI][startIndexJ - 1]->setCost(squares[startIndexI][startIndexJ]->getCost() + 
						squares[startIndexI][startIndexJ - 1]->getCost());
					squares[startIndexI][startIndexJ - 1]->setWeight(squares[startIndexI][startIndexJ - 1]->getCost() + 
						squares[startIndexI][startIndexJ - 1]->getHeuristicCost());
				}
			}
		}

		//Holds smallest weight value
		float smallest = FLT_MAX;
		int index = 0;

		//For all open squares
		for (int i = 0; i < open.size(); i++)
		{
			//If the weight is smaller than the smallest
			if (open[i]->getWeight() < smallest)
			{
				//Set the smallest
				smallest = open[i]->getWeight();

				//New start points
				startIndexI = open[i]->getID()[0];
				startIndexJ = open[i]->getID()[1];

				index = i;
			}
		}

		//Remove square from open list
		open.erase(open.begin() + index);

		//Add to closed list
		closed.push_back(squares[startIndexI][startIndexJ]);

		//Set square as closed
		squares[startIndexI][startIndexJ]->setClosed(true);
	}
}

//Check if the square is valid
bool Application::isValid(int x, int z)
{
	//If it is in bounds
	if (x > -1 && x < 10 && z > -1 && z < 10)
	{
		//It is valid
		return true;
	}

	return false;
}