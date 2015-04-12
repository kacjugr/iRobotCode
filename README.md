# iRobotCode
iRobot Code Challenge using A*

Build Instructions:
- Include JsonCpp library to build
  - Instructions: http://garajeando.blogspot.com/2011/06/how-to-install-jsoncpp-in-ubuntu-and.html
  - NOTE: current build number is 4.6 (not 4.4.5 as stated in instructions)

Run Instructions:
- Program prompts for obstacle set #(1-6). Enter only #, not full file name
- Program notifies of successful map load, and prompts for <Enter> to start A*
- Program notifies A* processing iteration for every 10,000 passes (to avoid appearing stalled)
- Program writes results of pathfinding to 'set#_out.txt', where '#' denotes obstacle data set #

A* Algorithm Notes:
- Basic A* algorithm demonstration: https://www.youtube.com/watch?v=KNXfSOx4eEE
- Closed list search was eliminated because it's technical reasons involving speed of processing:
  - I was unable to use hash_map or unordered_map for the Closed list
    - Compiler throws 'deprecated' error for 'hash_map'
    - Compiler throws 'experimental' error for 'unordered_map'
    - Allowing these requires adding flags to makefile > $CXX_FLAGS, but that variable doesn't exist in current makefile
  - Since I couldn't use a hash-map type variable for the Closed list, searching that vector took an extreme amount of time
    - Vector search of Closed list is linear, increasing with each tile position added
    - Recomputation of (G_cur + G_new) < G_old is always constant
  - Concluded that recomputation of G was always better than Closed list search: eliminated Closed list
  - Side note: even if hash lookup of position tile was possible, the hash algorithm is still probably slower than G compute

I enjoyed working on this problem immensely. If this is the type of work done at iRobot, I look forward to moving forward in the interview process.

Thanks,
Jon
