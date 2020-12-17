//
//  My_system_header.h
//  Preprocessor
//
//  Created by Michal Asherman on 19/08/2018.
//  Copyright © 2018 Michal Asherman. All rights reserved.
//

#define  D_EMPTYDEFINE
#define D_LONGDEFINE "This is a very long replacement text\
please notice that it continues into a second line!"

typedef struct {
    int id;
    char *name;
} employee;

#define NUMBER_OF_EMPLOYEES 100

/* prototypes */

void print_employee(employee);
int emloyee_id(employee);
char * employee_name(employee);
void set_employee_id(employee, int);
void set_employee_name(employee, char *);



