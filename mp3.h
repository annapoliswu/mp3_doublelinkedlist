#ifndef MP3_H
#define MP3_H

typedef struct mp3{ //look into struct tags? 
  char* artist;
  char* title;
  int year;
  struct mp3* next;
  struct mp3* prev; 
} mp3_t;


void initMenu(mp3_t** headptr, mp3_t** tailptr);
mp3_t* createNode();
void add(mp3_t** headptr, mp3_t** tailptr);
void printNode(mp3_t* node);
void printForwards(mp3_t* head); 
void printBackwards(mp3_t* tail);
void deleteNode(mp3_t** headptr, mp3_t** tailptr, mp3_t* del);
void delete(mp3_t** headptr, mp3_t** tailptr);
void deleteList(mp3_t** headptr, mp3_t** tailptr);
void cleanup();



#endif
