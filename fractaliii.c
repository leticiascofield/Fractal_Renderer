//Fractal Letícia Scofield
//Axioma: XF+YF+XF+YF+ (teta = 90º)
//Regras: X -> XF-XF+XF+XF-XF
//Y -> YF-YF+YF+YF-YF

//gcc -o fractaliii  fractaliii.c -lSDL2 -lm | ./fractalii
#include <stdio.h>
#include <string.h>
#include "SDL2/SDL.h"
#include <math.h>

int main() {
    char entrada[10000]; // XF+YF+XF+YF+
    char regraX[10000]; // XF-XF+XF+XF-XF
    char regraY[10000]; // YF-YF+YF+YF-YF
    char saida[10000];
    char axioma;
    int numFractal;
    int angulacao;
    int estagio = 4;

    printf("Número do fractal: ");
    scanf("%d", &numFractal);
    printf("Axioma: ");
    scanf("%s", entrada);
    printf("Angulo em graus: ");
    scanf("%d", &angulacao);
    printf("Regra de X: ");
    scanf("%s", regraX);
    printf("Regra de Y: ");
    scanf("%s", regraY);
    memset(saida, 0, sizeof(saida)); 
    int lenEntrada = strlen(entrada);
    int lenSaida;
    int j = 0;
    
    FILE *arquivo;
    arquivo = fopen("saidaFractaliii.txt", "w");  
    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    fprintf(arquivo, "Estágio 0: ");
    for (int e = 0; e < lenEntrada; e++) {
        fprintf(arquivo, "%c", entrada[e]);
    }
    fprintf(arquivo, "\n");

    for (int h = 0; h < estagio; h++){
        memset(saida, 0, sizeof(saida));
        for (int i = 0; i < lenEntrada; i++) {
            if (entrada[i] == 'X') {
                strcat(saida, regraX); 
            } else if (entrada[i] == 'Y') {
                strcat(saida, regraY);
            } else {
                strncat(saida, &entrada[i], 1);
            }
        }

        memset(entrada, 0, sizeof(entrada));
        strcat(entrada, saida);
        lenEntrada = strlen(entrada);
        lenSaida = strlen(saida);
        fprintf(arquivo, "Estágio %d: ", h+1);
        for (int s = 0; s < lenSaida; s++) {
            if(h != 3){
                fprintf(arquivo, "%c", saida[s]);
            } else{
                if(saida[s] == 'F' || saida[s] == '-' || saida[s] == '+'){
                    fprintf(arquivo, "%c", saida[s]);
                }
            }
        }
        fprintf(arquivo, "\n");
    }
   
    fclose(arquivo);

    /* IMAGEM FRACTAL - CÓDIGO COM SDL2*/

    SDL_Window *window = NULL;
    SDL_Renderer* renderer = NULL;

    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        fprintf(stderr, "SDL falhou ao inicializar: %s\n", SDL_GetError());
        return 1;
    }

    window = SDL_CreateWindow(
        "Fractal Letícia Scofield", 
        SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 
        800, 600, 0
    );

    if (window == NULL) { 
        fprintf(stderr, "Falha ao inicializar a janela SDL: %s\n", SDL_GetError());
        return 1;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_RenderClear(renderer);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    // ATENÇÂO: os nomes das variáveis estão relacionados às coordenadas (x,y), não aos X e Y das regras
    lenSaida = strlen(saida); 
    double inicioX = 300;
    double finalX = inicioX;
    double inicioY = 200;
    double finalY = inicioY;
    double tamanhoLinha = 200 / (pow(5, estagio - 1)) ; // para que a imagem continue do mesmo tamanho na tela
    double altura;
    double angulo = M_PI/2;
    double direcao = 0;
    

    for (int s = 0; s < lenSaida; s++) {
        if (saida[s] == 'F'){
            double deltaX = tamanhoLinha * cos(direcao);
            double deltaY = tamanhoLinha * sin(direcao);
            double finalX = inicioX + deltaX;
            double finalY = inicioY - deltaY;
            
            SDL_RenderDrawLine(renderer, inicioX, inicioY, finalX, finalY);
            
            inicioX = finalX;
            inicioY = finalY;
        } else if (saida[s] == '-') {
            direcao += angulo;
        } else if (saida[s] == '+') {
            direcao -= angulo;
        }
    }

    SDL_RenderPresent(renderer);
    SDL_Delay(5000);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    return 0;
}