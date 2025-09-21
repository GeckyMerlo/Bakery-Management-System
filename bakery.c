#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>
#include <ctype.h>
#define maxnames 256

typedef enum{BLACK,RED,NILL} node_color;

//check save

typedef struct { //definisco la variabile ingrediente
    int ind;
    int num;
}chiave;

typedef struct recipe{   // struttura a lista che descrive una ricetta ricetta; ogni nodo contiene un ingrediente con il peso necessario e il puntatore all'ingrediente seguente
    chiave key;
    int weight;
}recipe_t;

typedef struct nome_ing{
    recipe_t ingrediente;
    struct nome_ing* next_ingr;
}nome_ing_t;

typedef struct recipe_book{ //definisco la variabile ricettario che contiene una lista di ricette e su cui mi muoverò sempre con le funzioni aggiungi_ricetta e rimuovi_ricetta
    char recipe_name[maxnames];
    chiave key;
    int num_ingr;
    nome_ing_t* ricetta;
    struct recipe_book* next_recipe;
}book_t;

typedef struct lotto{
    int key;
    int weight;
    int scadenza;
    struct lotto* next_lotto;
   // struct lotto* next_ingredient;
} lotto_t;

typedef struct ingrediente{
    int num;
    char nome[maxnames];
    lotto_t* lotto;
    struct ingrediente* next_ingr;
}ingrediente_t;

typedef struct {
    chiave key;
    int weight;
} first_to_check;

typedef struct ordine{
    chiave key; //chiave ricetta
    int arrivo; //tempo di arrivo;
    int num_el; //numero elementi ordinati
    int peso;
}ordine_t;

typedef struct nodo_lista_attesa{
    ordine_t* ordine;
    first_to_check first;
    struct nodo_lista_attesa* next_ordine;
}nodo_attesa_t;

typedef struct lista_attesa{
    nodo_attesa_t* testa;
    nodo_attesa_t* coda;
}attesa_t;

typedef struct HashMagazzino{
    ingrediente_t** pile;
    int size;
}magazzino_t;

typedef struct vet{
    book_t* ric;
    bool val[1000];
}vet_t;

typedef struct HashRicettario{
    vet_t** ricette;
    int size;
    bool occ[100];
}ricettario_t;

typedef struct nodoRB{
    node_color colore;
    ordine_t* ordine;
    struct nodoRB* p;
    struct nodoRB* d;
    struct nodoRB* s;
}nodo_t;

typedef struct albero{
    nodo_t* nill;
    nodo_t* root;
}albero_t;

int hash_function(char* );
ricettario_t* crea_ricettario();
ingrediente_t* crea_nodo_magazzino(char* , int , int );
magazzino_t* crea_magazzino();
void lettura_ingredienti(magazzino_t** , int , char**, FILE*, attesa_t**, ricettario_t**, albero_t**);
void Rifornimento(magazzino_t** , ingrediente_t** , int , int );
void elimina_scaduti(magazzino_t**, lotto_t** , lotto_t** , lotto_t** , int );
void elimina_ingrediente(magazzino_t** ,ingrediente_t**,  lotto_t** , lotto_t** , int );
bool check_ingrediente(magazzino_t** , int , int, int t, int );
bool verifica(magazzino_t** , lotto_t** , int , int , int );
void rimuovi_ingredienti(magazzino_t** , int , int , int );
book_t* trova_ricetta(ricettario_t** , char*, chiave*,  bool);
bool controlla_ricetta(magazzino_t** , book_t** , int , int, first_to_check*);
int prepara(magazzino_t** , book_t** , int );
void preparazione_ordine(magazzino_t** , ricettario_t** , ordine_t* , char*,  albero_t** ,attesa_t** , int );
void aggiungi_ricetta(ricettario_t** , book_t* );
void crea_nuova_ricetta(ricettario_t**, magazzino_t**, char**, FILE*);
void rimuovi_ricetta(ricettario_t** , albero_t** , attesa_t**, FILE*);
albero_t* crea_albero_completi(ordine_t* );
void crea_coda_attesa(attesa_t** , ordine_t*, first_to_check);
void aggiungi_attesa(attesa_t** , ordine_t*, first_to_check);
void check_attesa(attesa_t** , magazzino_t** , ricettario_t** , int , albero_t**);
void rimuovi_attesa(attesa_t** , nodo_attesa_t** , nodo_attesa_t** );
void aggiungi_completi(albero_t** , ordine_t* );
bool ricerca_attesa(attesa_t** , chiave);
bool ricerchina(albero_t** ,nodo_t* , chiave );
bool ricerca_completi(albero_t** , chiave );
void insertion(albero_t** , ordine_t* );
void RB_INSERT_FIXUP(albero_t** , nodo_t** );
void right_rotate(albero_t** , nodo_t** );
void left_rotate(albero_t** , nodo_t** );
nodo_t* trova_next(albero_t** , nodo_t* );
void cammioncino(albero_t** ,ricettario_t**, int );
nodo_attesa_t* aggiungi_corriere(nodo_attesa_t** ,nodo_attesa_t** , ordine_t* );
nodo_t* rbdelete(albero_t** , nodo_t** );
void RB_DELETE_FIXUP(albero_t** , nodo_t** );
void sorta_corriere(nodo_attesa_t** , int );
void stampa_elimina(nodo_attesa_t**, ricettario_t**, int );
void leggi_ordine(magazzino_t** ,ricettario_t** , attesa_t** , albero_t** , int , FILE*);
void rimozione(magazzino_t** ,ingrediente_t** , int , int );
book_t* trova_ricetta_attesa(ricettario_t** , chiave , bool );
void rifornisci_ingrediente(magazzino_t** , lotto_t** , lotto_t* , int ,  int );
int cerca_ingrediente(magazzino_t**, char*, int);
bool controlla_first(magazzino_t**, first_to_check, int);
void aux_fun(attesa_t**);
void rb_transplant(albero_t** , nodo_t* , nodo_t* );
void aux_check_integrety(nodo_t* );

