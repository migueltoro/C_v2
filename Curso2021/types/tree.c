/*
 * binary_tree.c
 *
 *  Created on: 5 nov. 2018
 *      Author: migueltoro
 */


#include "tree.h"

heap heap_binary_tree;
heap heap_tree;

void ini_heap_trees(){
	heap_binary_tree = heap_empty();
	heap_tree  = heap_empty();
}

void heap_trees_clear(){
	heap_clear(&heap_binary_tree);
	heap_clear(&heap_tree);
}


binary_tree * binary_tree_empty_me(heap * hp) {
	binary_tree tree = {Empty_Binary_Tree,&null_type,NULL,NULL,NULL};
	return (binary_tree *) heap_copy(&tree,hp,sizeof(binary_tree));
}

binary_tree * binary_tree_leaf_me(void * label, type * label_type, heap * hp) {
	void * lb = heap_copy(label,hp,label_type->size);
	binary_tree tree = {Leaf_Binary_Tree,label_type,lb,NULL,NULL};
	return (binary_tree *) heap_copy(hp,&tree,sizeof(binary_tree));
}

binary_tree * binary_tree_new_me(void * label, type * label_type, binary_tree * left,  binary_tree * right, heap * hp){
	void * lb = heap_copy(label,hp,label_type->size);
	binary_tree tree = {Binary_Tree,label_type,lb,left,right};
	return (binary_tree *) heap_copy(hp,&tree,sizeof(binary_tree));
}

binary_tree * binary_tree_empty() {
	return binary_tree_empty_me(&heap_binary_tree);
}

binary_tree * binary_tree_leaf(void * label, type * type_label) {
	return binary_tree_leaf_me(label, type_label, &heap_binary_tree);
}

binary_tree * binary_tree_new(void * label,type * type_label, binary_tree * left, binary_tree * right) {
	return binary_tree_new_me(label,type_label,left,right,&heap_binary_tree);
}

token label_parse(tokens * tks) {
		token t = next_token(tks);
		switch (t.type) {
		case Operator:
			if(strcmp(t.text_token,"+") == 0 || strcmp(t.text_token,"-") ==0) {
				char  op[20];
				strcpy(op,t.text_token);
				token_class tps[] = {Integer,Double};
				t = match_token_types(tps,2,tks);
				char  nb[20];
				strcpy(nb,t.text_token);
				strcpy(t.text_token,op);
				strcat(t.text_token,nb);
			} else {
				error_token(&t);
			}
			break;
		case Integer:
		case Double:
		case Identifier:
			break;
		default: error_token(&t);
		}
		return t;
}

binary_tree * binary_tree_parse_private(tokens * tks, heap * hp) {
	binary_tree * r;
	token t = label_parse(tks);
	if (strcmp(t.text_token, "_") == 0) {
		r = binary_tree_empty_me(hp);
	} else {
		r = binary_tree_leaf_me(t.text_token, &string_fix_type, hp);
		token tn = see_next_token(tks);
		char * ln = tn.text_token;
		if (has_more_tokens(tks) && strcmp(ln, "(") == 0) {
			char tt1[][5] = { "(" };
			match_token_texts((char*) tt1, 5, 1, tks);
			binary_tree * left = binary_tree_parse_private(tks, hp);
			char tt2[][5] = { "," };
			match_token_texts((char*) tt2, 5, 1, tks);
			binary_tree * right = binary_tree_parse_private(tks, hp);
			char tt3[][5] = { ")" };
			match_token_texts((char*) tt3, 5, 1, tks);
			r = binary_tree_new_me(t.text_token, &string_fix_type, left, right, hp);
		}
	}
	return r;
}

binary_tree * binary_tree_parse_me(char * text, heap * hp) {
	tokens tks = get_tokens(text);
	binary_tree * tree = binary_tree_parse_private(&tks, hp);
	if (has_more_tokens(&tks)) {
		token t = current_token(&tks);
		error_token(&t);
	}
	return tree;
}

