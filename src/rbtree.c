#include "rbtree.h"

#include <stdlib.h>

rbtree *new_rbtree(void) {
  rbtree *p = (rbtree *)calloc(1, sizeof(rbtree));
  // TODO: initialize struct if needed
  node_t *NIL = (node_t *)calloc(1, sizeof(node_t));
  NIL->color = RBTREE_BLACK;
  p->root = NIL;
  p->nil = NIL;
  return p;
}

void delete_node(rbtree *t, node_t *node) {
  if (node == t->nil) {                     // 현재 노드가 트리 nil노드와 같으면 그냥 return
    return;
  }
  delete_node(t, node->left);               // 왼쪽 재귀 호출
  delete_node(t, node->right);              // 오른쪽 재귀 호출
  free(node);                               // 현재 노드가 가리키는 공간 할당 해제
  node = NULL;                              // 노드값 NULL로 초기화
  return;
}

void delete_rbtree(rbtree *t) {
  // TODO: reclaim the tree nodes's memory
  if (t == NULL) {
    return;                                 // 트리 없다면 return
  }
  delete_node(t, t->root);                  // 생성된 노드가 가리키는 공간 할당 해제
  free(t->nil);                             // 트리의 nil노드가 가리키는 공간 할당 해제
  t->nil = NULL;                            // 트리의 nil노드 값 NULL로 초기화 
  free(t);                                  // 트리가 가리키는 공간 할당 해제
  t = NULL;                                 // 트리 값 NULL로 초기화
  return;
}

void left_rotate(rbtree *t, node_t *x){
  node_t *y = x->right;                     // y를 x의 오른쪽 자식으로 세팅
  x->right = y->left;                       // y 왼쪽 자녀를 x의 오른쪽으로 옮김

  if(y->left != t->nil) {                   // y 왼쪽 자녀 닐노드 아니라면
    y->left->parent = x;                    // 그 왼쪽 자녀의 부모값을 x로
  }
  y->parent = x->parent;                    // x부모를 y에 연결

  if (x->parent == t->nil) {                // x가 root인 경우 (부모가 nil)
    t->root = y;                            // 트리 루트 자리에 y
  
  } else if (x == x->parent->left) {        // x부모가 x 오른쪽 위에 있을 경우
    x->parent->left = y;                    // 방향 맞춰서 x부모 자녀에 y

  } else {                                  // x부모가 x 왼쪽 위에 있을 경우
    x->parent->right = y;                   // 방향 맞춰서 x부모 자녀에 y
  }
  y->left = x;                              // x를 y 왼쪽 자식으로
  x->parent = y;                            // y를 x의 부모로
  return;
}

void right_rotate(rbtree *t, node_t *x){    // left rotate 반대로
  node_t *y = x->left;
  x->left = y->right;

  if(y->right != t->nil){
    y->right->parent = x;
  }
  y->parent = x->parent;

  if (x->parent == t->nil){
    t->root = y;

  }else if (x == x->parent->right){
    x->parent->right = y;

  }else{
    x->parent->left = y;
  }

  y->right = x;
  x->parent = y;
  return;
}

