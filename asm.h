struct label
{
char name[20];
int jmp;
};

int is_RAM(char* str);

int get_binary_file(char* fin_name, char* fout_name);

bool check_syntax(indexes index);

int put_to_buff_char(char* buff, int printed_chars, char c);

int put_to_buff_num(char* buff, int printed_chars, char* a);

int put_to_buff_int(char* buff, int printed_chars, char* a);

int put_to_buff_string(char* buff, int printed_chars, char* str);


int label_exists(char* name, label* labels, int label_count);

bool is_label(char* str);

int make_jump(label* labels, char* command_buffer, int char_count, int* label_counter);


int sizeof_word(char* ptr);

int sizeof_string(char* ptr);

bool is_register(char* str);



