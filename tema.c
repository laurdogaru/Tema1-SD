#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct node {
    char caracter;
    struct node *next;
    struct node *prev;
} node;

typedef struct cuvant {
	node *head;
	node *tail;
	struct cuvant *next;
	struct cuvant *prev;
	int len;
} cuvant;

typedef struct linie {
    cuvant *head;
    cuvant *tail;
    struct linie *next;
    struct linie *prev;
    int len;
} linie;

typedef struct text {
    linie *head;
    linie *tail;
    int len;
} text;

typedef struct node_stack {
	char *comanda;
	struct node_stack *next;
} node_stack;

typedef struct Stack {
	struct node_stack *top;
	int len;
} Stack;

Stack *undo_stack,*redo_stack;
text *t_final;

void init_stack(Stack **s) {
	*s = malloc(sizeof(Stack));
	(*s)->len = 0;
	(*s)->top = NULL;
}

void init_text(text **t) {
	*t = malloc(sizeof(text));
	(*t)->len = 0;
	(*t)->head = NULL;
	(*t)->tail = NULL;
}

int add_stack(Stack *s,char *c) {

	node_stack *q;
	q = (node_stack*)malloc(sizeof(node_stack));
	if( q == NULL ) {
		return 0;
	}
	q->comanda = c;
	q->next = s->top;
	s->top = q;
	s->len++;
	return 1;
}
char *pop_stack(Stack *s) {
	char *aux;
	node_stack *q;
	if( s->top == NULL ) {
		printf("stiva vida\n");
		return 0;
	}
	aux = s->top->comanda;
	q = s->top;
	s->top = s->top->next;
	free(q);
	s->len--;
	return aux;
}


void free_stack( Stack **s) {
	node_stack *q = (*s)->top;
	while( q != NULL ) {
		(*s)->top = (*s)->top->next;
		free(q);
		q = (*s)->top;
	}
}



void init_cuvant(cuvant **c) {
	*c = malloc(sizeof(cuvant));
	(*c)->len = 0;
	(*c)->head = NULL;
	(*c)->tail = NULL;
}

void init_line(linie  **l) {
    *l = malloc(sizeof(linie));
    (*l)->len = 0;
    (*l)->head = NULL;
    (*l)->tail = NULL;
}

int add_char_to_cuvant(cuvant *list, int n, char c) {
	node *nou =(node *)malloc(sizeof(node));
    nou->caracter = c;
    if (n < 0 || n > list->len) {
        return -1;
	} else if(list->len==0) {
        nou->next = NULL;
        nou->prev = NULL;
        list->head = nou;
        list->tail = nou;
        list->len++;
        return 1;
    } else if(n == 0 && list->head != NULL) {
        list->head->prev = nou;
        nou->next = list->head;
        nou->prev = NULL;
        list->head = nou;
        list->len++;
        return 1;
    } else if(n == list->len && list->tail != NULL) {
        list->tail->next = nou;
        nou->prev = list->tail;
        nou->next = NULL;
        list->tail = nou;
        list->len++;
        return 1;
    } else {
        node *q;
        int m = n;
        if(n < list->len/2) {
            q = list->head;
            while(m - 1 > 0) {
                q=q->next;
                m--;
            }
            nou->next = q->next;
            nou->prev = q;
            q->next->prev = nou;
            q->next = nou;
            list->len++;
        } else {
            q = list->tail;
            m = list->len - n;
            while(m > 0) {
                q = q->prev;
                m--;
            }
            nou->next = q->next;
            nou->prev = q;
            q->next->prev = nou;
            q->next = nou;
            list->len++;
        }
        return 1;
    }
}

