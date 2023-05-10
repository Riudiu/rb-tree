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
  if(t->root != t->nil)
    delete_node(t, t->root);
  free(t->nil);
  free(t);
}
void delete_node(rbtree *t, node_t *node) {
  if (node->left != t->nil)
    delete_node(t, node->left);
  if (node->right != t->nil)
    delete_node(t, node->right);
  free(node);
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

///노드 삽입 후 검사
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

///해당 노드의 successor 노드 찾기, root -> 타겟 노드의 오른쪽 자식
node_t *rbtree_find_successor(const rbtree *t, node_t *root) {
  while (root->left != t->nil)
    root = root->left;
  return root;
}

///노드 삭제시 삭제하려는 노드의 부모와 삭제하려는 노드의 자리를 대체할 노드를 서로 연결
void rbtree_transplant(rbtree *t, node_t *target, node_t *replace) {
  //삭제하려는 노드가 root일 경우 -> 대체 노드가 root 노드
  if (target->parent == t->nil) {
    t->root = replace;
  }  
  //삭제하려는 노드가 왼쪽 자식인지 오른쪽 자식인지 판단 후 
  //삭제하려는 노드의 부모가 대체 노드를 가리키도록
  else if(target == target->parent->left) {
    target->parent->left = replace;
  }
  else {
    target->parent->right = replace;
  }
  //대체 노드가 삭제하려는 노드의 부모를 가리키도록
  replace->parent = target->parent;
}

///노드 삭제, target - 삭제할 노드
int rbtree_erase(rbtree *t, node_t *target) {
  color_t erased_color = target->color;  //삭제할 노드의 색
  node_t *replace_child = t->nil;

  ///삭제하려는 노드의 자식이 0개 또는 1개일 때 - 자식 노드가 이어받는다 
  if (target->left == t->nil) {
    //왼쪽 자식에 nil 노드가 있다면 오른쪽 자식을 대체 노드로, 자식이 없다면 nil 노드가 대체 노드로
    replace_child = target->right;
    rbtree_transplant(t, target, replace_child);
  }
  else if(target->right == t->nil) {
    replace_child = target->left;
    rbtree_transplant(t, target, replace_child);
  }
  ///삭제하려는 노드의 자식이 2개일 때
  else {
    //대체 노드를 successor 노드로
    node_t *replace = rbtree_find_successor(t, target->right); 
    erased_color = replace->color;  //삭제할 노드의 색을 successor의 색으로 변경
    replace_child = replace->right;

    ///대체하려는 노드가 오른쪽 자식 노드를 가질 경우를 대비해 대체 노드의 부모와 오른쪽 자식을 연결
    rbtree_transplant(t, replace, replace_child);
    //successor(target)을 대체 노드로 원래 삭제하려던 노드의 부모와 연결
    rbtree_transplant(t, target, replace);

    ///대체 노드와 삭제할 노드의 자식들을 연결
    replace->left = target->left;
    replace->right = target->right;
    target->left->parent = replace;
    target->right->parent = replace;

    ///대체 노드는 기존 자신의 색깔을 버리고 삭제할 노드의 색을 이어받음
    replace->color = target->color;
  }
  // 삭제되는 색이 BLACK이라면 extra black을 처리해줄 추가 작업
  if (erased_color == RBTREE_BLACK) {
    rbtree_erase_fix(t, replace_child);
  }
  //삭제할 노드 메모리 동적 할당 해제
  free(target);
  return 0;
}

///노드 삭제 후 검사
void rbtree_erase_fix(rbtree *t, node_t *x) {
  while (x != t->root && x->color == RBTREE_BLACK) {
    if(x == x->parent->left) 
    {
      node_t *w = x->parent->right;
      //case 1
      if(w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rbtree_left_rotate(t, x->parent);
        w = x->parent->right;
      }
      //case 2
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        //case 3
        if (w->right->color == RBTREE_BLACK) {
          w->left->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rbtree_right_rotate(t, w);
          w = x->parent->right;
        }
        //case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->right->color = RBTREE_BLACK;
        rbtree_left_rotate(t, x->parent);
        x = t->root;
      }
    } 
    else 
    {
      node_t *w = x->parent->left;
      //case 1
      if(w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        rbtree_right_rotate(t, x->parent);
        w = x->parent->left;
      }
      //case 2
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        //case 3
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          rbtree_left_rotate(t, w);
          w = x->parent->left;
        }
        //case 4
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        rbtree_right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;  
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) {
  int idx = 0;
  in_order_traverse(t, arr, t->root, &idx);
  return 0;
}

void in_order_traverse(const rbtree *t, key_t *arr, node_t *node, int* idx) {
  if (node->left != t->nil) 
    in_order_traverse(t, arr, node->left, idx);
  arr[*idx] = node->key;
  (*idx)++;
  if (node->right != t->nil) 
    in_order_traverse(t, arr, node->right, idx);
}
