/*********************************************************************/
/**   ACH2002 - Introducao a Analise de Algoritmos                  **/
/**   EACH-USP - Segundo Semestre de 2023                           **/
/**   Turma 04 - Prof. Luciano Digiampietri                         **/
/**                                                                 **/
/**   Segundo Exercicio-Programa                                    **/
/**                                                                 **/
/**   <Matheus Ferreira Azevedo>                <14760276>          **/
/**                                                                 **/
/*********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0

#define MAX_PAISES 500

typedef int bool;


typedef struct aux {
    int id;
    int cor;
    int numVizinhos;
    struct aux** vizinhos;
} PAIS;


typedef struct {
    int numPaises;
    PAIS* paises;
} MAPA;


/* Funcao desenvolvida para criar mapas considerando o numero de paises e o 
   numero medio de vizinhos de cada pais.                                 
*/
MAPA criarMapa(int numPaises, int mediaVizinhos){
    printf("################# Criando mapa com %i pais(es) e com media de vizinhos igual a %i. #################\n",numPaises, mediaVizinhos);
    int x, y, v, total;
    MAPA map;
    if (numPaises < 1 || mediaVizinhos >= numPaises) {
        printf("Parametros invalidos, mapa nao sera gerado.\n");
        map.numPaises = 0;
        return map;    
    }
    map.numPaises = numPaises;
    map.paises = (PAIS*) malloc(sizeof(PAIS)*numPaises);
    
    int vizinhanca[numPaises][numPaises];
    for (x=0;x<numPaises;x++)
        for (y=0;y<numPaises;y++)
            vizinhanca[x][y] = 0;
            
    total = numPaises*mediaVizinhos/2;
    for (v=0;v<total;v++){
        x = rand()%numPaises;
        y = rand()%numPaises;
        if (x!=y && vizinhanca[x][y] == 0){
            vizinhanca[x][y] = 1;
            vizinhanca[y][x] = 1;
        }else v--; // precisara escolher outro
    }
    
    for (x=0;x<numPaises;x++){
        map.paises[x].id = x;  
        map.paises[x].cor = -1;
        v = 0;
        for (y=0;y<numPaises;y++) if (vizinhanca[x][y]) v++;
        map.paises[x].vizinhos = (PAIS**) malloc(sizeof(PAIS*)*v);
        map.paises[x].numVizinhos = v;
    }
        
    for (x=0;x<numPaises;x++){
        v = 0;
        for (y=0;y<numPaises;y++) 
            if (vizinhanca[x][y]){
                map.paises[x].vizinhos[v] = &(map.paises[y]);
                v++;
            }
    }
    return map;
}

/* Funcao desenvolvida para imprimir um mapa.
   Para cada pais, a funcao imprime seu id, sua cor e o conjunto de paises
   vizinhos com suas respectivas cores.      
*/
void imprimirMapa(MAPA map){
    int x, y;
    printf("Imprimindo mapa\n");
    for (x=0;x<map.numPaises;x++){
        printf("Pais %3i[%2i], Vizinhos:", map.paises[x].id, map.paises[x].cor);
        for (y=0;y<map.paises[x].numVizinhos;y++)
            printf(" %3i[%2i]", map.paises[x].vizinhos[y]->id, map.paises[x].vizinhos[y]->cor);
        printf("\n");
    }
}


/* Funcao que coloca a cor 'invalida' -1 em todos os paises do mapa. */
void zerarCores(MAPA map){
    int x;
    for (x=0;x<map.numPaises;x++) map.paises[x].cor = -1;
    
}


/* Funcao que recebe um mapa como parametro, colore este mapa e retorna o 
   numero de cores utilizadas para colorir, de acordo com o criterio guloso 
   apresentado a seguir.
   Para cada pais do arranjo de paises (comecando do primeiro, isto e, daquele
   da posicao 0 (zero) do arranjo) colocar a primeira cor valida/viavel 
   (comecando da cor zero em diante [um, dois ...]). Uma cor e considerada 
   valida para o presente pais, caso nenhum de seus vizinhos ja esteja colorido
   com essa cor.
*/
int resolveGuloso(MAPA map) {
    int coresUsadas = 0;

    for (int i = 0; i < map.numPaises; i++) {
        // Inicializa a cor do país como 0
        map.paises[i].cor = 0;

        // Verifica se a cor atual já é usada por algum vizinho
        for (int j = 0; j < map.paises[i].numVizinhos; j++) {
            PAIS* vizinho = map.paises[i].vizinhos[j];

            // Verifica se a cor atual já é usada pelo vizinho
            while (map.paises[i].cor == vizinho->cor) {
                map.paises[i].cor++;

                // Reinicia a verificação com o primeiro vizinho
                j = 0;
                vizinho = map.paises[i].vizinhos[j];
            }
        }

        // Atualiza o número total de cores usadas, se necessário
        if (map.paises[i].cor >= coresUsadas) {
            coresUsadas = map.paises[i].cor + 1;
        }
    }

    return coresUsadas;
}




