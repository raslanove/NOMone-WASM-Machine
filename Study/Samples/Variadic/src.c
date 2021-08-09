typedef __builtin_va_list va_list;

#define va_start(v,l)   __builtin_va_start(v,l)
#define va_end(v)       __builtin_va_end(v)
#define va_arg(v,l)     __builtin_va_arg(v,l)
#define va_copy(d,s)    __builtin_va_copy(d,s)

void variadicFunction(int firstArgument, ...) {
    va_list vaList;
    va_start(vaList, firstArgument);
    int* argument = va_arg(vaList, int*);
    *argument = 123;
    va_end(vaList);
}

void main(void) {
    int a;
    variadicFunction(567, &a);
}
