#include "musthave_include.h"
#include "header.h"

#define TOKEN_CNTR_INC \
    token_cntr++;          \
    if (token_cntr >= max_token_len) \
    {                                   \
        max_token_len *= 2;          \
        tokenizated_code = (tree_branch**) realloc(tokenizated_code, sizeof(tree_branch*)* max_token_len);   \
    }


char* VARIABLES;
int VARIABLE_COUNT = 0;

code_function CODE_FUNCTIONS[100] = {};
int CODE_FUNCTION_COUNT = 0;


tree_branch** GLOBAL_INDEX = 0;

int MAX_NAME_SIZE = 50;

tree_branch** tokenizate_code(poem code)
{
    int max_token_len = code.string_count;
    tree_branch** tokenizated_code = (tree_branch**) calloc(sizeof(tree_branch*), max_token_len + 1);
    int token_cntr = 0;
    int got_chars = 1;
    char* str = 0;
    for (int i = 0; i < code.string_count; ++i)
    {
        str = strtok(code.index[i].start, "\t ");
        bool flag = false;
        int cntr = 0;
        bool is_str = false;
        while (str)
        {
            if (flag == true && strcmp(str, "(") && strcmp(str, ")"))
            {
                cntr++;
            }
            if (str[strlen(str) - 1] == ':' && strcmp(str, "do:"))
            {
                CODE_FUNCTIONS[CODE_FUNCTION_COUNT].name = (char*) calloc(sizeof(char), strlen(str));
                strcpy(CODE_FUNCTIONS[CODE_FUNCTION_COUNT].name, str);
                CODE_FUNCTIONS[CODE_FUNCTION_COUNT].name[strlen(str) - 1] = 0;
                flag = true;
            }
            tokenizated_code[token_cntr] = new_branch_construct();
            tokenizated_code[token_cntr]->data = str;
            if (is_str)
            {
                tokenizated_code[token_cntr]->type = TREE_STRING;
                is_str = false;
            }
            TOKEN_CNTR_INC;

            if (!strcmp(str, "print"))
            {
                if(*(str + 6) == '\"')
                {
                    str = strtok(NULL, "\"");
                    is_str = true;
                }
                else
                    str = strtok(NULL, " \t\n\0");
            }
            else
                str = strtok(NULL, " \t\n\0");
        }

        if (flag == true)
        {
            CODE_FUNCTIONS[CODE_FUNCTION_COUNT].args = cntr;
            CODE_FUNCTION_COUNT++;
            flag = false;
            cntr = 0;
        }
        got_chars = 1;
    }
    tokenizated_code = (tree_branch**) realloc(tokenizated_code, (token_cntr + 1) * sizeof(tree_branch));
    tokenizated_code[token_cntr] = 0;
    //free(str);
    return tokenizated_code;
}


tree_branch* code_get_G(tree_branch** tokenizated_code)
{
    GLOBAL_INDEX = tokenizated_code;
    tree_branch* value = make_branch("PROGRAM", TREE_HEAD);
    value->type = TREE_HEAD;
    while (*GLOBAL_INDEX != 0)
    {
        if (strlen((char*)(*GLOBAL_INDEX)->data) != 0)
        {
            add_kid(value);
            value->next[value->kids] = code_get_E();

            if (!value->next[value->kids])
                return value->next[value->kids];

            value->kids++;
        }
    }
    return value;
}

