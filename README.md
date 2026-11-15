# C Travelling Salesman Visualizer 

This is my first project in C and I am happy to present a visualizer for the famous NP hard problem; **The Travelling Salesman**. 

Made using the raylib API, it generates a random point map then using my terrbile permutation algorithm, itterates through every possible path along the route to form a complete Hamiltonian path. The best path is saved and after all permutations the most optimal path is guaranteed to be found and displayed. This is the brute force method for finding the most optimal path, not nearest neighbour which is faster but on avarage gives a path 25% longer then the most optimal. 

This program can currently execute on a maximum of 10 points because of the large memory consumption of the algorithm, but maybe i will revisit this some time and improve it. 

## About TSM

The travelling salesman is one of the most well known examples of a NP hard problem meaning the compute time doesn't scale in a normal polynomial timescale, instead it scales on the basis of `O(N!)`, one of the worst possible time complexities. The problem involes a plane of randomly generated points (representing cities on a map), the goal is to find the shortest path you can take to go round every point and making it back to where you started. 

## Permutation Algorithm

For this, I made a custom tree based algorithm that populates a tree with a root node of the first number in the permutation sequence. `N` total trees are generated where `N` is the number of points. To get a permutation sequence, the tree is traversed in a depth first order for where the edge path is the sequence. This is my best attempt at making a permutation algorithm and it's not that good due to the large memory required by the tree. Making it only have a `N < 11` limit. This is also an unoptimised version of a permutation algorithm meaning that while each permutation is distint, there will be many that yield the same result, such as `[0,1,2,3]` and `[2,3,0,1]`, both creating the same path and the same distance. This means that many checks are redundant but I plan on improving this and removing redundancy.

## Program Info

I highly recommend cloning this repo if you are to use it because then you can change some boolean settings in the source code to get differnt behaviour. 
`const unsigned int numberOfPoints` - Controls how many points are generated on the map.
`const bool showOnlyBest` - If true, only the current best route is shown on screen, otherwise if false, every permutation is drawn.
`const bool hyperOptimizationMode` - If true, this will remove all UI draw calls, making each render pass quicker. 
The macros at the top are pretty self explanitory so feel free to play around with thoes :) 

Since I am not good at programming and don't know anything about C, the code quality is poor and I am aware of at least 1 current memory leak :> But we can ignore that for now :) Any tips on how to improve my C will be greatly appriciated <3

---

First build compiled on GCC Version 16.2.1.
API: Raylib Version 6.3-1

---

## External Sources and More Info: 

[Wikipedia Page On TSMP](https://en.wikipedia.org/wiki/Travelling_salesman_problem)

