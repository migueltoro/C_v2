/*
 * binary_tree.h
 *
 *  Created on: 5 nov. 2018
 *      Author: migueltoro
 */

#ifndef TREE_H_
#define TREE_H_


#include "../types/list.h"
#include "../types/types.h"
#include "../types/tokenizer.h"
#include "../types/memory_heap.h"
#include "../types/math2.h"

typedef enum{Empty_Binary_Tree, Leaf_Binary_Tree, Binary_Tree} binary_tree_subtype;
typedef enum{Empty_Tree, Leaf_Tree, Nary_Tree} tree_subtype;

typedef struct bt {
	binary_tree_subtype tree_type;
	type * label_type;
	void * label;
	struct bt * left;
	struct bt * right;
}binary_tree;



typedef struct tr {
	tree_subtype tree_type;
	type * label_type;
	void * label;
	int num_children;
	struct tr ** children;
}tree;

extern memory_heap memory_heap_tree;
extern memory_heap memory_heap_binary_tree;

binary_tree * binary_tree_empty(memory_heap * hp);
binary_tree * binary_tree_leaf(void * label, type * type_label, memory_heap * hp);
binary_tree * binary_tree_new(void * label,type * type_label, binary_tree * left, binary_tree * right, memory_heap * hp);

binary_tree * binary_tree_empty_m();
binary_tree * binary_tree_leaf_m(void * label, type * type_label);
binary_tree * binary_tree_new_m(void * label,type * type_label, binary_tree * left, binary_tree * right);

bool binary_tree_is_empty(const binary_tree * tree);
bool binary_tree_is_leaf(const binary_tree * tree);
bool binary_tree_is_binary(const binary_tree * tree);

binary_tree_subtype binary_tree_get_subtype(const binary_tree * tree);
void * binary_tree_label(const binary_tree * tree);
binary_tree * binary_tree_left(const binary_tree * tree);
binary_tree * binary_tree_right(const binary_tree * tree);
list binary_tree_to_list(const binary_tree * tree);
int binary_tree_size(const binary_tree * tree);
void binary_tree_toconsole(const binary_tree * tree);

binary_tree * binary_tree_parse(char * text, memory_heap * hp);
binary_tree * binary_tree_parse_m(char * text);
binary_tree * binary_tree_parse_m_f(binary_tree * t, char * text);
char* binary_tree_tostring(const binary_tree * e, char * out);
bool binary_tree_equals(const binary_tree * e1, const binary_tree * e2);
int binary_tree_naturalorder(const binary_tree * e1,const binary_tree * e2);

binary_tree* binary_tree_map(binary_tree* tree_in, type * tipo_out, void* fmap(void* out, const void* in), memory_heap* hp);
binary_tree* binary_tree_map_m(binary_tree* tree_in, type * tipo_out, void* fmap(void* out, const void* in));

extern type binary_tree_type;


tree * tree_empty(memory_heap * hp);
tree * tree_leaf(void * label, type * type_label, memory_heap * hp);
tree * tree_new(void * label, type * type_label, int num_children, tree ** children, memory_heap * hp);

tree * tree_empty_m();
tree * tree_leaf_m(void * label, type * type_label);
tree * tree_new_m(void * label, type * type_label, int num_children, tree ** children);

bool tree_is_empty(tree * t);
bool tree_is_not_empty(tree * t);
bool tree_is_leaf(tree * t);
bool tree_is_nary(tree * t);

tree_subtype tree_get_subtype(const tree * tree);
void * tree_label(const tree * tree);
void * tree_label_f(void * out, const tree * tree);
tree * tree_get_child(const tree * tree, int i);
int tree_child_number(const tree * tree);
list tree_to_list_preorder(const tree * tree);
int tree_size(const tree * tree);
int tree_height(const tree * tree);
void tree_toconsole(const tree * tree);

tree * tree_parse(char * text, memory_heap * hp);
tree* tree_parse_m(char * text);
tree* tree_parse_m_f(tree * t,char * text);
char* tree_tostring(const tree * e, char * out);
bool tree_equals(const tree * e1, const tree * e2);
int tree_naturalorder(const tree * e1,const tree * e2);

tree* tree_map(tree* t, type * tipo_out, void* fmap(void* out, const void* in), memory_heap* hp);
tree* tree_map_m(tree* t, type * tipo_out, void* fmap(void* out, const void* in));
list tree_level(tree * t, int n);
list tree_next_level(list * level);

extern type tree_type;

void test_binary_tree_1();
void test_binary_tree_2();
void test_tree_1();
void test_tree_2();
void test_parse_binary_tree_1();
void test_parse_binary_tree_2();
void test_parse_tree_1();
void test_parse_tree_2();

#endif /* TREE_H_ */
