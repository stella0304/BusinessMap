#ifndef DATA_H
	#define DATA_H


typedef struct {
	int census_year;
	int block_ID;
	int property_ID;
	int base_property_ID;
	char *clue_small_area;
	char *trading_name;
	int industry_code;
	char *industry_description;
	double x_coord;
	double y_coord;
	char* location;
} record_t;

record_t *line_to_struct(char *one_line);
void free_record(record_t *one_record);
void print_record(record_t *one_record, FILE *file);


#endif