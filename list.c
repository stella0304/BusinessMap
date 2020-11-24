/* The following code is written by Yunzhi Li (880371) for Assignment 1&2 of
 * Algorithms and Data Structures (COMP20003)with reference to P170-176 of 
 * Programming, Problem Solving and Abstraction with C by Alistair Moffat.
 */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "data.h"
#include "list.h"

list_t *make_empty_list() {
	// make a new empty list
	list_t *list = malloc(sizeof(*list));
	assert(list != NULL);
	list->head = NULL;
	list->foot = NULL;
	return list;
}


void free_list(list_t *list){
	// frees linked list
	assert(list!=NULL);
	listnode_t *curr, *prev;
	curr = list->head; 
	while(curr) {
		prev = curr;
		curr = curr->next;
		free_record(prev->data); //from data.h
		free(prev);
		prev = NULL;
	}
	free(list);
	list = NULL;
}


list_t *insert_at_foot(list_t *list, data_t value) {
	// insert a record into the linked list
	listnode_t *new;
	new = malloc(sizeof(*new));
	assert(list!=NULL && new!= NULL);
	new->data = value;
	new->next = NULL;
	if (list->foot==NULL) { // first insertion into list
		list->head = new;
		list->foot = new;
	} else {
		list->foot->next = new;
		list->foot = new;
	}
	return list;
}

void print_list_by_loc(list_t *list, double x, double y, FILE *output_file) {
	// prints info about every record in the linked list with respect to x,y
	
	listnode_t *curr = list->head;
	
	while (curr) {
		fprintf(output_file, "%0.6f %0.6f --> ", x, y);
		print_record(curr->data, output_file);
		curr = curr->next;
		}
}

void print_list_by_radius(list_t *list, double x, double y, 
						  double radius, FILE *output_file) {
	// prints info about every record in the linked list with respect to x,y,radius
	
	listnode_t *curr = list->head;
	
	while (curr) {
		fprintf(output_file, "%0.6f %0.6f %0.4f --> ", x, y, radius);
		print_record(curr->data, output_file);
		curr = curr->next;
		}
}
