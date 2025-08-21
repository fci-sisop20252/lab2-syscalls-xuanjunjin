/*
 * Exercício 2 - Leitura Básica de Arquivo
 * 
 * OBJETIVO: Implementar leitura de arquivo usando syscalls
 * 
 * TAREFA: Complete os TODOs marcados abaixo
 * 1. Compile: gcc src/ex2_leitura.c -o ex2_leitura
 * 2. Execute: ./ex2_leitura
 * 3. Observe: strace -e openat,read,close ./ex2_leitura
 */

#include <fcntl.h>    // Para open() e flags O_RDONLY
#include <unistd.h>   // Para read() e close()
#include <stdio.h>    // Para printf() e perror()
#include <errno.h>    // Para códigos de erro

#define BUFFER_SIZE 10

int main() {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_lidos;
    int fd;
    
    printf("=== Exercício 2: Leitura de Arquivo ===\n\n");
    
    /*
     * TODO 1: Abrir o arquivo 'dados/teste1.txt' para leitura
     * Use open() com O_RDONLY
     */
    fd = open("dados/teste3.txt", O_RDONLY)/* COMPLETE AQUI */;
    
    /*
     * TODO 2: Verificar se a abertura foi bem-sucedida
     * Se fd < 0, houve erro
     */
    if (fd < 0/* COMPLETE AQUI */) {
        perror("Erro ao abrir arquivo");
        return 1;
    }
    
    printf("Arquivo aberto! File descriptor: %d\n", fd);
    
    /*
     * TODO 3: Ler dados do arquivo
     * Use read() para ler até (BUFFER_SIZE - 1) bytes
     */
    bytes_lidos = read(fd, buffer, BUFFER_SIZE - 1)/* COMPLETE AQUI */;
    
    /*
     * TODO 4: Verificar se a leitura foi bem-sucedida
     */
    if (bytes_lidos < 0/* COMPLETE AQUI */) {
        perror("Erro na leitura");
        close(fd);
        return 1;
    }
    
    /*
     * TODO 5: Adicionar terminador nulo
     * Para tratar o buffer como string
     */
    /* COMPLETE AQUI */
    buffer[bytes_lidos]='\0';
    
    // Exibir resultados
    printf("Bytes lidos: %ld\n", bytes_lidos);
    printf("Conteúdo:\n%s\n", buffer);
    
    /*
     * TODO 6: Fechar o arquivo
     * Use close() para liberar o file descriptor
     */
    if (close(fd)/* COMPLETE AQUI */) {
        perror("Erro ao fechar arquivo");
        return 1;
    }
    
    printf("Arquivo fechado!\n\n");
    printf("Execute: strace -e openat,read,close ./ex2_leitura\n");
    
    return 0;
}