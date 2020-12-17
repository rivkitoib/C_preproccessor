




























typedef struct {
    int id;
    char *name;
} employee;




void print_employee(employee);
int emloyee_id(employee);
char * employee_name(employee);
void set_employee_id(employee, int);
void set_employee_name(employee, char *);


















 

int mainnnnn(int argc, const char * argv[]) {
  
    int num =0, sum = 0;
    int mult = 20;
    
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
    
    if (num <      100)
    {
        sum = mult * num / 2;
    }
    
    printf("The value of sum is=%d", sum);


    fclose(fptr);
    int MAXT;
    char * name;
    
    printf("The preprocessor should /* not see this as a comment*/\n");
    printf("%s","This is a very long replacement text\
please notice that it continues into a second line!");

    
    employee all_employees[     100];
    for (int i=0; i < 100; i++)
    {
        printf("Please enter an employees' ID.(do not exceed      100 ID)\n");
        scanf("%d",&MAXT);
        name = (char *)malloc(sizeof(char)*     100);
        printf("Please enter an employees' name.\n");
        scanf("%s",name);
        
        set_employee_id(all_employees[i], MAXT);
        set_employee_name(all_employees[i], name);
    }
    
    
    int MAX_T[     100];
    
    for (int i =0; i<     100; i++)
    {
        MAX_T[i]=i;
    }
    return 0;
}




