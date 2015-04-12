/**
 * GameMap.hpp
 *  Created on: Apr 7, 2015
 *      Author: Jonathan Lessner
 *     Purpose: Loads and houses data for the game map and obstacles
 */

#include "../headers/GameMap.hpp"

using namespace std;

// Default constructor
GameMap::GameMap()
{

}

// Initialization constructor
GameMap::GameMap(const Json::Value &obstacles, unsigned int max_width, int max_height)
{
	cout << "Constructing a GameMap (init)" << endl;

	// Create the initial matrix
	// Store the matrix in row-major order, so that debugging prints appear
	//  in a human-readable fashion
	the_map = vector< vector<bool> > (max_height,vector<bool>(max_width, true));

#if DEBUG
	// Make some obstacles (debugging)
	createObstacles();
#else
	// Initialize the obstacles
	unsigned int obs_size = obstacles.size();
	for(unsigned int obs_idx = 0; obs_idx < obs_size; ++obs_idx)
	{
		// Set the map location to 'false' (not traversable)
		unsigned int obs_x = obstacles[obs_idx][(unsigned int)0].asUInt();
		unsigned int obs_y = obstacles[obs_idx][(unsigned int)1].asUInt();

		the_map[obs_y][obs_x] = false;
	}
#endif

}

// Destructor
GameMap::~GameMap()
{

}

// Create obstacles (for debugging)
void GameMap::createObstacles()
{
	// Seed the random number generator
	srand(time(NULL));

	unsigned int map_height = the_map.size();
	unsigned int map_width  = the_map[0].size();
	unsigned int num_obs = (map_height*map_width)/2;

	for(unsigned int obs_idx = 0; obs_idx < num_obs; ++obs_idx)
	{
		unsigned int rand_x = rand()%map_width;
		unsigned int rand_y = rand()%map_height;

		if((rand_x == 1 && rand_y == 1) || (rand_x == 19 && rand_y == 19))
			continue;

		the_map[rand_y][rand_x] = false;

	}
}

// Print map (for debugging)
void GameMap::printMap()
{
	// Fetch the width/height
	unsigned int map_width, map_height;
	map_width =  the_map[0].size();
	map_height = the_map.size();

	// Cycle through the object printing the vectors
	for(unsigned int row_idx = 0; row_idx < map_height; ++row_idx)
	{
		cout << "row: " << ((row_idx < 10)?" ":"") << row_idx << "\t";
		for(unsigned int col_idx = 0; col_idx < map_width; ++col_idx)
			cout << (the_map[row_idx][col_idx]?1:0);	// '1' = traversable, '0' = obstacle
		cout << endl;
	}
}

