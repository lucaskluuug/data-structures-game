#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

typedef struct match{
    char name[50];
    int groups[10];
    int n_groups;
    struct match *next;
}MATCH;

typedef struct finished_match{
    char name_game[50];
    char name_coor[50];
    int winner_group;
    int n_finished;
    struct finished_match *next;
}FMATCH;
FMATCH *init_finished;
int n_finished;

typedef struct queue{
    MATCH *beg;
    MATCH *end;
}QUEUE;

typedef struct game{
    char name[50];
    char link[50];
    struct game *next;
    QUEUE *queue;
}GAME;
GAME *init;

void insert_game(char *name, char *link){
    GAME *aux, *pre, *new = (GAME*)malloc(sizeof(GAME));

    strcpy(new->name, name);
    strcpy(new->link, link);
    new->queue = NULL;

    if(init == NULL){
        new->next = NULL;
        init = new;
    } else if(strcmp(new->name, init->name)<=0){
        new->next = init;
        init = new;
    } else{
        aux = init;
        while(aux != NULL && strcmp(new->name, aux->name)>0){
            pre = aux;
            aux = aux->next;
        }
        pre->next = new;
        if(aux != NULL){
            new->next = aux;
        }
    }
}

void show_all_games(){
    GAME *aux = init;

    printf("\n");
    while(aux != NULL){
        printf("Name: %s\nLink: %s\n\n", aux->name, aux->link);
        aux = aux->next;
    }
}

void games_count(){
    GAME *aux = init;
    int count = 0;

    printf("\n");
    while(aux != NULL){
        aux = aux->next;
        count++;
    }

    if(count==0){
        printf("There is no game created yet.\n");
    } else if(count == 1){
        printf("There is just one game.\n");
    } else{
        printf("There are %d games.\n", count);
    }
}

void show_game_queue(char *name){
    GAME *aux = init;
    MATCH *m = NULL;
    int i, count = 0;

    while (aux != NULL && strcmp(aux->name, name) != 0) {
        aux = aux->next;
    }
    
    if (aux == NULL){
        printf("Error: Game not found.\n");
    } else{
        m = aux->queue->beg;
        printf("Game: %s\n\n", name);
        while(m != NULL){
            printf("Match %d\nCoordinator name: %s\nGroups: ", count++, m->name);
            for(i=0; i<m->n_groups; i++){
                printf("%d ", m->groups[i]);
            }
            printf("\n\n");
            m = m->next;
        }
    }
}

void insert_new_match(char *name_game, int n_groups, int *groups, char *name_coord){
    GAME *aux = init;
    int i;

    while (aux != NULL && strcmp(aux->name, name_game) != 0) {
        aux = aux->next;
    }

    if (aux == NULL){
        printf("Error: Game not found.\n");
    } else{
        MATCH *m = (MATCH*)malloc(sizeof(MATCH));
        m->next = NULL;
        strcpy(m->name, name_coord);
        m->n_groups = n_groups;
        for(i=0; i<n_groups; i++){
            m->groups[i] = groups[i];
        }
        if(aux->queue == NULL){
            QUEUE *q = (QUEUE*)malloc(sizeof(QUEUE));
            aux->queue = q;
            q->beg = m;
            q->end = m;
        } else{
            if(aux->queue->beg == NULL){
                aux->queue->end = m;
                aux->queue->beg = m;
            } else{
                aux->queue->end->next = m;
                aux->queue->end = m;
            }
        }
    }
}

/*void insert_finished_list(MATCH *m, char *game, int num, int winner){
    FMATCH *aux = init_finished;
    FMATCH *new = (FMATCH*)malloc(sizeof(FMATCH)), *pre;

    strcpy(new->name_coor, m->name);
    strcpy(new->name_game, game);
    new->next = NULL;
    new->winner_group = winner;
    new->n_finished = num;

    if(aux == NULL){
        init_finished = new;
    } else if(strcmp(new->name_game, aux->name_game)<=0){
        new->next = init_finished;
        init_finished = new;
    } else{
        while(aux != NULL && strcmp(new->name_game, aux->name_game)>0){
            pre = aux;
            aux = aux->next;
        }
        pre->next = new;
        if(aux != NULL){
            new->next = aux;
        }
    }
}*/

void insert_finished_list(MATCH *m, char *game, int num, int winner){
    FMATCH *aux = init_finished;
    FMATCH *new = (FMATCH*)malloc(sizeof(FMATCH)), *pre;

    strcpy(new->name_coor, m->name);
    strcpy(new->name_game, game);
    new->next = NULL;
    new->winner_group = winner;
    new->n_finished = num;

    if(aux == NULL){
        aux = new;
    } else if(strcmp(new->name_game, aux->name_game)<=0){
        new->next = aux;
        aux = new;
    } else{
        while(aux != NULL && strcmp(new->name_game, aux->name_game)>0){
            pre = aux;
            aux = aux->next;
        }
        pre->next = new;
        if(aux != NULL){
            new->next = aux;
        }
    }
}

