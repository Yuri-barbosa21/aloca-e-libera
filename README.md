Este projeto implementa funções personalizadas de alocação e liberação de memória, semelhantes às funções `malloc` e `free` da biblioteca padrão C `(stdlib.h)`. Além disso, utiliza uma lista ligada para demonstrar o funcionamento dessas funções.

Existem diversas abordagens para fazer isso. Eu escolhi um método que gastasse pouca memória para implementação e que, ao mesmo tempo, não tivesse alocações tão custosas.

## Processo e Método

O processo para criar as funções personalizadas de alocação e liberação de memória envolveu a criação de um buffer de memória e um controle de memória. O buffer de memória é um array do tipo `unsigned char` de tamanho fixo que armazena os dados, enquanto o controle de memória é um array que mantém o controle de quais partes do buffer de memória estão sendo usadas.

Cada `byte` do meu controle faz o gerenciamento de `8 bytes` do meu buffer. Isso funciona da seguinte forma: a iteração pelo controle é feita bit a bit. Como tenho 8 bits em 1 byte, consigo gerenciar 8 bytes do meu buffer. Um bit com valor 1 indica que tem memória alocada e valor 0 indica que está livre. Por isso, sempre o tamanho do meu controle será o tamanho do meu buffer dividido por 8.

A função `aloca` percorre o controle de memória bit a bit, procurando por um espaço livre que seja grande o suficiente para alocar a quantidade de memória solicitada. Se encontrar um espaço adequado, a função aloca a memória e atualiza o controle de memória para refletir a alocação.

A função `libera` também percorre o controle de memória bit a bit, procurando pelo bloco de memória que corresponde ao ponteiro fornecido. Quando encontra o bloco de memória correspondente, a função libera a memória e atualiza o controle de memória para refletir a liberação.

## Código

O código é composto por várias funções, descritas abaixo:

- `unsigned char last_byte(unsigned int num)`: Esta função extrai o último byte de um número.
- `void *aloca(size_t size)`: Esta função aloca um bloco de memória do tamanho especificado. Se o tamanho for maior que 255, a função retorna NULL.
- `void libera(void *ptr)`: Esta função libera um bloco de memória previamente alocado.
- `void print_buffer()`: Esta função imprime o buffer.
- `void print_control()`: Esta função imprime o controle.
- `char *minha_strdup(const char *str)`: Esta função duplica uma string.
- `Lista novo_no(unsigned int chave, char *texto)`: Esta função cria um novo nó para a lista ligada.
- `Lista insere_final(Lista lista, unsigned int chave, char *texto)`: Esta função insere um novo nó no final da lista ligada.
- `Lista deletar(Lista lista, unsigned int alvo)`: Esta função deleta um nó da lista ligada.
- `Lista modificar(Lista lista, unsigned int alvo, char *texto)`: Esta função modifica um nó da lista ligada.
- `void imprime_lista(Lista lista)`: Esta função imprime a lista ligada.
- `void libera_lista(Lista lista)`: Esta função libera a lista ligada.

O código também inclui uma função `main`, que fornece um menu interativo para o usuário inserir, mostrar, deletar e modificar textos na lista ligada.

## Como Compilar e Executar

Para compilar o código, você pode usar o seguinte comando:

```bash
gcc nome_do_arquivo.c -o nome_do_programa
```

Para executar o programa, você pode usar o seguinte comando:

```bash
./nome_do_programa
```

Substitua `nome_do_arquivo.c` pelo nome do arquivo C que contém o código e `nome_do_programa` pelo nome que você deseja dar ao programa compilado.
