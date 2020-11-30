#pragma once
#ifndef _ASTAR_H_
#define _ASTAR_H_

namespace Simplex
{
	class AStar
	{
	public:
		AStar(); //Default constructor
		AStar(const AStar& otherAStar); //Copy constructor
		AStar& operator=(AStar const& otherAStar); //Copy assignment operator
		~AStar(); //Destructor
		int* getID(); //Gets pointer of ID
		void setID(int x, int z); //Sets ID
		float getCost(); //Gets cost from one point to another
		void setCost(float c); //Sets cost from one point to another
		float getWeight(); //Get weight
		void setWeight(float w); //Set weight
		float getHeuristicCost(); //Get heuristic cost
		void setHeuristicCost(float h); //Sets heuristic cost
		bool getClosed(); //Check if closed
		void setClosed(bool closed); //Set on closed list
		bool getHole(); //Get hole if it exists
		void setHole(bool hole); //Set hole if it exists
		AStar* getParent(); //Gets parent of current square
		void setParent(AStar* parent); //Sets parent

	private:
		int* ID;
		int IDSize;
		float c;
		float h;
		float w;
		bool hole;
		bool closed;
		AStar* parent;
	};
}

#endif