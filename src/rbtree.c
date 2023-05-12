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
  if (t->root != t->nil)
    delete_node(t, t->root);
  free(t->nil);
  free(t);
}
void delete_node(rbtree *t, node_t *node) {
  //nil 노드에 도달할 때까지 재귀로 타고 내려가서 노드 삭제
  if (node->left != t->nil)
    delete_node(t, node->left);
  if (node->right != t->nil)
    delete_node(t, node->right);
  free(node);
}

///노드 삽입
node_t *rbtree_insert(rbtree *t, const key_t key) {
  node_t *p = t->nil;
  node_t *current = t->root;
  //입력 받은 key를 가지는 노드 생성
  node_t *node = (node_t *)calloc(1, sizeof(node_t));
  node->key = key;

  ///위에서부터 key 값이 들어갈 위치 탐색
  while (current != t->nil) {
    //들어갈 위치를 찾았을 때 해당 위치의 부모를 알기 위해 캐싱 
    p = current;  
    if (node->key < current->key) {
      current = current->left;
    } else {
      current = current->right;
    }
  }
  //새 node가 들어갈 위치의 부모와 연결
  node->parent = p;
  if (p == t->nil) 
    t->root = node;
  else if (node->key < p->key) 
    p->left = node;
  else 
    p->right = node;

  node->left = t->nil;
  node->right = t->nil;
  node->color = RBTREE_RED;  //삽입되는 node는 항상 RED

  rbtree_insert_fix(t, node);
  return node;
}

///노드 삽입 후 검사
void rbtree_insert_fix(rbtree *t, node_t *node) {
  ///삽입한 노드의 부모가 red일 경우 -> #4 속성을 위반
  while (node->parent->color == RBTREE_RED) {
    //할아버지 기준 왼쪽에 있을 때
    if (node->parent == node->parent->parent->left) 
    {
      //case 1 - 부모의 형제 노드가 red일 경우
      if (node->parent->parent->right->color == RBTREE_RED)
      {
        //부모와 부모형제(=삼촌)의 RED를 한데모아 할아버지에게 전달
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->right->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        //할아버지가 새로운 RED가 되었으니 #4를 위반하는지 다시 검사
        node = node->parent->parent;
      }
      else
      {
        //case 2 - 부모의 형제 노드가 black이며 꺾인 형태일 경우
        //부모를 기준으로 회전하여 뻗은 형태로 바꿔주기 
        if (node == node->parent->right) {
          node = node->parent;
          rbtree_left_rotate(t, node);
        }
        //case 3 - 부모의 형제 노드가 black이며 뻗은 형태일 경우
        //부모와 할아버지의 색을 바꾸고 회전
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        rbtree_right_rotate(t, node->parent->parent);
      }
    }
    //할아버지 기준 오른쪽에 있을 때
    else 
    {
      //case 1
      if (node->parent->parent->left->color == RBTREE_RED)
      {
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->left->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        node = node->parent->parent;
      }
      else
      {
        //case 2
        if (node == node->parent->left) {
          node = node->parent;
          rbtree_right_rotate(t, node);
        }
        //case 3
        node->parent->color = RBTREE_BLACK;
        node->parent->parent->color = RBTREE_RED;
        rbtree_left_rotate(t, node->parent->parent);
      }
    }
  }
  //#2 속성을 위반하는 경우 -> root의 색 black으로 변경
  t->root->color = RBTREE_BLACK;
}

///좌측 회전
void rbtree_left_rotate(rbtree *t, node_t *node) {
  node_t *new_p = node->right;
  node->right = new_p->left;

  //위로 올라갈 노드의 왼쪽 자식이 있을 경우
  if (new_p->left != t->nil) 
    new_p->left->parent = node;
  
  //new_p와 node->parent 상호 연결
  new_p->parent = node->parent;
  if (node->parent == t->nil) 
    t->root = new_p;
  else if (node == node->parent->left) 
    node->parent->left = new_p;
  else
    node->parent->right = new_p;

  //위로 올라간 new_p와 왼쪽 아래로 이동한 node 상호연결
  new_p->left = node;
  node->parent = new_p;
}
//우측 회전
void rbtree_right_rotate(rbtree *t, node_t *node) {
  node_t *new_p = node->left;
  node->left = new_p->right;
  
  //위로 올라갈 노드의 오른쪽 자식이 있을 경우
  if (new_p->right != t->nil)
    new_p->right->parent = node;
  
  //new_p와 node->parent 상호 연결
  new_p->parent = node->parent;
  if (node->parent == t->nil)
    t->root = new_p;
  else if (node == node->parent->right) 
    node->parent->right = new_p;
  else 
    node->parent->left = new_p;

  //위로 올라간 new_p와 오른쪽 아래로 이동한 node 상호연결
  new_p->right = node;
  node->parent = new_p;
}

