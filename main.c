#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

// check git 1
// check git 2
// check git 3
// 
// function prototype - forward declaration
static double read_double(const char* const ptr);

// compile time constants
enum
{
    LINE_WIDTH     = 80 + 2, // end of line and zero symbol
    DATA_WIDTH     = 66,     // length of endf line data field
    MAT_WIDTH      = 4,      // length of endf material field
    MF_WIDTH       = 2,      // length of endf file field
    MT_WIDTH       = 3,      // length of endf section field
    NS_WIDTH       = 5,      // length of endf string number field
    FIELD_WIDTH    = 11,     // length of one endf number
    PAIRS_PER_LINE = 3       // number of pairs (x,y) in endf data
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

int read_int(const char *const ptr)
{
    char buf[FIELD_WIDTH + 1]; // 11 + '\0'
    memcpy(buf, ptr, FIELD_WIDTH);
    buf[FIELD_WIDTH] = 0;

    errno        = 0;
    char *endptr = NULL;
    int   result = strtol(buf, &endptr, 10);
    if (endptr != buf + FIELD_WIDTH + 1 || errno == ERANGE)
    {
        fprintf(stderr, "- Error in parse_int() - conversion failed\n");
        exit(EXIT_FAILURE);
    }
    return result;
}
