/* Ruler 1         2         3         4         5         6         7        */

#define UNICODE

char *ovni_e[] = {
"\u2800\u2800\u2584\u2588\u2588\u2584\u2800\u2800",
"\u2597\u259B\u2588\u259C\u259B\u2588\u259C\u2596",
"\u259C\u2588\u2588\u2588\u2588\u2588\u2588\u259B",
"\u2800\u259C\u2598\u2800\u2800\u259D\u259B\u2800"
};

#include <stdio.h>
#include <locale.h>

int main1(void) {
    setlocale(LC_ALL, "");
    
    for (size_t i = 0; i < 4; ++i) {
        puts(ovni_e[i]);
    }
    
    
    return 0;
}