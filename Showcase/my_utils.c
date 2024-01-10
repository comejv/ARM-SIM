#include "my_utils.h"

int my_strlen(char *str){
  int i = 0;
  while (str[i] != '\0'){
    i++;
  }
  return i+1;
}

void my_clock(int x){
  for (int i = 0; i < x; i++){
    
  }
}
int my_modulo(int a, int b)
{
    while (a >= b)
    {
        a = a - b;
    }
    return a;
}