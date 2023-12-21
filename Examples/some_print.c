#include "my_syscalls.h"

void fibo(int a, int b, int x){
    if (x == 0){
        return;
    }
    my_putchar('0' + (b));
    my_putchar('\n'); 
    fibo(b, a+b, x-1);
}

int main(){
    fibo(1,1,5);
    return 0;
}