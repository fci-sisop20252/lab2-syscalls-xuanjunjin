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
Resultado: [✓] Idênticos [ ] Diferentes

### 🔍 Análise

**1. Por que devemos verificar que bytes_escritos == bytes_lidos?**

```
É essencial garantir que o número de bytes escritos no arquivo destino seja exatamente igual ao número de bytes lidos do arquivo origem para assegurar que a cópia foi feita corretamente e integralmente. Caso bytes_escritos seja menor que bytes_lidos, parte dos dados não foi gravada, indicando erro ou interrupção na escrita, o que pode levar a arquivos corrompidos ou incompletos.
```

**2. Que flags são essenciais no open() do destino?**

```
As flags essenciais para o open() do arquivo destino são:
- O_WRONLY (para abrir o arquivo em modo escrita)
- O_CREAT (para criar o arquivo se ele não existir)
- O_TRUNC (para truncar o arquivo existente e sobrescrevê-lo)
Essas flags garantem que o arquivo destino seja aberto para escrita, criado se necessário e que não contenha dados antigos antes da cópia.
```

**3. O número de reads e writes é igual? Por quê?**

```
Sim, o número de chamadas read() e write() será igual, pois para cada leitura de um bloco de dados, há uma escrita correspondente. Cada bloco lido do arquivo origem deve ser escrito no arquivo destino, mantendo a mesma quantidade de operações. Diferenças podem ocorrer em casos especiais, como erros ou tratamento de buffers parciais.
```

**4. Como você saberia se o disco ficou cheio?**

```
Se o disco estiver cheio, a chamada write() retornará um valor menor que o número de bytes que tentou gravar, ou retornará -1 com errno setado para ENOSPC (No space left on device). Monitorando o valor de retorno das operações write() e os códigos de erro, o programa pode detectar que não há espaço suficiente no disco.
```

**5. O que acontece se esquecer de fechar os arquivos?**

```
Se os arquivos não forem fechados com close(), os buffers podem não ser liberados corretamente, o que pode levar a perda de dados ainda não escritos no disco (buffers em memória). Além disso, pode ocorrer vazamento de descritores de arquivos, esgotando o limite de arquivos abertos pelo processo, o que impede novas operações de I/O.
```

---

## 🎯 Análise Geral

### 📖 Conceitos Fundamentais

**1. Como as syscalls demonstram a transição usuário → kernel?**

```
As syscalls representam a interface onde o processo em espaço usuário solicita serviços ao kernel, como acesso a arquivos, I/O e gerenciamento de recursos. Quando uma syscall é chamada, ocorre uma interrupção ou trap que troca o contexto para o modo kernel, permitindo acesso a recursos privilegiados, garantindo segurança e controle. Após a execução da operação, o controle retorna ao espaço usuário.
```

**2. Qual é o seu entendimento sobre a importância dos file descriptors?**

```
File descriptors são inteiros que representam arquivos abertos pelo processo, atuando como "pointers" abstratos para recursos de I/O gerenciados pelo kernel. Eles permitem manipular arquivos, pipes, sockets, etc., de forma uniforme, facilitando a leitura, escrita, fechamento e controle de arquivos. São essenciais para a organização e controle do acesso aos recursos do sistema.
```

**3. Discorra sobre a relação entre o tamanho do buffer e performance:**

```
O tamanho do buffer impacta diretamente a performance da cópia. Buffers muito pequenos aumentam o número de syscalls (read/write), gerando overhead de troca de contexto e lentidão. Buffers muito grandes podem consumir muita memória e causar gargalos em sistemas com pouca RAM. Um buffer balanceado melhora o throughput, reduzindo syscalls sem sobrecarregar recursos.

```

### ⚡ Comparação de Performance

```bash
# Teste seu programa vs cp do sistema
time ./ex4_copia
time cp dados/origem.txt dados/destino_cp.txt
```

**Qual foi mais rápido?** __cp___

**Por que você acha que foi mais rápido?**

```
O comando `cp` é uma ferramenta altamente otimizada, escrita para aproveitar melhor as chamadas de sistema, utilizar buffers ajustados dinamicamente e otimizações específicas do sistema de arquivos. Além disso, `cp` pode usar métodos avançados como mmap ou operações atômicas específicas para acelerar a cópia, enquanto o programa simples depende de uma implementação básica e buffer fixo.
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
