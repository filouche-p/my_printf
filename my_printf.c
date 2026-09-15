#include <stdarg.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>


//** ----- **
//** UTILS **
//** ----- **

int len_positive_int(int number) {
    int len = 0;
    int temp = number;

    do {
        len++;
        temp /= 10;
    } while (temp != 0);

    return len;
}


char *int_to_str(int number) {
    int is_negative = (number < 0) ? 1 : 0;
    int len = 0;
    unsigned int abs_num = number;

    if (is_negative) {
        len++;
        abs_num *= -1;
    }
    len += len_positive_int(abs_num);
    char *str = calloc(len + 1, sizeof(char));
    if (str == NULL) {
        return NULL;
    }

    str[len] = '\0';

    if (number != 0) {
        int i = len-1;
        while (abs_num > 0) {
            int digit = abs_num % 10;
            str[i] = '0' + digit;
            abs_num /= 10;
            i--;
        }
        
        if (is_negative) {
            str[0] = '-';
        }

    } else {
        str[0] = '0';
    }

    return str;
}


char *base_to_str(unsigned int n, int base) {
    char* str;
    char* digits = "0123456789ABCDEF";
    int len = 0;
    unsigned int temp = n;
    
    do {
        len++;
        temp /= base;
    } while (temp != 0);
    
    str = malloc(sizeof(char)*(len+1));
    if (!str) {
        return NULL;
    }
    str[len] = '\0';

    do {
        str[--len] = digits[n%base];
        n /= base;
    } while (n != 0);
    
    return str;
}


char *char_to_str(char char_to_convert) {
    char* string = calloc(2, sizeof(char));
    if (string == NULL) {
        return 0;
    }
    string[0] = char_to_convert;
    string[1] = '\0';

    return string;
}


char *ptr_to_hex(void* ptr) {
    char* string = calloc(20, sizeof(char));
    if (string == NULL) {
        return NULL;
    }

    uintptr_t addr = (uintptr_t)ptr;
    const char hex_digits[] = "0123456789abcdef";
    int i = 0;
    
    string[i++] = '0';
    string[i++] = 'x';

    int started = 0;
    for (int shift = (sizeof(void*) * 8) - 4; shift >= 0; shift -= 4) {
        int digit = (addr >> shift) & 0xF;
        if (digit != 0 || started || shift == 0) {
            string[i++] = hex_digits[digit];
            started = 1;
        }
    }
    
    string[i] = '\0';
    return string;
}


int my_strlen(char* string) {
    int len=0;

    while (*string) {
        len++;
        string++;
    }

    return len;
}


// ** ----- **
// ** WRITE **
// ** ----- **

int write_stdout(char* string) {
    size_t len = my_strlen(string);
    ssize_t bytes_write = write(STDOUT_FILENO, string, len);
    
    return (bytes_write == (ssize_t)len) ? (int)bytes_write : -1;
}


// ** -------------- **
// ** WRITE_FORMATED **
// ** -------------- **

int write_d(va_list args) {
    int number = va_arg(args, int);
    char* str = int_to_str(number);
    int bw = write_stdout(str);
    free(str);
    return bw;
}


int write_o(va_list args) {
    unsigned int number = va_arg(args, unsigned int);
    char *str = base_to_str(number, 8);
    int bw = write_stdout(str);
    free(str);
    return bw;
}


int write_u(va_list args) {
    unsigned int number = va_arg(args, unsigned int);
    char *str = base_to_str(number, 10);
    int bw = write_stdout(str);
    free(str);
    return bw;
}


int write_x(va_list args) {
    unsigned int number = va_arg(args, unsigned int);
    char *str = base_to_str(number, 16);
    int bw = write_stdout(str);
    free(str);
    return bw;
}


int write_s(va_list args) {
    char *string = va_arg(args, char*);
    if (!string) {
        string = "(null)";
    }
    int bw = write_stdout(string);
    return bw;
}


int write_c(va_list args) {
    char char_to_write = (char)va_arg(args, int);
    return (write(STDOUT_FILENO, &char_to_write, 1) == 1) ? 1 : -1;
}


int write_p(va_list args) {
    void *pointer = va_arg(args, void*);
    char *str = ptr_to_hex(pointer);
    int bw = write_stdout(str);
    free(str);
    return bw;
}


// ** ------ **
// ** PRINTF **
// ** ------ **

typedef struct s_write {
    char format;
    int (*function_print)(va_list args);
} t_write;


int my_printf(const char* restrict format, ...) {
    va_list args;
    va_start(args, format);
    
    t_write format_tab[] = {
        {'d', write_d},
        {'o', write_o},
        {'u', write_u},
        {'x', write_x},
        {'s', write_s},
        {'c', write_c},
        {'p', write_p},
        {0, NULL}
    };
    
    int count = 0;
    
    while (*format) {
        if (*format == '%' && *(format + 1)) {
            if (*(format + 1) == '%') {
                write(1, "%", 1);
                count++;
                format += 2;
                continue;
            }
            
            format++;
            
            t_write *tab = format_tab;
            int found = 0;
            
            while (tab->format != 0) {
                if (*format == tab->format) {
                    count += tab->function_print(args);
                    found = 1;
                    break;
                }
                tab++;
            }
            
            if (!found) {
                write(1, "%", 1);
                write(1, format, 1);
                count += 2;
            }

        } else {
            write(1, format, 1);
            count++;
        }
        format++;
    }
    
    va_end(args);
    return count;
}

int main() {
    my_printf("%d %%\n", 17);
    my_printf("Test: %%%% double\n");
    my_printf("%%\n");

    return 0;
}