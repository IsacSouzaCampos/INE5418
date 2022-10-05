#define MAX_SIZE 256
#define MAX_LINES 1000
#define MAX_CLIENTS 3 // Counting the reading thread

#define GET 0
#define ADD 1

struct request {
  int type;
  int index;
  char text[MAX_SIZE];
};