int add_cuvant_to_line(linie *l, int n, cuvant *c) {
	cuvant *nou;
	init_cuvant(&nou);
	nou->head = c->head;
	nou->tail = c->tail;
	nou->len = c->len;
	if(n < 0 || n > l->len) {
		return -1;
	} else if(l->len == 0) {
		nou->next = NULL;
		nou->prev = NULL;
		l->head = nou;
		l->tail = nou;
		l->len++;
		return 1;
	} else if(n == 0 && l->head != NULL) {
		l->head->prev = nou;
		nou->next = l->head;
		nou->prev = NULL;
		l->head = nou;
		l->len++;
		return 1;
	} else if(n == l->len && l->tail != NULL) {
		l->tail->next = nou;
		nou->prev = l->tail;
		nou->next = NULL;
		l->tail = nou;
		l->len++;
		return 1;
	} else {
		cuvant *q;
		int m = n;
		if(n < l->len / 2) {
			q = l->head;
			while (m - 1 > 0) {
				q = q->next;
				m--;
			}
			nou->next = q->next;
			nou->prev = q;
			q->next->prev = nou;
			q->next = nou;
			l->len++;	
		} else {
			q = l->tail;
			m = l->len - n;
			while(m > 0) {
				q = q->prev;
				m--;
			}
			nou->next = q->next;
			nou->prev = q;
			q->next->prev = nou;
			q->next = nou;
			l->len++;
		}
		return 1;
	}
}

int add_line_to_text(text *t, int n, linie *l) {
	linie *nou;
	init_line(&nou);
	nou->head = l->head;
	nou->tail = l->tail;
	nou->len = l->len;
	if(n < 0 || n > t->len) {
		return -1;
	}
	else if(t->len == 0) {
		nou->next = NULL;
		nou->prev = NULL;
		t->head = nou;
		t->tail = nou;
		t->len++;
		return 1;
	} else if(n == 0 && t->head != NULL) {
		t->head->prev = nou;
		nou->next = t->head;
		nou->prev = NULL;
		t->head = nou;
		t->len++;
		return 1;
	} else if(n == t->len && t->tail != NULL) {
		t->tail->next = nou;
		nou->prev = t->tail;
		nou->next = NULL;
		t->tail = nou;
		t->len++;
		return 1;
	} else {
		linie *q;
		int m = n;
		if(n < t->len/2) {
			q = t->head;
			while(m - 1 > 0) {
				q = q->next;
				m--;
			}
			nou->next = q->next;
			nou->prev = q;
			q->next->prev = nou;
			q->next = nou;
			t->len++;
		}
		return 1;
	}
}

/*functie pentru cazul cand cursorul nu ramane la sfarsitul ultimului
text adaugat*/
void add_in_middle(text *t, node *cursor, linie *l) {
	linie *lin = t->head;
	cuvant *cuv;
	node *nod;
	int aux = 0, i = 0;
	while(lin != NULL) {
		cuv = lin->head;
		i = 0;
		while(cuv != NULL) {
			nod = cuv->head;
			while(nod != NULL) {
				if( nod == cursor->next) {
					aux = 1;
					break;
				}
				i++;
				nod = nod->next;
			}
			if(aux == 1) {
				break;
			}
			cuv = cuv->next;
		}
		if(aux == 1) {
			break;
		}
		lin = lin->next;
	}
	cuvant *cuv_aux = l->tail;
	node *nod_aux;
	while(cuv_aux != NULL)
	{
		nod_aux = cuv_aux->tail;
		while(nod_aux != NULL)
		{
			if(nod_aux->caracter!='\n') {
				add_char_to_cuvant(cuv, i, nod_aux->caracter);
			}
			nod_aux = nod_aux->prev;
		}
		cuv_aux = cuv_aux->prev;
	}
}


node *remove_char(node *n,cuvant *cuv) {
	node *q,*p;
	if(n->prev == NULL && n->next == NULL) {
        q = n;
        node *r = q;
        cuv->head = cuv->tail = NULL;
        free(q);
        cuv->len--;
        return r;
    } else if(n->prev == NULL) {
		q = n;
		node *r = q;
		cuv->head = cuv->head->next;
		cuv->head->prev = NULL;
		free(q);
		cuv->len--;
		return r;
	} else if(n->next == NULL) {
		q = n;
		node *r = q;
		cuv->tail = cuv->tail->prev;
		cuv->tail->next = NULL;
		free(q);
		cuv->len--;
		return r;
	} else {
		p = cuv->head;
		while(p) {
			if(p == n) {
				break;
			}
			p = p->next;
		}
		q = p;
	    p = p->prev;
		node *r = q;
		p->next = p->next->next;
		p->next->prev = p;
		free(q);
		cuv->len--;
		return r;
	}
}

