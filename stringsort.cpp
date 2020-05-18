#include <assert.h>
#include <cstring>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <windows.h>
#include <io.h>
#include <fcntl.h>
#include "stringsort.h"

const int FILENAMESIZE = 50, SPAMSIZE = 10;

poem string_to_poem(char* str)
{
    int onegin_size = 0;
    char* onegin = str;
    int string_count = get_string_count(onegin, onegin_size);
    struct indexes* index = (indexes*) get_str_index(onegin, string_count, onegin_size);

    struct poem poem_onegin = {0};

    assert(&poem_onegin);

    poem_onegin.text         = onegin;
    poem_onegin.size         = onegin_size;
    poem_onegin.string_count = string_count;
    poem_onegin.index        = index;
    return poem_onegin;
}


poem files_to_poem(char* fin_name, char* fout_name)
{
    int onegin_size = 0;
    char* onegin = file_read(fin_name, &onegin_size);
    int string_count = get_string_count(onegin, onegin_size);
    struct indexes* index = (indexes*) get_str_index(onegin, string_count, onegin_size);

    struct poem poem_onegin = {0};

    assert(&poem_onegin);

    poem_onegin.text         = onegin;
    poem_onegin.size         = onegin_size;
    poem_onegin.string_count = string_count;
    poem_onegin.index        = index;
    poem_onegin.fin_name     = (char*)fin_name;
    poem_onegin.fout_name    = (char*)fout_name;
    return poem_onegin;
}

/*! This func gets values for struct poem*/
struct poem get_poem()
{
    char *fin_name = (char*)calloc(FILENAMESIZE, sizeof(char)), *fout_name = (char*)calloc(FILENAMESIZE, sizeof(char));
    get_file_names (fin_name, fout_name); assert(fin_name[0] != 0);
    return files_to_poem(fin_name, fout_name);
}
/*!Function gets files names.
@param [in] fin_name - input file name variable.
@param [in] fout_name - output file name variable*/
void get_file_names(char* fin_name, char* fout_name)
{
    printf ("Input filename: ");
    scanf ("%s", fin_name);

    printf ("Output filename: ");
    scanf ("%s", fout_name);
}

/*!This function returns count of characters in input file.
@param [in] fin - input file.*/
int file_size(FILE * fin)
{
    int needspace = 0;
    fseek(fin, 0, 2);
    needspace = ftell(fin);
    assert (needspace > 0);
    fseek(fin, 0, 0);
    return needspace;
}

/*!This function returns count of strings in onegin.
@param [in] fin - input file
@param [in] size_of_file - size of file in bytes.*/
char* file_read(char* fin_name, int *size_of_file)
{
    FILE *fin = fopen(fin_name, "r");
    *size_of_file = file_size(fin);

    char* file_pointer = (char*) calloc(*size_of_file+1, sizeof(char));
    assert (file_pointer != nullptr);

    int symbols = fread(file_pointer, sizeof(char), *size_of_file, fin);
    file_pointer[symbols] = 0;
    *size_of_file = symbols;
    fclose (fin);
    return file_pointer;
}

/*!This function returns count of strings in onegin.
@param [in] onegin - input file turned to one string.
@param [in] size_of_file - size of onegin in bytes. */
int get_string_count(char* onegin, int size_of_file)
{
    int cnt = 0, i = 0;
    for (i = 0; i < size_of_file; ++i)

        if (onegin[i] == '\n' || onegin[i] == '\0' || onegin[i] == EOF)
            ++cnt;
    assert(cnt >= 0);
    return cnt+1;
}

/*!This function writes pointers of strings saved in one big string onegin (where strings are devided by '\n') to array index. Also it replaces '\n' with '\0'.
@param [in] onegin - input file turned to one string.
@param [in] index - array of pointers of strings.
@param [in] size_of_file - size of onegin in bytes.
@param [in] parameter - bool variable(true = sort by last symbols, false == sort by first symbols)*/
void* get_str_index(char* onegin, int string_count, int size_of_file)
{
    struct indexes* index = (indexes*)calloc(string_count, sizeof(indexes));
    assert (index != nullptr);
    int i = 0, j = 0;
    index[j].start = onegin;
    for(i = 0; i < size_of_file && j < string_count; ++i)
    {
        if (onegin[i] == '\n' || onegin[i] == '\0' || onegin[i] == EOF)
        {
            index[j].ending = onegin + i;
            ++j;
            if (j < string_count)
                index[j].start = onegin + i + 1;
            onegin[i] = '\0';
        }
    }
        index[j].ending = onegin + i + strlen(index[j].start);
    return index;
}

/*!Thif func prints all things that we needed in file fout.
@param [in] fout - file we need to write in.
@param [in] index - array of struct indexes, in the right order.
@param [in] string_count - count of strings we need to write. Also count of elements in index.
@param [in] onegin - pointer on original text.
@param [in] spam_size - size of spam we need to put between texts.*/
void print_all_to(char* fout_name, indexes* index, int string_count, char* onegin)
{
    FILE *fout = fopen(fout_name, "w");
    qsort(index, string_count, sizeof(indexes), comp_strings_end);
    print (index, string_count, fout);

    spam(fout, SPAMSIZE);

    qsort(index, string_count, sizeof(indexes), comp_strings_start);
    print (index, string_count, fout);

    spam(fout, SPAMSIZE);

    fprintf (fout, "%s", onegin);

    fclose(fout);
}

