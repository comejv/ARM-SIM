#include "my_syscalls.h"
#define SIZE 10

void print_array(int t[], int size) {
  for (int i = 0; i < size; i++) {
    my_putchar('0'+t[i]);
    my_putchar(' ');
  }
  my_putchar('\n');
}

void main() {
    int t[SIZE] = { 3, 6, 2, 0, 4, 1, 9, 8, 5, 7 };
    int size = SIZE;
    int i, j;
	int tmp;

    print_array(t, size);
    for (i = 0; i < size; i++) {
        j = i;
        while (j > 0) {
            if (t[j-1] > t[j]) {
               tmp = t[j-1];
               t[j-1] = t[j];
               t[j] = tmp;
            }
            j--;
        }
    }
    print_array(t, size);

    my_exit();
}