int main(){
    int t = 0; //counter
    int freq; // frequenza del corriere
    int cap; //capienza del corriere
    int c =1;
    magazzino_t* magazzino = crea_magazzino();
    ricettario_t* ricettario = crea_ricettario();
    char* comando = (char*)malloc(sizeof(char)*17);
    attesa_t* attesa = NULL;
    albero_t* completi = NULL;
    FILE *fp;
    fp = fopen("/Users/giacomomerlo/API/testo.txt", "r"); //apro il file in modalità lettura;
    if (!fp){
        fp = stdin;
    }
    if (fscanf(fp, "%d %d\n", &freq, &cap) != EOF){}
    if (fscanf(fp, "%s ", comando) != EOF){}
    while(feof(fp) == 0){
        if( t != 0){
            if (t % freq == 0){
                c++;
                cammioncino(&completi, &ricettario, cap);
            }
        }
        if(strncmp(comando, "aggiungi_ricetta", 16)==0){
            crea_nuova_ricetta(&ricettario, &magazzino, &comando, fp);
            t++;
        }
        else{
            if(strncmp(comando, "rimuovi_ricetta", 16)==0){
                rimuovi_ricetta(&ricettario, &completi, &attesa, fp);
                if (fscanf(fp, "%s ", comando) != EOF){}
                t++;
            }
            else{
                if(strncmp(comando, "rifornimento", 16)==0){
                    lettura_ingredienti(&magazzino, t, &comando, fp, &attesa, &ricettario, &completi);
                    t++;
                }
                else{
                    leggi_ordine(&magazzino, &ricettario, &attesa, &completi, t, fp);
                    if (fscanf(fp, "%s ", comando) != EOF){}
                    t++;
                }
            }
        }
        
    }
    if( t != 0){
        if (t % freq == 0){
            cammioncino(&completi, &ricettario, cap);
        }
    }
    return 0;
}

int hash_function(char* word)
 {
     unsigned long hash = 5381;

     for (const char* ptr = word; *ptr != '\0'; ptr++)
     {
         hash = ((hash << 5) + hash) + tolower(*ptr);
     }

     return hash % 100;
 }

ricettario_t* crea_ricettario(){
    ricettario_t* tabella = (ricettario_t*) malloc(sizeof(ricettario_t));
    tabella->ricette = (vet_t**)malloc(sizeof(vet_t)*100);
    tabella->size = 100;
    memset(tabella->occ, 0, sizeof(bool)* 100);
    /*
    for (i = 0; i < tabella->size; i++){
        tabella->ricette[i] = (vet_t*)malloc(sizeof(vet_t));  
        tabella->ricette[i]->ric = NULL; 
        memset(tabella->ricette[i]->val, 0, sizeof(bool)*20); 
    }
    */
    return tabella;
}

ingrediente_t* crea_nodo_magazzino(char* nome, int peso, int scadenza){
    ingrediente_t* ingrediente = (ingrediente_t*) malloc(sizeof(ingrediente_t));
    strcpy(ingrediente->nome, nome);
    ingrediente->lotto = (lotto_t*)malloc(sizeof(lotto_t));
    ingrediente->next_ingr = NULL;
    ingrediente->lotto->scadenza = scadenza;
    ingrediente->lotto->weight = peso;
    ingrediente->lotto->next_lotto = NULL;
    return ingrediente;
}

magazzino_t* crea_magazzino(){
    int i;
    magazzino_t* tabella = (magazzino_t*) malloc(sizeof(magazzino_t));
    tabella->size = 100;
    tabella->pile = (ingrediente_t**) malloc(tabella->size * sizeof(ingrediente_t*));
    for (i = 0; i < tabella->size; i++){
        tabella->pile[i] = NULL;
    }
    return tabella;
}

void crea_nuova_ricetta(ricettario_t** ricettario, magazzino_t** magazzino, char** comando, FILE* fp){
    book_t* tmp = (book_t*)malloc(sizeof(book_t));
    tmp->next_recipe = NULL;
    tmp->num_ingr = 0;
    tmp->ricetta = NULL;
    memset(tmp->recipe_name, 0, sizeof(tmp->recipe_name));
    char nome_ingrediente[maxnames];
    int peso;
    if (fscanf(fp, "%s", tmp->recipe_name) != 1){} //trovo il nome della nuova ricetta
    if (fscanf(fp, "%s", nome_ingrediente) != EOF){}
    tmp->num_ingr = 0; //qui può essere che sia il caso di usare getline e strtok
    while(strncmp(nome_ingrediente, "rimuovi_ricetta", 16) != 0 && strncmp(nome_ingrediente, "aggiungi_ricetta", 16) != 0 && strncmp(nome_ingrediente, "rifornimento", 16) != 0 && strncmp(nome_ingrediente, "ordine", 16)!= 0){
        tmp->num_ingr++;
        if (fscanf(fp, "%d", &peso) != EOF){}
        nome_ing_t* new_ingredient = (nome_ing_t*)malloc(sizeof(nome_ing_t));
       // new_ingredient->ingrediente = (recipe_t*)malloc(sizeof(recipe_t));
        new_ingredient->ingrediente.weight = 0;
        new_ingredient->ingrediente.key.ind = 0;
        new_ingredient->ingrediente.key.num = 0;
        new_ingredient->next_ingr = NULL;
        //strcpy(new_ingredient->nome_ingr, nome_ingrediente);
        new_ingredient->ingrediente.key.ind = hash_function(nome_ingrediente);
        new_ingredient->ingrediente.key.num = cerca_ingrediente(magazzino, nome_ingrediente,new_ingredient->ingrediente.key.ind);
        new_ingredient->ingrediente.weight = peso;
        new_ingredient->next_ingr = tmp->ricetta;
        tmp->ricetta = new_ingredient;
        if (fscanf(fp, "%s", nome_ingrediente) != EOF){};
        if (feof(fp) == 1){
            break;
        }
    }
    if(feof(fp) == 0){
        strcpy(*comando, nome_ingrediente);
    }
    aggiungi_ricetta(ricettario, tmp);
    return;
}

int cerca_ingrediente(magazzino_t** magazzino, char* nome, int ind){
    if((*magazzino)->pile[ind] == NULL){
        (*magazzino)->pile[ind] = (ingrediente_t*)malloc(sizeof(ingrediente_t));
         (*magazzino)->pile[ind]->lotto = NULL;
         (*magazzino)->pile[ind]->num = 1;
         strcpy((*magazzino)->pile[ind]->nome, nome);
         return 1;
    }
    ingrediente_t* cur = (*magazzino)->pile[ind];
    ingrediente_t* prev = NULL;
    while(cur != NULL && strncmp(cur->nome, nome, maxnames) != 0){
        prev = cur;
        cur = cur->next_ingr;
    }
    if (cur == NULL){
        prev->next_ingr = (ingrediente_t*)malloc(sizeof(ingrediente_t));
        strcpy(prev->next_ingr->nome, nome);
        prev->next_ingr->num = prev->num + 1;
        prev->next_ingr->lotto = NULL;
        return prev->next_ingr->num;   
    }
    else{
        return cur->num;
    }
}