cuvant *remove_cuvant(cuvant *cuv, linie *lin) {
	cuvant *p,*q;
	if( cuv->prev == NULL && cuv->next == NULL ) {
		q = cuv;
		cuvant *r = q;
		lin->head = lin->tail = NULL;
		free(q);
		lin->len--;
		return r;
	} else if( cuv->prev == NULL ) {
		q = cuv;
		cuvant *r = q;
		lin->head = lin->head->next;
		lin->head->prev = NULL;
		free(q);
		lin->len--;
		return r;
	} else if( cuv->next == NULL ) {
		q = cuv;
		cuvant *r =q;
		lin->tail = lin->tail->prev;
		lin->tail->next = NULL;
		free(q);
		lin->len--;
		return r;
	} else {
		p = lin->head;
		while(p) {
			if(p == cuv) {
				break;
			}
			p = p->next;
		}
		q = p;
		p = p->prev;
		cuvant *r = q;
		p->next = p->next->next;
		p->next->prev = p;
		free(q);
		lin->len--;
		return r;
	}
}

linie *remove_line_from_text(text *t, int n) {
	linie *p,*q;
	int m;
	if(n == 0) {
		q = t->head;
		linie *r = q;
		t->head = t->head->next;
		t->head->prev = NULL;
		free(q);
		t->len--;
		return r;
	} else if(n == t->len - 1) {
		q = t->tail;
		linie *r = q;
		t->tail = t->tail->prev;
		t->tail->next = NULL;
		free(q);
		t->len--;
		return r;
	} else if( n < t->len/  2 ) {
		p = t->head;
		m = n;
		while(m - 1 > 0) {
			p = p->next;
			m--;
		}
		q = p->next;
		linie *r = q;
		p->next = p->next->next;
		p->next->prev = p;
		free(q);
		t->len--;
		return r;
	} else {
		p = t->tail;
		m = t->len - n;
		while( m > 0 ) {
			p = p->prev;
			m--;
		}
		q = p->next;
		linie *r = q;
		p->next = p->next->next;
		p->next->prev = p;
		free(q);
		t->len--;
		return r;
	}
}

linie *remove_line(linie *lin, text *t) {
	linie *p,*q;
	if(lin->prev == NULL && lin->next == NULL) {
		q = lin;
		linie *r = q;
		t->head = t->tail = NULL;
		free(q);
		t->len--;
		return r;
	} else if(lin->prev == NULL) {
		q = lin;
		linie *r = q;
		t->head = t->head->next;
		t->head->prev = NULL;
		free(q);
		t->len--;
		return r;
	} else if(lin->next == NULL) {
		linie *r = lin;
		remove_line_from_text(t, t->len);
		return r;
	} else {
		p = t->head;
		while(p) {
			if(p == lin) {
				break;
			}
			p = p->next;
		}
		q = p;
		p = p->prev;
		linie *r = q;
		p->next = p->next->next;
		p->next->prev = p;
		free(q);
		t->len--;
		return r;
	}
}

void print_cuvant_to_file(cuvant *cuv, FILE *f) {
	node *q = cuv->head;
    if(q == NULL) {
        return;
    }
    while(q != NULL) {
        fputc(q->caracter, f);
        q = q->next;
    }    
    
}

void print_line_to_file(linie *l, FILE *f) {
	cuvant *aux = l->head;
	while( aux!= NULL) {
		print_cuvant_to_file(aux, f);
		aux = aux->next;
	}
}

void print_text_to_file(text *t,FILE *f) {
	linie *aux = t->head;
	while( aux!= NULL) {
		print_line_to_file(aux, f);
		aux = aux->next;
	}
}