void rbtree_insert_fixup(rbtree *t, node_t *z) {
  while (z->parent->color == RBTREE_RED) {           // z부모가 RED일 때,

    if (z->parent == z->parent->parent->left) {      // z부모가 z조부모의 왼쪽자식이라면,
      node_t *y = z->parent->parent->right;          // y는 z조부모의 오른쪽 자식(z삼촌)
      
      if (y->color == RBTREE_RED) {                  // case 1. z삼촌의 색이 RED일 때,
        z->parent->color = RBTREE_BLACK;             // z부모 색을 BLACK으로 변경
        y->color = RBTREE_BLACK;                     // z삼촌 색을 BLACK으로 변경 
        z->parent->parent->color = RBTREE_RED;       // z조부모 색을 RED로 변경 
        z = z->parent->parent;                       // z를 z조상으로 변경
      
      } else {                                       // case 2. z삼촌 색이 BLACK일 때,
        if (z == z->parent->right) {                 // z가 z부모의 오른쪽 자식일때
          z = z->parent;                             // z를 z부모로 변경
          left_rotate(t, z);                         // z를 기준으로 좌회전
        }
                                                     // case 3. z삼촌 색 BLACK, z가 z부모의 왼쪽 자식일 때
        z->parent->color = RBTREE_BLACK;             // z부모 색을 BLACK으로
        z->parent->parent->color = RBTREE_RED;       // z조부모 색을 RED로
        right_rotate(t, z->parent->parent);          // z조부모 기준으로 우회전
      }

    } else {                                         // z부모가 z조부모 오른쪽 자식이라면, 반대로
      node_t *y = z->parent->parent->left;
      if (y->color == RBTREE_RED) {
        z->parent->color = RBTREE_BLACK;
        y->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        z = z->parent->parent;
      } else {
          if (z == z->parent->left) {
            z = z->parent;
            right_rotate(t, z);
        }
        z->parent->color = RBTREE_BLACK;
        z->parent->parent->color = RBTREE_RED;
        left_rotate(t, z->parent->parent);
      }
    }
  }
  t->root->color = RBTREE_BLACK;                     // 특성 2 유지 위해 root는 BLACK으로
  return;
}

node_t *rbtree_insert(rbtree *t, const key_t key) {
  // TODO: implement insert
  node_t *y = t->nil;                                // nil노드 y노드 생성
  node_t *x = t->root;                               // root노드 x노드 생성
  while (x != t->nil) {                              // x가 nil노드 아니면 계속 탐색
    y = x;                                           // y를 x값으로 갱신
    if (key < x->key) {                              // 삽입할 키 값이 x의 key 값보다 작으면,
      x = x->left;                                   // 왼쪽 아래로
    } else {                                         // 삽입할 key 값이 더 크거나 같으면,
      x = x->right;                                  // 오른쪽 아래로
    }
  }
  node_t *z = (node_t *)calloc(1, sizeof(node_t));   // z노드 생성
  z->parent = y;                                     // z부모로 y 넣음
  if (y == t->nil) {                                 // y가 트리 nil일 때, (첫 노드 삽입)
    t->root = z;                                     // 트리 루트에 z
  } else if (key < y->key) {                         // y의 key값이 삽입할 키 값보다 작을 때
    y->left = z;                                     // y 왼쪽 자식에 z 넣음
  } else {                                           // y의 key값이 삽입할 키 값보다 크거나 같을 때
    y->right = z;                                    // y 오른쪽 자식에 z
  }
  z->key = key;                                      // z의 key값에 현재 key
  z->color = RBTREE_RED;                             // z의 color는 red, 삽입할때 처음에 무조건 red
  z->left = t->nil;                                  // z 왼쪽 자식에 트리 nil
  z->right = t->nil;                                 // z 오른쪽 자식에 트리 nil, 삽입 때 주모건 자식은 nil임
  rbtree_insert_fixup(t, z);                         // fixup 호출
  return t->root;
}

node_t *rbtree_find(const rbtree *t, const key_t key) {
  // TODO: implement find
  node_t *x = t->root;
  while (x != t->nil && x->key != key) {      // nil노드까지 내려가면서 x의 키값이 키값과 일치할 때까지 돌림 
    if (x->key < key) {
      x = x->right;
    } else {
      x = x->left;
    }
  }
  if (x == t->nil) {                          // nil노드까지 갔는데 못찾음
    return NULL;                              // NULL 반환
  }
  return x;                                   // 찾은 주소 반환
}

node_t *rbtree_min(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;          // x는 트리의 루트
  while (x->left != t->nil) {   // x 왼쪽 자식이 nil이 아닐 때
    x = x->left;                // x를 x 왼쪽 자식으로
  }
  return x;                     // x 반환
}

node_t *rbtree_max(const rbtree *t) {
  // TODO: implement find
  node_t *x = t->root;          // x는 트리의 루트
  while (x->right != t->nil) {  // x 오른쪽 자식이 nil이 아닐 때
    x = x->right;               // x는 x의 오른쪽 자식
  }
  return x;                     // x 반환
}

