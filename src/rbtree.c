#include "rbtree.h"

#include <stdlib.h>

///RBTree 생성
rbtree *new_rbtree(void) {
  rbtree *t = (rbtree *)calloc(1, sizeof(rbtree));

  //트리에 nil 노드 초기화
  node_t *nil = (node_t *)calloc(1, sizeof(node_t));
  nil->color = RBTREE_BLACK;

  //초기 생성시 트리의 root와 nil이 단일 nil을 가리키도록
  t->root = t->nil = nil;
  return t;
}

///RBTree 삭제
void delete_rbtree(rbtree *t) {
  //reclaim the tree nodes's memory
  free(t->nil);
  free(t);
}

///노드 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *y = t->nil;
  node_t *x = t->root;
  node_t *z = (node_t *)calloc(1, sizeof(node_t));
  z->key = key;

  while(x != t->nil) {
    y = x;
    if (z->key < x->key) {
      x = x->left;
    } else {
      x = x->right;
    }
  }
  z->parent = y;
  if(y == t->nil) {
    t->root = z;
  } 
  else if (z->key < y->key) {
    y->left = z;
  }
  else {
    y->right = z;
  }
  z->left = t->nil;
  z->right = t->nil;
  z->color = RBTREE_RED;

  rbtree_insert_fix(t, z);
  return z;
}
//노드 삽입 후 검사
void rbtree_insert_fix(rbtree *t, node_t *z) {
  while (z->parent->color == RBTREE_RED) {
    if(z->parent == z->parent->parent->left) 
    {
      node_t *y = z->parent->parent->right;
      //case 1
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        //case 2
        if(z == z->parent->right){
          z = z->parent;
          rbtree_left_rotate(t, z);
        }
        //case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rbtree_right_rotate(t, z->parent->parent);
      }
    }
    else 
    {
      node_t *y = z->parent->parent->left;
      //case 1
      if (y->color == RBTREE_RED)
      {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      }
      else
      {
        //case 2
        if(z == z->parent->left){
          z = z->parent;
          rbtree_right_rotate(t, z);
        }
        //case 3
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        rbtree_left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;
}

///좌측 회전
void rbtree_left_rotate(rbtree *t, node_t *x) {
  node_t *y = x->right;
  x->right = y->left;
  if (y->left != t->nil) { 
    y->left->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  }
  else if (x == x->parent->left) {
    x->parent->left = y;
  } 
  else {  
    x->parent->right = y;
  }
  y->left = x;
  x->parent = y;
}
//우측 회전
void rbtree_right_rotate(rbtree *t, node_t *x) {
  node_t *y = x->left;
  x->left = y->right;
  if (y->right != t->nil) { 
    y->right->parent = x;
  }
  y->parent = x->parent;
  if (x->parent == t->nil) {
    t->root = y;
  }
  else if (x == x->parent->right) {
    x->parent->right = y;
  } 
  else {  
    x->parent->left = y;
  }
  y->right = x;
  x->parent = y;
}

///찾는 키 값에 해당하는 노드 찾기
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *root = t->root;
  
  //루트 노드가 nil노드가 같으면 종료 후 NULL 리턴
  while(root != t->nil) {
    //루트 노드의 키 값이 찾는 키값과 같다면 바로 리턴
    if (key == root->key) return root;

    //찾는 키 값이 루트 노드의 키 값보다 작다면 왼쪽 서브 트리로 이동
    if (key < root->key) {
      root = root->left;
    } 
    //찾는 키 값이 루트 노드의 키 값보다 크다면 오른쪽 서브 트리로 이동
    if (key > root->key) {
      root = root->right;
    } 
  }
  return NULL;
}

///트리의 최솟값에 해당하는 노드 찾기
node_t *rbtree_min(const rbtree *t) {
  node_t *root = t->root;
  while (root->left != t->nil)
  {
    root = root->left;
  }
  return root;
}

///트리의 최댓값에 해당하는 노드 찾기
node_t *rbtree_max(const rbtree *t) {
  node_t *root = t->root;
  while (root->right != t->nil)
  {
    root = root->right;
  }
  return root;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  return 0;
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  // TODO: implement to_array 123
  return 0;
}
