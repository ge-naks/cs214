typedef struct ArrayList {
  int length;
  int capacity;
  char **items;
} ArrayList;

typedef struct Command {
    ArrayList* arguments;
    char *execpath;
    char *inputfile;
    char *outputfile;
} Command;

ArrayList *newList(void);
void check(ArrayList *list);
void add(ArrayList *list, const char *s);
void addCommand(ArrayList *list, const Command *cmd);
void freelist(ArrayList *list);
void freelistcontent(ArrayList *list);