///찾는 키 값에 해당하는 노드 찾기
node_t *rbtree_find(const rbtree *t, const key_t key) {
  node_t *root = t->root;
  while (root != t->nil) {
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

///노드 삭제시 삭제하려는 노드의 부모(target->parent)와 대체할 노드(replace)를 서로 연결
void rbtree_transplant(rbtree *t, node_t *target, node_t *replace) {
  if (target->parent == t->nil) {
    t->root = replace;
  }  
  //삭제하려는 노드가 왼쪽 자식인지 오른쪽 자식인지 판단
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
  node_t *child = t->nil;

  ///삭제하려는 노드의 자식이 0개 또는 1개일 때 - 자식 노드가 이어받는다 
  ///양쪽 모두 자식이 없다면 nil 노드가 대체한다
  if (target->left == t->nil) {
    child = target->right;
    rbtree_transplant(t, target, child);
  }
  else if(target->right == t->nil) {
    child = target->left;
    rbtree_transplant(t, target, child);
  }
  //삭제하려는 노드의 자식이 2개일 때
  else {
    //대체 노드를 successor 노드로
    node_t *replace = rbtree_find_successor(t, target->right); 
    erased_color = replace->color;  //삭제할 노드의 색을 successor의 색으로 변경
    child = replace->right;  //successor 노드의 자식을 캐싱

    ///대체 노드의 부모와 그 오른쪽 자식을 연결
    rbtree_transplant(t, replace, child);
    //target의 부모와 대체 노드를 연결
    rbtree_transplant(t, target, replace);

    ///대체 노드와 target의 자식들을 연결
    replace->left = target->left;
    replace->right = target->right;
    target->left->parent = replace;
    target->right->parent = replace;

    ///대체 노드는 기존 자신의 색깔을 버리고 삭제할 노드의 색을 이어받음
    replace->color = target->color;
  }
  // 삭제되는 색이 BLACK이라면 extra black을 처리해줄 추가 작업
  if (erased_color == RBTREE_BLACK) {
    rbtree_erase_fix(t, child);
  }
  //삭제할 노드 메모리 동적 할당 해제
  free(target);
  return 0;
}

///노드 삭제 후 검사
void rbtree_erase_fix(rbtree *t, node_t *node) {
  /// node->color = black은 doubly black
  /// node->color = red은 red-and-black
  /// 노드가 root거나 red-and-black이라면 탈출
  while (node != t->root && node->color == RBTREE_BLACK) {
    //부모 기준 왼쪽에 있을 때
    if(node == node->parent->left) 
    {
      node_t *sibling = node->parent->right;
      //case 1 - doubly black의 형제가 red일 경우
      if(sibling->color == RBTREE_RED) {
        //부모와 형제의 색을 변경
        sibling->color = RBTREE_BLACK;
        node->parent->color = RBTREE_RED;
        //부모를 기준으로 회전
        rbtree_left_rotate(t, node->parent);
        sibling = node->parent->right;
      }
      //case 2,3,4 - doubly black의 형제가 black
      //case 2 - 형제의 자식이 모두 black일 경우
      if (sibling->left->color == RBTREE_BLACK && sibling->right->color == RBTREE_BLACK) {
        // doubly black과 그 형제의 black을 모아 부모에게 전달
        sibling->color = RBTREE_RED;
        node = node->parent;
      } else {
        //case 3 - 형제의 red 자식이 꺾인 형태일 경우
        if (sibling->right->color == RBTREE_BLACK) {
          //형제와 형제의 자식의 색을 변경
          sibling->left->color = RBTREE_BLACK;
          sibling->color = RBTREE_RED;
          //형제를 기준으로 회전하여 뻗은 형태로 만들기
          rbtree_right_rotate(t, sibling);
          sibling = node->parent->right;
        }
        //case 4 - 형제의 red 자식이 뻗은 형태일 경우
        sibling->color = node->parent->color;
        node->parent->color = RBTREE_BLACK;
        sibling->right->color = RBTREE_BLACK;
        rbtree_left_rotate(t, node->parent);
        node = t->root;
      }
    } 
    //부모 기준 오른쪽에 있을 때
    else 
    {
      node_t *sibling = node->parent->left;
      //case 1
      if(sibling->color == RBTREE_RED) {
        sibling->color = RBTREE_BLACK;
        node->parent->color = RBTREE_RED;
        rbtree_right_rotate(t, node->parent);
        sibling = node->parent->left;
      }
      //case 2
      if (sibling->right->color == RBTREE_BLACK && sibling->left->color == RBTREE_BLACK) {
        sibling->color = RBTREE_RED;
        node = node->parent;
      } else {
        //case 3
        if (sibling->left->color == RBTREE_BLACK) {
          sibling->right->color = RBTREE_BLACK;
          sibling->color = RBTREE_RED;
          rbtree_left_rotate(t, sibling);
          sibling = node->parent->left;
        }
        //case 4
        sibling->color = node->parent->color;
        node->parent->color = RBTREE_BLACK;
        sibling->left->color = RBTREE_BLACK;
        rbtree_right_rotate(t, node->parent);
        node = t->root;
      }
    }
  }
  //노드가 root거나 red-and-black일 경우 -> black으로 변경
  node->color = RBTREE_BLACK;  
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
