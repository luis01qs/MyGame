Video of the game while playing: https://youtu.be/q-J1iUcRaB8.

This is a self-played game. It uses genetic algorithms to be solved. It has no name as it has no player.

It consists on a room with five different obstacles. And there are several posible obstacles:
-  A vertical static wall at the center (in the Y-coordinate) of the room.
-  A pair of two vertical static walls at the bottom and the top of the room.
-  A horizontal wall in the middle (in the Y-coordinate) of the room.
-  A pair of two rotating walls, one on top of each other.
-  A single vertical wall moving upwards and downwards.

The idea of the obstacles are to make it harder to find a path from the beginning of the room to the exit wall (the right vertical green wall). Depending on the difficulty of the map, the amount of non-static obstacles is going to increase. Being 0, 1, and 2 the different amount of non-static obstacles for the easy, medium and hard levels respectively. Each time the game is restarted (creating the first generation), the room with obstacles is going to perform a permutation of the position of the different kind of possible obstacles, depending on the level. This is to ensure that the actual algorithm does not reward an specific solution. 

The main characters in the game are a set of 100 particles per generation. Each particle is represented with a triangular form. The movement of the particles has a constant speed, and the spike is always pointing to the direction of the movement. Since the speed is constant, the only thing variable in each frame is the velocity vector, which point in a certain angule and has the same speed. The movement is modeled by a vector of changes in the angle of the path, namely deltaAngle. It starts with a angle of 0 degrees (pointing to the right). Then, every 10 frames, the angle increases or decreases, depending on the current position of the vector deltaAngle. 

While the first generation is created completely randomly (each position of the deltaAngle vector is created using a normal distribution with mean equal to 0 and standard deviation of 6 degrees, then multyplying the result by 3) for each particle, the next generation will be created mostly from the previous one. The process of creating the next generation goes as follows. First, it orders in increasing order the particles by the minimum euclidian distance it had at any given moment to the exit wall. If some particle found the exit wall, instead of the distance it uses the time it took to get there. The formula for that case would be distance = (amount_of_frames_needed_to_find_the_exit - total_amount_of_frames_for_each_generation). Obviously, the frames it took to find the exit would be less or equal to the amount of frames it run that generation. This means that if a particle gets to the exit wall, the distance for that particular solution would be negativa, and so, better in the eyes of our algorithm. 

After ordering the solutions, the idea was to have a proportion of 20%, 50%, and 30% of ways to create new particles. The first kind, which was of 20%, are made from the best 20% of the previous solutions without any modifications. The reason behin it was to truly reward the best solutions, and try to work from them. For the next 50%, the were created by the best 50% of the previous generation. This process was by randomly splitting two pairwaise solutions (1 and 2, 3 and 4, 5 and 6, 7 and 8, ...) and combining them. Formally, if the deltaAngle for solution A is (a_1, a_2,..., a_n) and for B is (b_1, b_2,...,b_n)