void aggiungi_ricetta(ricettario_t** ricettario, book_t* nuova_ricetta){
    nuova_ricetta->key.ind = hash_function(nuova_ricetta->recipe_name);
    nuova_ricetta->key.num = 0;
    if((*ricettario)->occ[nuova_ricetta->key.ind] == 0){
        (*ricettario)->occ[nuova_ricetta->key.ind]=1;
        (*ricettario)->ricette[nuova_ricetta->key.ind] = (vet_t*)malloc(sizeof(vet_t));  
        (*ricettario)->ricette[nuova_ricetta->key.ind]->ric = NULL; 
        memset((*ricettario)->ricette[nuova_ricetta->key.ind]->val, 0, sizeof(bool)*1000); 
    } 
    while(nuova_ricetta->key.num < 1000){
        if((*ricettario)->ricette[nuova_ricetta->key.ind]->val[nuova_ricetta->key.num] == 0){
            (*ricettario)->ricette[nuova_ricetta->key.ind]->val[nuova_ricetta->key.num] = 1;
            break;
        }
        nuova_ricetta->key.num++;
    }
    if((*ricettario)->ricette[nuova_ricetta->key.ind]->ric == NULL){
        (*ricettario)->ricette[nuova_ricetta->key.ind]->ric = nuova_ricetta;
        printf("aggiunta\n");
    }
    else{
        book_t* cur = (*ricettario)->ricette[nuova_ricetta->key.ind]->ric;
        while (cur->next_recipe != NULL){
            if(strncmp(nuova_ricetta->recipe_name, cur->recipe_name, maxnames) == 0){
                nome_ing_t* cur = nuova_ricetta->ricetta;
                while(nuova_ricetta->ricetta != NULL){
                    nuova_ricetta->ricetta = nuova_ricetta->ricetta->next_ingr;
                    free(cur);
                    cur = NULL;
                    cur = nuova_ricetta->ricetta;
                }
                free(nuova_ricetta->ricetta);
                nuova_ricetta->ricetta = NULL;
                free(nuova_ricetta);
                nuova_ricetta = NULL;
                printf("ignorato\n");
                return;
            }
            cur = cur->next_recipe;
            nuova_ricetta->key.num ++;
        }
        if(cur->next_recipe == NULL){{}
            if(strncmp(nuova_ricetta->recipe_name, cur->recipe_name, maxnames) == 0){
                nome_ing_t* cur = nuova_ricetta->ricetta;
                while(nuova_ricetta->ricetta != NULL){
                    nuova_ricetta->ricetta = nuova_ricetta->ricetta->next_ingr;
                    free(cur);
                    cur = NULL;
                    cur = nuova_ricetta->ricetta;
                }
                free(nuova_ricetta->ricetta);
                nuova_ricetta->ricetta = NULL;
                free(nuova_ricetta);
                nuova_ricetta = NULL;
                printf("ignorato\n");
                return;
            }
        }
        nuova_ricetta->next_recipe = NULL;
        cur->next_recipe = nuova_ricetta;
        printf("aggiunta\n");
    }
    return;
}

void lettura_ingredienti(magazzino_t** magazzino, int t, char** comando, FILE* fp, attesa_t** attesa, ricettario_t** ricettario, albero_t** completi){
    char nome_ingrediente[maxnames];
    if (fscanf(fp, "%s", nome_ingrediente) != EOF){}
    while(strncmp(nome_ingrediente, "rimuovi_ricetta", 16) && strncmp(nome_ingrediente, "aggiungi_ricetta", 16) && strncmp(nome_ingrediente, "rifornimento", 16) && strncmp(nome_ingrediente, "ordine", 16) && feof(fp) == 0){
        ingrediente_t* nuovo_ingrediente = (ingrediente_t*)malloc(sizeof(ingrediente_t));
        nuovo_ingrediente->next_ingr = NULL;
        nuovo_ingrediente->lotto = (lotto_t*)malloc(sizeof(lotto_t));
        nuovo_ingrediente->lotto->scadenza= 0;
        nuovo_ingrediente->lotto->weight= 0;
        memset(nuovo_ingrediente->nome, 0, sizeof(nuovo_ingrediente->nome));
        nuovo_ingrediente->lotto->next_lotto = NULL;
        strcpy(nuovo_ingrediente->nome, nome_ingrediente);
        if (fscanf(fp, "%d", &(nuovo_ingrediente->lotto->weight)) != EOF){}
        if (fscanf(fp, "%d", &(nuovo_ingrediente->lotto->scadenza)) != EOF){}
        nuovo_ingrediente->lotto->key = hash_function(nuovo_ingrediente->nome);
        Rifornimento(magazzino, &nuovo_ingrediente, nuovo_ingrediente->lotto->key, t);
        if (fscanf(fp, "%s", nome_ingrediente) != EOF){}
    }
    if(feof(fp) == 0){
        strcpy(*comando, nome_ingrediente);
    }
    check_attesa(attesa, magazzino, ricettario, t, completi);
    printf("rifornito\n");    
    return;
}

void Rifornimento(magazzino_t** magazzino, ingrediente_t** ingr, int ind, int t){
    if((*magazzino)->pile[ind] == NULL){
        (*ingr)->num = 1;
        (*magazzino)->pile[ind] = *ingr;
    }
    else{
        ingrediente_t* cur = (*magazzino)->pile[ind];
        ingrediente_t* prev = NULL;
       while (cur != NULL && strncmp(cur->nome, (*ingr)->nome, maxnames) != 0){
            prev = cur;
            cur = cur->next_ingr;
        }
        if (cur == NULL){
            (*ingr)->next_ingr = NULL;
            (*ingr)->num = prev->num + 1;
            prev->next_ingr = *ingr;
        }
        else{
            rifornisci_ingrediente(magazzino, &(cur->lotto), (*ingr)->lotto, ind, t);
            (*ingr)->lotto = NULL;
            free(*ingr);
            (*ingr) = NULL;
        }
    }
    return;
}

void rifornisci_ingrediente(magazzino_t** magazzino, lotto_t** ingrediente, lotto_t* ingr, int ind,  int t){
    lotto_t* cur = (*ingrediente);
    lotto_t* prev = NULL;
    if(cur == NULL){
        (*ingrediente) = ingr;
        return;
    }
    while(cur != NULL && ingr->scadenza > cur->scadenza){
        if(cur->scadenza <= t){
            elimina_scaduti(magazzino, ingrediente, &cur, &prev, ind);
        }
        else{
            prev = cur;
            cur = cur->next_lotto;
        }
    }
    if(cur == NULL){
        if(prev != NULL){
            ingr->next_lotto = NULL;
            prev->next_lotto = ingr;
        }
        else{
            (*ingrediente) = ingr;
        }
    }
    else{
        if(cur->scadenza == ingr->scadenza){
            cur->weight = ingr->weight + cur->weight;
            free(ingr);
            ingr = NULL;
        }
        else{
            if(prev == NULL){
                ingr->next_lotto = cur;
                (*ingrediente) = ingr;
            }
            else{
                ingr->next_lotto = cur;
                prev->next_lotto = ingr;
            }
        }
    }
    return;
}