/*! Analog of default qsort function.
@param [in] arr - array we need to sort.
@param [in] cnt - count of elements in array.
@param [in] el_size - size of element in array.
@param [in] comp - comparing function.*/
void my_qsort(void* arr, int cnt, int el_size, int (*comp)(const void* a, const void* b))
{
    int i = 1, j = 0;
    if (cnt <= 1)
        return;
    if (cnt == 2)
    {
        swap(arr, (arr + el_size), el_size);
        return;
    }
    swap ((arr + el_size * (cnt / 2)), arr, el_size);
    for (i = 1; i < cnt; ++i)
    {
        if (comp(arr, (arr+i * el_size)) > 0)
        {
            ++j;
            swap((arr + i * el_size), (arr + el_size * j), el_size);
        }

    }
    swap(arr, (arr+ j * el_size), el_size);
    my_qsort(arr, j, el_size, comp);
    my_qsort(arr + el_size * (j + 1), cnt - j - 1, el_size, comp);
}

/*!This function compares two strings by first letters.
Returns 1 if str1 > str2, returns -1 if str2 < str1, returns 0 if str1 = str2.
@param [in] str1_pointer - pointer on pointer on string 1.
@param [in] str2_pointer - pointer on pointer on string 2*/
int comp_strings_start(const void* str1_pointer, const void* str2_pointer)
{
    int i = 0, j = 0;
    indexes index1 = *(indexes*) str1_pointer, index2 = *(indexes*) str2_pointer;
    char c1 = 0, c2 = 0;
    int size1 = index1.ending - index1.start, size2 = index2.ending - index2.start;

    do
    {
        while (is_letter(*(index1.start + i)) < 0 && i < size1)
            ++i;
        while (is_letter(*(index2.start + j)) < 0 && j < size2)
            ++j;

        c1 = is_letter(*(index1.start + i));
        c2 = is_letter(*(index2.start + j));

        if ((c1 == '\0' || c2 == '\0' || c1 != c2))
            return 0;
        ++i; ++j;

    } while (i < size1 || j < size2);
    return c1 - c2;
}

/*!This function compares two strings by first letters.
Returns 1 if str1 > str2, returns -1 if str2 < str1, returns 0 if str1 = str2.
@param [in] str1_pointer - pointer on pointer on string 1.
@param [in] str2_pointer - pointer on pointer on string 2*/
int comp_strings_end(const void* str1_pointer, const void* str2_pointer)
{
    int i = 0, j = 0;
    char c1 = 0, c2 = 0;
    indexes index1 = *(indexes*) str1_pointer, index2 = *(indexes*) str2_pointer;
    int size1 = (int) (index1.ending - index1.start), size2 = (int) (index2.ending - index2.start);
    do
    {
        ++i; ++j;

        while (is_letter(*(index1.ending - i)) < 0 && i < size1)
            ++i;

        while (is_letter(*(index2.ending - j)) < 0 && j < size2)
            ++j;

        c1 = is_letter(*(index1.ending - i));
        c2 = is_letter(*(index2.ending - j));

        if (c1 == '\0' || c2 == '\0' || c1 != c2)
            return c1 - c2;

    } while (i < size1 || j < size2);
    return c1 - c2;
}

/*!This functions swap 2 elements by bytes.
@param [in] a - pointer of element 1
@paran [in] b - pointer of element 2*/
void swap(void* a, void* b, int el_size)
{
    int i = 0;
    char char_cch;
    for (i = 0; i < el_size; ++i)
    {
        char_cch = *(char*) (a + i);
        *(char*) (a + i) = *(char*) (b + i);
        *(char*) (b + i) = char_cch;
    }
}

/*!This function returns a if a is '\0' or a letter from lower register.
If a is a letter of upper register, it returns this letter in lower register for comparing.
If a is other symbol, it returns -1.
@param [in] a - char we need information about.  */
int is_letter(char c)
{
    if (('a' <= c && c <= 'z') || (c =='\0'))
        return c;
    if (c == '\n')
        return '\0';
    if (('A' <= c && c <= 'Z'))
        return c - 'A' + 'a';
    else
        return -1;
}

/*!This function puts strings saved in index[] to fout file for text sorted by first letters.
@param [in] index - array of sorted strings indexes.
@param [in] string_count - count of strings in index[].
@param [in] fout - output file.*/
void print(indexes* index, int string_count, FILE* fout)
{
    int i = 0;
    char *j = index[i].start;
    for(i = 0; i < string_count; ++i)
    {
        j = index[i].start;
        while (is_letter(*j) < 0)
            ++j;
        if (j < index[i].ending)
        {
            *index[i].ending = '\0';
            fprintf (fout, "%s\n", index[i].start);
            *index[i].ending = '\n';
        }
    }
}

/*! This function spams 50 empty strings in file
@param [in] fout - file we need to spam in.*/
void spam(FILE* fout, int spam_size)
{
    int i = 0;
    for (i = 0; i < spam_size; ++i)
        fprintf (fout, " %s ", "meow\n");
}
/*This function asks for doing some other function*/
void ask_for_func(void(*func)(), const char* func_name)
{
char c = 'n';
    printf ("Do %s? (Y/N): ", func_name);
    c = getchar();
    printf ("\n");
    if (c == 'Y' || c == 'y')
        func();
}
/*! This func clears memory used for text.*/
void get_free(struct poem onegin)
{
    free(onegin.text);
    free(onegin.index);
    free(onegin.fin_name);
    free(onegin.fout_name);
}






