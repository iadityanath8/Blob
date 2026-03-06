#include "blob.h"
#include <stdio.h>

#define EXPECT(p,t,e) expect(p,t,e,__FILE__,__LINE__)

#define auto __auto_type

void expect(char *pattern, char *text, B_Result expected,
            const char *file, int line) {

    auto result = l_blob(pattern, text);
    
    if (result != expected) {
        printf("%s:%d: expectation failed\n", file, line);
        printf("pattern: %s\n", pattern);
        printf("text: %s\n", text);
        printf("expected: %d got: %d\n", expected, result);
    }else{
        printf("Test case Passed successfully\n");
    }
}

int main() {
    EXPECT("?MEo","bMEo",     B_MATCHED);    
    EXPECT("main.?","main.c", B_MATCHED);     
    EXPECT("main.?","main.js",B_UNMATCHED);     
    
    printf("---------------------------------------------------------------\n");
    EXPECT("maina*","mainajs",B_MATCHED);   
    EXPECT("*Law*","GrokLaw", B_MATCHED);
    EXPECT("*Law*", "Lawyer", B_MATCHED);
    printf("---------------------------------------------------------------\n");
    EXPECT("[ab]de", "cde",   B_UNMATCHED);
    EXPECT("[ab]de","ade",    B_MATCHED);
    printf("\n");

    EXPECT("[0-9]de[a-z]","2dez",B_MATCHED);
    printf("---------------------------------------------------------------\n");
    EXPECT("[!a-d0-2]pop","3pop",B_MATCHED);
    
    EXPECT("?[a-f]*", "dfBHow", B_MATCHED); 
    EXPECT("*a", "thisa is londona", B_MATCHED);
    return 0;
}
