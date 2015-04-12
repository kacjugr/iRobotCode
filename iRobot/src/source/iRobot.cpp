/**
 * iRobot.cpp
 *  Created on: Apr 7, 2015
 *      Author: Jonathan Lessner
 *     Purpose: Accepts an input file 'set#.dat', containing obstacle locations, start and end positions
 *              Creates a tile map of sufficient size
 *              Uses A* algorithm to find shortest path from start to end
 */


// Include all the standard modules
#include "../headers/Includes.h"

// Include the modules for pathfinding
#include "../headers/GameMap.hpp"
#include "../headers/AStar.hpp"


using namespace std;

/**
 * Function: main()
 * Use: Prompts user for input file number (1-6)
 *      Loads obstacles, start, end data
 *      Creates map of sufficience size
 *      Uses A* to find shortest path from start to end
 *
 * @param void
 * @return 0 found path from start to end
 *         1 no path from start to end
 */

int main() {

	// Ask for the input file name
	string datInputLine;
	stringstream datFileName;
	cout << "Please enter input file number (1-6) ('ENTER' for default (1)): ";
	getline(cin, datInputLine);
	if(datInputLine == "")
		datInputLine = "1";
	datFileName << "set" << datInputLine << ".dat";


	// Try to load the .dat file
	ifstream datFile;
	string datString;
	datFile.open(datFileName.str().c_str());
	if(datFile.is_open())
	{
		// All data provided as samples is formatted as a single line
		// If other dat files contain multiple lines, the JSON parse below will not work
		getline(datFile,datString);
	}
	else
	{
		cout << "Failed to open file: " << datFileName << endl;
		return 1;
	}


	// Parse the .dat string
	Json::Value root;
	Json::Reader reader;
	bool parsedSuccess = reader.parse(datString, root, false);
	if(!parsedSuccess)
	{
		// Report failures and their locations in the document.
		cout << "Failed to parse JSON" << endl
			 <<	reader.getFormatedErrorMessages()
			 << endl;
		return 1;
	}

	// Find the max_x and max_y within the obstacles
	Json::Value obstacles = root["obstacles"];
	unsigned int obstacles_size = obstacles.size();
	unsigned int max_obs_x = 0, max_obs_y = 0;
	for(unsigned int obs_idx = 0; obs_idx < obstacles_size; ++obs_idx)
	{
		unsigned int cur_x = obstacles[obs_idx][(unsigned int)0].asUInt();
		unsigned int cur_y = obstacles[obs_idx][(unsigned int)1].asUInt();

		if(cur_x > max_obs_x) max_obs_x = cur_x;
		if(cur_y > max_obs_y) max_obs_y = cur_y;
	}
	cout << "Max x/y = (" << max_obs_x << "," << max_obs_y << ")" << endl;

	// Now that the data is loaded, build a GameMap
#if DEBUG
	// Use a size of (20,20) for readability in debugging
	GameMap m_game_map = GameMap(obstacles, 20, 20);
	m_game_map.printMap();
#else
	// We'll use a default (1500,1500) because of supplied input data parameters
	GameMap m_game_map = GameMap(obstacles, 1500, 1500);
#endif

	// Load the start and end points
	Json::Value robot_start_dat = root["robotStart"];
	Json::Value robot_end_dat   = root["robotEnd"];
#if DEBUG
	posTile robot_start = {1, 1};
	posTile robot_end = {19, 19};
#else
	PosTile robot_start = {robot_start_dat[(unsigned int)0].asUInt(),
			                robot_start_dat[(unsigned int)1].asUInt()};
	PosTile robot_end = {robot_end_dat[(unsigned int)0].asUInt(),
			              robot_end_dat[(unsigned int)1].asUInt()};
#endif

	// Prepare A* by building a F/G/H map and pushing the start point onto the Open list
	AStar m_star_map = AStar(m_game_map, robot_start, robot_end);
//	m_star_map.initOpenList();

	// Run A* until it sets 'done' or 'blocked' to true
	string inputLine;
	cout << "Hit <Enter> to run A*";
	getline(cin, inputLine);
	bool blocked = false, done = false;
	while(!blocked && !done)
	{
		// Run A* for 1 step
		m_star_map.runStep(m_game_map, blocked, done);
	}

	// A* has finished, open a file to write results
	ofstream outputFile;
	stringstream outFileName;
	outFileName << "set" << datInputLine << "_out.txt";
	outputFile.open(outFileName.str().c_str());

	// Add input filename to beginning of output file
	//     (comment this out if pure path sequence is desired)
	stringstream outputLine;
	outputLine << "Path results for file: " << datFileName.str() << endl;
	outputFile << outputLine.str();

	// If the path is blocked (no route from robot_start to robot_end), print an error
	if(blocked)
	{
		outputFile << "There is no navigable path from (" << robot_start.x << "," << robot_start.y
				   << ") to (" << robot_end.x << "," << robot_end.y << ")" << endl;
	}

	// If the path is done (found route from robot_start to robot_end), print the path
	if(done)
	{
		stringstream path_string;
		m_star_map.getPathString(path_string);
		outputFile << path_string.str();
	}

	// Close the output file
	outputFile.close();

	// Finish and exit
	if(blocked)
		return 1;
	else
		return 0;
}
