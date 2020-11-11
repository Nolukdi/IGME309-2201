#include "MyOctant.h"
using namespace Simplex;

//MyOctant variables
uint MyOctant::octantNum = 0;
uint MyOctant::maxLevel = 3;
uint MyOctant::idealEntNum = 5;
uint MyOctant::GetOctants(void) { return octantNum; };

//Contructor
Simplex::MyOctant::MyOctant(uint a_nMaxLevel, uint a_nIdeaEntityCount)
{
	Init();

	//Sets up initial variables
	octantNum = 0;
	maxLevel = a_nMaxLevel;
	uint nObjects = m_pEntityMngr->GetEntityCount();
	idealEntNum = a_nIdeaEntityCount;
	id = octantNum;

	child.clear();
	base = this;

	//List of all mins and maxes
	std::vector<vector3> minMax;

	//For every object
	for (uint i = 0; i < nObjects; i++)
	{
		//Assign the entity (&rigidbody) at the index
		MyEntity* pEntity = m_pEntityMngr->GetEntity(i);
		MyRigidBody* pRigidBody = pEntity->GetRigidBody();

		//Add mins and maxes
		minMax.push_back(pRigidBody->GetMinGlobal());
		minMax.push_back(pRigidBody->GetMaxGlobal());
	}

	//Use mins and maxes for rigidbody
	MyRigidBody* pRigidBody = new MyRigidBody(minMax);

	//Compute halfwidth
	vector3 vHalfWidth = pRigidBody->GetHalfWidth();
	float fMax = vHalfWidth.x;

	//For every index 1-2
	for (int i = 1; i < 3; i++)
	{
		//If the max is less than the halfwidth
		if (fMax < vHalfWidth[i])
		{
			//Set it equal
			fMax = vHalfWidth[i];
		}
	}

	//Set up center
	vector3 v3Center = pRigidBody->GetCenterLocal();

	//Clear and reset
	minMax.clear();
	SafeDelete(pRigidBody);

	//Adjust size, center, min, and max
	size = fMax * 2.0f;
	center = v3Center;
	min = center - (vector3(fMax));
	max = center + (vector3(fMax));

	//Increment octant count
	octantNum++;
	ConstructTree(maxLevel);
}

//Contrsuctor
Simplex::MyOctant::MyOctant(vector3 a_v3Center, float a_fSize)
{
	Init();

	//Intialize variables
	center = a_v3Center;
	size = a_fSize;

	//Compute min and max
	min = center - (vector3(size) / 2.0f);
	max = center + (vector3(size) / 2.0f);

	//Update count
	octantNum++;
}

//Copy constructor
Simplex::MyOctant::MyOctant(MyOctant const& other)
{
	childNum = other.childNum;
	center = other.center;
	min = other.min;
	max = other.max;

	size = other.size;
	id = other.id;
	level = other.level;
	parent = other.parent;

	base = other.base;
	child = other.child;

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	for (uint i = 0; i < 8; i++)
	{
		children[i] = other.children[i];
	}
}

//Copy assignment operator
MyOctant& Simplex::MyOctant::operator=(MyOctant const& other)
{
	if (this != &other)
	{
		Release();
		Init();
		MyOctant temp(other);
		Swap(temp);
	}

	return *this;
}

//Deconstructor
Simplex::MyOctant::~MyOctant(void)
{
	Release();
}

//Swap method
void Simplex::MyOctant::Swap(MyOctant& other)
{
	std::swap(childNum, other.childNum);

	std::swap(size, other.size);
	std::swap(id, other.id);
	std::swap(base, other.base);
	std::swap(child, other.child);

	std::swap(center, other.center);
	std::swap(min, other.min);
	std::swap(max, other.max);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	std::swap(level, other.level);
	std::swap(parent, other.parent);
	for (uint i = 0; i < 8; i++)
	{
		std::swap(children[i], other.children[i]);
	}
}

//Method to size of octant
float Simplex::MyOctant::GetSize(void)
{
	return size;
}

//Method to get the global center
vector3 Simplex::MyOctant::GetCenterGlobal(void)
{
	return center;
}

//Method to get the global min
vector3 Simplex::MyOctant::GetMinGlobal(void)
{
	return min;
}