node *backspace(text *t, node *cursor) {
	int aux = 0,i;
	linie *lin = t->head;
	cuvant *cuv;
	node *nod;
	while(lin != NULL) {
		cuv = lin->head;
		while(cuv!=NULL) {
			i = 0;
			nod = cuv->head;
			while(nod != NULL) {
				if(nod == cursor) {
					aux = 1;
					break;
				}
				nod = nod->next;
				i++;
			}
			if(aux == 1) {
				break;
			}
			cuv = cuv->next;
		}
		if(aux == 1) {
			break;
		}
		lin = lin->next;
	}
	if(nod->prev!=NULL) {
		remove_char(nod, cuv);
		nod = cuv->head;
		while(i > 0) {
			nod = nod->next;
			i--;
		}	
		cursor = nod;
	} else if(cuv->prev != NULL) {
    	cursor = cuv->prev->tail;
    	remove_char(nod, cuv);
    	if(cuv->head == NULL && cuv->tail == NULL) {
    		remove_cuvant(cuv, lin);
    	}
    } else {
    	cursor = nod->next;
    	remove_char(nod, cuv);
    }
	return cursor;
}

node *delete_line(text *t, int n, node *cursor) {
	if(n == -1) {
		linie *lin = t->head;
		cuvant *cuv;
		node *nod;
		int aux = 0;
		while(lin != NULL) {
			cuv = lin->head;
			while(cuv != NULL) {
				nod = cuv->head;
				while(nod != NULL) {
					if( nod == cursor) {
						aux = 1;
						break;
					}
					nod = nod->next;
				}
				if( aux == 1) {
					break;
				}
				cuv = cuv->next;
			}
			if(aux == 1) {
				break;
			}
			lin = lin->next;
		}
		if( lin->next == NULL ) {
			cursor = lin->prev->head->head;
			remove_line_from_text(t, t->len);
			return cursor;
		} else {
			cursor = lin->next->head->head;
		}
		remove_line(lin, t);
		return cursor;
	} else {
		linie *lin = t->head;
		int aux = n - 1;
		while (aux > 0) {
			lin = lin->next;
			aux--;
		}
		cuvant *cuv;
		node *nod;
		aux = 0;
		cuv = lin->head;
		while(cuv != NULL) {
			nod = cuv->head;
			while(nod != NULL) {
				if( nod == cursor) {
					aux = 1;
					break;
				}
				nod = nod->next;
			}
			if( aux == 1) {
				break;
			}
			cuv = cuv->next;
		}
		if(aux == 0) {
			remove_line_from_text(t, n-1);
			return cursor;
		} else {
			if(lin == t->tail) {
				cursor = lin->prev->head->head;
			}
			else {
				cursor = lin->next->head->head;
			}
			remove_line_from_text(t, n-1);
			return cursor;
		}
	}
}

node *go_to_line(text *t, int n) {
	node *cursor;
	int aux = n - 1;
	linie *lin = t->head;
	while(aux > 0) {
		lin = lin->next;
		aux --;
	}
	cursor = lin->head->head;
	return cursor;
}

node *go_to_char(text *t, int n, int m, node *cursor) {
	if(m == -1) {
		linie *lin = t->head;
		cuvant *cuv;
		node *nod;
		int aux = 0;
		while(lin != NULL) {
			cuv = lin->head;
			while(cuv != NULL) {
				nod = cuv->head;
				while(nod != NULL) {
					if(nod == cursor) {
						aux = 1;
						break;
					}
					nod = nod->next;
				}
				if( aux == 1) {
					break;
				}
				cuv = cuv->next;
			}
			if(aux == 1) {
				break;
			}
			lin = lin->next;
		}
		cuv = lin->head;
		nod = lin->head->head;
		aux = n - 1;
		while(aux > 0)
		{
			if( nod->next != NULL ) {
				nod = nod->next;
			}
			else {
				cuv = cuv->next;
				nod = cuv->head;
			}
			aux--;
		}
		cursor = nod;
		return cursor;
	} else {
		linie *lin = t->head;
		cuvant *cuv;
		node *nod;
		int aux = m - 1;
		while(aux > 0) {
			lin = lin->next;
			aux--;
		}
		aux = n-1;
		cuv = lin->head;
		nod = lin->head->head;
		while(aux > 0) {
			if( nod->next != NULL ) {
				nod = nod->next;
			}
			else {
				cuv = cuv->next;
				nod = cuv->head;
			}
			aux--;
		}
		cursor = nod;
		return cursor;
	}
}

