#include <stdio.h>
#include <math.h>

#define BUFFERSIZE    32
#define CONTROLSIZE    BUFFERSIZE/8

unsigned char BUFFER[BUFFERSIZE];
unsigned char CONTROL[CONTROLSIZE] = {0};

size_t otimizer_binary(unsigned int num){
    size_t bytes = ceil(log2(num + 1) / 8);
    return bytes;
}

unsigned char last_byte(unsigned int num){
    return num & 0xFF; // Extrai o ultimo byte de um numero
}

void binary(unsigned int num){
    unsigned int num_bits = otimizer_binary(num) * 8;

    for(int i = num_bits - 1; i >= 0; i--){
        int bit = (num >> i) & 1;
        printf("bit: %d\n", bit);
        printf("indice: %d\n", i);
    }
    printf("\n");
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
                if(ptr == &BUFFER[bit_count] + 1){
                    int bit_offset = (bit_count % 8);
                    CONTROL[i] &= ~(1 << bit_offset);
                    BUFFER[bit_count] = (char)0;
                    return;
                }
                bit_count += (BUFFER[bit_count] + 1);
                i = (int)(bit_count/8);
                j = (bit_count % 8) - 1;
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
