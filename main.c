#include "blob.h"
#include <stdio.h>

#define EXPECT(p,t,e) expect(p,t,e,__FILE__,__LINE__)

#define auto __auto_type

//void expect(char *pattern, char *text, B_Result expected,
//    const char *file, int line) {
//
//  int n = strlen(pattern);
//  int m = strlen(text);
//  auto result = blob_indexed(pattern, text,n,m,NULL);
//
//  if (result != expected) {
//    printf("%s:%d: expectation failed\n", file, line);
//    printf("pattern: %s\n", pattern);
//    printf("text: %s\n", text);
//    printf("expected: %d got: %d\n", expected, result);
//  }else{
//    printf("Test case Passed successfully\n");
//  }
//}
//
int main() {
//  EXPECT("?MEo","bMEo",     B_MATCHED);
//  EXPECT("main.?","main.c", B_MATCHED);
//  EXPECT("main.?","main.js",B_UNMATCHED);
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
  
//  str 
  str patt = ");?";
  B_Result res = B_UNMATCHED;
  str s1 = "EXPECT(\"?MEo\", \"bMEo\", B_MATCHED);a";
  auto arr = BLB_match(patt, s1, NULL,&res);
  
//  res =  BLB_Sblob(patt, s1, strlen(patt),strlen(s1),NULL);
  printf("%s\n",b_to_str(res));

  for (i32 i = 0;i < arr.count;i++){
   printf("%.*s and  %d\n",arr.items[i].len,arr.items[i].v,arr.items[i].len);
 }

  //  BLB_free_Arr(arr);
  return 0;
}