//Method to get the global max
vector3 Simplex::MyOctant::GetMaxGlobal(void)
{
	return max;
}

//Method to check if objects are colliding
bool Simplex::MyOctant::IsCollidiing(uint a_uRBIndex)
{
	uint nObjectCount = m_pEntityMngr->GetEntityCount();

	//If the index is out of the object count range
	if (a_uRBIndex >= nObjectCount)
	{
		//Not colliding
		return false;
	}

	//Get entity & rigidbody by index
	MyEntity* pEntity = m_pEntityMngr->GetEntity(a_uRBIndex);
	MyRigidBody* pRigidBody = pEntity->GetRigidBody();

	//Compute min and max to check against
	vector3 v3MinO = pRigidBody->GetMinGlobal();
	vector3 v3MaxO = pRigidBody->GetMaxGlobal();

	//If max x is less than min x
	if (max.x < v3MinO.x)
	{
		return false;
	}

	//If min x is more than max x
	if (min.x > v3MaxO.x)
	{
		return false;
	}

	//If max y is less than min y
	if (max.y < v3MinO.y)
	{
		return false;
	}

	//If min y is more than max y
	if (min.y > v3MaxO.y)
	{
		return false;
	}

	//If max z is less than min z
	if (max.z < v3MinO.z)
	{
		return false;
	}

	//If min z is more than max z
	if (min.z > v3MaxO.z)
	{
		return false;
	}

	//If all checks fail
	return true;
}

//Method to display the octant using index and color
void Simplex::MyOctant::Display(uint a_nIndex, vector3 a_v3Color)
{
	if (id == a_nIndex)
	{
		m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center) * glm::scale(vector3(size)), a_v3Color, RENDER_WIRE);
		return;
	}
	for (uint nIndex = 0; nIndex < childNum; nIndex++)
	{
		children[nIndex]->Display(a_nIndex);
	}
}

//Method to display the octant using color
void Simplex::MyOctant::Display(vector3 a_v3Color)
{
	for (uint nIndex = 0; nIndex < childNum; nIndex++)
	{
		children[nIndex]->Display(a_v3Color);
	}

	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center) * glm::scale(vector3(size)), a_v3Color, RENDER_WIRE);
}

//Method to display all leaves (node with no child)
void Simplex::MyOctant::DisplayLeaves(vector3 a_v3Color)
{
	uint nLeafs = child.size();
	for (uint nChild = 0; nChild < nLeafs; nChild++)
	{
		child[nChild]->DisplayLeaves(a_v3Color);
	}
	m_pMeshMngr->AddWireCubeToRenderList(glm::translate(IDENTITY_M4, center) * glm::scale(vector3(size)), a_v3Color, RENDER_WIRE);
}

//Method to clear the list of entities
void Simplex::MyOctant::ClearEntityList(void)
{
	for (uint nChild = 0; nChild < childNum; nChild++)
	{
		children[nChild]->ClearEntityList();
	}
}

//Method to subdivide octants further
void Simplex::MyOctant::Subdivide(void)
{
	//If already past max level
	if (level >= maxLevel)
	{
		return;
	}

	//If node has children
	if (childNum != 0)
	{
		return;
	}

	childNum = 8;

	//Computing size
	vector3 v3Center;
	float fSize = size / 4.0f;
	float fSizeD = fSize * 2.0f;

	//Getting center and x/y/z
	v3Center = center;
	v3Center.x -= fSize;
	v3Center.y -= fSize;
	v3Center.z -= fSize;

	//Sets up all new children
	children[0] = new MyOctant(v3Center, fSizeD);
	v3Center.x += fSizeD;
	children[1] = new MyOctant(v3Center, fSizeD);
	v3Center.z += fSizeD;
	children[2] = new MyOctant(v3Center, fSizeD);
	v3Center.x -= fSizeD;
	children[3] = new MyOctant(v3Center, fSizeD);
	v3Center.y += fSizeD;
	children[4] = new MyOctant(v3Center, fSizeD);
	v3Center.z -= fSizeD;
	children[5] = new MyOctant(v3Center, fSizeD);
	v3Center.x += fSizeD;
	children[6] = new MyOctant(v3Center, fSizeD);
	v3Center.z += fSizeD;
	children[7] = new MyOctant(v3Center, fSizeD);

	//For all of the children
	for (uint nIndex = 0; nIndex < 8; nIndex++)
	{
		//Initialize variables
		children[nIndex]->base = base;
		children[nIndex]->parent = this;
		children[nIndex]->level = level + 1;

		//If there are too many entities
		if (children[nIndex]->ContainsMoreThan(idealEntNum))
		{
			//Subdivide again
			children[nIndex]->Subdivide();
		}
	}
}

