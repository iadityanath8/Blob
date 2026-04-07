#include "blob.h"
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

#define LINE_BUFF 1811

typedef enum {
    CMD_NONE = 0,
    CMD_FS = 1,
    CMD_LS = 2,
    CMD_SS = 3
}CMD_Type;

typedef struct {
    i32 ignore_case;
    i32 thread_cnt;
}CMD_Flags;

typedef struct {
    str* items;
    i32 count;
    i32 capacity;
}file_list;

typedef struct {
    CMD_Type type;
    CMD_Flags flags;
    str pattern;
    file_list files;
}ArgList;

bool starts_with(str str, const char* prefix) {
    return strncmp(str, prefix, strlen(prefix)) == 0;
}

static inline void parse_flags(ArgList* list, i32 argc, str* argv, i32* i) {
    while (*i < argc) {
        if (strcmp(argv[*i], "--ignore-case") == 0) {
            (*i)++;
            list->flags.ignore_case = 1;
        }
        else if (strncmp(argv[*i], "--runthread=", 12) == 0) {
            str loc = strchr(argv[*i], '=');
            if (loc) loc++;
            u32 num = atoi(loc);
            list->flags.thread_cnt = num;
            (*i)++;
        }else {
            break;
        }
    }
}

static inline ArgList parse_args(i32 argc, str* argv) {
    ArgList args = {0};
    str* ptr = argv;
    file_list list = {0};
    i32 i = 1;
    u8 command_use = 0;

    if (strcmp(ptr[i],"verbose") == 0) {
        args.type = CMD_FS;
        command_use = 1;
    }else if (strcmp(ptr[i], "less") == 0) {
        args.type = CMD_SS;
        command_use = 1;
    }else if (strcmp(ptr[i], "skipl") == 0) {
        args.type = CMD_LS;
        command_use = 1;
    }else {
        args.type = CMD_LS;
    }

    if (command_use == 1) i++;
    str pattern = argv[i++];
    args.pattern = pattern;

    while (i < argc) {  
        if (ptr[i][0] == '-' && ptr[i][1] == '-') {
            parse_flags(&args,argc,argv,&i);
            continue;
        }
        da_append(&list, ptr[i]);
        i++;
    }
    args.files = list;
    return args;
}

static inline void lowestr(str ss) {
  for (u32 i = 0;ss[i] != '\0';i++) {
    ss[i] = tolower(ss[i]);
  } 
}

static char* TABLE[] = {
    [CMD_FS] = "fm",
    [CMD_LS] = "lm",
    [CMD_SS] = "sm"
};

static inline void print_search_lines(str filename, str pattern, bool ignore_case, CMD_Type cmd) {
    FILE* fp = fopen(filename,"r");
    if (!fp) perror("Cannot open file FILE does not exist");
    
    u32 line_count =0;
    char buf[LINE_BUFF];
    B_Result matched = 0;
    String_Arr arr = {0};
    bool inarr = false;

    while(fgets(buf,LINE_BUFF,fp)) {
        matched = B_UNMATCHED;
        buf[strcspn(buf,"\n\r")] = '\0';
        arr = BLB_match(pattern, buf,"fm",&matched);
        if (ignore_case) {
            char* dupped = strdup(buf);
            lowestr(dupped);
            lowestr(pattern);
            arr = BLB_match(pattern, dupped,TABLE[cmd], &matched);
            if (cmd != CMD_LS) inarr = true;
            free(dupped);
        }else {
          arr = BLB_match(pattern, buf,TABLE[cmd], &matched);
          if (cmd != CMD_LS) inarr = true;
        } 
        if (inarr) {
          for (i32 i = 0;i < arr.count;i++) {
            printf("%s:%d: %.*s\n",filename,line_count + 1,arr.items[i].len,arr.items[i].v);
          }
        }
        if (matched == B_MATCHED) {
            printf("%s:%d: %s\n",filename,line_count + 1,buf); 
        }
        line_count++;
    }
    free(arr.items);
  fclose(fp);
}

void usage() {
    printf("Usage:\n");
    printf("  blob <command> <pattern> [options] [files...]\n\n");

    printf("Commands:\n");
    printf("  verbose           verbose search matches every char in single line\n");
    printf("  skipl             skip matching lines after single match in one line\n");
    printf("  less              matching small character leaving \n\n");

    printf("Options:\n");
    printf("  --ignore-case           Perform case-insensitive matching\n");
    printf("  --runthread=<num>       Number of threads to use\n\n");

    printf("Arguments:\n");
    printf("  pattern                 Pattern to search for\n");
    printf("  files                   One or more files (optional depending on command)\n\n");

    printf("Examples:\n");
    printf("  blob fs --ignore-case hello file1.txt file2.txt\n");
    printf("  blob ss --runthread=4 \"test pattern\"\n");
    printf("  blob ls pattern\n");
    
    exit(1);
}

static inline void execute_cmd(ArgList* arg) {
    file_list l = arg->files;
    CMD_Flags f = arg->flags;

    for (i32 i = 0;i < l.count;i++) {
        print_search_lines(l.items[i],arg->pattern,f.ignore_case, arg->type);
    }
}

/** blob "*." main.c test experiment for single file in here*/
i32 main(i32 argc, str* argv) {
    if (argc < 3) {
        usage();
    }
    ArgList args = parse_args(argc,argv);
    execute_cmd(&args);
    free(args.files.items);
    return 0;
}