/* Funcao que recebe um mapa como parametro e um numero de cores (maxCor) e
   tenta colorir o mapa usando o numero de cores passado como parametro.
   Se for possivel colorir o mapa, a funcao devera atribuir cores aos paises
   e retornar true, caso contrario, devera retornar false. Esta funcao deve 
   seguir estritamente a estrategia de Tentativa e Erro apresentada a seguir.
   ESTRATEGIA - Tentativa e Erro: a partir do pais atual (comecando do 
   primeiro do arranjo) ha maxCor acoes potencialmente possiveis (que devem
   ser seguidas nesta ordem): colocar a cor 0 (zero) no pais atual, colocar a
   cor 1 no pais atual, colocar a cor 2 no pais atual, ... colocar a cor 
   maxCor-1 no pais atual. Uma acao e considera viavel caso nenhum dos paises 
   vizinhos do pais atual ja esteja colorido com a cor da acao atual. Caso seja
   possivel tomar a acao, o pais atual devera receber a cor atual e o algoritmo
   devera tentar colorir o proximo pais. Se todos os paises estiverem coloridos,
   a funcao devera retornar true. Se nao for possivel colorir o pais com a cor 
   atual, o algoritmo devera tentar colorir com a proxima cor. Se nao for
   possivel colorir o pais atual com nenhuma cor, o algoritmo devera marcar o
   pais atual com a ``cor'' -1 e retornar (backtracking) para tentar outra 
   coloracao (tentar, por exemplo, colorir o pais anterior com outra cor.
*/
bool corUsada(PAIS* pais, int cor) { // verifica se a cor pode ser utilizada, ou já possui algum vizinho utilizando.
    for (int i = 0; i < pais->numVizinhos; i++) {
        if (pais->vizinhos[i]->cor == cor) {
            return false; // se já houver algum vizinho com a cor ele retorna false.
        }
    }
    return true; // caso não tiver ele retorna true, possibilitando colorir o pais.
}

bool resolveTentativaEErroRecursivo(MAPA map, int maxCor, int paisAtual) {
    // Condição de parada é se já houver passado por todos paises, logo o paisAtual é igual ao numPaises.
    if (paisAtual == map.numPaises) {
        return true;
    }

    // Tenta colorir o país atual com cada cor de 0 a maxCor
    for (int cor = 0; cor < maxCor; cor++) {
        if (corUsada(&(map.paises[paisAtual]), cor)) {
            // Se a cor é válida, atribui ao país e tenta colorir o próximo país
            map.paises[paisAtual].cor = cor;
            if (resolveTentativaEErroRecursivo(map, maxCor, paisAtual + 1)) {
                return true;  // A cor foi concluida com sucesso
            }
            // Se a tentativa não for concluida, fazer o backtracking
            map.paises[paisAtual].cor = -1;
        }
    }

    // Nenhuma cor é válida para o país atual
    return false;
}

bool resolveTentativaEErro(MAPA map, int maxCor) {
    zerarCores(map);  // Inicializa todas as cores como -1 como padrão
    return resolveTentativaEErroRecursivo(map, maxCor, 0); 
}



/* Funcao main que cria alguns mapas e invoca as funcoes gulosa e de tentativa
   e erro para tentar colorir cada um dos mapas.
   Esta funcao nao testa todos os detalhes envolvidos no problema de coloracao
   de mapas.
*/
int main() {
    // Inicializando gerador de numeros pseudo aleatorios
    srand(42);
    int cores;
    int coresGuloso;
    MAPA map1;
      
    
    map1 = criarMapa(1, 0);
    imprimirMapa(map1);
    coresGuloso = resolveGuloso(map1);
    printf("\n\nO algoritmo guloso usou %i cor(es)\n",coresGuloso);
    imprimirMapa(map1);
    printf("\n");
    
    for (cores=1; cores<=coresGuloso; cores++){
        zerarCores(map1);
        printf("\n");
        if (resolveTentativaEErro(map1,cores)) {
            printf("Foi possivel resolver o problema com %i cor(es).\n", cores);
            imprimirMapa(map1);
            break;
        }
        else printf("Nao foi possivel resolver o problema com %i cor(es).\n", cores);
    }
    
    printf("\n\n");
    
    
        
    map1 = criarMapa(2, 1);
    imprimirMapa(map1);
    coresGuloso = resolveGuloso(map1);
    printf("\n\nO algoritmo guloso usou %i cor(es)\n",coresGuloso);
    imprimirMapa(map1);
    printf("\n");
    
    for (cores=1; cores<=coresGuloso; cores++){
        zerarCores(map1);
        printf("\n");
        if (resolveTentativaEErro(map1,cores)) {
            printf("Foi possivel resolver o problema com %i cor(es).\n", cores);
            imprimirMapa(map1);
            break;
        }
        else printf("Nao foi possivel resolver o problema com %i cor(es).\n", cores);
    }
    
    printf("\n\n");
    
    
    map1 = criarMapa(4, 1);
    imprimirMapa(map1);
    coresGuloso = resolveGuloso(map1);
    printf("\n\nO algoritmo guloso usou %i cores\n",coresGuloso);
    imprimirMapa(map1);
    printf("\n");
    
    for (cores=1; cores<=coresGuloso; cores++){
        zerarCores(map1);
        printf("\n");
        if (resolveTentativaEErro(map1,cores)) {
            printf("Foi possivel resolver o problema com %i cor(es).\n", cores);
            imprimirMapa(map1);
            break;
        }
        else printf("Nao foi possivel resolver o problema com %i cor(es).\n", cores);
    }
    
    printf("\n\n");
    
    
    
    map1 = criarMapa(20, 9);
    imprimirMapa(map1);
    coresGuloso = resolveGuloso(map1);
    printf("\n\nO algoritmo guloso usou %i cor(es)\n",coresGuloso);
    imprimirMapa(map1);
    printf("\n");
    
    for (cores=1; cores<=coresGuloso; cores++){
        zerarCores(map1);
        printf("\n");
        if (resolveTentativaEErro(map1,cores)) {
            printf("Foi possivel resolver o problema com %i cor(es).\n", cores);
            imprimirMapa(map1);
            break;
        }
        else printf("Nao foi possivel resolver o problema com %i cor(es).\n", cores);
    }
    return 0;    
}
