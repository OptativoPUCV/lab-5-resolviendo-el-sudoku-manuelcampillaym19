#include <stdio.h>
#include <stdlib.h>
#include "list.h"


typedef struct{
   int sudo[9][9];
}Node;

Node* createNode(){
  Node* n=(Node*) malloc(sizeof(Node));
  return n;
}

Node* copy(Node* n){
    Node* new=(Node*) malloc(sizeof(Node));
    *new = *n;
    return new;
}

Node* read_file (char* file_name){
  Node* n = createNode();
  FILE* file = fopen (file_name, "r");
  int i,j;
  for(i=0;i<9;i++){
       for(j=0;j<9;j++){
          if(!fscanf (file, "%d", &n->sudo[i][j]))
            printf("failed to read data!");
       }
  }

  fclose (file);
  return n;
}

void print_node(Node* n){
    int i,j;
    for(i=0;i<9;i++){
       for(j=0;j<9;j++)
          printf("%d ", n->sudo[i][j]);
       printf("\n");
    }
    printf("\n");
}

int is_valid(Node* n){
    int i, j, k, p;

    // Verificar filas
    for(i = 0; i < 9; i++){
        int seen[10] = {0}; // arreglo de marcas
        for(j = 0; j < 9; j++){
            int num = n->sudo[i][j];
            if(num != 0){
                if(seen[num]) return 0; // número repetido en fila
                seen[num] = 1;
            }
        }
    }

    // Verificar columnas
    for(j = 0; j < 9; j++){
        int seen[10] = {0};
        for(i = 0; i < 9; i++){
            int num = n->sudo[i][j];
            if(num != 0){
                if(seen[num]) return 0; // repetido en columna
                seen[num] = 1;
            }
        }
    }

    // Verificar submatrices 3x3
    for(k = 0; k < 9; k++){
        int seen[10] = {0};
        for(p = 0; p < 9; p++){
            int i = 3*(k/3) + (p/3);
            int j = 3*(k%3) + (p%3);
            int num = n->sudo[i][j];
            if(num != 0){
                if(seen[num]) return 0; // repetido en submatriz
                seen[num] = 1;
            }
        }
    }

    return 1; // tablero válido
}




List* get_adj_nodes(Node* n){
    List* list = createList();
    int i, j;

    // Buscar la primera celda vacía
    for(i = 0; i < 9; i++){
        for(j = 0; j < 9; j++){
            if(n->sudo[i][j] == 0){
                // Probar con números del 1 al 9
                for(int val = 1; val <= 9; val++){
                    Node* adj = copy(n);         // crear copia del tablero
                    adj->sudo[i][j] = val;       // colocar el número
                    if(is_valid(adj))            // si el tablero es válido
                        pushBack(list, adj);     // agregarlo a la lista
                    else
                        free(adj);               // si no, liberar memoria
                }
                // Solo generar en la PRIMERA celda vacía encontrada
                return list;
            }
        }
    }

    // Si no había celdas vacías, retornar lista vacía
    return list;
}



int is_final(Node* n){
    for(int i = 0; i < 9; i++){
        for(int j = 0; j < 9; j++){
            if(n->sudo[i][j] == 0)
                return 0; // aún hay casillas vacías
        }
    }
    return 1; // todas las casillas están llenas
}


Node* DFS(Node* initial, int* cont){
    Stack* S = createStack();
    push(S, initial);

    while(top(S) != NULL){
        Node* current = top(S);
        pop(S);
        (*cont)++;

        if(is_final(current)){
            return current; // se encontró una solución
        }

        List* adj = get_adj_nodes(current);
        Node* adjNode = first(adj);
        while(adjNode != NULL){
            push(S, adjNode);
            adjNode = next(adj);
        }

        free(current); // liberar nodo explorado
    }

    return NULL; // no se encontró solución
}




/*
int main( int argc, char *argv[] ){

  Node* initial= read_file("s12a.txt");;

  int cont=0;
  Node* final = DFS(initial, &cont);
  printf("iterations:%d\n",cont);
  print_node(final);

  return 0;
}*/