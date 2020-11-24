/* Following code written by Yunzhi Li (880371) for COMP20003 at the University 
 * of Melbourne. Read a file with Melbourne Business records and store it in a
 * 2D tree. Then search through the tree for the closest business(es) to a 
 * point from stdin. Writes the result to a file.
 *
 * Algorithms are fun!!
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"
#include "list.h"
#include "kdtree.h"

#define MAX_LINE_LEN 512
#define COMPARE_X 0

treenode_t *read_csv(char *filename, treenode_t *storage_tree);
void search_and_print(char *filename, treenode_t *storage_tree);

int main(int argc, char *argv[]) {
	// initialise tree
	treenode_t *storage_tree = NULL;
	
	// read data into tree
	storage_tree = read_csv(argv[1], storage_tree);
	
	// search tree & print
	search_and_print(argv[2], storage_tree);
	
	// free everything
	free_tree(storage_tree);
}

treenode_t *read_csv(char *filename, treenode_t *storage_tree) {
	// reads csv data from the file and store it in a 2d tree
	
	FILE *input_file = fopen(filename, "r");
	assert(input_file!=NULL);
	
	//read file line by line and store it in storage_tree
	char one_line[MAX_LINE_LEN+2];
	fgets(one_line, MAX_LINE_LEN+2, input_file); // read headder line
	while(fgets(one_line, MAX_LINE_LEN+2, input_file)!=NULL && 
		  strcmp(one_line, "")!=0) {
		record_t *one_record = line_to_struct(one_line);
		storage_tree = insert_to_tree(storage_tree, one_record, COMPARE_X);
	}
	fclose(input_file);
	
	return storage_tree;
}

void search_and_print(char *filename, treenode_t *storage_tree) {
	// find stored info in storage_tree for each search key
	
	FILE *output_file = fopen(filename, "w");
	assert(output_file!=NULL);
	
	double search_x, search_y;
	
	while (scanf("%lf %lf\n", &search_x, &search_y)==2) {
		// assuming that each new key to search for is on a new line
		
		// find the node that is closest to x and y
		treenode_t *closest_node = find_closest(storage_tree, search_x, search_y);
		
		// traverse through the linked list and print out things
		print_list_by_loc(closest_node->data, search_x, search_y, output_file);
	}
	fclose(output_file);
}