void rbtree_transplant(rbtree *t, node_t *u, node_t *v) { // u 부모와 v 연결
  if (u->parent == t->nil) {                              // u부모 nil일때, 삭제할 노드가 트리 root일 때
    t->root = v;                                          // 트리 root를 v로

  } else if (u == u->parent->left) {                      // u가 u부모 왼쪽 자식일 때
    u->parent->left = v;                                  // u부모 왼쪽 자식을 v로

  } else {                                                // u가 u부모 오른쪽 자식일 때
    u->parent->right = v;                                 // u부모 오른쪽 자식을 v로
  }
  v->parent = u->parent;                                  // v부모를 u부모로
  return;
}

node_t *rbtree_successor(rbtree *t, node_t *x) {    // 직후 원소 찾기, x는 삭제할 노드 오른쪽 자식
  node_t *y = x;                           // y에 x 넣음
  while (y->left != t->nil) {              // y 왼쪽 자식이 nil 아닐 때 계속
    y = y->left;                           // y를 y 왼쪽 자식으로
  }
  return y;                                // y반환
}

void rb_delete_fixup(rbtree *t, node_t *x) {
  node_t *w;
  while ((x != t->root) && (x->color == RBTREE_BLACK)) {                          // 삭제 노드 자식이 루트가 아니고 색깔이 BLACK일 때 계속

    if (x == x->parent->left) {                                                   // case 1. 삭제 노드 자식이 삭제 노드 부모 왼쪽 자식일 때
      w = x->parent->right;                                                       // w를 x 오른쪽 형제로
      if (w->color == RBTREE_RED) {                                               // w 색깔이 RED일 때
        w->color = RBTREE_BLACK;                                                  // w 색깔 BLACK으로
        x->parent->color = RBTREE_RED;                                            // x 부모 색 RED로
        left_rotate(t, x->parent);                                                // x 부모 기준으로 좌회전
        w = x->parent->right;                                                     // ws를 x 오른쪽 형제로
      }
      if (w->left->color == RBTREE_BLACK && w->right->color == RBTREE_BLACK) {    // case 2. w 왼쪽, 오른쪽 자식이 둘다 BLACK일 때 
        w->color = RBTREE_RED;                                                    // w 색 RED로
        x = x->parent;                                                            // x를 x부모로 변경
      } else {
        if (w->right->color == RBTREE_BLACK) {                                    // case 3. case 2 아닐 때, w 오른쪽 자식 BLACK일 때,
          w->left->color = RBTREE_BLACK;                                          // w 왼쪽 자식 색 BLACK으로 변경
          w->color = RBTREE_RED;                                                  // w 색 RED로 변경
          right_rotate(t, w);                                                     // w 기준으로 우회전
          w = x->parent->right;                                                   // w를 x부모 오른쪽 자식으로
        }
        w->color = x->parent->color;                                              // case 4. case 2 아닐 때, w 오른쪽 자식 RED일 때,
        x->parent->color = RBTREE_BLACK;                                          // x부모 색을 BLACK으로
        w->right->color = RBTREE_BLACK;                                           // w 오른쪽 자식 색을 BLACK으로
        left_rotate(t, x->parent);                                                // x부모 기준으로 좌회전
        x = t->root;                                                              // x를 트리 루트로
      }
    } else {                                                                      // 삭제 노드 자식이 삭제 노드 부모 오른쪽 자식일 때, 왼쪽일 때 반대
      w = x->parent->left;
      if (w->color == RBTREE_RED) {
        w->color = RBTREE_BLACK;
        x->parent->color = RBTREE_RED;
        right_rotate(t, x->parent);
        w = x->parent->left;
      }
      if (w->right->color == RBTREE_BLACK && w->left->color == RBTREE_BLACK) {
        w->color = RBTREE_RED;
        x = x->parent;
      } else {
        if (w->left->color == RBTREE_BLACK) {
          w->right->color = RBTREE_BLACK;
          w->color = RBTREE_RED;
          left_rotate(t, w);
          w = x->parent->left;
        }
        w->color = x->parent->color;
        x->parent->color = RBTREE_BLACK;
        w->left->color = RBTREE_BLACK;
        right_rotate(t, x->parent);
        x = t->root;
      }
    }
  }
  x->color = RBTREE_BLACK;                                                        // 트리 루트 BLACK으로
  return;
}

