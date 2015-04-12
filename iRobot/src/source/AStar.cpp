/**
 * GameMap.hpp
 *  Created on: Apr 7, 2015
 *      Author: Jonathan Lessner
 *     Purpose: Runs the A* algorithm
 *              See for tutorial: https://www.youtube.com/watch?v=KNXfSOx4eEE
 *              See README.txt for discussion of specific optimizations
 */
#include "../headers/AStar.hpp"

using namespace std;

// Generic constructor
AStar::AStar()
: iteration_num(0)
{

}

// Initialization constructor
AStar::AStar(const GameMap &game_map, const PosTile &start_pos_arg, const PosTile &end_pos_arg)
: iteration_num(0)
{
	// Create the map
	// Store the matrix in row-major order, so that debugging prints appear
	//  in a human-readable fashion
	the_map = vector< vector<NodeInfo> > (game_map.getHeight(),vector<NodeInfo>(game_map.getWidth()));
	map_width =  the_map[0].size();
	map_height = the_map.size();

	// Set the start/end points
	start_pos = start_pos_arg;
	end_pos = end_pos_arg;

	// Calculate the H values for each tile (Manhattan Distance)
	calcHVals();

	// Set the G and F values to a value signifying "UNINITIALIZED"
	initFGVals();

	// Push the starting point to the Open list
	initOpenList();
}

// Destructor
AStar::~AStar()
{

}