void elimina_scaduti(magazzino_t** magazzino, lotto_t** ingrediente, lotto_t** cur, lotto_t** prev, int ind){
    if(*prev == NULL){
        if((*cur)->next_lotto == NULL){
            free(*cur);
            (*ingrediente) = NULL;
            *cur = NULL;
        }
        else{
            (*ingrediente) = (*ingrediente)->next_lotto;
            free(*cur);
            *cur = NULL;
            *cur = (*ingrediente);
        }
    }
    else{
        (*prev)->next_lotto = (*cur)->next_lotto;
        free(*cur);
        *cur = NULL;
        *cur = (*prev)->next_lotto;
    }
    return;
}

void rimuovi_ricetta(ricettario_t** ricettario, albero_t** completi, attesa_t** attesa, FILE* fp){
    int ind;
    char nome[maxnames];
    if (fscanf(fp, "%s", nome) != 1){} 
    ind = hash_function(nome);
    if((*ricettario)->occ[ind] == 0){
        printf("non presente\n");
        return;
    }
    if((*ricettario)->ricette[ind]->ric == NULL){
        printf("non presente\n");
        return;
    }
    book_t* cur = (*ricettario)->ricette[ind]->ric;
    book_t* prev = NULL;
    while(cur != NULL && strncmp(nome, cur->recipe_name, maxnames) != 0 ){
        prev = cur;
        cur = cur->next_recipe;
    }
    if(cur == NULL){
        printf("non presente\n");
    }
    else{
        bool condizione = true;
        condizione = ricerca_attesa(attesa, cur->key);
        if(condizione == true){
            condizione = ricerca_completi(completi, cur->key);
        }
        if(condizione == true){
            nome_ing_t* tmp = cur->ricetta;
            if(prev==NULL){
                (*ricettario)->ricette[ind]->ric = cur->next_recipe;
                while(cur->ricetta!= NULL){
                    cur->ricetta = cur->ricetta->next_ingr;
                    free(tmp);
                    tmp = NULL;
                    tmp = cur->ricetta;
                }
                free(cur->ricetta);
                cur->ricetta = NULL;
                free(cur);
                cur = NULL;
                printf("rimossa\n");
            }
            else{
                prev->next_recipe = cur->next_recipe;
                while(cur->ricetta!= NULL){
                    cur->ricetta = cur->ricetta->next_ingr;
                    free(tmp);
                    tmp = NULL;
                    tmp = cur->ricetta;
                }
                free(cur->ricetta);
                cur->ricetta = NULL;
                free(cur);
                cur = NULL;
                printf("rimossa\n");
            }
        }
        else{
            printf("ordini in sospeso\n");
        }
    }
    return;
}

bool ricerca_attesa(attesa_t** attesa, chiave key){
    if(*attesa == NULL){
        return true;
    }
    nodo_attesa_t* cur = (*attesa)->testa;
    while(cur != NULL && (cur->ordine->key.ind != key.ind || cur->ordine->key.num != key.num)){
        cur = cur->next_ordine;
    }
    if (cur == NULL){
        return true;
    }
    else return false;
}

bool ricerca_completi(albero_t** completi, chiave key){
    if((*completi) == NULL || (*completi)->root== NULL){
        return true;
    }
    else{
        nodo_t* nodo = (*completi)->root;
        return ricerchina(completi, nodo, key);
    }
}

bool ricerchina(albero_t** completi, nodo_t* nodo, chiave key){
    if(nodo == (*completi)->nill || (nodo)->colore == NILL){
        return true;
    }
    else{
        if(nodo->ordine->key.ind == key.ind && nodo->ordine->key.num == key.num)
        {
            return false;
        }
        else{
            bool conds, condd;
            conds = ricerchina(completi, (nodo)->s, key);
            condd = ricerchina(completi, (nodo)->d, key);
            if(condd == true && conds == true){
                return true;
            }
            else{
                return false;
            }
        }
    }
}

void leggi_ordine(magazzino_t** magazzino,ricettario_t** ricettario, attesa_t** attesa, albero_t** completi, int t, FILE* fp){
    ordine_t* ordine = (ordine_t*)malloc(sizeof(ordine_t));
    char nome[maxnames];
    if (fscanf(fp, "%s", nome) != EOF){}
    if (fscanf(fp, "%d", &(ordine->num_el)) != EOF){}
    ordine->arrivo = t;
    preparazione_ordine(magazzino, ricettario, ordine, nome, completi, attesa, t);
    return;
}

void preparazione_ordine(magazzino_t** magazzino, ricettario_t** ricettario, ordine_t* ordine, char* nome, albero_t** completi,attesa_t** attesa, int t){
    book_t* ricetta = trova_ricetta(ricettario, nome, &(ordine->key), false);
    if(ricetta != NULL){
        first_to_check first; first.key.ind = 0; first.key.num = 0; first.weight = 0;
        if(controlla_ricetta(magazzino, &ricetta, t, ordine->num_el, &first) == true){
            ordine->peso = prepara(magazzino, &ricetta, ordine->num_el);
            aggiungi_completi(completi, ordine);
            //printf(" e aggiunto ai completi\n");
        }
        else{
            aggiungi_attesa(attesa, ordine, first);
            //printf(" e aggiunto all'attesa\n");
        }
    }
    //non sono sicuro vada fatta sta roba
    
    return;
}

book_t* trova_ricetta(ricettario_t** ricettario, char* nome_ricetta, chiave* key, bool type){
    key->ind = hash_function(nome_ricetta);
    key->num = 0;
    if((*ricettario)->occ[key->ind] == 0){
        printf("rifiutato\n"); 
        return NULL;
    }
    book_t* cur = (*ricettario)->ricette[key->ind]->ric;
    while(cur != NULL && strncmp(cur->recipe_name, nome_ricetta, maxnames) != 0){
        cur = cur->next_recipe;
    }
    if (cur == NULL){
        if(type == false){
            printf("rifiutato\n"); 
        }       
        return NULL;
    }
    else {
        if(type == false){
            key->num = cur->key.num;
            printf("accettato\n");
        }
        return cur;
    }
}