binary_tree* binary_tree_parse(char * text) {
	return binary_tree_parse(text);
}


binary_tree* binary_tree_parse_f(binary_tree * out, char * text) {
	binary_tree* r =binary_tree_parse(text);
	*out = *r;
	return r;
}


binary_tree_subtype binary_tree_get_subtype(const binary_tree * tree) {
	return tree->tree_type;
}

void * binary_tree_label(const binary_tree * tree){
	check_argument(tree->tree_type != Empty_Binary_Tree,__FILE__,__LINE__,"el árbol es vacío");
	return tree->label;
}

binary_tree * binary_tree_left(const binary_tree * tree){
	check_argument(tree->tree_type == Binary_Tree,__FILE__,__LINE__,"el árbol no es binario");
	return tree->left;
}

binary_tree * binary_tree_right(const binary_tree * tree){
	check_argument(tree->tree_type == Binary_Tree,__FILE__,__LINE__,"el árbol no es binario");
	return tree->right;
}

bool binary_tree_is_empty(const binary_tree * tree){
	return tree->tree_type == Empty_Binary_Tree;
}

bool binary_tree_is_leaf(const binary_tree * tree){
	return tree->tree_type == Leaf_Binary_Tree;
}

bool binary_tree_is_binary(const binary_tree * tree){
	return tree->tree_type == Binary_Tree;
}

int binary_tree_size(const binary_tree * tree){
	int r;
	switch(tree->tree_type){
	case Empty_Binary_Tree: r = 0; break;
	case Leaf_Binary_Tree: r = 1; break;
	case Binary_Tree: r = 1 + binary_tree_size(binary_tree_left(tree))+ binary_tree_size(binary_tree_right(tree));
	}
	return r;
}

int binary_tree_height(const binary_tree * tree){
	int r;
	switch(tree->tree_type){
	case Empty_Binary_Tree: r = 0; break;
	case Leaf_Binary_Tree: r = 0; break;
	case Binary_Tree: r = 1 + MAX(
			binary_tree_height(binary_tree_left(tree)),
			binary_tree_height(binary_tree_right(tree)));
	}
	return r;
}

void binary_tree_toconsole(const binary_tree * tree) {
	type * label_type = tree->label_type;
	switch (tree->tree_type) {
	char mem[256];
	case Empty_Binary_Tree:
		printf("_");
		break;
	case Leaf_Binary_Tree:
		printf("%s",tostring(binary_tree_label(tree),mem,label_type));
		break;
	case Binary_Tree:
		printf("%s",tostring(binary_tree_label(tree), mem,label_type));
		printf("(");
		binary_tree_toconsole(binary_tree_left(tree));
		printf(",");
		binary_tree_toconsole(binary_tree_right(tree));
		printf(")");
	}
}

char* binary_tree_tostring_private(const binary_tree * tree, char * out) {
	type * label_type = tree->label_type;
	char mem[256];
	switch (binary_tree_get_subtype(tree)) {
	case Empty_Binary_Tree:
		strcat(out, "_");
		break;
	case Leaf_Binary_Tree:
		strcat(out, tostring(binary_tree_label(tree), mem,label_type));
		break;
	case Binary_Tree:
		strcat(out, tostring(binary_tree_label(tree), mem,label_type));
		strcat(out, "(");
		binary_tree_tostring_private(binary_tree_left(tree), out);
		strcat(out, ",");
		binary_tree_tostring_private(binary_tree_right(tree), out);
		strcat(out, ")");
	}
	return out;
}

char* binary_tree_tostring(const binary_tree * tree, char * out) {
	out[0] = '\0';
	return binary_tree_tostring_private(tree,out);
}

