#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  //RBTree 생성
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));

  //트리에 nil 노드 초기화
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;

  //초기 생성시 트리의 root와 nil이 단일 nil을 가리키도록
  t->root = t->nil = nil;
  return t;
}

void delete_rbtree(rbtree *t) {
  //reclaim the tree nodes's memory
  free(t->nil);
  free(t);
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  return t->root;
}

////////

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array 123
  return 0;
}