bool controlla_ricetta(magazzino_t** magazzino, book_t** ricetta, int t, int num, first_to_check* first){
    int i = 0;
    bool controllo = true;
    nome_ing_t* cur =(*ricetta)->ricetta;
    while(i < (*ricetta)->num_ingr && controllo == true){
        controllo = check_ingrediente(magazzino, cur->ingrediente.key.ind, cur->ingrediente.key.num, t, num * cur->ingrediente.weight);
        if(controllo == false){
            first->key.ind = cur->ingrediente.key.ind;
            first->key.num = cur->ingrediente.key.num;
            first->weight = num * cur->ingrediente.weight;
        }
        i++;
        cur = cur->next_ingr;
    }
    return controllo;
}

bool check_ingrediente(magazzino_t** magazzino, int ind, int cc, int t, int num){ //in questa versione a num passo già il peso necessario (peso ricetta * num elementi)
    bool condizione = false;
    ingrediente_t* cur = (*magazzino)->pile[ind];
    while(cur != NULL && cur->num != cc){
        cur = cur->next_ingr;
    }
    if(cur != NULL){
        condizione = verifica(magazzino, &(cur->lotto), t, num, ind);
    }
    return condizione;
}

bool verifica(magazzino_t** magazzino, lotto_t** ingr, int t, int num, int ind){
    bool condizione = false;
    bool flag = true;
    lotto_t* cur = (*ingr);
    lotto_t* prev = NULL;
    if(cur != NULL){
        while(flag == true && condizione == false){
            if(cur == NULL){
                flag = false;
            }
            else{
                if(cur->scadenza <= t){
                    elimina_scaduti(magazzino,ingr, &cur, &prev, ind);
                }
                else{
                    if(cur->weight >= num){
                        condizione = true;
                    }
                    else
                    {
                        num = num - cur->weight;
                        cur = cur ->next_lotto;
                    }
                    
                }
            }
        }
    }
    return condizione;
}

int prepara(magazzino_t** magazzino, book_t** ricetta, int num){
    int i = 0;
    int peso = 0;
    int peso_tot = 0;
    nome_ing_t* cur = (*ricetta)->ricetta;
    while(i < (*ricetta)->num_ingr){
        peso = cur->ingrediente.weight * num;
        peso_tot= peso_tot +peso;
        rimuovi_ingredienti(magazzino, cur->ingrediente.key.ind, cur->ingrediente.key.num, peso);
        i++;
        cur = cur->next_ingr;
    }
    return peso_tot;
}

void rimuovi_ingredienti(magazzino_t** magazzino, int ind, int ingrediente, int num){
    ingrediente_t* cur = (*magazzino)->pile[ind];
    while(cur->num != ingrediente){
        cur = cur->next_ingr;
    }
    rimozione(magazzino, &(cur), ind, num);
    return;
}

void rimozione(magazzino_t** magazzino, ingrediente_t** ingrediente, int ind, int num){
    lotto_t* cur = (*ingrediente)->lotto;
    lotto_t* prev = NULL;
    while(num != 0){
        if(cur->weight > num){
            cur->weight = cur->weight - num;
            num = 0;
        }
        else{
            num = num - cur->weight;
            elimina_ingrediente(magazzino , ingrediente, &cur, &prev, ind);
        }
    }
    return;
}

void elimina_ingrediente(magazzino_t** magazzino, ingrediente_t** ingrediente, lotto_t** cur, lotto_t** prev, int ind){
    if(*prev == NULL){
        if((*cur)->next_lotto == NULL){
            free(*cur);
            (*ingrediente)->lotto = NULL;
            *cur = NULL;
        }
        else{
            (*ingrediente)->lotto = (*cur)->next_lotto;
            free(*cur);
            *cur = NULL;
            *cur = (*ingrediente)->lotto;
        }
    }
    else{
        (*prev)->next_lotto = (*cur)->next_lotto;
        free(*cur);
        *cur = NULL;
        *cur = (*prev)->next_lotto;
    }
    return;
}

void aggiungi_completi(albero_t** completi, ordine_t* ordine){
    if(*completi == NULL){
        *completi = crea_albero_completi(ordine); 
    }
    else{
        insertion(completi, ordine);
    }
    return;
}

albero_t* crea_albero_completi(ordine_t* ordine){
    albero_t* albero = (albero_t*)malloc(sizeof(albero_t));
    nodo_t* nodo = (nodo_t*)malloc(sizeof(nodo_t));
    nodo_t* nill = (nodo_t*)malloc(sizeof(nodo_t));
    nodo->ordine = ordine;
    nodo->colore = RED;
    nodo->d = nill;
    nodo->s = nill;
    nodo->p = NULL;
    nill->colore = NILL;
    nill->s = NULL;
    nill->d = NULL;
    nill->p = NULL;
    albero->root= nodo;
    albero->nill= nill;
    return albero;
} 

void insertion(albero_t** completi, ordine_t* ordine){
    if (completi == NULL || *completi == NULL || ordine == NULL) {
        return;
    }
    nodo_t* nodo = (nodo_t*)malloc(sizeof(nodo_t));
    nodo->ordine = ordine;
    nodo->p = NULL;
    nodo->d = NULL;
    nodo->s = NULL;
    nodo->colore = RED;
    nodo_t* y = (*completi)->nill;
    nodo_t* x = (*completi)->root;
    while( x != (*completi)->nill){
        y = x;
        if (nodo->ordine->arrivo < x->ordine->arrivo){
            x = x->s;
        }
        else{
            x = x->d;
        }
    }
    nodo->p= y;
    if(y == (*completi)->nill || y == NULL || y->colore == NILL){
        (*completi)->root = nodo;
    }
    else{
        if (nodo->ordine->arrivo<y->ordine->arrivo){
            y->s= nodo;
        }
        else{
            y->d = nodo;
        }
    }
    nodo->d = (*completi)->nill;
    nodo->s = (*completi)->nill;
    nodo->colore = RED;
    RB_INSERT_FIXUP(completi, &nodo);
}

