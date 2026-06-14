#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

static double read_double(const char* const ptr);

enum
{
    LINE_WIDTH     = 80 + 2, // конец строки и нуль
    DATA_WIDTH     = 66,     // длина данных
    MAT_WIDTH      = 4,      // длина поля номера материала
    MF_WIDTH       = 2,      // длина поля номера файла
    MT_WIDTH       = 3,      // длина поля номера секции/реакции
    NS_WIDTH       = 5,      // длина поля номера строки
    FIELD_WIDTH    = 11,     // длина одного поля данных
    PAIRS_PER_LINE = 3       // количество пар x,y в строчке
};

int main()
{
	char* xxxx = "12345678901";
	char* str1 = " 2.34567891";
	char* str2 = " 1.245678+1";
	char* str3 = " 1.24567+11";
	char* str4 = "-1.245678+1";

    double x1, x2, x3, x4;

    x1 = read_double(str1);
    x2 = read_double(str2);
    x3 = read_double(str3);
    x4 = read_double(str4);

	return 0;
}

// copy FIELD_WIDTH symbols to the buf and add E symbol if +- found
// then use strtod() for conversion
double read_double(const char *const ptr)
{
    char buf[FIELD_WIDTH + 2]; // 11 + 'E' + '\0'
    int  e_inserted = 0;
    int  j = 0;

    // get first symbol
    buf[j++] = ptr[0];

    // other symbols plus E and \0
    for (int i = 1; i < FIELD_WIDTH; i++)
    {
        char current = ptr[i];
        if (current == 'e' || current == 'E')
        {
            fprintf(
                stderr,
                "- Error in read_double() - found e or E in eless format\n");
            exit(EXIT_FAILURE);
        }
        if (!e_inserted && (current == '+' || current == '-'))
        {
            buf[j++]   = 'E';
            e_inserted = 1;
        }
        buf[j++] = current;
    }
    buf[j] = '\0';

    // convert to double - можно sscanf! потренироваться.
    errno         = 0; // global from the libc
    char  *endptr = NULL;
    double result = strtod(buf, &endptr);
    if (endptr != buf + j || errno == ERANGE)
    {
        fprintf(stderr, "- Error in read_double() - conversion failed\n");
        exit(EXIT_FAILURE);
    }

    return result;
}

