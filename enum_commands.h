#define LOCAL_COMMAND_DEF(command, args, to_do) CPU_##command,
enum ASM_COMMANDS
{
    #include "cpu_commands.h"
    CPU_COMMAND_COUNT
};
#undef LOCAL_COMMAND_DEF

enum param_type
{
    NEXT_NUM,
    NEXT_REG,
    NEXT_RAM,
    NEXT_NOTHING,
    NEXT_STR,
};