void RB_INSERT_FIXUP(albero_t** completi, nodo_t** nodo){
    if (*nodo == NULL || *completi == NULL || (*completi)->root == NULL) {
        return;
    }
    if(*nodo == (*completi)->root){
        (*completi)->root->colore = BLACK;
        return;
    }
    else{
        nodo_t* x = (*nodo)->p;
        if (x != NULL && x->colore == RED){
            if(x -> p != NULL && x->p->s != NULL && x == x->p->s){
                nodo_t* y = x->p->d;
                if(y != NULL && y->colore == RED){
                    x->colore= BLACK;
                    y->colore= BLACK;
                    x->p->colore = RED;
                    RB_INSERT_FIXUP(completi, &(x->p));
                }
                else{
                    if(*nodo == x->d){
                        *nodo = x;
                        left_rotate(completi, nodo);
                        x = (*nodo)->p;
                    }
                    if(x != NULL && x->p != NULL){
                        x->colore= BLACK;
                        x->p->colore = RED;
                        right_rotate(completi, &(x->p));
                    }
                }
            }
            else{
                if(x->p != NULL){
                    nodo_t* y = x->p->s;
                    if(y != NULL && y->colore == RED){
                        x->colore= BLACK;
                        y->colore= BLACK;
                        x->p->colore = RED;
                        RB_INSERT_FIXUP(completi, &(x->p));
                    }
                    else{
                        if(*nodo == x->s){
                            *nodo = x;
                            right_rotate(completi, nodo);
                            x = (*nodo)->p;
                        }
                        if (x != NULL && x->p != NULL) {
                            x->colore = BLACK;
                            x->p->colore = RED;
                            left_rotate(completi, &(x->p));
                        }
                    }
                }
            }
        }
        
    }
    return;
}

void right_rotate(albero_t** completi, nodo_t** nodo){
   if (completi == NULL || *completi == NULL || nodo == NULL || *nodo == NULL) {
        return;
    }
    nodo_t* y = (*nodo);
    nodo_t* x = (*nodo)->s;
    if (x == NULL) {
        return; 
    }
    y->s = x->d;
    if (x->d != NULL && x->d != (*completi)->nill){
        x->d->p = y;
    }
    x->p = y->p;
    if (y->p == NULL || y->p == (*completi)->nill){
        (*completi)->root = x;
    }
    else{
        if(y == y->p->d){
            y->p->d = x;
        }
        else{
            y->p->s = x;
        }
    }
    x->d = y;
    y->p = x;
    return;
}

void left_rotate(albero_t** completi, nodo_t** nodo) {
    if (completi == NULL || *completi == NULL || nodo == NULL || *nodo == NULL) {
        return;
    }

    nodo_t* x = *nodo;
    nodo_t* y = x->d;
    if (y == NULL || y == (*completi)->nill) {
        return;
    }

    x->d = y->s;
    if (y->s != NULL && y->s != (*completi)->nill) {
        y->s->p = x;
    }
    y->p = x->p;
    if (x->p == NULL || x->p == (*completi)->nill) {
        (*completi)->root = y;
    } else {
        if (x == x->p->s) {
            x->p->s = y;
        } else {
            x->p->d = y;
        }
    }
    y->s = x;
    x->p = y;
    return;
}

nodo_t* trova_next(albero_t** completi, nodo_t* cur){
    if (completi == NULL || *completi == NULL || cur == NULL) {
        return NULL; 
    }

    nodo_t* next = NULL;

    if (cur == (*completi)->root && cur == (*completi)->nill) {
        return NULL; 
    }

    if (cur->d != NULL && cur->d != (*completi)->nill) {
        next = cur->d;
        while (next->s != NULL && next->s != (*completi)->nill) {
            next = next->s;
        }
        return next;
    } else {
        next = cur;
        while (next->p != NULL && next == next->p->d) {
            next = next->p;
        }
        if (next->p != NULL) {
            next = next->p;
        } else {
            next = NULL;
        }
    }

    return next;
}
/*
nodo_t* trova_next(albero_t** completi, nodo_t* cur) {
    if (completi == NULL || *completi == NULL || cur == NULL) {
        return NULL; 
    }

    nodo_t* nill = (*completi)->nill;  // Puntatore al nodo sentinella `nill`
    nodo_t* next = NULL;

    // Caso 1: Se `cur` ha un sottoalbero destro, il successore è il minimo nel sottoalbero destro.
    if (cur->d != nill) {
        next = cur->d;
        while (next->s != nill) {
            next = next->s;
        }
        return next;
    } 
    
    // Caso 2: Se `cur` non ha un sottoalbero destro, risaliamo l'albero fino a trovare un nodo che sia il figlio sinistro.
    next = cur->p;
    while (next != NULL && cur == next->d) {
        cur = next;
        next = next->p;
    }

    return next;
}*/

void crea_coda_attesa(attesa_t** attesa, ordine_t* ordine, first_to_check first){
    (*attesa) = (attesa_t*)malloc(sizeof(attesa_t));
    nodo_attesa_t* nodo = (nodo_attesa_t*)malloc(sizeof(nodo_attesa_t));
    nodo->ordine = ordine;
    nodo->first = first;
    nodo->next_ordine = NULL;
    (*attesa)->testa = nodo;
    (*attesa)->coda = (*attesa)->testa;
}

void aggiungi_attesa(attesa_t** attesa, ordine_t* ordine, first_to_check first){ //questi la prima volta che ci entrano sono a NULL, poi la funzione li modifica
    if(*attesa == NULL){
        crea_coda_attesa(attesa, ordine, first);
    }
    else{
        nodo_attesa_t* cur = (*attesa)->coda;
        cur->next_ordine = (nodo_attesa_t*)malloc(sizeof(nodo_attesa_t));
        cur->next_ordine->next_ordine = NULL;
        cur->next_ordine->ordine = ordine;
        cur->next_ordine->first = first;
        (*attesa)->coda= cur->next_ordine;
    }
    return;
}

void check_attesa(attesa_t** attesa, magazzino_t** magazzino, ricettario_t** ricettario, int t, albero_t** completi){
    int c = 0;
    
    if(*attesa == NULL){
        return;
    }
    nodo_attesa_t* cur = (*attesa)->testa;
    nodo_attesa_t* prev = NULL;
    while(cur != NULL){
        c++;
        if (controlla_first(magazzino, cur->first, t) == true){
            book_t* ricetta = trova_ricetta_attesa(ricettario, cur->ordine->key, true);
            if(controlla_ricetta(magazzino, &ricetta, t, cur->ordine->num_el, &cur->first) == true){
                cur->ordine->peso = prepara(magazzino, &ricetta, cur->ordine->num_el);
                //printf("tempo: %d, ordine: %d, attesa->completi\n", t, cur->ordine->arrivo);
                aggiungi_completi(completi, cur->ordine);
                rimuovi_attesa(attesa, &cur, &prev);
            }
            else{
                prev = cur;
                cur = cur->next_ordine;
            }
       }
        else{
            prev = cur;
            cur = cur->next_ordine;
        }
       /*nome_ing_t* tmp = ricetta->ricetta;
        while (tmp != NULL){
            ricetta->ricetta = ricetta->ricetta->next_ingr;
            free(tmp);
            tmp = NULL;
            tmp = ricetta->ricetta;
        }
        free(cur);
        cur = NULL;*/
    }
    return;
}

bool controlla_first(magazzino_t** magazzino, first_to_check first, int t){
    return check_ingrediente(magazzino, first.key.ind, first.key.num, t, first.weight);    
}