node *delete_char(text *t, node *cursor) {
	linie *lin = t->head;
	cuvant *cuv;
	node *nod;
	int aux = 0;
	while(lin != NULL) {
		cuv = lin->head;
		while(cuv != NULL) {
			nod = cuv->head;
			while(nod != NULL) {
				if(nod == cursor) {
					aux = 1;
					break;
				}
				nod = nod->next;
			}
			if(aux == 1) {
				break;
			}
			cuv = cuv->next;
		}
		if(aux == 1) {
			break;
		}
		lin = lin->next;
	}
	if(nod->next!=NULL) {
		cursor = nod->next;
		remove_char(nod, cuv);
	} else if( cuv->next != NULL ) {
		cursor = cuv->next->head;
		remove_char(nod, cuv);
	} else {
		cursor = lin->next->head->head;
		remove_char(nod, cuv);
	}
	return cursor;
}

/*functie pentru gasirea secventei "word" in cadrul listei "text"*/
node *strlist(text *t, node *cursor, char *word) {
	linie *lin = t->head;
	cuvant *cuv;
	node *nod;
	int aux = 0;
	while(lin != NULL) {
		cuv = lin->head;
		while(cuv != NULL) {
			nod = cuv->head;
			while(nod != NULL) {
				if(nod == cursor) {
					aux = 1;
					break;
				}
				nod = nod->next;
			}
			if(aux == 1) {
				break;
			}
			cuv = cuv->next;
		}
		if(aux == 1) {
			break;
		}
		lin = lin->next;
	}
	linie *lin_aux  = lin;
	cuvant *cuv_aux = cuv;
	while(lin != NULL) {
		if(lin != lin_aux) {
			cuv = lin->head;
		}
		while(cuv != NULL) {
			if(cuv != cuv_aux ) {
				nod = cuv->head;
			}
			while( nod != NULL) {
				node *start_node = nod;
				char *start_char = word;
				while(start_node && *start_char && start_node->caracter == *start_char) {
					start_node = start_node->next;
					start_char++;
				}
				if (*start_char == '\0') {
					return nod;
				}
				nod = nod->next;
			}
			cuv = cuv->next;
		}
		lin = lin->next;
	}
	return 0;
}

node *delete_word(text *t, node *cursor, char *old_word) {
	int n = strlen(old_word);
	node *nod = strlist(t,cursor,old_word);
	while(n > 0) {
		nod = delete_char(t, nod);
		n--;
	}
	return cursor;
}

node *delete_all_words(text *t, node *cursor, char *old_word) {
	node *inceput = t->head->head->head;
	while( strlist(t,inceput,old_word) ) {
		delete_word(t,inceput,old_word);
	}
	linie *lin = t->head;
	while(lin != NULL) {
		if(lin->head->head->caracter == '\n') {
			remove_line(lin, t);
		}
		lin = lin->next;
	}
	return cursor;
}

node *replace_word(text *t, node *cursor, char *old_word, char *new_word) {
	int n = strlen(new_word);
	node *aux_nod = strlist(t, cursor, old_word);
	linie *lin = t->head;
	cuvant *cuv;
	node *nod;
	int aux, i = 0;
	while(lin != NULL) {
		cuv = lin->head;
		while(cuv != NULL) {
			i = 0;
			nod = cuv->head;
			while(nod != NULL) {
				if(nod == aux_nod) {
					aux = 1;
					break;
				}
				i++;
				nod = nod->next;
			}
			if( aux == 1) {
				break;
			}
			cuv = cuv->next;
		}
		if(aux == 1) {
			break;
		}
		lin = lin->next;
	}
	delete_word(t, cursor, old_word);
	while(n > 0) {
		add_char_to_cuvant(cuv,i,new_word[n-1]);
		n--;
	}
	return cursor;
}