//Gets the child
MyOctant* Simplex::MyOctant::GetChild(uint a_nChild)
{
	if (a_nChild > 7) return nullptr;
	return children[a_nChild];
}

//Gets the parent
MyOctant* Simplex::MyOctant::GetParent(void)
{
	return parent;
}

//Checks if node is leaf
bool Simplex::MyOctant::IsLeaf(void)
{
	//If it has no children
	if (childNum == 0)
	{
		//Is Leaf
		return true;
	}

	//If it does
	else
	{
		//Not a leaf
		return false;
	}
}

//Method to check if the octant contains too many entities
bool Simplex::MyOctant::ContainsMoreThan(uint a_nEntities)
{
	uint nCount = 0;
	uint nObjectCount = m_pEntityMngr->GetEntityCount();
	for (uint n = 0; n < nObjectCount; n++)
	{
		if (IsCollidiing(n))
			nCount++;
		if (nCount > a_nEntities)
			return true;
	}

	return false;
}

//Method to delete branch
void Simplex::MyOctant::DeleteBranch(void)
{
	//Until there are no children left
	for (uint nIndex = 0; nIndex < childNum; nIndex++)
	{
		//Move down tree and delete branch
		children[nIndex]->DeleteBranch();
		delete children[nIndex];
		children[nIndex] = nullptr;
	}

	//No children
	childNum = 0;
}

//Method to build the tree
void Simplex::MyOctant::ConstructTree(uint a_nMaxLevel)
{
	//If the level is not 0
	if (level != 0)
	{
		//Bail
		return;
	}

	//Set up max levels
	maxLevel = a_nMaxLevel;
	octantNum = 1;

	//Clearing the entity list
	m_EntityList.clear();

	//Clearing the entire tree
	DeleteBranch();
	child.clear();

	//If the base tree contains more than ideal
	if (ContainsMoreThan(idealEntNum))
	{
		//Subdivide it further
		Subdivide();
	}

	AssignIDtoEntity();
	ConstructList();
}

//Method to assign IDs to entities
void Simplex::MyOctant::AssignIDtoEntity(void)
{
	//Until there is a leaf
	for (uint nChild = 0; nChild < childNum; nChild++)
	{
		//Assign IDs to children
		children[nChild]->AssignIDtoEntity();
	}

	//If a leaf is found
	if (childNum == 0)
	{
		//Get the amount of entities
		uint nEnities = m_pEntityMngr->GetEntityCount();

		//For all entities
		for (uint nIndex = 0; nIndex < nEnities; nIndex++)
		{
			//If they are colliding
			if (IsCollidiing(nIndex))
			{
				//Add them to the entitylist
				m_EntityList.push_back(nIndex);
				m_pEntityMngr->AddDimension(nIndex, id);
			}
		}
	}
}

//Method to release
void Simplex::MyOctant::Release(void)
{
	if (level == 0)
	{
		DeleteBranch();
	}

	childNum = 0;
	size = 0.0f;
	m_EntityList.clear();
	child.clear();
}

//Method to initialize variables
void Simplex::MyOctant::Init(void)
{
	childNum = 0;

	size = 0.0f;

	id = octantNum;
	level = 0;

	center = vector3(0.0f);
	min = vector3(0.0f);
	max = vector3(0.0f);

	m_pMeshMngr = MeshManager::GetInstance();
	m_pEntityMngr = MyEntityManager::GetInstance();

	base = nullptr;
	parent = nullptr;
	for (uint n = 0; n < 8; n++)
	{
		children[n] = nullptr;
	}
}

//Method to get a list of all children
void Simplex::MyOctant::ConstructList(void)
{
	for (uint nChild = 0; nChild < childNum; nChild++)
	{
		children[nChild]->ConstructList();
	}

	if (m_EntityList.size() > 0)
	{
		base->child.push_back(this);
	}
}