bool binary_tree_equals(const binary_tree * e1, const binary_tree * e2) {
	type * label_type = e1->label_type;
	bool res=false;
	switch (binary_tree_get_subtype(e1)) {
	case Empty_Binary_Tree:
		res = binary_tree_get_subtype(e2)==Empty_Binary_Tree;
		break;
	case Leaf_Binary_Tree:
		res = binary_tree_get_subtype(e2)==Leaf_Binary_Tree && equals(e1->label,e2->label,label_type);
		break;
	case Binary_Tree:
		res = binary_tree_get_subtype(e2)==Binary_Tree && equals(e1->label,e2->label,label_type) &&
		binary_tree_equals(binary_tree_left(e1), binary_tree_left(e2)) &&
		binary_tree_equals(binary_tree_right(e1), binary_tree_right(e2));
	}
	return res;
}


void binary_tree_to_list_private(const binary_tree * tree, list * ls) {
	switch (tree->tree_type) {
	case Empty_Binary_Tree: break;
	case Leaf_Binary_Tree: list_add_pointer(ls,binary_tree_label(tree)); break;
	case Binary_Tree:
		list_add_pointer(ls,binary_tree_label(tree));
		binary_tree_to_list_private(binary_tree_left(tree),ls);
		binary_tree_to_list_private(binary_tree_right(tree),ls);
	}
}

list binary_tree_to_list(const binary_tree * tree){
	list ls = list_empty(tree->label_type);
	binary_tree_to_list_private(tree,&ls);
	return ls;
}

binary_tree* binary_tree_map_me(binary_tree* tree_in, type * tipo_out,void* fmap(void* out, const void* in), heap* hp) {
	binary_tree * res = NULL;
	binary_tree * left = NULL;
	binary_tree * right = NULL;
	void *etq_in, *etq_out;
	char mem[tipo_out->size];
	switch (binary_tree_get_subtype(tree_in)) {
	case Empty_Binary_Tree:
		res = binary_tree_empty_me(hp);
		break;
	case Leaf_Binary_Tree:
		etq_in = binary_tree_label(tree_in);
		etq_out = fmap(mem, etq_in);
		res = binary_tree_leaf_me(etq_out, tipo_out, hp);
		break;
	case Binary_Tree:
		etq_in = binary_tree_label(tree_in);
		etq_out = fmap(mem, etq_in);
		left = binary_tree_map(binary_tree_left(tree_in), tipo_out, fmap);
		right = binary_tree_map(binary_tree_right(tree_in), tipo_out, fmap);
		res = binary_tree_new_me(etq_out, tipo_out, left, right, hp);
	}
	return res;
}

binary_tree* binary_tree_map(binary_tree* t, type * tipo_out, void* fmap(void* out, const void* in)) {
	if(heap_isnull(&heap_binary_tree)) heap_binary_tree = heap_empty();
	return binary_tree_map(t,tipo_out,fmap);
}

type binary_tree_type = {"binary_tree",binary_tree_equals, binary_tree_tostring, NULL, binary_tree_parse_f,free_0,copy_0,sizeof(binary_tree),1,NULL};


///////////////////////////////

tree * tree_empty_me(heap * hp) {
	tree t = {Empty_Tree,&null_type,NULL,0,NULL};
	return (tree *) heap_copy(hp,&t,sizeof(tree));
}

tree * tree_leaf_me(void * label, type * label_type, heap * hp) {
	void * lb = heap_copy(hp,label,label_type->size);
	tree t = {Leaf_Tree,label_type,lb,0,NULL};
	return (tree *) heap_copy(hp,&t,sizeof(tree));
}

tree * tree_new_me(void * label, type * label_type, int num_children, tree ** children, heap * hp){
	void * lb = heap_copy(hp,label,label_type->size);
	tree ** ch = heap_copy(hp,children,num_children*sizeof(tree *));
	tree t = {Nary_Tree,label_type,lb,num_children,ch};
	return (tree *) heap_copy(hp,&t,sizeof(tree));
}

tree * tree_empty() {
	return tree_empty_me(&heap_tree);
}