node *replace_all_words(text *t, node *cursor, char *old_word, char *new_word) {
	node *inceput = t->head->head->head;
	while(strlist(t, inceput, old_word)) {
		replace_word(t, inceput, old_word, new_word);
	}
	return cursor;
}

int separator(linie *lin) {
	node *n = lin->head->head;
	if(n->caracter == ':') {
		return 1;
	}
	return 0;
}

void add_line_to_string(linie *lin, char string[]) {
	cuvant *cuv;
	node *nod;
	cuv = lin->head;
	int i = 0;
	while(cuv != NULL) {
		nod = cuv->head;
		while( nod != NULL ) {
			string[i] = nod->caracter;
			i++;
			nod = nod->next;
		}
		cuv = cuv->next;
	}
	string[i-1] = '\0';
}

char **extragere_comenzi(text *t) {
	int i,j;
	char **comenzi;
	comenzi = (char**)malloc(200 * sizeof(char*));
	for(i=0; i<200; i++) {
		comenzi[i] = (char*)malloc(30 * sizeof(char));
	}
	linie *lin = t->head;
	i = 0;
	j = 0;
	while(lin != NULL) {
		if(separator(lin) == 1) {
			i++;
		}
		if(i%2 == 1 && separator(lin) == 0) {
			add_line_to_string(lin, comenzi[j]);
			j++;
		}
		lin = lin->next;
	}
	return comenzi;
}

/*functie pentru asocierea unu sir de caractere cu functia careia corespunde*/
node *interpretare_comanda(text *t, node *cursor, char *comanda) {
	if(comanda[0]=='b') {
		cursor = backspace(t, cursor);
	}
	if(comanda[0]=='d' && comanda[1]=='l') {
		int x = -1;
		if(strchr(comanda, ' ')) {
			x = atoi(comanda + 3);
		}
		cursor = delete_line(t, x, cursor);
	}
	if(comanda[0]=='g' && comanda[1] == 'l') {
		int x = atoi(comanda + 3);
		cursor = go_to_line(t, x);
	}
	if(comanda[0]=='g' && comanda[1]=='c') {
		int x, y = -1;
		char *primul_numar = strchr(comanda, ' ');
		x = atoi(primul_numar);
		if(strchr(primul_numar+1, ' ')) {
			char *al_doilea_numar = strchr(primul_numar+1, ' ');
			y = atoi(al_doilea_numar);
		}
		cursor = go_to_char(t, x, y, cursor);
	}
	if(comanda[0] =='d' && comanda[1]!= 'l' && comanda[1] != 'a' && comanda[1] != 'w') {
		int x;
		if( strchr(comanda,' ') ) {
			x = atoi(comanda + 1);
			while(x > 0) {
				cursor = delete_char(t, cursor);
				x--;
			}
		} else {
			cursor = delete_char(t, cursor);
		}
	}
	if(comanda[0] == 'r' && comanda[1] == 'e') {
		int i = 0;
		char old_word[20], new_word[20], copie[50];
		strcpy(copie, comanda);
		char *token = strtok(copie, " ");
		while(token!=NULL) {
			if( i == 1) {
				strcpy(old_word, token);
			}
			if( i == 2) {
				strcpy(new_word, token);
			}
			i++;
			token = strtok(NULL, " ");
		}
		cursor = replace_word(t, cursor, old_word, new_word);
	}
	if(comanda[0] == 'r' && comanda[1] == 'a') {
		int i = 0;
		char old_word[20], new_word[20], copie[50];
		strcpy(copie,comanda);
		char *token = strtok(copie, " ");
		while(token != NULL) {
			if(i == 1) {
				strcpy(old_word, token);
			}
			if(i == 2) {
				strcpy(new_word, token);
			}
			i++;
			token = strtok(NULL, " ");
		}
		cursor = replace_all_words(t, cursor, old_word, new_word);
	}
	if(comanda[0] == 'd' && comanda[1] == 'w') {
		int i = 0;
		char word[20], copie[30];
		strcpy(copie, comanda);
		char *token = strtok(copie, " ");
		while(token != NULL) {
			if(i == 1) {
				strcpy(word, token);
			}
			i++;
			token = strtok(NULL, " ");
		}
		cursor = delete_word(t, cursor, word);
	}
	if(comanda[0] == 'd' && comanda[1] == 'a') {
		int i = 0;
		char word[20], copie[30];
		strcpy(copie, comanda);
		char *token = strtok(copie, " ");
		while(token!=NULL) {
			if(i == 1) {
				strcpy(word, token);
			}
			i++;
			token = strtok(NULL, " ");
		}
		cursor = delete_all_words(t, cursor, word);
	}
	return cursor;
}

