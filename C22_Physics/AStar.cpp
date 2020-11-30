#include "AStar.h"
#include "MyEntityManager.h"

using namespace std;
using namespace Simplex;

Simplex::AStar::AStar()
{
	//Sets up each square in the path
	IDSize = 2;
	ID = new int[IDSize]();
	parent = nullptr;
	c = FLT_MAX; //Set to maximum to be changed later
	h = FLT_MAX;
	w = FLT_MAX;
	hole = false;
	closed = false;
}

Simplex::AStar::AStar(const AStar& otherSquare)
{
	//Copying square elements
	IDSize = otherSquare.IDSize;
	ID = new int[IDSize]();
	copy(otherSquare.ID, otherSquare.ID + otherSquare.IDSize, ID);
	parent = nullptr;
	c = otherSquare.c;
	h = otherSquare.h;
	w = otherSquare.w;
	hole = otherSquare.hole;
	closed = otherSquare.closed;
}


AStar& Simplex::AStar::operator=(AStar const& otherSquare)
{
	//If the square is the same
	if (this == &otherSquare)
	{
		return *this;
	}

	//If not
	if (ID != nullptr)
	{
		//Set the id
		delete[] ID;
		ID = nullptr;
		IDSize = 2;
	}

	//Copy things over
	IDSize = otherSquare.IDSize;
	ID = new int[IDSize]();
	copy(otherSquare.ID, otherSquare.ID + otherSquare.IDSize, ID);

	parent = nullptr;
	c = otherSquare.c;
	h = otherSquare.h;
	w = otherSquare.w;
	hole = otherSquare.hole;
	closed = otherSquare.closed;

	return *this;
}

Simplex::AStar::~AStar()
{
	//Delete values and reset
	delete[] ID;
	ID = nullptr;
	IDSize = 2;
	parent = nullptr;
	c = FLT_MAX;
	h = FLT_MAX;
	w = FLT_MAX;
	hole = false;
}

int* Simplex::AStar::getID()
{
	return ID;
}

void Simplex::AStar::setID(int x, int z)
{
	//Sets parameters
	ID[0] = x;
	ID[1] = z;
}

float Simplex::AStar::getCost()
{
	//Returns current cost
	return c;
}

void Simplex::AStar::setCost(float c)
{
	//Sets cost to parameter
	this->c = c;
}

float Simplex::AStar::getWeight()
{
	//Returns current weight
	return w;
}

void Simplex::AStar::setWeight(float w)
{
	//Sets weight to parameter
	this->w = w;
}

float Simplex::AStar::getHeuristicCost()
{
	//Returns current heuristic
	return h;
}

void Simplex::AStar::setHeuristicCost(float h)
{
	//Sets heuristic to parameter
	this->h = h;
}

bool Simplex::AStar::getClosed()
{
	//Returns closed value
	return closed;
}

void Simplex::AStar::setClosed(bool closed)
{
	//Sets closed as parameter
	this->closed = closed;
}

bool Simplex::AStar::getHole()
{
	//Returns hole value
	return hole;
}

void Simplex::AStar::setHole(bool hole)
{
	//Sets hole to parameter
	this->hole = hole;
}

AStar* Simplex::AStar::getParent()
{
	//Returns parent
	return parent;
}

void Simplex::AStar::setParent(AStar* parent)
{
	//Sets parent to parameter
	this->parent = parent;
}