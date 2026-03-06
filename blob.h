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

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

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
 */
BLOB_DECL B_Result l_blob(str pattern, str text){
  str star_pat = NULL;
  str text_pos = NULL;
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
                //str old_ptr = pattern++;
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
 *   using tables to minimize the recursion as minumum as possible
 *   two chocices of backtracking (+1)
 *   *a       GROKb, Meowa, Jefa
 *     
 *      p e a
 *   *  t t t
 *   a  f f t
 */
BLOB_DECL B_Result bmatch(str pattern, str text){
  // dp approach 
  unused(pattern);
  unused(text);
  return B_MATCHED;
}




#endif // BLOB_H 
