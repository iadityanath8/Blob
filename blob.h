/**
 *   06/03/2026
 *   code by Nemesis
 */

#ifndef BLOB_H
#define BLOB_H

#define unused(x) (void)x

#ifndef BLOB_DECL
#define BLOB_DECL static inline
#endif

#ifndef STRICT_MODE 
#define STRICT_MODE 0
#endif 

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define da_append(xs, x)                                                             \
  do {                                                                             \
    if ((xs)->count >= (xs)->capacity) {                                         \
      if ((xs)->capacity == 0) (xs)->capacity = 256;                           \
      else (xs)->capacity *= 2;                                                \
      (xs)->items = realloc((xs)->items, (xs)->capacity*sizeof(*(xs)->items)); \
    }                                                                            \
    \
    (xs)->items[(xs)->count++] = (x);                                            \
  } while (0)


typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
typedef uint64_t u64;

typedef int8_t i8;
typedef int16_t i16;
typedef int32_t i32;
typedef int64_t i64;

typedef char* str;


typedef enum {
  B_MATCHED,
  B_UNMATCHED,
  B_SYNTAX_ERROR_RANGE_NO_END,
  B_SYNTAX_ERROR_RANGE_NO_START,
  B_SYNTAX_ERROR_WRG_RANGE,
  B_SYNTAX_ERROR_UNCLOSED,
  B_SYNTAX_NEGATE_ERR
}B_Result;

typedef struct {
  str  v;
  u32 len;
}Blob_String_View;

typedef struct {
  Blob_String_View* items;
  i32 count;
  i32 capacity;
}String_Arr;

const char* b_to_str(B_Result res){
  switch(res){
    case B_UNMATCHED:                   return "not matched";
    case B_MATCHED:                     return "matched successfully";
    case B_SYNTAX_ERROR_RANGE_NO_START: return "range not started `[`";
    case B_SYNTAX_ERROR_RANGE_NO_END:   return "range did'nt end";
    case B_SYNTAX_ERROR_WRG_RANGE:      return "not a valid range";
    case B_SYNTAX_ERROR_UNCLOSED:       return "bracket not closed `]`";
    case B_SYNTAX_NEGATE_ERR:           return "character should not precced `!`";
    default: return "Invalid Error";
  }
}

/**
 *
 * string pattern match 
 * only matches the pattern to the text based
 * on the rule of glob in unix 1970 
 * with strict version matches full text 
 */
BLOB_DECL B_Result BLB_blob(str patt, str tex){
  str star_pat = NULL;
  str text_pos = NULL;
  str pattern = patt;
  str text = tex;

  while (*text!='\0'){
    switch(*pattern){
      case '?':{
                 pattern++;
                 text++;       
               }break;
      case '*':{
                 star_pat = ++pattern;
                 text_pos = text;
               }break;
      case '[':{
                 bool matcher = false;
                 bool negate = false;
                 while (*pattern != ']' && *pattern != '\0'){
                   switch(*pattern){
                     case '-':{
                                // check for chars in here 
                                if (pattern[1] == '\0' || pattern[1] == ']') return B_SYNTAX_ERROR_RANGE_NO_END;
                                str bg = pattern - 1; // peek before 
                                str ed = pattern + 1; // peek ahead
                                if (*bg == '[')                              return B_SYNTAX_ERROR_RANGE_NO_START;
                                if (*bg >= *ed)                              return B_SYNTAX_ERROR_WRG_RANGE;
                                matcher |= (*bg <= *text && *text <= *ed);
                              }break;
                     case '!':{
                                str prev = pattern - 1;
                                str next = pattern + 1;
                                if (*prev != '[')  {
                                  return B_SYNTAX_NEGATE_ERR;
                                }
                                if (*next == '\0') return B_SYNTAX_ERROR_UNCLOSED;
                                if (*next == ']')  return B_SYNTAX_ERROR_RANGE_NO_END;
                                negate=true;
                              }break;         
                     default:{   
                               matcher |= (*pattern == *text);
                             }break;
                   }
                   pattern++;
                 }
                 if (*pattern == '\0') return B_SYNTAX_ERROR_UNCLOSED;
                 if (negate) matcher = !matcher;
                 if (!matcher){
                   if (!star_pat) return B_UNMATCHED;
                   pattern = star_pat;
                   text = ++text_pos;
                   continue;
                 } 
                 pattern++;
                 text++;
               }break;
      default:{
                if (*pattern == *text){
                  pattern++;
                  text++;
                }else{
                  pattern = star_pat;
                  text = ++text_pos;
                  if (!star_pat) return B_UNMATCHED;
                }
              }break;
    }
  }

  if (*text == '\0'){
    while (*pattern == '*') pattern++;
    if (*pattern == '\0') return B_MATCHED;
  }
  return B_UNMATCHED;
}


