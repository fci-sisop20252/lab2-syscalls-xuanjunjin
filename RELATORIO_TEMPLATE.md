# 📝 Relatório do Laboratório 2 - Chamadas de Sistema

---

## 1️⃣ Exercício 1a - Observação printf() vs 1b - write()

### 💻 Comandos executados:
```bash
strace -e write ./ex1a_printf
strace -e write ./ex1b_write
```

### 🔍 Análise

**1. Quantas syscalls write() cada programa gerou?**
- ex1a_printf: ___8__ syscalls
- ex1b_write: ___7__ syscalls

**2. Por que há diferença entre os dois métodos? Consulte o docs/printf_vs_write.md**

```
A diferença entre printf() e write() ocorre porque printf() é uma função de alto nível que usa buffering, enquanto write() é uma syscall direta que envia dados imediatamente ao kernel.
```

**3. Qual método é mais previsível? Por quê você acha isso?**

```
write() é mais previsível porque sempre executa uma syscall por chamada, sem depender de buffering ou contexto de execução, ao contrário do printf(), que pode adiar a escrita real dependendo de seu buffer.
```

---

## 2️⃣ Exercício 2 - Leitura de Arquivo

### 📊 Resultados da execução:
- File descriptor: __3___
- Bytes lidos: __127___

### 🔧 Comando strace:
```bash
strace -e openat,read,close ./ex2_leitura
```

### 🔍 Análise

**1. Qual file descriptor foi usado? Por que não começou em 0, 1 ou 2?**

```
O file descriptor usado foi o 3, porque os descritores 0, 1 e 2 já estão reservados pelo sistema para entrada padrão (stdin), saída padrão (stdout) e erro padrão (stderr). Ao abrir um novo arquivo, o próximo número disponível é o 3.
```

**2. Como você sabe que o arquivo foi lido completamente?**

```
Sabemos que o arquivo foi lido completamente porque a função read() retornou menos bytes do que o tamanho do buffer (BUFFER_SIZE - 1). Isso indica que o fim do arquivo (EOF) foi alcançado. Além disso, se uma nova chamada a read() fosse feita, ela retornaria 0, o que é a confirmação oficial de EOF.
```

**3. Por que verificar retorno de cada syscall?**

```
Verificar o retorno de cada syscall é essencial para detectar falhas como: erro ao abrir um arquivo inexistente, erro de leitura (por exemplo, permissões insuficientes) ou falha ao fechar o arquivo. Ignorar esses retornos pode causar comportamentos inesperados e dificultar a identificação de bugs.
```

---

## 3️⃣ Exercício 3 - Contador com Loop

### 📋 Resultados (BUFFER_SIZE = 64):
- Linhas: __25___ (esperado: 25)
- Caracteres: ____1300_
- Chamadas read(): __21___
- Tempo: ___0.000459__ segundos

### 🧪 Experimentos com buffer:

| Buffer Size | Chamadas read() | Tempo (s) |
|-------------|-----------------|-----------|
| 16          |        82         |       0.000174    |
| 64          |         21        |        0.000459   |
| 256         |         6        |        0.000065   |
| 1024        |         2        |         0.000060  |

### 🔍 Análise

**1. Como o tamanho do buffer afeta o número de syscalls?**

```
O tamanho do buffer influencia diretamente a quantidade de chamadas read() necessárias para ler o arquivo completo. Buffers menores exigem mais chamadas read(), pois cada chamada lê apenas uma pequena parte do arquivo. Já buffers maiores conseguem ler mais conteúdo por vez, reduzindo o número total de chamadas.
```

**2. Todas as chamadas read() retornaram BUFFER_SIZE bytes? Discorra brevemente sobre**

```
Não. Apenas as primeiras chamadas retornam exatamente BUFFER_SIZE bytes. A última chamada geralmente retorna menos bytes, pois está lendo o final do arquivo, que pode ter menos dados restantes do que o tamanho do buffer. Isso é esperado e indica o fim da leitura. Também pode haver variação se o arquivo tiver tamanho que não é múltiplo exato do buffer.
```

**3. Qual é a relação entre syscalls e performance?**

```
Cada syscall (como read()) tem um custo computacional porque envolve a transição do modo usuário para o modo kernel, o que é relativamente lento. Portanto, quanto menos syscalls forem feitas, melhor a performance geral. Usar buffers maiores reduz a quantidade de chamadas necessárias e melhora a eficiência do programa.
```

---

## 4️⃣ Exercício 4 - Cópia de Arquivo

### 📈 Resultados:
- Bytes copiados: ___1364__
- Operações: ___6__
- Tempo: __0.000177___ segundos
- Throughput: __7525.6___ KB/s

### ✅ Verificação:
```bash
diff dados/origem.txt dados/destino.txt
```
Resultado: [x] Idênticos [ ] Diferentes

### 🔍 Análise

**1. Por que devemos verificar que bytes_escritos == bytes_lidos?**

```
[Sua análise aqui]
```

**2. Que flags são essenciais no open() do destino?**

```
[Sua análise aqui]
```

**3. O número de reads e writes é igual? Por quê?**

```
[Sua análise aqui]
```

**4. Como você saberia se o disco ficou cheio?**

```
[Sua análise aqui]
```

**5. O que acontece se esquecer de fechar os arquivos?**

```
[Sua análise aqui]
```

---

## 🎯 Análise Geral

### 📖 Conceitos Fundamentais

**1. Como as syscalls demonstram a transição usuário → kernel?**

```
[Sua análise aqui]
```

**2. Qual é o seu entendimento sobre a importância dos file descriptors?**

```
[Sua análise aqui]
```

**3. Discorra sobre a relação entre o tamanho do buffer e performance:**

```
[Sua análise aqui]
```

### ⚡ Comparação de Performance

```bash
# Teste seu programa vs cp do sistema
time ./ex4_copia
time cp dados/origem.txt dados/destino_cp.txt
```

**Qual foi mais rápido?** _____

**Por que você acha que foi mais rápido?**

```
[Sua análise aqui]
```

---

## 📤 Entrega
Certifique-se de ter:
- [ ] Todos os códigos com TODOs completados
- [ ] Traces salvos em `traces/`
- [ ] Este relatório preenchido como `RELATORIO.md`

```bash
strace -e write -o traces/ex1a_trace.txt ./ex1a_printf
strace -e write -o traces/ex1b_trace.txt ./ex1b_write
strace -o traces/ex2_trace.txt ./ex2_leitura
strace -c -o traces/ex3_stats.txt ./ex3_contador
strace -o traces/ex4_trace.txt ./ex4_copia
```
# Bom trabalho!
