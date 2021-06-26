#include "SafeTrash.h"

typedef struct st_node_t {
  pointer ptr;
  struct st_node_t* next;
} st_node;

st_node* head;

void st_init() {
  /*
  head = (st_node *) malloc(sizeof(st_node));
  head->ptr = NULL;
  head->next = NULL;
  */
}

void st_free(st_node* node) {
  /*
  if(node->ptr)
  free(node->ptr);
  free(node);
  */
}

void st_track(pointer ptr) {
  /*
  st_node * current = head;
  while (current->next) {
    current = current->next;
  }
  current->next = (st_node *) malloc(sizeof(st_node));
  current->next->ptr = ptr;
  current->next->next = NULL;
  */
}

void st_untrack(pointer ptr) {
  /*
  st_node * current = head;
  while (current->next) {

    if(current->next->ptr == ptr) {
      st_node * free_me = current->next;
      current->next = free_me->next;
      free(free_me);
      return;
    }

    current = current->next;
  }
  */
}

void st_clean() {
  /*
  st_node* current = head;
  st_node* next = head->next;
  while(current) {
    next = current->next;
    st_free(current);
    current = next;
  }
  */
}