tree_branch* code_get_E()
{
    char* curr_func = (char*)(*GLOBAL_INDEX)->data;
    tree_branch* value = new_branch_construct();
    if (!strcmp(curr_func, "main"))
    {
        value->data = calloc(sizeof(func_def), 1);
        ((func_def*)value->data)->name = (char*)(*GLOBAL_INDEX)->data;
        VARIABLES = ((func_def*)(value->data))->vars;
        GLOBAL_INDEX++;
        value->type = TREE_MAIN;
    }
    else
    {
        value->type = TREE_FUNCTION_DEFINE;
        value->data = calloc(sizeof(func_def), 1);
        ((func_def*)(value->data))->name = curr_func;
        if(strcmp(curr_func, "do:"))
        {
            VARIABLES = ((func_def*)(value->data))->vars;
            VARIABLE_COUNT = 0;
        }
        if (curr_func[strlen(curr_func) - 1] == ':')
            curr_func[strlen(curr_func) - 1] = 0;

        char arg = 0;
        GLOBAL_INDEX++;

        bool need_sqobki = false;

        if (*(char*)(*GLOBAL_INDEX)->data == '(')
        {
            need_sqobki = true;
            GLOBAL_INDEX++;
        }

        while(*(char*)(*GLOBAL_INDEX)->data != '{' && *(char*)(*GLOBAL_INDEX)->data != ')')
        {
            add_kid(value);
            VARIABLES[VARIABLE_COUNT] = *(char*)(*GLOBAL_INDEX)->data;
            VARIABLE_COUNT++;
            value->next[value->kids] = make_branch((char*)(*GLOBAL_INDEX)->data, TREE_ARG);
            value->kids++;
            GLOBAL_INDEX++;
        }

        if (need_sqobki && *(char*)(*GLOBAL_INDEX)->data != ')')
            return error_on_line(*GLOBAL_INDEX, 0);
        else if (need_sqobki)
            GLOBAL_INDEX++;

    }
    if (*(char*)(*GLOBAL_INDEX)->data != '{')
    {
        return error_on_line(*GLOBAL_INDEX, 0);
    }

    GLOBAL_INDEX++;

    while (*(char*)(*GLOBAL_INDEX)->data != '}')
    {
        add_kid(value);
        value->next[value->kids] = code_get_T();

        if (!value->next[value->kids])
            return value->next[value->kids];

        value->kids++;
    }

    GLOBAL_INDEX++;

    if(strcmp(curr_func, "do"))
    {
        ((func_def*)(value->data))->var_count = VARIABLE_COUNT;
        VARIABLE_COUNT = 0;
    }
    return value;
}

tree_branch* make_branch(char* data, int type)
{
    tree_branch* value = new_branch_construct();
    value->data = data;
    value->type = type;
    return value;
}

tree_branch* code_get_T()
{

    tree_branch* value = *GLOBAL_INDEX;

    char* str = (char*)(*GLOBAL_INDEX)->data;
    int number = 0;

    if (*str == '(')
    {
        value = if_maker();
    }
    else if (!strcmp(str, "while"))
    {
        value = while_maker();
    }

    else if (!strcmp(str, "print"))
    {
        value = print_maker();
    }
    else if (!strcmp(str, "scan"))
    {
        value = scan_maker();
    }
    else if (!strcmp(str, "double"))
    {
        value = variable_maker();
    }
    else if (!strcmp(str, "return"))
    {
        value = return_maker();
    }
    else if ((number = got_function()) != -1)
    {
        value = func_maker(number);
    }
    else if ((number = got_variable()) != -1)
    {
        value = get_G(&GLOBAL_INDEX);
    }
    else if ((*GLOBAL_INDEX)->type == TREE_STRING)
    {
        value = *GLOBAL_INDEX;
        GLOBAL_INDEX++;
    }
    else if (!strcmp((char*)(*GLOBAL_INDEX)->data, "derr"))
    {
        value = derr_maker();
        printf("DERRRRR FINALY MADE\n");
    }
    else
    {
        return error_on_line(*GLOBAL_INDEX, 0);
    }
    return value;
}

tree_branch* derr_maker()
{
    GLOBAL_INDEX++;

    if (*(char*)(*GLOBAL_INDEX)->data != '(')
        return error_on_line(*GLOBAL_INDEX, 0);
    GLOBAL_INDEX++;

    tree_branch* func_to_derr = get_G(&GLOBAL_INDEX);

    if (strcmp((char*)(*GLOBAL_INDEX)->data, "by"))
        return error_on_line(*GLOBAL_INDEX, 0);
    GLOBAL_INDEX++;

    char variable = *(char*)(*GLOBAL_INDEX)->data;
    GLOBAL_INDEX++;

    if (*(char*)(*GLOBAL_INDEX)->data != ')')
        return error_on_line(*GLOBAL_INDEX, 0);


    GLOBAL_INDEX++;
    make_tree_easier(func_to_derr);

    tree_branch* func_derr = diffirintiate_tree(func_to_derr, variable);


    make_tree_easier(func_derr);

    return func_derr;
}


tree_branch* return_maker()
{
    tree_branch* value = 0;
    value = make_branch("return", TREE_OPERATOR);
    GLOBAL_INDEX++;
    add_kid(value);
    value->next[0] = get_G(&GLOBAL_INDEX);
    value->kids++;
    return value;
}

tree_branch* variable_maker()
{
    tree_branch* value = 0;
    value = make_branch("double", TREE_VAR_DEFINE);
    GLOBAL_INDEX++;
    add_kid(value);

    VARIABLES[VARIABLE_COUNT] = *(char*)(*GLOBAL_INDEX)->data;
    VARIABLE_COUNT++;

    value->next[0] = get_G(&GLOBAL_INDEX);
    value->kids++;
    return value;
}

