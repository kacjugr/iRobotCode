/**
 * GameMap.hpp
 *  Created on: Apr 7, 2015
 *      Author: Jonathan Lessner
 *     Purpose: Runs the A* algorithm
 *              See for tutorial: https://www.youtube.com/watch?v=KNXfSOx4eEE
 *              See README.txt for discussion of specific optimizations
 */

#ifndef ASTAR_HPP_
#define ASTAR_HPP_

#include "../headers/Includes.h"
#include "../headers/GameMap.hpp"

#define FG_UNINIT	(-1.0f)

using namespace std;

// Tile definition for the A* map
struct NodeInfo
{
	PosTile parent;	// The tile which offers the shortest path to the start point
	float F,		// Total estimated value of the tile (G+H) (lower is better)
	      G,		// Total effort needed to reach this tile
	      H;		// Heuristic guess at the remaining distance to the end point

};

// Entry data for Open list. Contains F score for fast sorting
struct OpenListNode
{
	PosTile position;
	float F;
};

// Possible movement directions for the pathfinding agent
namespace MoveDir
{
	enum Type
	{
        DIR_U,	// UP
        DIR_UR, // UP-RIGHT
        DIR_R,	// RIGHT
        DIR_DR,	// DOWN-RIGHT
        DIR_D,	// DOWN
        DIR_DL,	// DOWN-LEFT
        DIR_L,	// LEFT
        DIR_UL,	// UP_LEFT
		NUM_DIRS // DO NOT ADD BELOW THIS
	} ;
}

// Sorting priority definition for Open list
// Use with sort(vector.begin(), vector.end(), open_sorter)
struct OpenSorter
{
	bool operator ()(const OpenListNode &a, const OpenListNode &b ) {
		return a.F < b.F;	// Sort by smallest-first
	}
};



class AStar
{
public:
	//---------------------------------------------------------------------
	// Constructors/Destructor
	//---------------------------------------------------------------------
	/**
	 * Function: AStar()
	 * Use: Non-initialization constructor
	 *      Not for general use
	 *      Initializes nothing
	 *
	 * @param void
	 */
	AStar();

	/**
	 * Function: AStar(const GameMap &game_map, const PosTile &start_pos_arg, const PosTile &end_pos_arg)
	 * Use: Initializes A* data map from GameMap traversability map and start/end positions
	 *      Creates 2D matrix of NodeInfos the same width/height as game_map
	 *      Initializes all of the F/G/H values
	 *
	 * @param game_map The traversability map (must be initialized prior to passing to A*)
	 * @param start_pos_arg The start [x,y] tile for the A* agent
	 * @param end_pos_arg The end [x,y] tile for the A* agent
	 */
	AStar(const GameMap &game_map, const PosTile &start_pos_arg, const PosTile &end_pos_arg);

	/**
	 * Function: ~GameMap()
	 * Use: Destructor
	 *
	 * @param void
	 */
	~AStar();


	/**
	 * Function: runStep()
	 * Use: Run one step of the A* algorithm
	 *      Set 'blocked' to 'true' if no path can be found
	 *      Set 'done' to 'true' if path has been found
	 *
	 * @param game_map The traversibility GameMap
	 * @param blocked The flag variable for failure
	 * @param done The flag variable for completion
	 */
	void runStep(const GameMap &game_map, bool &blocked, bool &done);

	/**
	 * Function: getPathString()
	 * Use: Compile a stringstream of all tile positions from start to end poinst
	 *
	 * @param path_string The return string (used referenced variable for efficiency)
	 * @return void
	 */
	void getPathString(stringstream &path_string);


	//---------------------------------------------------------------------
	// Fetch functions
	//---------------------------------------------------------------------
	/**
	 * Function: getF()
	 * Use: Fetch F score from a specific tile
	 *
	 * @param pos The tile position
	 * @return float The F value
	 */
	float getF(PosTile pos) const { return the_map[pos.y][pos.x].F; }
	/**
	 * Function: getG()
	 * Use: Fetch G score from a specific tile
	 *
	 * @param pos The tile position
	 * @return float The G value
	 */
	float getG(PosTile pos) const { return the_map[pos.y][pos.x].G; }
	/**
	 * Function: getH()
	 * Use: Fetch H score from a specific tile
	 *
	 * @param pos The tile position
	 * @return float The H value
	 */
	float getH(PosTile pos) const { return the_map[pos.y][pos.x].H; }



private:

	// The map (a NodeInfo vector matrix showing whether each tile is/isn't traversable)
	std::vector< std::vector<NodeInfo> > the_map;

	// Map width/height
	unsigned int map_width, map_height;

	// The start and end points
	PosTile start_pos, end_pos;

	// A* iteration number
	unsigned long iteration_num;

	// The Open/Closed lists
	deque<OpenListNode> open_list;
	vector<PosTile> closed_list;

	// The sorter for the Open list
	OpenSorter open_sort;

	/**
	 * Function: calcHVals()
	 * Use: Calculates the (H)euristic values for all tiles
	 *
	 * @param void
	 * @return void
	 */
	void calcHVals();

	/**
	 * Function: calcHVals()
	 * Use: Initialize the F/G values to a value signifying "UNINITIALIZED"
	 *
	 * @param void
	 * @return void
	 */
	void initFGVals();

	/**
	 * Function: calcHVals()
	 * Use: Prepare the first node
	 *
	 * @param void
	 * @return void
	 */
	void initOpenList();

	/**
	 * Function: findInOpenList()
	 * Use: Search the Open list for specific position and F value
	 *
	 * @param search_node The node to find in the Open list
	 * @return int the index of object found in list (-1 if not found)
	 */
	int findInOpenList(OpenListNode &search_node);

	/**
	 * Function: findInClosedList()
	 * Use: Search the Closed list for specific position
	 *
	 * @param search_node The node to find in the Closed list
	 * @return int the index of object found in list (-1 if not found)
	 */
	int findInClosedList(PosTile &search_tile);

	/**
	 * Function: printMap()
	 * Use: Prints each cell in A* map with [x,y] and F/G/H scores to console
	 *      For debugging purposes
	 *
	 * @param void
	 * @return void
	 */
	void printMap();

	/**
	 * Function: printOpenList()
	 * Use: Prints each entry in Open list with [x,y] and F score to console
	 *      For debugging purposes
	 *
	 * @param void
	 * @return void
	 */
	void printOpenList();
};



#endif /* GAMEMAP_HPP_ */
