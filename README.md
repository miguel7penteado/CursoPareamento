# Curso de Pareamento de DataSets com PNADC

## Determinístico x Probabilístico

(Gerando BookDown)

# AULA 1

Limpeza e Padronização
Blocagem
Estimativa de Parâmetros
Comparação e Classificação
Conferência


Pareamento de Registros

Pareamento determinístico:


Pareamento probabilístico - Parâmetros:

u, m e missing

u       - falso positivo
m       - verdadeiro positivo
missing - faltante

algorítmos de comparação string
jaro-winkler
levenshtein

expectation maximization

# AULA 2
A blocagem 
1o deterministico => nome sexo data_nasc 
2o probabilística

Peso M = log2 M/U

Peso U = log2 (1-M)/(1-U)

Pesso Missing = (pesoM + PesoU)/2

Peso M = log2 (1)/freq_relativa

## 2.1 - Comparação

"Chave primária ?" => sonho meu...

### 2.2.1 - Deduplicação:
* Selecine os registros com que vai trabalhar
* Selecione registros semelhantes na mesma base
* Atributos de registros (elementos) semelhantes => Notas

## 2.2 - Pós-Processamento
Resultado pareamento em pares => Clusterização => Geração de Identificadores Únicos

## 2.3 - Avaliação de um Pareamento
* Falso Positivo => neste processo, pareou mas errou
* Falso Negativo => neste processo, deixou de parear

