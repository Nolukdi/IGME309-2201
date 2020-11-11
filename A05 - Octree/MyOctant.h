#pragma once
/*----------------------------------------------
Programmer: Alex Di Filippo (ald1289@gmail.com)
Date: 2017/06
----------------------------------------------*/
#ifndef _MYOCTANTCLASS_H_
#define _MYOCTANTCLASS_H_

#include "MyEntityManager.h"

namespace Simplex
{
	class MyOctant
	{
		//Counting variables
		static uint octantNum; //# of Octants
		static uint maxLevel;    //Max octant level
		static uint idealEntNum;

		//Currents
		uint level = 0;	   //Current level
		uint id = 0;		   //Current octant

		//Size of octant
		float size = 0.0f;

		//Mesh and Entity managers
		MeshManager* m_pMeshMngr = nullptr;
		MyEntityManager* m_pEntityMngr = nullptr;
		std::vector<uint> m_EntityList;

		//Points in octant
		vector3 center = vector3(0.0f);
		vector3 min = vector3(0.0f);
		vector3 max = vector3(0.0f);

		//Parents and children
		MyOctant* parent = nullptr;
		MyOctant* children[8];
		uint childNum = 0;
		std::vector<MyOctant*> child;

		//Base octant
		MyOctant* base = nullptr;

	public:
		/*
		USAGE: Default constructor
		ARGUMENTS:
		-uint a_nMaxKevek: Sets default maximum level
		-uitn nIdealEntityCount: Sets the idea level of objects per octant
		OUTPUT: class object
		*/
		MyOctant(uint a_nMaxLevel = 2, uint a_nIdeaEntityCount = 5);
		/*
		USAGE: Constructor
		ARGUMENTS:
		-vector3 a_v3Center: Center of octant
		-float a_fSize
		OUTPUT: Octant object
		*/
		MyOctant(vector3 a_v3Center, float a_fSize);
		/*
		USAGE: Rule of Three - Copy Constructor
		ARGUMENTS: Copy object
		OUTPUT: Octant object
		*/
		MyOctant(MyOctant const& other);
		/*
		USAGE: Rule of Three - Copy Asignment Operator
		ARGUMENTS: Copy object
		*/
		MyOctant& operator=(MyOctant const& other);
		/*
		USAGE: Rule of Three - Destructor
		*/
		~MyOctant(void);
		/*
		USAGE: Swaps one content of an object with another
		ARGUMENTS:
		-MyOctant& other: Swapped object
		*/
		void Swap(MyOctant& other);
		/*
		USAGE: Gets the size of the current octant
		*/
		float GetSize(void);
		/*
		USAGE: Gets center of octant in global
		*/
		vector3 GetCenterGlobal(void);
		/*
		USAGE: Gets the min of octant in global
		*/
		vector3 GetMinGlobal(void);
		/*
		USAGE: Gets the max of octant in global
		*/
		vector3 GetMaxGlobal(void);
		/*
		USAGE: Is there a collision with the specified entity index?
		ARGUMENTS:
		-int a_uRBIndex: Index of entity
		*/
		bool IsCollidiing(uint a_uRBIndex);
		/*
		USAGE: Displays the octant at index
		ARGUMENTS: 
		-vector3 a_v3Color = CYELLOW
		*/
		void Display(uint a_nIndex, vector3 a_v3Color = C_YELLOW);
		/*
		USAGE: Displays the octant in color
		ARGUMENTS:
		-vector3 a_v3Color = CYELLOW
		*/
		void Display(vector3 a_v3Color = C_YELLOW);
		/*
		USAGE: Displays the leaves
		ARGUMENTS: 
		-vector3 a v_3Colors = CYELLOW
		*/
		void DisplayLeaves(vector3 a_v3Color = C_YELLOW);
		/*
		USAGE: Clears the entity list
		*/
		void ClearEntityList(void);
		/*
		USAGE: Divides octants into further subdivisions
		*/
		void Subdivide(void);
		/*
		USAGE: Returns the children
		ARGUMENTS: 
		-uint a_nCHild: Child index
		OUTPUT: Octant object
		*/
		MyOctant* GetChild(uint a_nChild);
		/*
		USAGE: Returns the parent
		OUTPUT: Octant object
		*/
		MyOctant* GetParent(void);
		/*
		USAGE: Is this a branch or a leaf?
		*/
		bool IsLeaf(void);
		/*
		USAGE: Is the current number of entities greater than this value?
		ARGUMENTS: 
		-uint a_nEntities: # of entities
		*/
		bool ContainsMoreThan(uint a_nEntities);
		/*
		USAGE: Deletes all branches
		*/
		void DeleteBranch(void);
		/*
		USAGE: Creates a tree
		ARGUMENTS: 
		-uint a_nMaxLevel = 3: Maximum level for tree
		*/
		void ConstructTree(uint a_nMaxLevel = 3);
		/*
		USAGE: Assigns ID to the object
		*/
		void AssignIDtoEntity(void);
		/*
		USAGE: Gets total octants
		*/
		uint GetOctants(void);

	private:
		/*
		USAGE: Deallocates member fields
		*/
		void Release(void);
		/*
		USAGE: Allocates memeber fields
		*/
		void Init(void);
		/*
		USAGE: Creates list of leaves containing objects
		*/
		void ConstructList(void);

	};

}

#endif //_MYOCTANTCLASS_H_