// Run one step
void AStar::runStep(const GameMap &game_map, bool &blocked, bool &done)
{
	// Notify (so it doesn't look like it's stalled)
	if(iteration_num%10000 == 0)
	{
		PosTile cur_pos = open_list.front().position;
		cout << "Running A* iteration: " << iteration_num << ",\t"
		                                 << open_list.size() << ","
		                                 << the_map[cur_pos.y][cur_pos.x].F << ","
        								 << the_map[cur_pos.y][cur_pos.x].G << ","
        								 << the_map[cur_pos.y][cur_pos.x].H << endl;
	}
	++iteration_num;

	// Grab the front position from the Open list
	PosTile cur_tile = open_list.front().position;
	int cur_x = (int)cur_tile.x;	// Convert x to int for manipulation in the switch
	int cur_y = (int)cur_tile.y;	// Convert y to int for manipulation in the switch

	// Fetch the current Movement Cost
	float cur_G = the_map[cur_y][cur_x].G;

	// For each direction
	for(unsigned int dir = 0; dir < (unsigned int) MoveDir::NUM_DIRS; ++dir)
	{
		int new_x = cur_x, new_y= cur_y;

		// Assess all the movement possibilities
		if(dir == MoveDir::DIR_UL || dir == MoveDir::DIR_U || dir == MoveDir::DIR_UR)
			new_y -= 1;	// Attempting to move UP (of any flavor)
		else if(dir == MoveDir::DIR_DL || dir == MoveDir::DIR_D || dir == MoveDir::DIR_DR)
			new_y += 1;	// Attempting to move DOWN (of any flavor)

		if(dir == MoveDir::DIR_UL || dir == MoveDir::DIR_L || dir == MoveDir::DIR_DL)
			new_x -= 1;	// Attempting to move LEFT (of any flavor)
		else if(dir == MoveDir::DIR_UR || dir == MoveDir::DIR_R || dir == MoveDir::DIR_DR)
			new_x += 1;	// Attempting to move RIGHT (of any flavor)

		// Calc the movement cost
		float new_G;
		if(dir == MoveDir::DIR_U || dir == MoveDir::DIR_R || dir == MoveDir::DIR_L || dir == MoveDir::DIR_D)
			new_G = 1.0f; // The cost of one horiz/vert movement
		else
			new_G = 1.4f; // The cost of one diag movement (approx sqrt(2))

		// Check to see if it's off the edge of the board, skip this one if it is
		if(new_x < 0 || (unsigned int) new_x >= map_width ||
		   new_y < 0 || (unsigned int) new_y >= map_height )
			continue;

		// Since it's not off the egde, we know it's a valid tile position, so create one
		PosTile new_tile = { new_x, new_y };



		// Check to see if it's blocked, skip this one if it is
		if(!game_map.getTraversable(new_x, new_y))
			continue;

/*
 * 		NOTE: THIS HAS BEEN REMOVED FOR SPEED PURPOSES. See README.txt for discussion
		// Check to see if it's already on the Closed list, skip this one if it is
		// Find it on the closed list (had to use a custom function, since find() won't take a custom struct
		//		vector<posTile>::iterator c_it = find(closed_list.begin(), closed_list.end(), new_tile);
		int c_node_idx = findInClosedList(new_tile);
		if(c_node_idx != -1)
			continue;
*/

		// Calculate the cost to get there
		float sum_G = cur_G + new_G;

		// Fetch the current cost to get there
		float new_pos_G = the_map[new_y][new_x].G;
		float new_pos_F = the_map[new_y][new_x].F;

		// If the travel cost to get to the new tile from the current tile is either
		//  uninitialized or lower than previous attemtps, push it onto the list
		if(new_pos_G == -1 || sum_G < new_pos_G)
		{
			// Set the parent of the new tile to current tile
			the_map[new_y][new_x].parent = cur_tile;

			// Reset the G, and recalculate the F
			the_map[new_y][new_x].G = sum_G;
			the_map[new_y][new_x].F = the_map[new_y][new_x].G + the_map[new_y][new_x].H;

			// If it's a completely uninitialized tile
			if(new_pos_G == -1)
			{
				// Push this tile onto the Open list
				OpenListNode new_oln = { new_tile, the_map[new_y][new_x].F };
				open_list.push_back( new_oln );
			}
			// If we're updating the score of a tile already on the Open list
			else
			{
				// Create an OpenListNode of the old pattern, to search for it
				OpenListNode search_node = { new_tile, new_pos_F };

				// Find it on the open list (had to use a custom function, since find() won't take a custom struct
				//				deque<OpenListNode>::iterator o_it = find(open_list.begin(), open_list.end(), search_node);
				int o_node_idx = findInOpenList(search_node);
				if(o_node_idx != -1)
					open_list[o_node_idx].F = the_map[new_y][new_x].F;
			}
		}
	}

	// Put the used tile on the Closed list, and remove it from the Open list
	closed_list.push_back(cur_tile);
	open_list.pop_front();

	// Sort the Open list (smallest F value first)
	std::sort(open_list.begin(), open_list.end(), open_sort );

#if DEBUG
	// Print the map (debugging)
//	printMap();

	// Print Open list
	printOpenList();
#endif

	// A* exit condition
	// If the Open list has been exhausted, and we haven't yet reached the end_pos, return blocked (true)
	if(open_list.size() == 0)
	{
		blocked = true;
		return;
	}

	// A* exit condition
	// If the Open list has the end_pos at the head, return done (true)
	if(open_list.front().position.x == end_pos.x &&
	   open_list.front().position.y == end_pos.y)
	{
		done = true;
		return;
	}
}

// Get the full path string
void AStar::getPathString(stringstream &path_string)
{
	// Assemble the completion path (starts at end, ends at start, because of how A* works)
	vector<PosTile> a_star_path;
	PosTile cur_tile = open_list.front().position;
	while(cur_tile.x != start_pos.x || cur_tile.y != start_pos.y)
	{
		a_star_path.push_back(cur_tile);
		cur_tile = the_map[cur_tile.y][cur_tile.x].parent;
	}
	a_star_path.push_back(start_pos); // Add the start pos to complete the path

	// Reverse the list
	reverse(a_star_path.begin(), a_star_path.end());

	// Add each location to the stringstream
	unsigned int path_size = a_star_path.size();
	for(unsigned int pos_idx = 0; pos_idx < path_size; ++pos_idx)
		path_string << a_star_path[pos_idx].x << "," << a_star_path[pos_idx].y << endl;
}


