/* The following code is written by Yunzhi Li (880371) for Assignment 1 of
 * Algorithms and Data Structures (COMP20003)with reference to P170-176 of 
 * Programming, Problem Solving and Abstraction with C by Alistair Moffat.
 */

#ifndef LIST_H
	#define LIST_H

#include "data.h"

typedef record_t *data_t;
// change data_t to suit other types of data

typedef struct listnode listnode_t;

struct listnode {
	data_t data;
	listnode_t *next;
};

typedef struct {
	listnode_t *head;
	listnode_t *foot;
} list_t;

list_t *make_empty_list();
void free_list(list_t *list);
list_t *insert_at_foot(list_t *list, data_t value);
void print_list_by_loc(list_t *list, double x, double y, FILE *output_file);
void print_list_by_radius(list_t *list, double x, double y, 
						  double radius, FILE *output_file);

#endif