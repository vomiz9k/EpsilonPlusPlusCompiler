#include "musthave_include.h"

/*!This func constructs stack_t object.
@param[in] _stack - pointer on stack_t object.
@param[in] stack_name - name of variable _stack.*/
bool stack_construct (stack_t* _stack, char stack_name[])
{
    if (stack_not_ok(_stack, CONSTRUCT))
        return false;

    memset (_stack, 0, sizeof (stack_t));
    _stack->max_size = 1;

    #ifdef _DEBUG
        _stack->data = (stack_type*)((long int*)calloc (1, sizeof (stack_type)*_stack->max_size + 2* sizeof (long int)) + 1);

        *((long int*)(_stack->data) - 1)                 = DATACANARYCONST1;
        * (long int*)(_stack->data + (_stack->max_size)) = DATACANARYCONST2;

        _stack->canary_1 = CANARYCONST1;
        _stack->canary_2 = CANARYCONST2;

        _stack->name     = stack_name + 1;
        _stack->data[0]  = POISON;
        _stack->hash_sum = get_hash (_stack);

        if (stack_not_ok (_stack, CONSTRUCT))
            return false;
    #else
        _stack->data = (stack_type*)calloc (sizeof (stack_type), _stack->max_size);
    #endif
        return true;

}
/*!This func checks size of stack. If size needs to be more/less it makes it more/less.
@param[in] _stack - pointer on stack_t object
@param[in] parameter - enum func name.*/
void check_size (stack_t* _stack, int parameter)
{
    if (parameter == NEEDPUSH)

        if ((_stack->size)  >= (_stack->max_size))
        {
            #ifdef _DEBUG
                int curr_size = _stack->max_size * STACK_INCREASER;

                _stack->data = (stack_type*)((long int*)realloc ((long int*)_stack->data - 1, sizeof (stack_type) * curr_size + 2 * sizeof (long int))+1);

                for (int i = curr_size / STACK_INCREASER; i < curr_size; ++i)
                    (_stack->data)[i] = POISON;

                *(long int*)(_stack->data + curr_size) = DATACANARYCONST2;

                _stack->max_size = curr_size;
            #else
                _stack->max_size *= 2;
                _stack->data = (stack_type*)realloc (_stack->data, sizeof (stack_type) * _stack->max_size);
            #endif
        }
    if (parameter == NEEDPOP)

        if ((_stack->size) < (_stack->max_size) / STACK_INCREASER  && (_stack->max_size) != 1)
        {
            #ifdef _DEBUG
                int curr_size = _stack->max_size / STACK_INCREASER;

                _stack->data = (stack_type*)((long int*)realloc ((long int*)_stack->data - 1, sizeof (stack_type) * curr_size + 2* sizeof (long int))+1);

                *(long int*)(_stack->data + curr_size) = DATACANARYCONST2;

                _stack->max_size = curr_size;
            #else
                _stack->max_size /= 2;
                _stack->data = (stack_type*)realloc (_stack->data, sizeof (stack_type) * _stack->max_size);
            #endif
        }
}



/*!This func pushes element in stack.
@param[in] _stack - pointer on stack_t object.
@param[in] value - value of element we need to push into stack.*/
bool stack_push (stack_t* _stack, stack_type value)
{
    if (stack_not_ok (_stack, NEEDPUSH))
        return POISON;
    check_size (_stack, NEEDPUSH);

    _stack->data[_stack->size] = value;

    _stack->size++;

    #ifdef _DEBUG

        _stack->hash_sum = get_hash (_stack);

        if (stack_not_ok (_stack, NEEDPUSH))
            return POISON;

    #endif
    return true;
}



/*!This func pops element from stack.
@param[in] _stack - pointer on stack_t object we need pop out.*/
int stack_pop (stack_t* _stack, stack_type* pop_to)
{
    int err_int = stack_not_ok (_stack, NEEDPOP);
    if (err_int)
        return err_int;

    (*_stack).size--;

    check_size (_stack, NEEDPOP);

    *pop_to = (*_stack).data[ (*_stack).size];

    #ifdef _DEBUG

        _stack->hash_sum = get_hash (_stack);

        (*_stack).data[ (*_stack).size] = POISON;

        err_int = stack_not_ok (_stack, NEEDPUSH);

    #endif
    return err_int;

}

/*!This func destructs stack_t object.
@param[in] _stack - pointer on stack_t object.
@param[in] stack_name - name of variable _stack.*/
int stack_destruct (stack_t* _stack)
{
    if (stack_not_ok (_stack, DESTRUCT))
        return POISON;
    #ifdef _DEBUG

        _stack->size = _stack->max_size = POISON;

        free ((long int*)_stack->data - 1);

        _stack->data = nullptr;

        printf ("stack_t \"%s\" was destructed.\n", _stack->name);

    #else

        _stack->size = _stack->max_size = -1;
        free (_stack->data);

    #endif
    return 0;
}


