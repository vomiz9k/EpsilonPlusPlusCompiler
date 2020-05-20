#include "musthave_include.h"
#include "header.h"

tree_branch* derr_of_div(tree_branch* branch, char variable)
{
    assert(branch);
    tree_branch* returning_branch = 0;
    *(char*)branch->data = '*';
    add_kid(branch);
    branch->next[branch->kids] = branch->next[0];
    branch->next[0] = 0;
    branch->kids++;
    tree_branch* denominator_diff = diffirintiate_tree(branch, variable);

    char* parce_str = "( 0 - y ) / ( Y * Y )";
    poem parce_poem = string_to_poem(parce_str);
    tree_branch** branch_ptr = tokenizate_code(parce_poem);
    returning_branch = get_G(&branch_ptr);

    returning_branch = change_y_for_branch(returning_branch, branch, denominator_diff);
    *(char*)branch->data = '/';
    branch_full_clr(denominator_diff);
    free(denominator_diff);

    return returning_branch;
}

tree_branch* derr_of_mul(tree_branch* branch, char variable)
{
    assert(branch);
    tree_branch* mul_diff = 0;
    mul_diff = make_char_branch(mul_diff, '+', TREE_OPERATOR);

    mul_diff->kids = branch->kids;
    mul_diff->next = (tree_branch**) calloc(branch->kids, sizeof(tree_branch*));

    for (int i = 1; i < branch->kids; ++i)
    {
        if (branch->next[i])
        {
            mul_diff->next[i] = branch_cpy(branch);

            branch_full_clr(mul_diff->next[i]->next[i]);

            mul_diff->next[i]->next[i] = diffirintiate_tree(branch->next[i], variable);
        }
    }
    return mul_diff;
}

tree_branch* derr_of_frac(tree_branch* branch, char variable)
{
    assert(branch);
    tree_branch* returning_branch = 0;

    *(char*)branch->next[0]->data = '*';
    tree_branch* denominator = branch_cpy(branch->next[0]);

    add_kid(denominator);
    denominator->next[denominator->kids] = denominator->next[0];
    denominator->next[0] = 0;
    denominator->kids++;

    *(char*)branch->next[0]->data = '/';

    tree_branch* cache = branch->next[0];
    branch->next[0] = 0;
    tree_branch* numerator = branch_cpy (branch);
    branch->next[0] = cache;

    char* parce_str = "( u * V - U * v ) / ( V * V )";
    poem parce_poem = string_to_poem(parce_str);
    tree_branch** branch_ptr = tokenizate_code(parce_poem);
    returning_branch = get_G(&branch_ptr);

    returning_branch = uv_deffirintiate(returning_branch, numerator, denominator, variable);
    branch_full_clr(numerator);
    free(numerator);
    branch_full_clr(denominator);
    free(denominator);
    assert(returning_branch);
    return returning_branch;
}

tree_branch* derr_of_pow(tree_branch* branch, char variable)
{
    assert(branch);
    tree_branch* returning_branch = 0;
    char parce_str[] = "( U ^ V ) * ( v * log ( U ) + u * V / U )";

    int real_kids = 0;
    int first_kid = -1;
    int last_kid = -1;

    for(int i = 0; i < branch->kids; ++i)
    {
        if (branch->next[i])
        {
            if (first_kid == -1)
                first_kid = i;
            real_kids++;
            last_kid = i;
        }
    }
    if (real_kids >= 3)
    {
        tree_branch* u_value = branch_cpy(branch->next[first_kid]);
        tree_branch* v_value = branch_cpy(branch);
        v_value->next[first_kid] = 0;

        poem parce_poem = string_to_poem(parce_str);
        tree_branch** branch_ptr = tokenizate_code(parce_poem);
        returning_branch = get_G(&branch_ptr);
        returning_branch = uv_deffirintiate(returning_branch, u_value, v_value, variable);
    }
    else if (real_kids == 2)
    {

        tree_branch* u_value = branch_cpy(branch->next[first_kid]);
        tree_branch* v_value = branch_cpy(branch->next[last_kid]);
        if (make_tree_easier(v_value) == TREE_INTEGER)
        {
            strcpy(parce_str, "u * V * U ^ ( V - 1 )");
        }
        poem parce_poem = string_to_poem(parce_str);
        tree_branch** branch_ptr = tokenizate_code(parce_poem);
        int i = 0;
        returning_branch = fast_get_G(&branch_ptr);
        returning_branch = uv_deffirintiate(returning_branch, u_value, v_value, variable);
    }
    else if (real_kids == 1)
    {
        returning_branch = diffirintiate_tree(branch->next[first_kid], variable);
    }
    assert(returning_branch);
    return returning_branch;
}

tree_branch* derr_of_add(tree_branch* branch, char variable)
{
    tree_branch* returning_branch = 0;
    returning_branch = make_char_branch(returning_branch, *(char*)branch->data, TREE_OPERATOR);
    returning_branch->kids = branch->kids;
    returning_branch->next = (tree_branch**)calloc(branch->kids, sizeof(tree_branch*));
    for(int i = 0; i < branch->kids; ++i)
        if(branch->next[i])
        {
            returning_branch->next[i] = diffirintiate_tree(branch->next[i], variable);
        }
    assert(returning_branch);
    return returning_branch;
}
