#ifndef KDTREE_H
	#define KDTREE_H

#include "data.h"
#include "list.h"


typedef struct treenode treenode_t;

struct treenode {
	int reference_coord;
	list_t *data;
	treenode_t *left;
	treenode_t *right;
};

treenode_t *make_new_node();
treenode_t *insert_to_tree(treenode_t *node, record_t *one_record, int coord_count);
treenode_t *find_closest(treenode_t *search_node, double x, double y);
void find_in_radius(treenode_t *root, double x, double y, double radius, FILE *output_file);
void free_tree(treenode_t *node);


#endif