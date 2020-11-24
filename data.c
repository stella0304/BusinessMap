/* The following code is written by Yunzhi Li (880371) for Assignment 2 of
 * Algorithms and Data Structures (COMP20003).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "data.h"

#define MAX_STR_LEN 128

// ---------------------- static function declarations ----------------------
static int read_int(char* to_read, int *len_read);
static double read_double(char* to_read, int *len_read);
static void read_str(char* to_read, char *buff_str, int *len_read, int *str_len);
// --------------------------------------------------------------------------

record_t *line_to_struct(char *one_line) {
	// read one line of the csv file and store it in the input_t struct
	record_t *one_entry = malloc(sizeof(record_t));
	assert(one_entry != NULL);
	
	int len_read=0, str_len=0, int_read; //str_lenn includes the '\0'
	double double_read;
	char str_read[MAX_STR_LEN+2];
	
	//read in census_year
	int_read = read_int(one_line, &len_read);
	one_entry->census_year = int_read;
	one_line += len_read; // moves along the string to read the next column
	
	//read in block_ID
	int_read = read_int(one_line, &len_read);
	one_entry->block_ID = int_read;
	one_line += len_read;
	
	// read in property_ID
	int_read = read_int(one_line, &len_read);
	one_entry->property_ID = int_read;
	one_line += len_read;
	
	// read in base_property_ID
	int_read = read_int(one_line, &len_read);
	one_entry->base_property_ID = int_read;
	one_line += len_read;
	
	// read in clue_small_area
	read_str(one_line, str_read, &len_read, &str_len);
	one_entry->clue_small_area = malloc(str_len*sizeof(char));
	strcpy(one_entry->clue_small_area, str_read);
	one_line += len_read;
	
	// read in trading_name
	read_str(one_line, str_read, &len_read, &str_len);
	one_entry->trading_name = malloc(str_len*sizeof(char));
	strcpy(one_entry->trading_name, str_read);
	one_line += len_read;
	
	// read in industry_code
	int_read = read_int(one_line, &len_read);
	one_entry->industry_code = int_read;
	one_line += len_read;
	
	// read in industry_description
	read_str(one_line, str_read, &len_read, &str_len);
	one_entry->industry_description = malloc(str_len*sizeof(char));
	strcpy(one_entry->industry_description, str_read);
	one_line += len_read;
	
	// read in x_coord
	double_read = read_double(one_line, &len_read);
	one_entry->x_coord = double_read;
	one_line += len_read;
	
	// read in y_coord
	double_read = read_double(one_line, &len_read);
	one_entry->y_coord = double_read;
	one_line += len_read;
	
	// read in location
	read_str(one_line, str_read, &len_read, &str_len);
	one_entry->location = malloc(str_len*sizeof(char));
	strcpy(one_entry->location, str_read);
	
	return one_entry;
}

static int read_int(char* to_read, int *len_read) {
	/* converts the next integer from one line of csv (to_read) into an integer */
	int j=0;
	char int_str[MAX_STR_LEN+2];
	
	while (to_read[j] != ',' && to_read[j] != '\n' && to_read[j] != EOF) {
		int_str[j] = to_read[j];
		j++;
	}
	int_str[j] = '\0';
	
	*len_read = j+1;
	return atoi(int_str);
}

static double read_double(char* to_read, int *len_read) {
	/* converts the next double from one line of csv (to_read) into a double */
	int j=0;
	char double_str[MAX_STR_LEN+2];
	
	while (to_read[j] != ',' && to_read[j] != '\n' && to_read[j] != EOF) {
		double_str[j] = to_read[j];
		j++;
	}
	double_str[j] = '\0';
	
	*len_read = j+1;
	return atof(double_str);
}

static void read_str(char* to_read, char *buff_str, int *len_read, int *str_len) {
	/* converts the next string from one line of csv (to_read) into an independent
	   string */
	int quoted=0, end_of_str=0, j=0, k=0;
	// j traverses to_read and k traverses buff_str
	
	if (to_read[j] == '\"') {
		// string enclosed by quotation marks
		quoted = 1;
		j++;
	}
	
	while (end_of_str == 0 && to_read[j] != '\n' && to_read[j] != EOF) {
		if (quoted==0 && to_read[j]==',') {
			end_of_str = 1;
		
		} else if (quoted==1 && to_read[j] == '\"' && 
				   (to_read[j+1]==',' || to_read[j+1]=='\n' || to_read[j+1]=='\0'))  {
			end_of_str = 1;
			j++;
		
		} else if (quoted==1 && to_read[j]=='\"' && to_read[j+1]=='\"') {
			buff_str[k] = to_read[j];
			k++;
			j+=2;
		
		} else {
			buff_str[k] = to_read[j];
			j++;
			k++;
		}
	}
	
	buff_str[k] = '\0';
	*len_read = j+1;
	*str_len = k+1;
}

void free_record(record_t *one_record) {
	// free one struct 
	assert(one_record!=NULL);
	
	free(one_record->clue_small_area);
	one_record->clue_small_area = NULL;
	free(one_record->trading_name);
	one_record->trading_name = NULL;
	free(one_record->industry_description);
	one_record->industry_description = NULL;
	free(one_record->location);
	one_record->location = NULL;
	
	free(one_record);
	one_record = NULL;
}

void print_record(record_t *one_record, FILE *file) {
	// prints info about one_record into file
	assert(file!=NULL);
	fprintf(file, "Census year: %d || Block ID: %d || Property ID: %d || "
			"Base property ID: %d || CLUE small area: %s || Trading Name: %s || "
			"Industry (ANZSIC4) code: %d || Industry (ANZSIC4) description: %s || "
			"x coordinate: %0.4f || y coordinate: %0.4f || Location: %s || \n", 
			one_record->census_year, one_record->block_ID, one_record->property_ID, 
			one_record->base_property_ID, one_record->clue_small_area, 
			one_record->trading_name, one_record->industry_code, 
			one_record->industry_description, one_record->x_coord, one_record->y_coord, 
			one_record->location);
}