tree * tree_leaf(void * label, type * type_label) {
	return tree_leaf_me(label,type_label,&heap_tree);
}

tree * tree_new(void * label, type * type_label, int num_children, tree ** children) {
	return tree_new_me(label,type_label,num_children,children,&heap_tree);
}

tree_subtype tree_get_subtype(const tree * tree) {
	return tree->tree_type;
}

void * tree_label(const tree * tree){
	check_argument(tree->tree_type != Empty_Tree,__FILE__,__LINE__,"el árbol es vacío");
	return tree->label;
}

void * tree_label_f(void * out, const tree * tree){
	void * label = tree_label(tree);
	copy_size(out,label,tree->label_type->size);
	return out;
}

bool tree_is_empty(tree * t){
	return t->tree_type == Empty_Tree;
}

bool tree_is_not_empty(tree * t){
	return t->tree_type != Empty_Tree;
}

bool tree_is_leaf(tree * t){
	return t->tree_type == Leaf_Tree;
}

bool tree_is_nary(tree * t){
	return t->tree_type == Nary_Tree;
}

int tree_child_number(const tree * tree) {
	return tree->num_children;
}

tree * tree_get_child(const tree * tree, int child){
	check_argument(tree->tree_type == Nary_Tree,__FILE__,__LINE__,"el árbol no es nario");
	check_argument(child < tree->num_children,__FILE__,__LINE__,"el árbol no tiene ese hijo");
	return tree->children[child];
}

tree * tree_parse_private_me(tokens * tks, heap * hp) {
	tree * r;
	token t = label_parse(tks);
	if (strcmp(t.text_token, "_") == 0) {
		r = tree_empty_me(hp);
	} else {
		r = tree_leaf_me(t.text_token, &string_fix_type, hp);
		token tn = see_next_token(tks);
		char * ln = tn.text_token;
		if (has_more_tokens(tks) && strcmp(ln, "(") == 0) {
			char tt1[][5] = { "(" };
			match_token_texts((char*) tt1, 5, 1, tks);
			tree * children[10];
			tree * tt = tree_parse_private_me(tks, hp);
			int nh = 0;
			children[nh] = tt;
			nh++;
			char tt2[][5] = { "," };
			tn = see_next_token(tks);
			ln = tn.text_token;
			while(has_more_tokens(tks) && strcmp(ln, ",") == 0){
				match_token_texts((char*) tt2, 5, 1, tks);
				tt = tree_parse_private_me(tks, hp);
				children[nh] = tt;
				nh++;
				tn = see_next_token(tks);
				ln = tn.text_token;
			}
			char tt3[][5] = { ")" };
			match_token_texts((char*) tt3, 5, 1, tks);
			r = tree_new_me(t.text_token, &string_fix_type, nh, children, hp);
		}
	}
	return r;
}

tree * tree_parse_me(char * text, heap * hp) {
	tokens tks = get_tokens(text);
	tree * tree = tree_parse_private_me(&tks, hp);
	if (has_more_tokens(&tks)) {
		token t = current_token(&tks);
		error_token(&t);
	}
	return tree;
}

tree* tree_parse(char * text) {
	if(heap_isnull(&heap_tree)) heap_tree = heap_empty();
	return tree_parse(text);
}

tree* tree_parse_f(tree * out, char * text) {
	if(heap_isnull(&heap_tree)) heap_tree = heap_empty();
	tree* r =tree_parse(text);
	*out = *r;
	return r;
}

int tree_size(const tree * tree){
	int r;
	switch(tree->tree_type){
	case Empty_Tree: r = 0; break;
	case Leaf_Tree: r = 1; break;
	case Nary_Tree:
		r = 0;
		for (int i=0; i < tree->num_children; i++){
			r = r + tree_size(tree_get_child(tree,i));
		}
		r = 1 + r;
		break;
	}
	return r;
}


