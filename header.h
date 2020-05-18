//#include "stringsort.h"

struct tree_branch
{
    void* data;
    char type;
    tree_branch** next;
    int kids;
};

enum branch_types
{
    TREE_HEAD,
    TREE_FUNCTION_DEFINE,
    TREE_FUNCTION,

    TREE_MAIN,
    TREE_OPERATOR,

    TREE_INTEGER,
    TREE_VAR,
    TREE_ARG,

    TREE_COMMANDS,
    TREE_VAR_DEFINE,
    TREE_STRING
};

struct func_def
{
    char* name;
    char vars[100];
    int var_count;
};


struct code_function
{
    char* name;
    int args;
};


int got_variable();
int got_function();
int print_condition(char* condition, FILE* assm);

tree_branch* new_branch_construct();
tree_branch* make_branch(char* data, int type);
void add_kid(tree_branch* branch);
tree_branch** tokenizate_code(poem code);

tree_branch* code_get_G(tree_branch** code);
tree_branch* code_get_E();
tree_branch* code_get_T();
tree_branch* code_get_C();
tree_branch* code_get_P();
tree_branch* code_get_N();

void add_kid(tree_branch* branch);
tree_branch* new_branch_construct();

void tree_print(tree_branch* head);
void recursive_tree_print (tree_branch* branch, FILE* fout);



int get_var_index(char var);
int recursive_asm(tree_branch* branch, FILE* assm);
int tree_to_asm(tree_branch* branch);

tree_branch** tokenizate_code(poem code);
tree_branch* code_get_G(tree_branch** tokenizated_code);
tree_branch* code_get_E();
tree_branch* code_get_T();

int got_function();
int got_variable();



tree_branch* make_branch(char* data, int type);

tree_branch* return_maker();
tree_branch* func_maker(int number);
tree_branch* print_maker();
tree_branch* scan_maker();
tree_branch* if_maker();
tree_branch* while_maker();
tree_branch* variable_maker();
tree_branch* derr_maker();

bool is_my_function(tree_branch* branch);


tree_branch* error_on_line(tree_branch* branch, poem* onegin);

int work_with_code();
bool got_number();
int return_to_assm(tree_branch* branch, FILE* assm);
int while_to_assm(tree_branch* branch, FILE* assm);
int if_to_assm(tree_branch* branch, FILE* assm);
int scan_to_assm(tree_branch* branch, FILE* assm);
int print_to_assm(tree_branch* branch, FILE* assm);
int assign_to_assm(tree_branch* branch, FILE* assm);
int variable_define_to_assm(tree_branch* branch, FILE* assm);
int math_to_assm(tree_branch* branch, FILE* assm);
int func_call_to_assm(tree_branch* branch, FILE* assm);
int main_to_assm(tree_branch* branch, FILE* assm);
int func_define_to_assm(tree_branch* branch, FILE* assm);
int pow_to_assm(tree_branch* branch, FILE* assm);

#include "differentiator.h"