MATCH *start_match(char *name){
    GAME *aux = init;
    int i;

    while (aux != NULL && strcmp(aux->name, name) != 0) {
        aux = aux->next;
    }

    if (aux == NULL){
        printf("Error: Game not found.\n");
    } else{
        if(aux->queue == NULL){
            printf("Error: No match in this game.\n");
        } else{
            MATCH *m = aux->queue->beg;
            if(m == NULL){
                printf("Error: No match in this game.\n");
            } else{
                aux->queue->beg = m->next;
                m->next = NULL;
                if(aux->queue->beg == NULL){
                    aux->queue->end = NULL;
                }
                n_finished++;
                srand(time(NULL));
                i = rand()%m->n_groups;
                insert_finished_list(m, aux->name, n_finished, m->groups[i]);
                free(m);
                return aux->queue->beg;
            }
        }
    }
}

void show_finished_list(){
    FMATCH *aux = init_finished;

    if(aux == NULL){
        printf("Error: No finished match.\n");
    } else{
        while(aux != NULL){
            printf("Finish number: %d\nName game: %s\nName coordinator: %s\nWinner group: %d\n\n", aux->n_finished, aux->name_game, aux->name_coor, aux->winner_group);
            aux = aux->next;
        }
    }
}

void show_filtered_finished_list(char *name){
    FMATCH *aux = init_finished;

    if(aux == NULL){
        printf("Error: No finished match.\n");
    } else{
        while(aux != NULL){
            if(strcmp(name, aux->name_game)==0){
                printf("Finish number: %d\nName game: %s\nName coordinator: %s\nWinner group: %d\n\n", aux->n_finished, aux->name_game, aux->name_coor, aux->winner_group);
            }
            aux = aux->next;
        }
    }
}

void finish_game_queue(char *name){
    MATCH *m = NULL;

    do{
        m = start_match(name);
    }while(m != NULL);
}

void remove_game(char *name){
    GAME *pre = NULL, *aux = init;

    while (aux != NULL && strcmp(aux->name, name) != 0) {
            pre = aux;
            aux = aux->next;
        }

        if (aux == NULL){
            printf("Error: Game not found.\n");
        } else{
            finish_game_queue(name);
            if (pre == NULL){
                if(aux->next == NULL){
                    init = NULL;
                } else{
                    init = aux->next;
                }
            } else {
                pre->next = aux->next;
            }
        }
        free(aux);
}

void show_winner_group(){
    int i, bigger, winner, arr[50];
    bigger = 0;
    winner = 0;
    FMATCH *aux = init_finished;

    for(i=0; i<50; i++){
        arr[i] = 0;
    }

    if(aux == NULL){
        printf("Error: No match was finished.\n");
    } else{
        while(aux != NULL){
            arr[aux->winner_group]++;
            aux = aux->next;
        }
        for(i=0; i<50; i++){
            if(arr[i]>bigger){
                bigger = arr[i];
                winner = i; 
            }
        }
        printf("Winner group: %d\nGames won: %d\n", winner, bigger);
    }
}

void user_interface(){
    char name[50], link[50], name_coord[50];
    int i, n, groups[10], n_grps = 0;
    

    printf("\n0. Exit. \n1. Insert new game. \n2. Remove an existing game. \n3. Show a game queue. \n4. Show all games. \n5. Start match. \n6. Insert new match. \n7. Finish game queue. \n8. Show finished matches. \n9. Show finished matches filtered by game.\n10. Group that won the most matches.\n11. How many active games exists?\n");
    scanf("%d", &n);
    switch(n){
        default: 
            printf("Error - Invalid option.\n");
            break;
        case 0:
            exit(0);
        case 1: 
            printf("Game name: \n");
            getchar();
            gets(name);
            printf("Link: \n");
            gets(link);
            insert_game(name, link);
            break;

        case 2:
            printf("Game name: \n");
            getchar();
            gets(name);
            remove_game(name);
            break;

        case 3:
            printf("Game name: \n");
            getchar();
            gets(name);
            show_game_queue(name);
            break;

        case 4:
            show_all_games();
            break;

        case 5: 
            //ok void insert_finished_list(MATCH *m, char *game){ BUG
            printf("Game name: \n");
            getchar();
            gets(name);
            start_match(name);
            break;

        case 6:
            printf("Game name: \n");
            getchar();
            gets(name);
            printf("Coordinator name: \n");
            gets(name_coord);
            printf("How many groups? \n");
            scanf("%d", &n_grps);
            for(i=0; i<n_grps; i++){
                scanf("%d", &groups[i]);
            }
            insert_new_match(name, n_grps, groups, name_coord);
            break;

        case 7:
            printf("Game name: \n");
            getchar();
            gets(name);
            finish_game_queue(name);
            break;
        
        case 8:
            show_finished_list();
            break;
        
        case 9:
            printf("Game name: \n");
            getchar();
            gets(name);
            show_filtered_finished_list(name);
            break;
        
        case 10:
            show_winner_group();
            break;
        
        case 11:
            games_count();

        break;
    }
    user_interface();
}

int main(){
    init_finished = NULL;
    init = NULL;

    user_interface();
}

// Bugs:
//Aux na função insert_finished_list