int tree_height(const tree * tree){
	int r;
	switch(tree->tree_type){
	case Empty_Tree: r = 0; break;
	case Leaf_Tree: r = 0; break;
	case Nary_Tree:
		r = -100;
		for (int i=0; i < tree->num_children; i++){
			r = MAX(r,tree_height(tree_get_child(tree,i)));
		}
		r = 1 + r;
		break;
	}
	return r;
}

list * tree_to_list_private(const tree * tree, list * ls) {
	switch (tree->tree_type) {
	case Empty_Tree: break;
	case Leaf_Tree: list_add_pointer(ls,binary_tree_label(tree)); break;
	case Nary_Tree:
		list_add_pointer(ls,tree_label(tree));
		for(int i=0; i < tree_child_number(tree); i++) {
			tree_to_list_private(tree_get_child(tree,i),ls);
		}
	}
	return ls;
}

list tree_to_list_preorder(const tree * tree){
	list ls = list_empty(tree->label_type);
	tree_to_list_private(tree,&ls);
	return ls;
}

void tree_toconsole(const tree * tree) {
	type * label_type = tree->label_type;
	switch (tree->tree_type) {
	char mem[256];
	case Empty_Tree:
		printf("_");
		break;
	case Leaf_Tree:
		printf("%s",tostring(tree_label(tree), mem,label_type));
		break;
	case Nary_Tree:
		printf("%s",tostring(tree_label(tree), mem,label_type));
		printf("(");
		tree_toconsole(tree_get_child(tree,0));
		for(int i = 1; i<tree->num_children;i++){
			printf(",");
			tree_toconsole(tree_get_child(tree,i));
		}
		printf(")");
	}
}

char* tree_tostring_private(const tree * tree, char * out) {
	type * label_type = tree->label_type;
	char mem[256];
	switch (tree_get_subtype(tree)) {
	case Empty_Tree:
		strcat(out, "_");
		break;
	case Leaf_Tree:
		strcat(out,tostring(tree_label(tree), mem, label_type));
		break;
	case Nary_Tree:
		strcat(out,tostring(tree_label(tree), mem, label_type));
		strcat(out, "(");
		tree_tostring_private(tree_get_child(tree, 0), out);
		for (int i = 1; i < tree_child_number(tree); i++) {
			strcat(out, ",");
			tree_tostring_private(tree_get_child(tree, i), out);
		}
		strcat(out, ")");
	}
	return out;
}

char* tree_tostring(const tree * tree, char * out) {
	out[0] = '\0';
	return tree_tostring_private(tree,out);
}

bool tree_equals(const tree * e1, const tree * e2) {
	type * label_type = e1->label_type;
	bool res=false;
	switch (tree_get_subtype(e1)) {
	case Empty_Tree:
		res = tree_get_subtype(e2)==Empty_Tree;
		break;
	case Leaf_Tree:
		res = tree_get_subtype(e2)==Leaf_Tree && equals(e1->label,e2->label,label_type);
		break;
	case Nary_Tree:
		res = tree_get_subtype(e2)==Nary_Tree && equals(e1->label,e2->label,label_type);
		int i=0, n = e1->num_children;
		while(res && i<n && n==e2->num_children) {
			res = tree_equals(tree_get_child(e1, i), tree_get_child(e2, i));
		}
	}
	return res;
}

tree* tree_map(tree* tree_in, type * tipo_out, void* fmap(void* out, const void* in)) {
	tree *res = NULL;
	void * etq_in;
	void * etq_out;
	char mem[tipo_out->size];
	switch (tree_get_subtype(tree_in)) {
	case Empty_Tree:
		res = tree_empty();
		break;
	case Leaf_Tree:
		etq_in = tree_label(tree_in);
		etq_out = fmap(mem, etq_in);
		res = tree_leaf(etq_out, tipo_out);
		break;
	case Nary_Tree:
		etq_in = tree_label(tree_in);
		etq_out = fmap(mem, etq_in);
		tree* children[tree_in->num_children];
		for (int i = 0; i < tree_in->num_children; i++) {
			children[i] = tree_map(tree_get_child(tree_in, i), tipo_out, fmap);
		}
		res = tree_new(etq_out, tipo_out,tree_in->num_children,children);
	}
	return res;

}


