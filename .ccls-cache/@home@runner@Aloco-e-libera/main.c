#include <stdio.h>
#include <math.h>
#include <string.h>

#define BUFFERSIZE    4096
#define CONTROLSIZE    BUFFERSIZE/8

unsigned char BUFFER[BUFFERSIZE];
unsigned char CONTROL[CONTROLSIZE] = {0};

unsigned char last_byte(unsigned int num){
    return num & 0xFF; // Extrai o ultimo byte de um numero
}


void *aloca(size_t size){
    if(size > 255) return NULL;

    unsigned char last_bits = last_byte(size);
    
    // Percorre CONTROLE bit a bit
    int bit_count = 0;
    for(int i = 0; i < CONTROLSIZE && (bit_count + size < BUFFERSIZE); i++){
        for(int j = 0; j < 8 && (bit_count + size < BUFFERSIZE); j++){
            short int bit = (CONTROL[i] >> j) & 1;
            
            if(bit == 0){
                int count_size = 0;
                int no_size = 1;

                for(int k = i; count_size < (size + 1) && no_size == 1; k++){
                    for(int l = j; l < 8 && count_size < (size + 1) && no_size == 1; l++){
                        short int bit = (CONTROL[k] >> l) & 1;
                        
                        if(bit == 0){
                            count_size++;
                        }
                        else{
                            no_size = 0;
                        }
                    }
                }
                if(no_size == 1){
                    int bit_offset = bit_count % 8;
                    CONTROL[i] |= (1 << bit_offset);
                    BUFFER[bit_count] = last_bits;
                    return &BUFFER[++bit_count];
                }

            }
            bit_count += (BUFFER[bit_count] + 1);
            i = (int)(bit_count/8);
            j = (bit_count % 8) - 1;
            
        }
    }
    return NULL;
}

void libera(void *ptr){
    int bit_count = 0;
    for(int i = 0; i < CONTROLSIZE; i++){
        for(int j = 0; j < 8; j++){
            short int bit = (CONTROL[i] >> j) & 1;
            if(bit == 1){
                if(ptr == &BUFFER[bit_count + 1]) {
                    int bit_offset = (bit_count % 8);
                    CONTROL[i] &= ~(1 << bit_offset);
                    BUFFER[bit_count] = (char)0;
                    return;
                }
                bit_count += (BUFFER[bit_count] + 1);
                i = (int)(bit_count/8);
                j = (bit_count % 8) - 1;
            }else{
                bit_count++;
            }
        }
    }
}

void print_buffer(){
    printf("\nPrint Buffer:\n");
    for(int i = 0; i < BUFFERSIZE; i++)
        printf("%d ", BUFFER[i]);
    printf("\n");
}

void print_control(){
    printf("\nPrint Control:\n");
    for(int i = 0; i < CONTROLSIZE; i++){
        for(int j = 0; j < 8; j++){
            short int bit = (CONTROL[i] >> j) & 1;
            printf("%d ", bit);
        }
    }
    printf("\n");
}

char *minha_strdup(const char *str) {
    size_t len = strlen(str) + 1;
    char *nova_str = (char *) aloca(len); 

    if (nova_str) {
        strcpy(nova_str, str);
    }

    return nova_str;
}

#define RED "\033[1;31m"
#define GREEN "\033[1;32m"
#define BLUE "\033[1;36m"
#define PURPLE "\033[1;35m"
#define RESET "\033[0m"

typedef struct no {
    unsigned int chave;
    char *texto;
    struct no *prox;
} No;

typedef No * Lista;

Lista novo_no(unsigned int chave, char *texto){
    Lista novo = (No *) aloca(sizeof(No));
    novo->chave = chave;
    novo->texto = minha_strdup(texto);
    novo->prox = NULL;
    return novo;
}

Lista insere_final(Lista lista, unsigned int chave, char *texto){
    Lista novo = novo_no(chave, texto);
    
    if(lista == NULL){
        lista = novo;
    }else{
        Lista p = lista;
        while(p->prox != NULL){
            p = p->prox;
        }
        p->prox = novo;
    }
    return lista; 
}

Lista deletar(Lista lista, unsigned int alvo){
    if(lista == NULL){
        printf(RED "Não pode deletar, a lista está vazia!" RESET);
        return lista;
    } 
    
    if(lista->chave == alvo){
        Lista aux = lista;
        lista = lista->prox;
        libera(aux->texto);
        libera(aux);
        return lista;
    }

    Lista p = lista;
    while(p->prox != NULL){
        if(p->prox->chave == alvo){
            Lista aux = p->prox;
            if(p->prox->prox == NULL)
                p->prox = NULL;
            else
                p->prox = p->prox->prox;    
            libera(aux->texto);
            libera(aux);
            return lista;
        }
        p = p->prox;
    }
    
    return lista;
}

Lista modificar(Lista lista, unsigned int alvo, char *texto){
    if(lista == NULL){
        printf(RED "Não pode modificar, a lista está vazia!" RESET);
        return lista;
    } 

    Lista p = lista;
    while(p != NULL){
        if(p->chave == alvo){
            char *aux = p->texto;
            p->texto = minha_strdup(texto);
            libera(aux);
            return lista;
        }
        p = p->prox;
    }
    
    return lista;
}

void imprime_lista(Lista lista){
    if(lista == NULL){
        printf(BLUE "A lista está vazia!\n" RESET);
        return;
    }

    Lista p = lista;
    while(p != NULL){
        printf("%d:\n%s\n\n", p->chave, p->texto);
        p = p->prox;
    }
}

void libera_lista(Lista lista){
    if(lista != NULL){
        libera_lista(lista->prox);
        libera(lista->texto);
        libera(lista);
    }
}

int main(){
    Lista lista = NULL;

    unsigned int chave = 0, mod_chave = 0, fim = 0;
    char texto[128], escolha;

    printf(PURPLE "Olá! Estamos em um sistema de armazenamento de textos\n" RESET);

    while (!fim) {
        printf("\nEscolha:\n"
               "i - " GREEN "Inserir texto\n" RESET
               "p - " BLUE "Mostrar textos\n" RESET
               "d - " RED "Deletar texto\n" RESET
               "m - " GREEN "Modificar texto\n" RESET
               "q - " PURPLE "Finalizar programa\n" RESET
               "> ");
        scanf(" %c", &escolha);

        switch (escolha) {
            case 'i':
                printf("Qual nome você deseja inserir: ");
                getchar();
                fgets(texto, sizeof(texto), stdin);
                texto[strcspn(texto, "\n")] = '\0';
                lista = insere_final(lista, chave, texto);
                chave++;
                printf(GREEN "Texto \"%s\" inserido!\n" RESET, texto);
                break;
            case 'p':
                imprime_lista(lista);
                break;
            case 'd':
                printf("Qual chave você deseja deletar: ");
                scanf("%d", &mod_chave);
                lista = deletar(lista, mod_chave);
                printf(RED "Texto deletado!\n" RESET);
                break;
            case 'm':
                printf("Qual chave você deseja modificar: ");
                scanf("%d", &mod_chave);
                printf("Qual o novo conteúdo: ");
                getchar();
                fgets(texto, sizeof(texto), stdin);
                texto[strcspn(texto, "\n")] = '\0';
                lista = modificar(lista, mod_chave, texto);
                printf(GREEN "Texto modificado!\n" RESET);
                break;
            case 'q':
                libera_lista(lista);
                printf(PURPLE "Programa encerrado!\n" RESET);
                fim = 1;
                break;
            default:
                printf(RED "Escolha inválida\n" RESET);
                break;
        }
    }
    print_buffer();
    print_control();
    
    return 0;
}