tree_branch* func_maker(int number)
{
    tree_branch* value = 0;
    value = make_branch((char*)(*GLOBAL_INDEX)->data, TREE_FUNCTION);
    GLOBAL_INDEX++;
    if (*(char*)(*GLOBAL_INDEX)->data != '(')
       return error_on_line(*GLOBAL_INDEX, 0);
    GLOBAL_INDEX++;
    for(int i = 0; i < CODE_FUNCTIONS[number].args; ++i)
    {
        add_kid(value);
        value->next[value->kids] = get_G(&GLOBAL_INDEX);
        value->kids++;
    }
    if (*(char*)(*GLOBAL_INDEX)->data != ')')
       return error_on_line(*GLOBAL_INDEX, 0);
    GLOBAL_INDEX++;
    return value;
}

tree_branch* print_maker()
{
    tree_branch* value = 0;
    value = make_branch("print", TREE_OPERATOR);
    GLOBAL_INDEX++;
    add_kid(value);
    if((*GLOBAL_INDEX)->type != TREE_STRING)
        value->next[0] = get_G(&GLOBAL_INDEX);
    else
        value->next[0] = code_get_T();
    value->kids++;
    return value;
}

tree_branch* if_maker()
{
    tree_branch* value = 0;
    value = make_branch("if", TREE_OPERATOR);

    add_kid(value);

    if (*(char*)(*GLOBAL_INDEX)->data != '(')
       return error_on_line(*GLOBAL_INDEX, 0);
    GLOBAL_INDEX++;

    value->next[0] = get_G(&GLOBAL_INDEX);

    if (*(char*)(*GLOBAL_INDEX)->data != ')')
       return error_on_line(*GLOBAL_INDEX, 0);
    GLOBAL_INDEX++;

    value->kids++;
    add_kid(value);
    value->next[1] = code_get_E();

    value->kids++;
    if (!strcmp((char*)(*GLOBAL_INDEX)->data, "else"))
    {
        add_kid(value);
        GLOBAL_INDEX++;
        value->next[2] = code_get_E();
        value->kids++;
    }

    return value;
}


tree_branch* scan_maker()
{
    tree_branch* value = 0;
    value = make_branch("scan", TREE_OPERATOR);
    GLOBAL_INDEX++;
    add_kid(value);
    value->next[0] = *GLOBAL_INDEX;
    value->kids++;
    GLOBAL_INDEX++;
    return value;
}

tree_branch* while_maker()
{
    tree_branch* value = 0;
    value = make_branch("while", TREE_OPERATOR);
    GLOBAL_INDEX++;
    add_kid(value);

    if (*(char*)(*GLOBAL_INDEX)->data != '(')
       return error_on_line(*GLOBAL_INDEX, 0);
    GLOBAL_INDEX++;

    value->next[0] = get_G(&GLOBAL_INDEX);

    if (*(char*)(*GLOBAL_INDEX)->data != ')')
       return error_on_line(*GLOBAL_INDEX, 0);
    GLOBAL_INDEX++;

    value->kids++;
    add_kid(value);
    value->next[1] = code_get_E();

    value->kids++;
    return value;
}

int got_function()
{
    for (int i = 0; i < CODE_FUNCTION_COUNT; ++i)
    {
        if (!strcmp((char*)(*GLOBAL_INDEX)->data, CODE_FUNCTIONS[i].name))
            return i;
    }
    return -1;
}

bool is_my_function(tree_branch* branch)
{
    for (int i = 0; i < CODE_FUNCTION_COUNT; ++i)
    {
        if (!strcmp((char*)branch->data, CODE_FUNCTIONS[i].name))
            return true;
    }
    return false;
}

int got_variable()
{
    for (int i = 0; i < VARIABLE_COUNT; ++i)
    {
        if (*(char*)(*GLOBAL_INDEX)->data == VARIABLES[i])
            return i;
    }
    return -1;
}

tree_branch* error_on_line(tree_branch* branch, poem* onegin)
{
    static poem* static_poem;
    if (onegin != 0)
    {
        static_poem = onegin;
        return (tree_branch*)1;
    }

    char* curr_ptr = (char*)branch->data;
    if (branch->type == TREE_MAIN || branch->type == TREE_FUNCTION_DEFINE)
        curr_ptr = ((func_def*)(branch->data))->name;
    for(int i = 0; i < static_poem->string_count; ++i)
    {
        if (static_poem->index[i].start <= curr_ptr && curr_ptr < static_poem->index[i].ending)
        {
            printf("Syntax error on line %d.\nUnknown: %s\n", i + 1, curr_ptr);
            system("pause");
            return (tree_branch*)0;
        }
    }




}