list tree_next_level(list * ls) {
	list r = list_empty(&tree_type);
	int ls_s = list_size(ls);
	for (int j = 0; j < ls_s; j++) {
		tree * t = (tree *) list_get(ls, j);
		int nh = tree_child_number(t);
		for (int h = 0; h < nh; h++) {
			list_add(&r, tree_get_child(t, h));
		}
	}
	return r;
}

list tree_level(tree * t, int n) {
	list r = list_empty(&tree_type);
	list_add(&r, t);
	list s;
	for(int i = 1;i <= n;i++) {
		s = tree_next_level(&r);
		list_free(&r);
		r = s;
	}
	return r;
}

type tree_type = {"tree",tree_equals,tree_tostring,NULL,tree_parse_f,free_0,copy_0,sizeof(tree),1,NULL};

void test_binary_tree_1(){
	printf("Binary Tree test\n");
//	heap hp =  heap_empty();
	ini_heap_trees();
	char mem[500];
	int a = 84;
	int b = 90;
	int c = 56;
	int d = 81;
	int e = 55;

	binary_tree * t0 = binary_tree_empty();
	binary_tree * t1 = binary_tree_leaf(&c,&int_type);
	binary_tree * t2 = binary_tree_new(&a,&int_type,t0,t1);
	binary_tree * tl = binary_tree_leaf(&e,&int_type);
	binary_tree * t3 = binary_tree_new(&b,&int_type,t2,tl);
	binary_tree * t4 = binary_tree_new(&d,&int_type,t3,t3);
	printf("size = %d\n", binary_tree_size(t4));
	printf("t4 height = %d\n",binary_tree_height(t4));
	list ls = binary_tree_to_list(t4);
	char * s = list_tostring(&ls,mem);
	printf("ls = %s\n", s);
	list_quick_sort(&ls,int_naturalorder);
	s = list_tostring(&ls, mem);
	printf("ls = %s\n\n", s);
	binary_tree_toconsole(t4);
	printf("\n\n");
}



void test_binary_tree_2(){
	printf("Binary Tree test 2\n\n");
//	memory_heap_binary_tree =  memory_heap_create();
	ini_heap_trees();
	char mem[1500];
	int a = 84;
	int b = 90;
	int c = 56;
	int d = 81;
	int e = 55;

	binary_tree * t0 = binary_tree_empty();
	binary_tree * t1 = binary_tree_leaf(&c,&int_type);
	binary_tree * t2 = binary_tree_new(&a,&int_type,t0,t1);
	binary_tree * tl = binary_tree_leaf(&e,&int_type);
	binary_tree * t3 = binary_tree_new(&b,&int_type,t2,tl);
	binary_tree * t4 = binary_tree_new(&d,&int_type,t3,t3);
	printf("size = %d\n\n", binary_tree_size(t4));
	list ls = binary_tree_to_list(t4);
	char * s = list_tostring(&ls, mem);
	printf("ls = %s\n", s);
	list_quick_sort(&ls,int_naturalorder);
	s = list_tostring(&ls, mem);
	printf("ls = %s\n\n", s);
	binary_tree_toconsole(t4);
	binary_tree_tostring(t4,mem);
	printf("\n\ntree t4 = %s\n", mem);
	binary_tree * t5 = binary_tree_map(t4,&int_type,square_int_f);
	binary_tree_tostring(t5,mem);
	printf("\n\ntree t5 = %s\n\n", mem);
	heap_clear(&heap_binary_tree);
}

