#include "my_syscalls.h"

void test(int a, int b){
  my_printint(a);
  my_putchar(' ');
  my_printint(b);
  my_putchar('\n');
}

int main(){
  test(1, 1);
  test(2, 5);
  test(8, 4);
  test(125, 52);
  test(12, 25);
  my_exit();
}