int rbtree_erase(rbtree *t, node_t *p) {
  // TODO: implement erase
  node_t *x;
  node_t *y = p;                            // y는 삭제할 노드
  color_t y_color = y->color;               // y_color는 y의 색

  if (p->left == t->nil) {                  // 삭제할 노드 왼쪽 자식이 nil노드일 때,
    x = p->right;                           // x를 삭제할 노드 오른쪽 자식으로
    rbtree_transplant(t, p, p->right);      // 삭제할 노드의 부모와 삭제할 노드 오른쪽 자식 연결

  } else if (p->right == t->nil) {          // 삭제할 노드 오른쪽 자식이 nil노드일 때,
    x = p->left;                            // x를 삭제할 노드 왼쪽 자식으로
    rbtree_transplant(t, p, p->left);       // 삭제할 노드 부모와 삭제할 노드 왼쪽 자식 연결

  } else {                                  // 삭제할 노드 왼쪽, 오른쪽 자식 둘 다 있을 때
    y = rbtree_successor(t, p->right);      // y는 직후 노드(?)
    y_color = y->color;                     // y_color를 직후 원소의 색으로
    x = y->right;                           // x를 y 오른쪽 자식으로

    if (y->parent == p) {                   // y부모가 삭제할 노드일 때
      x->parent = y;                        // x부모를 y로

    } else {                                // y부모가 삭제할 노드 아닐 때,
      rbtree_transplant(t, y, y->right);    // y부모와 y오른쪽 자식 연결
      y->right = p->right;                  // y 오른쪽 자식을 삭제할 노드의 오른쪽 자식으로
      y->right->parent = y;                 // y의 오른쪽 자식의 부모를 y로
    }
    rbtree_transplant(t, p, y);             // 삭제할 노드 부모와 y 연결
    y->left = p->left;                      // y 왼쪽 자식을 삭제할 노드 왼쪽 자식으로
    y->left->parent = y;                    // y 왼쪽 자식의 부모를 y로
    y->color = p->color;                    // y 색 삭제할 노드의 색으로
  }
  free(p);                                  // 삭제한 노드 가리키는 공간 메모리 해제
  p = NULL;                                 // 할당 해제 후 삭제한 노드값 NULL로 초기화
  if (y_color == RBTREE_BLACK) {            // y_color가 BLACK일 떄 (삭제한 노드 색 BLACK일 때) 특성 5 위반
    rb_delete_fixup(t, x);                  // 노드 색 바꿈
  }
  return 0;
}

int rbtree_inorder(node_t *nil, node_t *root, key_t *arr, const size_t n, int index) { // 트리 중위 순회
  if (root == nil) {                                        // 루트가 nil노드일 때
    return index;                                           // index 반환
  }
  if (index == n) {                                         // index가 n일 때
    return index;                                           // index 반환
  }
  index = rbtree_inorder(nil, root->left, arr, n, index);   // root노드 왼쪽 자식을 루트로 해서 재귀
  if (index < n) {                                          // index가 n보다 작을 때,
    arr[index++] = root->key;                               // arr[현재 index]에 현재 노드의 키 값
  }
  index = rbtree_inorder(nil, root->right, arr, n, index);  // root노드 오른쪽 자식을 루트로 재귀
  return index;                                             // index 반환
}

int rbtree_to_array(const rbtree *t, key_t *arr, const size_t n) { // 오름차 순 구현
  // TODO: implement to_array
  if (t->root == t->nil) {  // 트리 루트가 nil노드일 때, 종료
    return 0;
  }
  rbtree_inorder(t->nil, t->root, arr, n, 0); // 트리 중위 순회
  return 0;
}