void test_tree_1() {
	printf("Tree test\n\n");
	ini_heap_trees();
	int a = 84;
	int b = 90;
	int c = 56;
	int d = 81;
	int e = 55;

	tree *t0 = tree_empty();
	tree *t1 = tree_leaf(&c, &int_type);
	tree * ch1[] = {t0, t1};
	tree *t2 = tree_new(&a, &int_type, 2, ch1);
	tree *tl = tree_leaf(&e, &int_type);
	tree * ch2[] = {t2, tl};
	tree *t3 = tree_new(&b, &int_type, 2, ch2);
	tree * ch3[] = {t3, t3};
	tree *t4 = tree_new(&d, &int_type, 2, ch3);
	printf("size = %d\n", tree_size(t4));
	printf("size = %d, height = %d\n",tree_size(t4),tree_height(t4));
	tree * t5 = tree_map(t4,&int_type,square_int_f);
	tree_toconsole(t4);
	printf("__________\n");
	tree_toconsole(t5);
	printf("\n\n");
}



void test_tree_2() {
	printf("Tree test\n\n");
//	memory_heap_tree = memory_heap_create();
	ini_heap_trees();
	char mem[1500];
	int a = 84;
	int b = 90;
	int c = 56;
	int d = 81;
	int e = 55;

	tree *t0 = tree_empty();
	tree *t1 = tree_leaf(&c, &int_type);
	tree * ch1[] = {t0, t1};
	tree *t2 = tree_new(&a, &int_type, 2, ch1);
	tree *tl = tree_leaf(&e, &int_type);
	tree * ch2[] = {t2, tl};
	tree *t3 = tree_new(&b, &int_type, 2, ch2);
	tree * ch3[] = {t3, t3};
	tree *t4 = tree_new(&d, &int_type, 2, ch3);
	printf("size = %d\n\n", tree_size(t4));
	tree_toconsole(t4);
	tree_tostring(t4,mem);
	printf("\n\ntree t4 = %s\n", mem);
	tree * t5 = tree_map(t4,&int_type,square_int_f);
	tree_tostring(t5,mem);
	printf("\n\ntree t5 = %s\n\n", mem);
	heap_free(&heap_tree);
}

void test_parse_binary_tree_1(){
//	heap hp = heap_empty();
	ini_heap_trees();
	binary_tree * tree = binary_tree_parse("-24.7(-34.5,-51(_,57))");

	binary_tree_toconsole(tree);
	binary_tree * tree2 = binary_tree_parse("0(_,-1)");
	binary_tree_toconsole(tree2);
	binary_tree * tree3 = binary_tree_parse("12(-34,56(-78,_))");
	binary_tree_toconsole(tree3);

}

void test_parse_binary_tree_2(){
//	memory_heap_clear(&memory_heap_binary_tree);
	ini_heap_trees();
	binary_tree * tree = binary_tree_parse("-24.7(-34.5,-51(_,57))");
	binary_tree * tree_m = binary_tree_map(tree,&int_type,int_type.parse);
	binary_tree_toconsole(tree_m);
	printf("\n");
	binary_tree * tree2 = binary_tree_parse("0(_,-1)");
	binary_tree_toconsole(tree2);
	printf("\n");
	binary_tree * tree3 = binary_tree_parse("12(-34,56(-78,_))");
	binary_tree_toconsole(tree3);
	printf("\n");

}

void test_parse_tree_1(){
//	heap hp = heap_empty();
	ini_heap_trees();
	tree * tree = tree_parse("-24.7(34.5,-51(33,56(57)),-51(33,57),-51(33,57))");
	tree_toconsole(tree);

}

void test_parse_tree_2(){
//	memory_heap_clear(&memory_heap_tree);
	ini_heap_trees();
	tree * t = tree_parse("-24.7(34.5,-51(33,56(57)),-51(33,57),-51(33,57))");
	tree * t_m = tree_map(t,&double_type,double_type.parse);
	tree * t_m_2 = tree_map(t_m,&double_type,square_double_f);
	tree_toconsole(t_m_2);

}