/*functie pentru crearea unui duplicat al unui text*/
text *copy_text(text *t) {
	linie *lin = t->head;
	cuvant *cuv;
	node *nod;
	if(lin == NULL) {
		return 0;
	}
	text *aux_text;
	init_text(&aux_text);
	while(lin != NULL) {
		linie *aux_line;
		init_line(&aux_line);
		cuv = lin->head;
		while(cuv!=NULL) {
			cuvant *aux_cuv;
			init_cuvant(&aux_cuv);
			nod = cuv->head;
			while(nod!=NULL) {
				add_char_to_cuvant(aux_cuv, aux_cuv->len, nod->caracter);
				nod = nod->next;
			}
			add_cuvant_to_line(aux_line, aux_line->len, aux_cuv);
			cuv = cuv->next;
		}
		add_line_to_text(aux_text, aux_text->len, aux_line);
		lin = lin->next;
	}
	return aux_text;
}


/*functia in care se adauga text, se efectueaza comenzi pe textul adaugat
pana intr-un anumit punct si operatiile de undo/redo in cazul adaugarii de
text*/
text *editor(text *t,char **comenzi) {
	init_stack(&undo_stack);
	init_stack(&redo_stack);
	init_text(&t_final);
	text *saved_text;
	linie *lin = t->head, *lin_aux;
	node *cursor, *inceput;
	int i, j, k = 0, aux = 0;
	i = 0;
	j = 0;
	//se parcurge lista citita initial
	while( lin != NULL) {	
		inceput = lin->head->head;
		if(inceput->caracter == ':') {
			/*la intalnirea separatorului, daca "i" este par, se pune
			pe stiva de undo ca s-a adaugat un intreg fragment de text*/
			if(i % 2 == 0) {
				add_stack(undo_stack, "added text");
				cursor = t_final->tail->tail->tail;
			}
			i++;//"i" contorizeaza de cate ori s-a intalnit separatorul
		}
		/*cat timp i este par, se adauga linii la text, la pozitia la care
		a ramas cursorul*/
		if(i % 2 == 0 && inceput->caracter != ':') {
			/* "k" contorizeaza cate linii s-au adaugat ultima data*/
			if( i!= 0 && cursor!=NULL && cursor != t_final->tail->tail->tail) {
				add_in_middle(t_final, cursor, lin);
			}
			else {
				add_line_to_text(t_final, t_final->len, lin);
			}
			k++;
		}
		/*cat timp "i" este impar, editorul este in modul "comanda" */
		if(i % 2 == 1 && inceput->caracter != ':') {	
			aux = 0;
			/*daca se intalneste undo dupa operatie de adaugare, se vor sterge
			ultimele "k" linii si se va pune comanda de adaugare in varful
			stivei de redo*/
			if(comenzi[j][0]=='u') {
				add_stack(redo_stack, undo_stack->top->comanda);
				pop_stack(undo_stack);
				if(redo_stack->top->comanda[0]=='a') {
					while(aux < k) {
						cursor = delete_line(t_final, t_final->len, cursor);
						aux++;
					}
				}
				aux = 0;
				cursor = t_final->tail->tail->tail;	
			}
			/* daca undo este urmata de redo, atunci parcurgerea textului
			initial se va intoarce pana la ultimul punct in care s-a adaugat
			text*/
			else if(comenzi[j][0] == 'r' && comenzi[j][1] != 'a' && 
				comenzi[j][1] != 'e') {
				add_stack(undo_stack, redo_stack->top->comanda);
				pop_stack(redo_stack);
				/*se da pop la varful stivei de redo si se repune pe stiva de 
				undo*/
				lin_aux = lin;
				if(undo_stack->top->comanda[0] == 'a') {
					while(aux < k + 5) {
						lin = lin->prev;
						aux++;
					}		
					i = i-2;
					remove_line(lin_aux->prev, t);
					remove_line(lin_aux, t);
					free(comenzi[j]);
					free(comenzi[j-1]);
					/*se sterg operatiile undo si redo efectuate din vectorul
					de siruri de caractere, iar dupa intoarcerea in punctul
					de unde a inceput ultima adaugare de text, se vor efectua
					comenzi in mod normal*/
					j = j-1;
					cursor = t_final->tail->tail->tail;
				}
			} else if( comenzi[j][0] == 's' ) {
				/* se memoreaza ultima varianta a textului modificat
				atunci cand se intalneste comanda "save" */
				saved_text = copy_text(t_final);
				k = 0;
			}	
			else if( comenzi[j] && (comenzi[j][0] == 'd' || comenzi[j][0] == 'r'
			 || comenzi[j][0] == 'g'|| comenzi[j][0] == 'b')) {
				/*interpretarea comenzii si efectuare acesteia asupra textului*/
				cursor = interpretare_comanda(t_final,cursor,comenzi[j]);
				add_stack(undo_stack, comenzi[j]);
				k = 0;
			}
			j++;
		}	
		lin = lin->next;
	}
	return saved_text;
}

