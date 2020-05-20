
LOCAL_COMMAND_DEF(EXIT, 0,
    {
        printf("\nEnd.\n\n\n");
        i = bin_file_size;
    })

LOCAL_COMMAND_DEF(PUSH, 1,
    {
        switch (code[i + 1])
        {
            case NEXT_NUM:

                stack_push(&cpu_stack, get_num_from_buff(code, i + 2));
                i += sizeof(stack_type);

            break;

            case NEXT_REG:

                stack_push(&cpu_stack, cpu_register[code[i + 2] - 1]);
                i++;

            break;

            case NEXT_RAM:

                if (code[i + 2] == NEXT_NUM)
                {
                    stack_push(&cpu_stack, ram_mem[get_int_from_buff(code, i + 3)]);
                    i += sizeof(int) + 1;
                }

                else if (code[i + 2] == NEXT_REG)
                {
                    stack_push(&cpu_stack, ram_mem[(int)cpu_register[code[i + 3] - 1]]);
                    i += 2;
                }
                else
                    printf("----------\nWrong RAM call from PUSH\n----------\n");

            break;

            default:

                printf("----------\nPUSH got wrong parameter, check assembler for bugs\n----------\n");

            break;
        }

        i++;
    })

LOCAL_COMMAND_DEF(POP, 1,
    {
        switch (code[i + 1])
        {
            case NEXT_NOTHING:
            {
                stack_type trash = 0;
                assert(!stack_pop(&cpu_stack, &trash));
            }
            break;

            case NEXT_REG:
                assert(!stack_pop(&cpu_stack, &(cpu_register[code[i + 2] - 1])));
                i++;
            break;

            case NEXT_RAM:
                if (code[i + 2] == NEXT_NUM)
                {
                    assert(!stack_pop(&cpu_stack, &(ram_mem[get_int_from_buff(code, i + 3)])));
                    i += sizeof(int) + 1;
                }
                else if (code[i + 2] == NEXT_REG)
                {
                    assert(!stack_pop(&cpu_stack, &(ram_mem[(int)cpu_register[code[i + 3] - 1]])));
                    i += 2;
                }
                else
                    printf("----------\nWrong RAM call from POP\n----------\n");
            break;

            default:
                 printf("----------\nPOP got wrong parameter, check assembler for bugs\n----------\n");
            break;
        }
        i++;
    })

LOCAL_COMMAND_DEF(JMP, -1,
    {
        int jump_to = get_int_from_buff(code, i + 1);
        i = jump_to - 1;
    })

LOCAL_COMMAND_DEF(JA, -1,
    {
        CONDITION_JUMP( > );
    })

LOCAL_COMMAND_DEF(JAE, -1,
    {
        CONDITION_JUMP( >= );
    })

LOCAL_COMMAND_DEF(JB, -1,
    {
        CONDITION_JUMP( < );
    })

LOCAL_COMMAND_DEF(JBE, -1,
    {
        CONDITION_JUMP( <= );
    })

LOCAL_COMMAND_DEF(JE, -1,
    {
        CONDITION_JUMP( == );
    })

LOCAL_COMMAND_DEF(JNE, -1,
    {
        CONDITION_JUMP( != );
    })

LOCAL_COMMAND_DEF(CALL, -1,
    {
        int jump_to = get_int_from_buff(code, i + 1);
        stack_push(&return_stack, (stack_type)(i + sizeof(int)));
        i = jump_to - 1;
    })

LOCAL_COMMAND_DEF(RETURN, 0,
    {
        stack_type new_i = 0;
        assert(!stack_pop(&return_stack, &new_i));

        i = (int)(new_i);
    })

LOCAL_COMMAND_DEF(ADD, 0,
    {
        TWO_ARG_OPERATION( + );
    })

LOCAL_COMMAND_DEF(SUB, 0,
    {
        TWO_ARG_OPERATION( - );
    })

LOCAL_COMMAND_DEF(PROD, 0,
    {
        TWO_ARG_OPERATION( * );
    })

LOCAL_COMMAND_DEF(DIV, 0,
    {
        TWO_ARG_OPERATION( / );
    })

LOCAL_COMMAND_DEF(SQRT, 0,
    {
        ONE_ARG_FUNCTION( sqrt );
    })

LOCAL_COMMAND_DEF(SIN, 0,
    {
        ONE_ARG_FUNCTION( sin );
    })

LOCAL_COMMAND_DEF(ASIN, 0,
    {
        ONE_ARG_FUNCTION( asin );
    })

LOCAL_COMMAND_DEF(POW, 0,
    {
        TWO_ARG_FUNCTION( pow );
    })

LOCAL_COMMAND_DEF(LOG, 0,
    {
        ONE_ARG_FUNCTION( log );
    })

LOCAL_COMMAND_DEF(IN, 1,
    {
        switch (code[i + 1])
        {
            case NEXT_REG:
                scanf(IN_TYPE, &(cpu_register[code[i + 2] - 1]));
                i++;
            break;

            case NEXT_RAM:
                if (code[i + 2] == NEXT_NUM)
                {
                    scanf(IN_TYPE, &(ram_mem[get_int_from_buff(code, i + 3)]));
                    i += sizeof(int) + 1;
                }

                else if (code[i + 2] == NEXT_REG)
                {
                    scanf(IN_TYPE, &(ram_mem[(int)cpu_register[code[i + 3] - 1]]));
                    i += 2;
                }
                else
                    printf("----------\nWrong RAM call from IN\n----------\n");
            break;

            case NEXT_NOTHING:
            {
                stack_type a = 0;
                scanf(IN_TYPE, &a);
                stack_push(&cpu_stack, a);
            }
            break;

            case NEXT_STR:
            {
                printf("%s", code + i + 2);
                i += sizeof_string(code + i + 2);
                stack_type a = 0;
                scanf(IN_TYPE, &a);
                stack_push(&cpu_stack, a);
            }
            break;

            default:
                printf("----------\nIN got wrong parameter, check assembler for bugs\n----------\n");
            break;
        }
        i++;
    })

LOCAL_COMMAND_DEF(OUT, 1,
    {
        switch (code[i + 1])
        {
            case NEXT_REG:
                printf(OUT_TYPE, cpu_register[code[i + 2] - 1]);
                i++;
            break;

            case NEXT_RAM:
                if (code[i + 2] == NEXT_NUM)
                {
                    printf(OUT_TYPE, ram_mem[get_int_from_buff(code, i + 3)]);
                    i += sizeof(int) + 1;
                }

                else if (code[i + 2] == NEXT_REG)
                {
                    printf(OUT_TYPE, ram_mem[(int)cpu_register[code[i + 3] - 1]]);
                    i += 2;
                }
                else
                    printf("----------\nWrong RAM call from OUT\n----------\n");
            break;

            case NEXT_STR:
            {
                for(; *(code + i + 2) != 0; ++i)
                {
                    if (*(code + i + 2) == '\\' && *(code + i + 3) == 'n')
                    {
                        printf("\n");
                        i++;
                    }
                    else
                        printf("%c", *(code + i + 2));
                }
                i++;
            }
            break;

            case NEXT_NOTHING:
            {
                stack_type a = 0;
                assert(!stack_pop(&cpu_stack, &a));
                printf(OUT_TYPE, a);
            }
            break;

            default:
                printf("----------\nOUT got wrong parameter, check assembler for bugs\n----------\n");
            break;
        }
        i++;
    })








