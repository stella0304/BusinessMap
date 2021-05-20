# business_map

Written for assignment 2 of COMP20003. For a collection of businesses and their locations, map1 searches for the closest business for a given location and map2 searches for all the businesses within a certain radius of a given location.

The code reads in a csv file of businesses and their locations (latitude and longitude) and store them in a 2D Tree. For each location in the tree, the node stores a linked list of all the businesses at that location. The algorithm uses post order traversal to find the solutions.