// Calculate the (H)euristic values for all tiles
void AStar::calcHVals()
{
	// Use Manhattan Distance to estimate travel time from each node to the end point
	for(unsigned int row_idx = 0; row_idx < map_height; ++row_idx)
	{
		for(unsigned int col_idx = 0; col_idx < map_width; ++col_idx)
		{
			// Calculate the manhattan
			int manhattan = 0;									// Needs to be an 'int' so the abs() doen't create math errors
			manhattan += abs((int)end_pos.x - (int)col_idx);	// Add the absolute horizontal difference
			manhattan += abs((int)end_pos.y - (int)row_idx);	// Add the absolute vertical difference

			// Assign it to the AStar map
			the_map[row_idx][col_idx].H = (float) manhattan;
		}
	}
}

// Initialize the F/G values to a value signifying "UNINITIALIZED"
void AStar::initFGVals()
{
	for(unsigned int row_idx = 0; row_idx < map_height; ++row_idx)
	{
		for(unsigned int col_idx = 0; col_idx < map_width; ++col_idx)
		{
			the_map[row_idx][col_idx].F = FG_UNINIT;
			the_map[row_idx][col_idx].G = FG_UNINIT;
		}
	}
}

// Prepare the first node
void AStar::initOpenList()
{
	// Set the correct value for G and F
	the_map[start_pos.y][start_pos.x].G = 0;
	the_map[start_pos.y][start_pos.x].F = the_map[start_pos.y][start_pos.x].H;

	// Push the Start position onto the Open list
	OpenListNode new_oln = { start_pos, the_map[start_pos.y][start_pos.x].F };
	open_list.push_back(new_oln);
}

// Search algorithms for objects on open/closed lists
int AStar::findInOpenList(OpenListNode &search_node)
{
	int found_idx = -1;
	unsigned int list_size = open_list.size();

	for(unsigned int list_idx = 0; list_idx < list_size; ++list_idx)
	{
		if(search_node.position.x == open_list[list_idx].position.x &&
		   search_node.position.y == open_list[list_idx].position.y)
			return list_idx;
	}

	return found_idx;
}

int AStar::findInClosedList(PosTile &search_tile)
{
	int found_idx = -1;
	unsigned int list_size = closed_list.size();

	for(unsigned int list_idx = 0; list_idx < list_size; ++list_idx)
	{
		if(search_tile.x == closed_list[list_idx].x &&
		   search_tile.y == closed_list[list_idx].y)
			return list_idx;
	}

	return found_idx;
}




void AStar::printMap()
{
	cout << "AStar size: (" << the_map[0].size() << "," << the_map.size() << ")" << endl;
	cout << "AStar size: (" << map_width << "," << map_height << ")" << endl;

	// Cycle through the object printing the vectors
	for(unsigned int row_idx = 0; row_idx < map_height; ++row_idx)
	{
		cout << "row: " << row_idx << endl;
		for(unsigned int col_idx = 0; col_idx < map_width; ++col_idx)
		{
			cout << "AStar tile (" << col_idx << "," << row_idx << "):" << endl;
			cout << "\tParent (" << the_map[row_idx][col_idx].parent.x << "," <<
					the_map[row_idx][col_idx].parent.y << ")" << endl;
			cout << "\tF: " << the_map[row_idx][col_idx].F << endl;
			cout << "\tG: " << the_map[row_idx][col_idx].G << endl;
			cout << "\tH: " << the_map[row_idx][col_idx].H << endl;
		}
		//cout << endl;
//		cin.ignore();
	}
}

void AStar::printOpenList()
{
	unsigned int list_size = open_list.size();
	for(unsigned int list_idx = 0; list_idx < list_size; ++list_idx)
	{
		cout << "Open list idx " << list_idx << ": (" << open_list[list_idx].position.x
		     << "," << open_list[list_idx].position.y << "): " << open_list[list_idx].F << endl;
	}
}