/**
 *  
 *  Strict Version of BLOB which does something like   
 *  For matching whole text only use when in bmatch
 */
BLOB_DECL B_Result BLB_Sblob(str pattern, str text,i32 n,i32 m,u32* n_matched){
  i32 star_pat = -1;
  i32 text_pos = -1;
  i32 i = 0;
  i32 j = 0;

  while (j<m){
    switch(pattern[i]){
      case '?':{
                 i++;
                 j++;
               }break;
      case '*':{
                 i++;
                 star_pat = i;//pattern + i;
                 text_pos = j;//text;
               }break;
      case '[':{
                 bool matcher = false;
                 bool negate = false;
                 while (i < n && pattern[i] != ']'){
                   switch(pattern[i]){
                     case '-':{
                                if (i + 1 == n || pattern[i + 1] == ']') return B_SYNTAX_ERROR_RANGE_NO_END;
                                str bg = (pattern + i) - 1; // peek before 
                                str ed = (pattern + i) + 1; // peek ahead  range_start
                                if (*bg == '[')                              return B_SYNTAX_ERROR_RANGE_NO_START;
                                if (*bg >= *ed)                              return B_SYNTAX_ERROR_WRG_RANGE;
                                matcher |= (*bg <= text[j] && text[j] <= *ed);
                              }break;
                     case '!':{
                                str prev = pattern + i - 1;
                                str next = pattern + i + 1;
                                if (*prev != '[')  {
                                  return B_SYNTAX_NEGATE_ERR;
                                }
                                if ((i + 1) == n) return B_SYNTAX_ERROR_UNCLOSED;
                                if (*next == ']')  return B_SYNTAX_ERROR_RANGE_NO_END;
                                negate=true;
                              }break;         
                     default:{   
                               matcher |= (pattern[i] == text[j]);
                             }break;
                   }
                   i++;
                 }
                 if (i >= n) return B_SYNTAX_ERROR_UNCLOSED;
                 if (negate) matcher = !matcher;
                 if (!matcher){
                   if (star_pat == -1) return B_UNMATCHED;
                   i =  star_pat;
                   j = ++text_pos;
                   continue;
                 }
                 i++;
                 j++;
               }break;
      default:{
                if (pattern[i] == text[j]){
                  i++;
                  j++;
                }else{
                  if (star_pat == -1) return B_UNMATCHED;
                  i = star_pat;
                  j = ++text_pos;
                }
              }break;
    }
    if (i == n) {
      if (n_matched) *n_matched = j;
      return B_MATCHED;
    }
  }

  /** strict Mode will be removed in indexed version **/
#if 1 
  if (j == m){
    while (i < n && pattern[i] == '*') i++;
    if (i == n) {
      if (n_matched) *n_matched = j;
      return B_MATCHED;
    }
  }
#endif 
  return B_UNMATCHED;
}




/**
  function returns the Arr of String_View of the particular string matchied 

  typedef struct {
    str  v;
    u32 len;
  }Blob_String_View;
*/

/*
 *        Blob_String_View view = {.v = text + pos,.len = (u32)(n_matched)};
          da_append(&arr,view);
 *
 */

BLOB_DECL void BLB_free_Arr(String_Arr arr) {
  free(arr.items); 
}

BLOB_DECL String_Arr BLB_match(str pattern, str text, const char* mode,B_Result* matched){
  String_Arr arr = {0};
  u32 n = strlen(pattern);
  u32 m = strlen(text);
  if (mode == NULL) mode = "sm";

  u32 n_matched = 0;
  for (u32 pos = 0;pos < m;pos++) {
    if (BLB_Sblob(pattern,text + pos,n,m - pos,&n_matched) == B_MATCHED) {
      Blob_String_View view = {.v = text + pos,.len = (n_matched)};
      if (strcmp(mode,"fm") == 0) {
        da_append(&arr,view);
      }else if (strcmp(mode,"sm") == 0) {
        da_append(&arr, view);
        pos += n_matched;
      }else {
        if (matched) {
          *matched = B_MATCHED;
        }
        break;
      }
    }
  }
  return arr;
}



#endif // BLOB_H 