/*functie pentru punerea textului din fisierul de intrare intr-o lista*/
text *citire( FILE *f ) {
	text *t1;
	init_text(&t1);
	int i, j, k = 0;
	char c;
	linie *l1;
	cuvant *cuv1;
	while(1) {
		j = 0;
		init_line(&l1);
		while(1) {
			i = 0;
			init_cuvant(&cuv1);
			while(1) {
				if((c=fgetc(f)) == EOF) {
					break;
				}
				if(c == ' ') {
					add_char_to_cuvant(cuv1, i, ' ');
					break;
				}
				if(c == '\n') {
					add_char_to_cuvant(cuv1, i, '\n');
					break;
				}
				add_char_to_cuvant(cuv1, i, c);
				i++;
			}
			if(cuv1->head!=NULL) {
				add_cuvant_to_line(l1, j, cuv1);
			}	
			free(cuv1);
			j++;
			if(c == '\n') {
				break;
			}
			if((c=fgetc(f)) == EOF) {
				break;
			}
			fseek(f, -1, SEEK_CUR);
		}
		if( l1->head != NULL) {
			add_line_to_text(t1, k, l1);
		}
		k++;
		free(l1);
		if( (c=fgetc(f))==EOF) {
			break;
		}
		fseek(f, -1, SEEK_CUR);
	}	
	return t1;
}

int main() {
    text *t1,*t2;
    init_text(&t1);
    FILE *f1 = fopen("editor.in", "r");
    FILE *f2 = fopen("editor.out", "w");
    char **comenzi;

    t1 = citire(f1);
	comenzi = extragere_comenzi(t1);
	t2 = editor(t1, comenzi);

	int i = 0;
	while(comenzi[i][0] != '\0') {
		free(comenzi[i]);
		i++;
	}
	free(comenzi);
	free(t1);
	free_stack(&undo_stack);
	free_stack(&redo_stack);
	
   	print_text_to_file(t2, f2);
   	free(t2);
    fclose(f1);
    fclose(f2);
    return 0;
}
