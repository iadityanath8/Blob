#include "blob.h"
#include <stdio.h>

#define EXPECT(p,t,e) expect(p,t,e,__FILE__,__LINE__)

#define auto __auto_type

void expect(char *pattern, char *text, B_Result expected,
    const char *file, int line) {

  int n = strlen(pattern);
  int m = strlen(text);
  auto result = blob_indexed(pattern, text,n,m,NULL);

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
//  EXPECT("?MEo","bMEo",     B_MATCHED);    
//  EXPECT("main.?","main.c", B_MATCHED);     
//  EXPECT("main.?","main.js",B_UNMATCHED);     
//
//  printf("---------------------------------------------------------------\n");
//  EXPECT("*","",B_MATCHED);   
//  EXPECT("*Law*","GrokLaw", B_MATCHED);
//  EXPECT("*Law*", "Lawyer", B_MATCHED);
//  printf("---------------------------------------------------------------\n");
//  EXPECT("[ab]de", "cde",   B_UNMATCHED);
//  EXPECT("[ab]de","ade",    B_MATCHED);
//  printf("\n");
//
//  EXPECT("[0-9]de[a-z]","2dez",B_MATCHED);
//  printf("---------------------------------------------------------------\n");
//  EXPECT("[!a-d0-2]pop","epop",B_MATCHED);
//
//  EXPECT("?[a-f]*", "dfBHow", B_MATCHED); 
//  EXPECT("[0-9]hosadi", "5hosadi", B_MATCHED);

  str s2 = "this 99th is me in here woow dfBHOW bBHOW";
//  str pattern = "[0-9][0-9]thchar";
  str patt2 = "[db][df?]BHOW";
  auto arr = BLB_match(patt2, s2);

  for (i32 i = 0;i < arr.count;i++){
    printf("%.*s\n",arr.items[i].len,arr.items[i].v);
  }
  BLB_free_Arr(arr);
  return 0;
}