book_t* trova_ricetta_attesa(ricettario_t** ricettario, chiave key, bool cond){
    book_t* ric = (*ricettario)->ricette[key.ind]->ric;
    while(ric->key.num != key.num){
        ric = ric->next_recipe;
    }
    return ric;
}

void rimuovi_attesa(attesa_t** attesa, nodo_attesa_t** cur, nodo_attesa_t** prev){
    if(*prev == NULL){
        if((*cur)->next_ordine != NULL){ 
            (*attesa)->testa = (*attesa)->testa->next_ordine;
            (*cur)->ordine = NULL;
            free(*cur);
            *cur = NULL;
            *cur = (*attesa)->testa;
        }
        else{
            (*attesa)->testa = (*cur);
            (*attesa)->coda = (*attesa)->testa;
            (*cur)->ordine = NULL;
            free(*cur);
            *cur = NULL;
            (*attesa) = NULL;
        }
    }
    else{
        if((*cur)->next_ordine == NULL){
            (*prev)->next_ordine = NULL;
            (*attesa)->coda = (*prev);
            (*cur)->ordine = NULL;
            free(*cur);
            *cur = NULL;
        }
        else{
            (*prev)->next_ordine = (*cur)->next_ordine;
            (*cur)->ordine = NULL;
            free(*cur);
            (*cur) = NULL;
            (*cur) = (*prev)->next_ordine;
        }
    }
}

void cammioncino(albero_t** completi, ricettario_t** ricettario, int cap){
    if (*completi == NULL || (*completi)->root == (*completi)->nill){
        printf("camioncino vuoto\n");
        return;
    }
    nodo_t* cur = (*completi)->root;
    nodo_t* next = NULL;
    nodo_attesa_t* corriere = NULL;
    nodo_attesa_t* tmp = corriere;
    int i = 0;
    while (cur->s != (*completi)->nill){
        cur = cur->s;
    }
    while(cur != NULL && cur != (*completi)->nill && cur->ordine->peso <= cap){
        cap = cap - cur->ordine->peso;
        next = trova_next(completi, cur);
        tmp = aggiungi_corriere(&corriere, &tmp, cur->ordine);
        cur = rbdelete(completi, &cur);
        cur->p = NULL;
        cur->s = NULL;
        cur->d = NULL;
        free(cur);
        cur = NULL;
        cur = next;
        i++;
    }
    sorta_corriere(&corriere, i);
    stampa_elimina(&corriere, ricettario, i);
    return;
}

nodo_attesa_t* aggiungi_corriere(nodo_attesa_t**corriere ,nodo_attesa_t** tmp, ordine_t* ordine){
    if((*corriere) == NULL){
        (*corriere) = (nodo_attesa_t*)malloc(sizeof(nodo_attesa_t));
        (*corriere)->next_ordine = NULL;
        (*corriere)->ordine = ordine;
        *tmp = *corriere; 
    }
    else{
        (*tmp)->next_ordine = (nodo_attesa_t*)malloc(sizeof(nodo_attesa_t));
        (*tmp)->next_ordine->next_ordine = NULL;
        (*tmp)->next_ordine->ordine = ordine;
        (*tmp) = (*tmp)->next_ordine;
    }
    return (*tmp);
}
/*
nodo_t* rbdelete(albero_t** completi, nodo_t** cur){
    if (completi == NULL || *completi == NULL || cur == NULL || *cur == NULL) {
        return NULL;
    }

    nodo_t* y = NULL;
    nodo_t* x = NULL;

    if ((*cur)->s == NULL || (*cur)->d == NULL || (*cur)->s == (*completi)->nill || (*cur)->d == (*completi)->nill) {
        y = (*cur);
    } else {
        y = trova_next(completi, *cur);
    }

    if (y->s != NULL && y->s != (*completi)->nill) {
        x = y->s;
    } else {
        x = y->d;
    }

    if (x != NULL) {
        x->p = y->p;
    }

    if (y->p == NULL || y->p == (*completi)->nill) {
        (*completi)->root = x;
    } else {
        if (y == y->p->s) {
            y->p->s = x;
        } else {
            y->p->d = x;
        }
    }

    if (y != *cur) {
        (*cur)->ordine = y->ordine;
    }

    if (y->colore == BLACK && x != NULL) {
        RB_DELETE_FIXUP(completi, &x);
    }

    return y;
}

void RB_DELETE_FIXUP(albero_t** completi, nodo_t** x) {
    if (completi == NULL || *completi == NULL || x == NULL || *x == NULL) {
        return;
    }

    nodo_t* w = NULL;

    while ((*x) != (*completi)->root && (*x)->colore == BLACK) {
        if ((*x) == (*x)->p->s) {
            w = (*x)->p->d;

            if (w != NULL && w->colore == RED) {
                w->colore = BLACK;
                (*x)->p->colore = RED;
                left_rotate(completi, &((*x)->p));
                w = (*x)->p->d;
            }

            if ((w == NULL || w->s->colore == BLACK) && (w->d == NULL || w->d->colore == BLACK)) {
                if (w != NULL) w->colore = RED;
                *x = (*x)->p;
            } else {
                if (w != NULL && (w->d == NULL || w->d->colore == BLACK)) {
                    if (w->s != NULL) w->s->colore = BLACK;
                    w->colore = RED;
                    right_rotate(completi, &(w));
                    w = (*x)->p->d;
                }

                if (w != NULL) w->colore = (*x)->p->colore;
                (*x)->p->colore = BLACK;
                if (w != NULL && w->d != NULL) w->d->colore = BLACK;
                left_rotate(completi, &((*x)->p));
                *x = (*completi)->root;
            }
        } else {
            w = (*x)->p->s;

            if (w != NULL && w->colore == RED) {
                w->colore = BLACK;
                (*x)->p->colore = RED;
                right_rotate(completi, &((*x)->p));
                w = (*x)->p->s;
            }

            if ((w == NULL || w->d->colore == BLACK) && (w->s == NULL || w->s->colore == BLACK)) {
                if (w != NULL) w->colore = RED;
                *x = (*x)->p;
            } else {
                if (w != NULL && (w->s == NULL || w->s->colore == BLACK)) {
                    if (w->d != NULL) w->d->colore = BLACK;
                    w->colore = RED;
                    left_rotate(completi, &(w));
                    w = (*x)->p->s;
                }

                if (w != NULL) w->colore = (*x)->p->colore;
                (*x)->p->colore = BLACK;
                if (w != NULL && w->s != NULL) w->s->colore = BLACK;
                right_rotate(completi, &((*x)->p));
                *x = (*completi)->root;
            }
        }
    }

    (*x)->colore = BLACK;
}*/

