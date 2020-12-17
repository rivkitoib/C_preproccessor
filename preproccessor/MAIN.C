//
//  main.c
//  Preprocessor
//
//  Created by Michal Asherman on 19/08/2018.
//  Copyright © 2018 Michal Asherman. All rights reserved.
//

/*
 * Please read the comments in this file before implementing assignment21.
 */

/* remark: system includes are enclosed with < > (example: #include <stdio.h>, while local includes are enclosed with " ", #include "key.h". Includes that use absolute file names, (the full path definition of the file) make your program less portable. For assignment 21 write absolute file names, and don't differciate between,  #include <filename.h> and #include "filename.h". */

//remark:
//The C programming language does not allow nested comments!


/*  It shouldn't matter " that this part of the remark is in quotes" *    */

#include <My_system_header.h>
#include "My_local_header.h"
/* #include <stdio.h>
#include <ctype.h>
#include <stdlib.h>    */ 

int mainnnnn(int argc, const char * argv[]) {
  
    int num =0, sum = 0;
    int mult = MULT;
    
    FILE * fptr;
    fptr=fopen(argv[1],"r");
    
    if(fptr == NULL)
    {
        printf("Error!");
        exit(1);
    }
    
    if (fscanf(fptr,"%d", &num) != 1)
    {
        printf("Error!");
        exit(1);
    }
    
    if (num < MAX)
    {
        sum = mult * num / DIV;
    }
    
    printf("The value of sum is=%d", sum);


    fclose(fptr);
    int MAXT;
    char * name;
    
    printf("The preprocessor should /* not see this as a comment*/\n");
    printf("%s",D_LONGDEFINE);

    
    employee all_employees[MAX];
    for (int i=0; i < 100; i++)
    {
        printf("Please enter an employees' ID.(do not exceed MAX ID)\n");
        scanf("%d",&MAXT);
        name = (char *)malloc(sizeof(char)*MAX);
        printf("Please enter an employees' name.\n");
        scanf("%s",name);
        
        set_employee_id(all_employees[i], MAXT);
        set_employee_name(all_employees[i], name);
    }
    
    
    int MAX_T[MAX];
    
    for (int i =0; i<MAX; i++)
    {
        MAX_T[i]=i;
    }
    return 0;
}




