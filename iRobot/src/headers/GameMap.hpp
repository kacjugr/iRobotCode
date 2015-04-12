/**
 * GameMap.hpp
 *  Created on: Apr 7, 2015
 *      Author: Jonathan Lessner
 *     Purpose: Loads and houses data for the game map and obstacles
 */

#ifndef GAMEMAP_HPP_
#define GAMEMAP_HPP_

#include "../headers/Includes.h"

class GameMap
{
public:
	//---------------------------------------------------------------------
	// Constructors/Destructor
	//---------------------------------------------------------------------
	/**
	 * Function: GameMap()
	 * Use: Non-initialization constructor
	 *      Not for general use
	 *      Initializes nothing
	 *
	 * @param void
	 */
	GameMap();

	/**
	 * Function: GameMap(const Json::Value &obstacles, unsigned int max_width, int max_height)
	 * Use: Initialization constructor
	 *      Creates 2D bool matrix of specified (max_width,max_height), all initialize to 'true'
	 *      Initializes obstacle tiles to 'false'
	 *      if(DEBUG), automatically creates obstacles for traversal, rather than load from JSON
	 *
	 * @param obstacles JSON formatted array of [x,y] tile locations
	 * @param max_width The width for the map
	 * @param max_height The height for the map
	 */
	GameMap(const Json::Value &obstacles, unsigned int max_width, int max_height);

	/**
	 * Function: ~GameMap()
	 * Use: Destructor
	 *
	 * @param void
	 */
	~GameMap();

	//---------------------------------------------------------------------
	// Fetch functions
	//---------------------------------------------------------------------
	/**
	 * Function: getHeight()
	 *
	 * @param void
	 * @return Height of the game map
	 */
	unsigned int getHeight() const { return the_map.size(); }
	/**
	 * Function: getWidth()
	 *
	 * @param void
	 * @return Width of the game map
	 */
	unsigned int getWidth() const
	{
		if(getHeight() == 0)	return 0;
		else					return the_map[0].size();
	}
	/**
	 * Function: getTraversable()
	 *
	 * @param x The tile column
	 * @param y The tile row
	 * @return Traversability of specified tile (true/false)
	 */
	bool getTraversable(unsigned int x, unsigned int y) const { return the_map[y][x]; }

private:

	// The map (a bool vector matrix showing whether each tile is/isn't traversable)
	std::vector< std::vector<bool> > the_map;

	/**
	 * Function: createObstacles()
	 * Use: Creates obstacles in the game map (1/2 of total tiles)
	 *      For use in debugging
	 *
	 * @param void
	 */
	void createObstacles();

public:
	/**
	 * Function: printMap()
	 * Use: Prints the map to the console ('1' = traversable, '0' = obstacle)
	 *      For use in debugging
	 *
	 * @param void
	 */
	void printMap();
};



#endif /* GAMEMAP_HPP_ */