nodo_t* rbdelete(albero_t** completi, nodo_t** cur) {
    if (completi == NULL || *completi == NULL || cur == NULL || *cur == NULL) {
        return NULL;
    }

    nodo_t* y = *cur;
    nodo_t* x;
    nodo_t* nill = (*completi)->nill;
    int y_original_color = y->colore;

    if ((*cur)->s == nill) {
        x = (*cur)->d;
        rb_transplant(completi, *cur, (*cur)->d);
    } else if ((*cur)->d == nill) {
        x = (*cur)->s;
        rb_transplant(completi, *cur, (*cur)->s);
    } else {
        y = trova_next(completi, *cur);
        y_original_color = y->colore;
        x = (y->s != nill) ? y->s : y->d;

        if (y->p != NULL && y->p != nill && y->p != *cur) {
            rb_transplant(completi, y, y->d);
            y->d = (*cur)->d;
            if (y->d != nill) y->d->p = y;
        }

        rb_transplant(completi, *cur, y);
        y->s = (*cur)->s;
        if (y->s != nill) y->s->p = y;
        y->colore = (*cur)->colore;
    }

    if (y_original_color == BLACK && x != NULL) {
        RB_DELETE_FIXUP(completi, &x);
    }

    return y;
}

void RB_DELETE_FIXUP(albero_t** completi, nodo_t** x) {
    while (*x != (*completi)->root && (*x)->colore == BLACK) {
        if ((*x)->p == NULL) break; // Protezione aggiuntiva per evitare dereferenziazione di un puntatore NULL

        if (*x == (*x)->p->s) {
            nodo_t* w = (*x)->p->d;

            if (w->colore == RED) {
                w->colore = BLACK;
                (*x)->p->colore = RED;
                left_rotate(completi, &((*x)->p));
                w = (*x)->p->d;
            }

            if (w->s->colore == BLACK && w->d->colore == BLACK) {
                w->colore = RED;
                *x = (*x)->p;
            } else {
                if (w->d->colore == BLACK) {
                    w->s->colore = BLACK;
                    w->colore = RED;
                    right_rotate(completi, &w);
                    w = (*x)->p->d;
                }

                w->colore = (*x)->p->colore;
                (*x)->p->colore = BLACK;
                w->d->colore = BLACK;
                left_rotate(completi, &((*x)->p));
                *x = (*completi)->root;
            }
        } else {
            nodo_t* w = (*x)->p->s;

            if (w->colore == RED) {
                w->colore = BLACK;
                (*x)->p->colore = RED;
                right_rotate(completi, &((*x)->p));
                w = (*x)->p->s;
            }

            if (w->d->colore == BLACK && w->s->colore == BLACK) {
                w->colore = RED;
                *x = (*x)->p;
            } else {
                if (w->s->colore == BLACK) {
                    w->d->colore = BLACK;
                    w->colore = RED;
                    left_rotate(completi, &w);
                    w = (*x)->p->s;
                }

                w->colore = (*x)->p->colore;
                (*x)->p->colore = BLACK;
                w->s->colore = BLACK;
                right_rotate(completi, &((*x)->p));
                *x = (*completi)->root;
            }
        }
    }

    (*x)->colore = BLACK;
}

void rb_transplant(albero_t** completi, nodo_t* u, nodo_t* v) {
    nodo_t* nill = (*completi)->nill;

    if (u->p == NULL || u->p == nill) {  // Verifica se u è la radice
        (*completi)->root = v;
    } else if (u == u->p->s) {  // u è il figlio sinistro
        u->p->s = v;
    } else {  // u è il figlio destro
        u->p->d = v;
    }

    if (v != nill) {  // Assegna il padre solo se v non è il nodo sentinella
        v->p = u->p;
    }
}

void sorta_corriere(nodo_attesa_t** corriere, int i){
    nodo_attesa_t* cur = NULL;
    nodo_attesa_t* prev = NULL;
    nodo_attesa_t* next = NULL;
    for(i--; i != 0; i--){
        for(cur = *corriere, next = (*corriere)->next_ordine, prev = NULL; next != NULL; prev = cur, cur = next, next = next->next_ordine){
            if(next->ordine->peso > cur->ordine->peso){
                if(prev == NULL){
                    (*corriere) = cur->next_ordine;
                    cur->next_ordine = (*corriere)->next_ordine;
                    (*corriere)->next_ordine = cur;
                }
                else{
                    prev->next_ordine = next;
                    cur->next_ordine = next->next_ordine;
                    next->next_ordine = cur;
                    cur = next;
                    next = next->next_ordine;
                }
            }
        }
    }
    return;
}

void stampa_elimina(nodo_attesa_t** corriere, ricettario_t** ricettario, int i){
    nodo_attesa_t* cur = *corriere;
    nodo_attesa_t* prev = NULL;
    int c = 0;
    while(cur != NULL && c < i){
        book_t* tmp = (*ricettario)->ricette[cur->ordine->key.ind]->ric;
        while(tmp->key.num != cur->ordine->key.num){
            tmp = tmp->next_recipe;
        }
        printf("%d ", cur->ordine->arrivo);
        printf("%s ", tmp->recipe_name);
        printf("%d\n", cur->ordine->num_el);
        prev = cur;
        cur = cur->next_ordine;
        free(prev->ordine);
        free(prev);
        prev = NULL;
        c++;
    }
    return;
}
/*
void aux_fun(attesa_t** attesa){
    nodo_attesa_t* cur = (*attesa)->testa;
    while(cur != NULL && cur->ordine->arrivo != 6624){
        cur = cur->next_ordine;
    }
    if(cur != NULL){
        printf("trovato\n");
    }
    else{
        printf("no\n");
    }
}
*/
/**/ 
void aux_complete(nodo_t* nodo){
    if(nodo->ordine == NULL){
        return;
    }
    else if( nodo->ordine->arrivo == 2716){
        printf("trovato nei completi\n");
        printf("%d", nodo->p->ordine->arrivo);
    }
    aux_complete(nodo->s);
    aux_complete(nodo->d);
    return;
}

void aux_check_integrety(nodo_t* nodo){
    if(nodo->ordine == NULL){
        return;
    }
    if( nodo->d->ordine != NULL && nodo-> d -> ordine->arrivo < nodo-> ordine->arrivo){
        printf("error d\n");
    }
    if(nodo->s->ordine != NULL && nodo->s->ordine->arrivo > nodo->ordine->arrivo){
        printf("error s\n");
    }
    aux_check_integrety(nodo->s);
    aux_check_integrety(nodo->d);
    return;
}