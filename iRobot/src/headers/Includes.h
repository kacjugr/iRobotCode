/**
 * Includes.hpp
 *  Created on: Apr 7, 2015
 *      Author: Jonathan Lessner
 *     Purpose: Loads libraries and defines global data types
 */

#ifndef INCLUDES_H
#define INCLUDES_H

// Standard modules
#include <stdlib.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <cstring>

// For organizing Open and Closed lists
#include <vector>
#include <deque>

// For using 'abs()', 'find()', and 'sort()'
#include <cmath>
#include <climits>
#include <algorithm>

// gcc complains that hash_map is deprecated
// It ALSO complains that it's replacement 'unordered_map' is experimental
// Because of this, I have to use vector for my Closed list
//  and vector.find() on that list, which I'm sure is slower than unordered_map[key].
//  But it appears that's what I'm stuck with. All the sources online reference config
//  files that no longer exist in Ubuntu Eclipse.
//#include <hash_map>
//#include <unordered_map>

// This is the JSON header, included from JsonCpp
// http://garajeando.blogspot.com/2011/06/how-to-install-jsoncpp-in-ubuntu-and.html
#include "jsoncpp/json.h"

// Definition for a tile location
struct PosTile
{
	unsigned int x, y;
};

// Change DEBUG to '0' for production build
// Change DEBUG to '1' for testing algorithm on small map with process prints
#define DEBUG 0

#endif



