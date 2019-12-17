#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "mp3.h"
#define BUFFERSIZE 128
#define MINYEAR 1800
#define MAXYEAR 2019
mp3_t* head = NULL;
mp3_t* tail = NULL;


int main(int argc, char *argv[]){
  atexit(cleanup);
  initMenu(&head, &tail);
  exit(0);
}


//starts menu and deals with selection, breaks when 5 is pressed to let main exit program
void initMenu(mp3_t** headptr, mp3_t** tailptr){
  char buffer[BUFFERSIZE] = {'\0'};
  int selection = 0;
  int scanCheck;
  while(selection != 5){
    printf("\nPress numbers 1-5 to:\n(1) Add MP3\n(2) Delete MP3\n(3) Print forwards\n(4) Print backwards\n(5) Exit program\n"); 
    if(fgets(buffer, BUFFERSIZE, stdin) != NULL){ 
      scanCheck = sscanf(buffer, "%d", &selection);
      if(scanCheck == 0 || scanCheck == EOF){ //if no ints in string or nothing entered
	selection = 0;
      }
    }
    switch(selection){
    case 1:add(headptr, tailptr);
      break;
    case 2: delete(headptr, tailptr);
      break;
    case 3: printForwards(*headptr);
      break;
    case 4: printBackwards(*tailptr);
      break;
    case 5:
      break;
    default: printf("Invalid selection!\n");
      break;
    }
  }

}


/* Allocates memory for mp3 node, prompts user to enter data, and returns node.
 * If year is invalid or not between MINYEAR, MAXYEAR, year prompt repeats until valid
 */ 
mp3_t* createNode(){
  mp3_t* newNode = calloc(1,sizeof(mp3_t));
  char buffer[BUFFERSIZE] = {'\0'};
  int len;

  printf("Enter song title:");
  if(fgets(buffer, BUFFERSIZE, stdin) != NULL){ 
    len = strlen(buffer);
    newNode->title = calloc(len, sizeof(char));
    strncpy(newNode->title, buffer, len-1);
  }
  
  printf("Enter the artist's name:");
  if(fgets(buffer, BUFFERSIZE, stdin) != NULL){ 
    len = strlen(buffer);
    newNode->artist = calloc(len, sizeof(char));
    strncpy(newNode->artist, buffer, len-1);
  }

  printf("Enter year published:");
  int scanCheck;
  int year = MINYEAR-1;
  while(year < MINYEAR || year > MAXYEAR){
    if(fgets(buffer, BUFFERSIZE, stdin) != NULL){ 
      scanCheck = sscanf(buffer, "%d", &year);
      if(scanCheck == 0 || scanCheck == EOF || year < MINYEAR || year > MAXYEAR){ 
	year = MINYEAR-1;
	printf("Invalid year for an MP3\nPlease enter a year from %d - %d\n", MINYEAR, MAXYEAR);
      }else{
	newNode->year = year;
      }
    }
  }
  
  return newNode;
}


//note: must use pass-by-reference, ptr to ptr to change actual ptr values
void add( mp3_t** headptr, mp3_t** tailptr ){
  mp3_t* newNode = createNode();
  if(*headptr == NULL){
    *headptr = newNode;
    *tailptr = newNode;
    printf("Added new node - ");
    printNode(newNode);
  }else{
    (*tailptr)->next = newNode;
    newNode->prev = *tailptr;
    *tailptr = newNode;
  }
}

//reuseable print format
void printNode(mp3_t* node){
  printf("Title: %s, Artist: %s, Year:%d \n", node->title, node->artist, node->year);
}

//note: don't **, pass-by-value doesn't change head
void printForwards(mp3_t* head){
  if(head == NULL){
    printf("No MP3s in list!\n");
  }else{
    printf("Printed from front to back: \n");
  }
   while(head != NULL){
    printNode(head);
    head = head->next;
  }
}

void printBackwards(mp3_t* tail){
  if(tail == NULL){
    printf("No MP3s in list!\n");
  }else{
    printf("Printed from back to front: \n");
  }
  while(tail != NULL){
    printNode(tail);
    tail = tail->prev;
  }
}  

//deletes a single node that has already been found in list
void deleteNode(mp3_t** headptr, mp3_t** tailptr, mp3_t* del){
  if(*headptr == NULL || *tailptr == NULL || del == NULL){
    return;
  }
  if(*headptr == del){
    *headptr = del->next;
  }
  if(*tailptr == del){ //must account for tailptr
    *tailptr = del->prev;
  }
  if(del->prev != NULL){
    del->prev->next = del->next;
  }
  if(del->next != NULL){
    del->next->prev = del->prev;
  }
  free(del->artist);
  free(del->title);
  free(del);
    
}


/*
 *asks for artist name, finds mp3's with artist and calls deleteNode on them
 *if no nodes or name not found, exits function
 */
void delete(mp3_t** headptr, mp3_t** tailptr){
  char buffer[BUFFERSIZE] = {'\0'};
  char* artistName;
  if(*headptr == NULL){
    printf("Please add an MP3 before deleting!");
    return;
  }else{
    printf("Which artist's MP3s do you want to delete?");
    if(fgets(buffer, BUFFERSIZE, stdin) != NULL){ 
      int len = strlen(buffer);
      artistName = calloc(len,sizeof(char));
      strncpy(artistName, buffer, len-1); //don't copy last enter, but need full length for null term    
    }
  }
  bool flag = false;
  mp3_t* curr = *headptr;
  mp3_t* next = NULL;
  while(curr != NULL){
    if(strcmp(curr->artist, artistName) == 0 ){
      next = curr->next;
      deleteNode(headptr, tailptr, curr);
      curr = next;     
      flag = true;
    }else{
      curr = curr->next;
    }
  }
  
  if(flag == true){
    printf("Songs from %s have been deleted\n", artistName);
  }else if(flag == false){
    printf("Artist %s not found!\n", artistName);
  }
  
  free(artistName);
}

//loops through all nodes and calls deleteNode on them
void deleteList(mp3_t** headptr, mp3_t** tailptr){
  mp3_t* curr = *headptr;
  mp3_t* next = NULL;
  while(curr != NULL){
    next = curr->next;
    deleteNode(headptr, tailptr, curr);
    curr = next;
  }
  *headptr = NULL;
  *tailptr = NULL;
}

//called on exit, frees all dynamically allocated mem (aka the list)
void cleanup(){
  printf("\nCleaning up list...\n");
  deleteList(&head, &tail);
  printf("head: %p, tail: %p\nExiting...\n", head, tail);
} 