/*!This func checks is stack ok or not.
@param[in] _stack - pointer on stack_t object we need to check.
@param[in] func - enum type of func where from was skack_ok called.*/
int stack_not_ok (stack_t *_stack, int func)
{
    int err_int = 0, err_int_reverce = 0;

    if (_stack == nullptr)
        err_int = err_int * 10 + STACK_POINTER;
    if (func != CONSTRUCT && _stack != nullptr)
    {
        if (_stack->data == nullptr)
            err_int = err_int * 10 + DATA_POINTER;
        #ifdef _DEBUG
            else if ( _stack->canary_1 != CANARYCONST1 ||
                      _stack->canary_2 != CANARYCONST2 ||
                      *((long int*)(_stack->data) - 1) != DATACANARYCONST1 ||
                      *(long int*)(_stack->data + (_stack->max_size)) != DATACANARYCONST2 )
                {
                    err_int = err_int * 10 + CANARY;
                }
        #endif
        if (func == NEEDPOP && 0 >= _stack->size)
            err_int = err_int * 10 + IS_UNDERFLOW;

        else if (func != NEEDPOP && _stack->size > _stack->max_size)
            err_int = err_int * 10 + IS_OVERFLOW;

        else if (func != NEEDPOP && _stack->size < 0)
            err_int = err_int * 10 + BAD_SIZE;
        #ifdef _DEBUG
            if (get_hash (_stack) != _stack->hash_sum)
                err_int = err_int * 10 + BAD_HASH;
        #endif
    }

    while (err_int > 0)
    {
        err_int_reverce = err_int_reverce * 10 + (err_int % 10);
        err_int = err_int / 10;
    }
    #ifdef _DEBUG
        if (err_int_reverce > 0)
            STACKDUMP(_stack, err_int_reverce);
    #endif
    return err_int_reverce;
}

/*!This func counts hash_sum for stack.
@param[in] _stack - stack we need count hash for.*/
long long int get_hash (stack_t* _stack)
{
    long long int hash_sum = 0;
    int hash_key = 0xDEDFACEE;

    for (int i = 0; i < _stack->size * sizeof(stack_type); ++i)
        hash_sum += (((char*)_stack->data)[i] ^ hash_key) * (i + 2);

    for (char* i = (char*)_stack; i < (char*)((long long int*)(_stack + 1) - 1); ++i)
        hash_sum += (int)i * (int)(*i);

    hash_sum += _stack->size*_stack->max_size;
    return hash_sum;

}

/*!This func dumps information about error in stack.
@param[in] _stack - pointer on stack_t object we need to dump.
@param[in] err_name - enum object with information about error.*/
bool stack_dump (stack_t* _stack, int err_int, int line, char* file)
{
    printf("\n-------------------------------------------\n\n");
    printf("Error in file \"%s\" in line %d.\n", file, line);
    if (err_int % 10 != STACK_POINTER)
    {
        printf ("Error with stack_t \"%s\"\n\nElements:\n\n", _stack->name);

        for (int i = 0; i < _stack->size; ++i)
            printf ("\t[%05d] == %d\n", i, (_stack->data)[i]);

        if (_stack->size == _stack->max_size)
            printf ("\nNo empty elements.\n");

        else
        {
            printf ("\nEmpty elements:\n\n");
            for (int i = _stack->size; i < _stack->max_size; ++i)
                printf ("\t[%05d] == %-16d [POISON]\n", i, (_stack->data)[i]);
        }

        printf ("\n\tPOISON value must be %d\n", POISON);
    }
    bool beaten_data = false;
    while (err_int % 10 > 0)
    {
        switch (err_int % 10)
        {
            case STACK_POINTER:
                printf ("\nSTACK POINTER ERROR!!!\n\n\tStack pointer == [%x]\n", _stack);
                break;

            case DATA_POINTER:
                printf ("\nDATA POINTER ERROR!!!\n\n\tData pointer == [%x]\n", _stack->data);
                beaten_data = true;
                break;

            case CANARY:
                printf ("\nERROR WITH CANARIES!!!\n\nCanaries:\n\n");
                printf ("\tStack canary 1 == %d\tMust be %d\n\tStack canary 2 == %d\tMust be %d\n\n", _stack->canary_1, CANARYCONST1, _stack->canary_2, CANARYCONST2);

                if (beaten_data)
                    printf("\tData is beaten, no data canaries pointers found.\n");
                else
                    printf ("\tData canary 1 == %d\tMust be %d\n\tData canary 2 == %d\tMust be %d\n", *((long int*)_stack->data - 1), DATACANARYCONST1, *((long int*)_stack->data +_stack->max_size), DATACANARYCONST2);

                break;

            case IS_UNDERFLOW:
                printf ("\nUNDERFLOW ERROR!!!\n\n\tSize == %d\tMax size == %d\n", _stack->size, _stack->max_size);
                break;

            case IS_OVERFLOW:
                printf ("\nOVERFLOW ERROR!!!\n\n\tSize == %d\tMax size == %d\n", _stack->size, _stack->max_size);
                break;

            case BAD_SIZE:
                printf ("\nSIZE ERROR!!!\n\n\tSize == %d\tMax size == %d\n", _stack->size, _stack->max_size);
                break;

            case BAD_HASH:
                printf ("\nHASH SUM ERROR!!!\n\n\tHash sum == %lld\tMust be %d\n\n", _stack->hash_sum, get_hash (_stack));
                break;

            default:
                printf("\nFAKE stack_not_ok ARGUEMENT GIVEN\n\n");
                break;
        }
        err_int = err_int / 10;
    }
    printf("\n-------------------------------------------\n\n");
}



