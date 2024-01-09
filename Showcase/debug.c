#include "my_syscalls.h"

void test(int a, int b){
  if (a > b){
    my_printint(a);
    my_printstr(" > ");
    my_printint(b);
    my_putchar('\n');
  }
  if (a < b){
    my_printint(a);
    my_printstr(" < ");
    my_printint(b);
    my_putchar('\n');
  }
  if (a >= b){
    my_printint(a);
    my_printstr(" >= ");
    my_printint(b);
    my_putchar('\n');
  }
  if (a <= b){
    my_printint(a);
    my_printstr(" <= ");
    my_printint(b);
    my_putchar('\n');
  }
  if (a == b){
    my_printint(a);
    my_printstr(" == ");
    my_printint(b);
    my_putchar('\n');
  }
}


int main(){
  test(1, 1);
  test(2, 5);
  test(8, 4);
  test(12, 24);
  test(24, 12);
  my_exit();
}
