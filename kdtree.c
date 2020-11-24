/* The following code is written by Yunzhi Li (880371) for Assignment 1 of
 * Algorithms and Data Structures (COMP20003).
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <math.h>
#include "data.h"
#include "list.h"
#include "kdtree.h"

// whether a node is comparing the x or y coordinates
#define COMPARE_X 0
#define COMPARE_Y 1
#define COORD_UNASSIGNED -1

#define SMALL_DIFFERENCE 0.000000001
#define DIM 2

// ---------------------- static function declarations ----------------------
static treenode_t *find_closest_recursive(treenode_t *search_node, double x, 
										  double y, treenode_t *closest_node, 
										  double *min_dist, int *comparison_count);
static int perp_distance_in_radius(treenode_t *check_node, double x, double y,
								double radius);
static double find_distance(double x1, double y1, double x2, double y2);
static void find_in_radius_recursive(treenode_t *node, double x, double y, double radius, 
									 FILE *output_file, int *comparison_count, int *found);
// --------------------------------------------------------------------------


treenode_t *make_new_node(){
	// make a new node and initialise
	treenode_t *new_node = malloc(sizeof(treenode_t));
	assert(new_node);
	new_node->reference_coord = COORD_UNASSIGNED;
	new_node->data = make_empty_list();
	new_node->left = NULL;
	new_node->right = NULL;
	return new_node;
}


treenode_t *insert_to_tree(treenode_t *node, record_t *one_record, int coord_count){
	// insert a record_t data into the tree, 
	// coord_count is even if comparing x, odd if comparing y
	if (node == NULL) {
		// empty node
		node = make_new_node();
		node->reference_coord = coord_count;
		node->data = insert_at_foot(node->data, one_record);
	} else if ((fabs(node->data->head->data->x_coord - one_record->x_coord) < SMALL_DIFFERENCE) && 
			  (fabs(node->data->head->data->y_coord - one_record->y_coord) < SMALL_DIFFERENCE)) {
		// same location
		node->data = insert_at_foot(node->data, one_record);
		
	} else if ((coord_count%DIM == COMPARE_X && 
				one_record->x_coord < node->data->head->data->x_coord) || 
			   (coord_count%DIM == COMPARE_Y && 
			   one_record->y_coord < node->data->head->data->y_coord)) {
		// go to left
		node->left = insert_to_tree(node->left, one_record, coord_count+1);
		
	} else {
		// go to right
		node->right = insert_to_tree(node->right, one_record, coord_count+1);
	}
	
	return node;
}




treenode_t *find_closest(treenode_t *search_node, double x, double y) {
	// finds the closest node to x,y and return it
	assert(search_node != NULL);
	int comparison_count = 0;
	double min_dist = find_distance(search_node->data->head->data->x_coord, 
									 search_node->data->head->data->y_coord, 
									 x, y);
	
	treenode_t *closest = find_closest_recursive(search_node, x, y, search_node, 
												&min_dist, &comparison_count);
	
	printf("%f %f --> %d\n", x, y, comparison_count);
	return closest;
}


static treenode_t *find_closest_recursive(treenode_t *search_node, double x, 
										  double y, treenode_t *closest_node, 
										  double *min_dist, int *comparison_count) {
	// called by find_closest, finds the closest node to x,y recursevely and return it
	
	if (search_node == NULL) {
		return closest_node;
	} else if ((fabs(search_node->data->head->data->x_coord - x) < SMALL_DIFFERENCE) && 
			  (fabs(search_node->data->head->data->y_coord - y) < SMALL_DIFFERENCE)) {
		// exact same location
		return search_node;
	}
	
	*comparison_count+=1;
	
	int searched_left; // 1 when searched left, 0 when searched right
	double curr_dist = find_distance(search_node->data->head->data->x_coord, 
									 search_node->data->head->data->y_coord, 
									 x, y);
	
	// check if curr_dist is closer than min_dist, update accordingly
	if (curr_dist < *min_dist) {
		*min_dist = curr_dist;
		closest_node = search_node;
	}
	
	//decides if tree goes to left or right
	if ((search_node->reference_coord%DIM == COMPARE_X && 
		 x < search_node->data->head->data->x_coord) || 
		(search_node->reference_coord%DIM == COMPARE_Y && 
		 y < search_node->data->head->data->y_coord)) {
		// goes to left
		searched_left = 1;
		closest_node = find_closest_recursive(search_node->left, x, y, closest_node, 
											  min_dist, comparison_count);
	} else {
		// goes to right
		searched_left = 0;
		closest_node =  find_closest_recursive(search_node->right, x, y, closest_node, 
											   min_dist, comparison_count);
	}
	
	// check the other side of the tree if there is an even closer node
	if (perp_distance_in_radius(search_node, x, y, *min_dist)) {
		if (searched_left) {
			closest_node =  find_closest_recursive(search_node->right, x, y, closest_node, 
												   min_dist, comparison_count);
		} else {
			closest_node =  find_closest_recursive(search_node->left, x, y, closest_node, 
												   min_dist, comparison_count);
		}
	}
	
	return closest_node;
}

static int perp_distance_in_radius(treenode_t *check_node, double x, double y,
								double radius) {
	// check if a certain node is within the radius from x,y
	double dist; 
	assert(check_node->reference_coord != COORD_UNASSIGNED);
	
	if (check_node->reference_coord%DIM == COMPARE_X) {
		dist = fabs(x - check_node->data->head->data->x_coord);
	} else {
		dist = fabs(y - check_node->data->head->data->y_coord);
	}
	
	if (radius >= dist) {
		return 1;
	} 
	
	return 0;
	
}

static double find_distance(double x1, double y1, double x2, double y2) {
	return sqrt(pow((x1-x2), 2) + pow((y1-y2), 2));
}

void find_in_radius(treenode_t *root, double x, double y, double radius, FILE *output_file) {
	// finds all nodes within the given radius of the given x,y, and 
	// write to file
	int comparison_count = 0, found = 0;
	find_in_radius_recursive(root, x, y, radius, output_file, &comparison_count, &found);
	
	if (!found) {
		fprintf(output_file, "%0.6f %0.6f %0.4f --> NOT FOUND\n", x, y, radius);
	}
	
	printf("%0.6f %0.6f %0.4f --> %d\n", x, y, radius, comparison_count);
}

static void find_in_radius_recursive(treenode_t *node, double x, double y, double radius, 
									 FILE *output_file, int *comparison_count, int *found) {
	// called by find_in_radius, 
	if (node == NULL) {
		return;
	}
	
	*comparison_count += 1;
	
	// node within radius
	if (find_distance(node->data->head->data->x_coord, 
					  node->data->head->data->y_coord, x, y) <= radius) {
		print_list_by_radius(node->data, x, y, radius, output_file);
		*found = 1;
	}
	
	int searched_left; // 1 when searched left, 0 when searched right
	
	//decides if tree goes to left or right
	if ((node->reference_coord%DIM == COMPARE_X && 
		 x < node->data->head->data->x_coord) || 
		(node->reference_coord%DIM == COMPARE_Y && 
		 y < node->data->head->data->y_coord)) {
		// goes to left
		searched_left = 1;
		find_in_radius_recursive(node->left, x, y, radius, output_file, 
								 comparison_count, found);
	} else {
		// goes to right
		searched_left = 0;
		find_in_radius_recursive(node->right, x, y, radius, output_file, 
								 comparison_count, found);
	}
	
	if (perp_distance_in_radius(node, x, y, radius)) {
		if (searched_left) {
			find_in_radius_recursive(node->right, x, y, radius, output_file, 
									 comparison_count, found);
		} else {
			find_in_radius_recursive(node->left, x, y, radius, output_file, 
									 comparison_count, found);
		}
	}
}

void free_tree(treenode_t *node){
	// frees tree
	if (node == NULL) {
		return;
	}
	free_tree(node->left);
	free_tree(node->right);
	free_list(node->data);
	free(node);
	node = NULL;
}