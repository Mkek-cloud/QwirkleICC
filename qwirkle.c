#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"

int main (void){
  char pecas_disponiveis[108][3];
  preenche_pecas (pecas_disponiveis);

  int num_jogadores;
  do{
    printf ("Número de jogadores (2-4): ");
    scanf ("%d", &num_jogadores);
  }while(num_jogadores<2 || num_jogadores>4);

  /*Aloca num_jogadores structs do tipo car_jogador */
  car_jogador *jogador;
  jogador = malloc (num_jogadores * sizeof (car_jogador));
  if (jogador == NULL){
    printf ("ERRO");
    exit (1);
  }

  char cheat;
  printf("Cheat Mode(S/N)? ");
  do{
    cheat = getchar();
  }while(cheat!='S' && cheat!='N');

  char ***tabuleiro;// Tabuleiro é uma matriz que guardará strings portanto devemos adotar a terceira dimensão (no inicio devemos ter tabuleiro[0][0][3])
  tabuleiro = (char ***) malloc (sizeof (char **));
  tabuleiro[0] = (char **) malloc (sizeof (char *));
  tabuleiro[0][0] = (char *) malloc (3 * sizeof (char));
  if ((tabuleiro == NULL) || (tabuleiro[0] == NULL) || (tabuleiro[0][0] == NULL)){
    printf("ERRO");
    exit(1);
  }
  strcpy (tabuleiro[0][0], "  ");	// Dois espaços pois temos duas letras

  //Variáveis do jogo
  int linha = 0, coluna = 0, cont_pecas = 0, cont_jogadas = 0;
  // linha e coluna guardam as dimensões do tabuleiro; cont_pecas guarda quantas peças já foram jogadas; cont_jogadas conta quantas jogadas um jogador já fez em uma rodada;
  //Variáveis de auxíllio
  int ultimo_idx, ultimo_idy, guarda_posicao[12], pecas_no_baralho = 108-6*num_jogadores;
  // ultimo_idx e ultimo_idy guardam em qual posição foi a penúltima jogada do jogador; guarda_posicao é um vetor com linha e coluna dois a dois
  
  srand (time (NULL)); //Deixa os números de rand() aleatórios
  int tornada = rand () % num_jogadores;	// É um aleatório de 0 a num_jogadores-1;

  if (cheat == 'N'){
    //Gera os dados de todos os jogadores
    while (!getchar ());
    for (int i = 0; i < num_jogadores; i++){
      printf ("Nome do jogador #%d: ", i + 1);
      fgets (jogador[i].nome, 50, stdin);
      for (int j = 0; j < 6; j++){// Gera o deck do jogador
        int random;
        do{
            random = rand () % 108;	// Gera 0 a 107
        }while (strcmp (pecas_disponiveis[random], " ") == 0);
        strcpy (jogador[i].deck[j], pecas_disponiveis[random]);
        strcpy (pecas_disponiveis[random], " ");
      }
      jogador[i].pontos = 0;
    }

    //Inicializa o jogo
    while (cont_pecas != 108){
      mostra_tabuleiro (tabuleiro, linha, coluna);
      for (int i=0; i<num_jogadores; i++){
        printf ("Jogador %sPontos: %d\n", jogador[i].nome, jogador[i].pontos);
      }
      printf("Peças jogadas: %d\n", cont_pecas);
      printf ("Tornada de %s", jogador[tornada].nome);
      printf ("Deck: ");
      for (int i = 0; i < 6; i++){
        printf ("%s ", jogador[tornada].deck[i]);
      }

      if (cont_jogadas == 0){
        int flag;
        do{
          printf("\nopcoes: (T)trocar p1 [p2 p3...] | (J)jogar p1 y | (P)passar | (S)sair\n");
          char c;
          scanf(" %c", &c);
          flag = 0;
          switch (c){
            case 'T':
              if (pecas_no_baralho<0){
                printf("Não há mais peças no baralho para trocar\n");
                setbuf(stdin, NULL);
                flag = 1;
                break;
              }
              trocar_uma_peca (pecas_disponiveis, jogador, tornada);
              break;
            case 'J':
              jogar_uma_peca (&tabuleiro, pecas_disponiveis, jogador, tornada, &linha, &coluna, &cont_jogadas, &cont_pecas, &ultimo_idx, &ultimo_idy, guarda_posicao, cheat);
              break;
            case 'P':
              if (tornada == num_jogadores - 1){
                tornada = 0;
              }
              else{
                tornada++;
              }
              break;
            case 'S':
              cont_pecas=108;
              break;
            default:
              flag = 1;
          }
        }while (flag);
      }else{
        int flag;
        do{
          printf ("\nopcoes: (J)jogar p1 y | (P)passar | (S)sair\n");
          char c;
          scanf(" %c", &c);
          flag = 0;
          switch (c){
            case 'J':
              if (cont_jogadas == 6){
                printf ("Não pode jogar mais\n");
                setbuf(stdin, NULL);
                flag = 1;
                break;
              }
              jogar_uma_peca (&tabuleiro, pecas_disponiveis, jogador, tornada, &linha, &coluna, &cont_jogadas, &cont_pecas, &ultimo_idx, &ultimo_idy, guarda_posicao, cheat);
              break;
            case 'P':
              //Preenchemos o deck do jogador
              for (int i=0; (i<6 && pecas_no_baralho>0); i++){
                if (strcmp(jogador[tornada].deck[i], " ") == 0){
                  int random;
                  do{
                    random = rand()%108;
                  }while(strcmp(pecas_disponiveis[random], " ") == 0);
                  strcpy(jogador[tornada].deck[i], pecas_disponiveis[random]);
                  strcpy(pecas_disponiveis[random], " ");
                  pecas_no_baralho--;
                }
              }
              jogador[tornada].pontos += acrescenta_pontuacao (tabuleiro, guarda_posicao, cont_jogadas, cont_pecas, ultimo_idx, ultimo_idy, linha, coluna);
              //E vamos à tornada do próximo
              if (tornada == num_jogadores - 1){
                tornada = 0;
              }
              else{
                tornada++;
              }
              // Zeramos as variáveis de auxílio 
              memset(guarda_posicao, 0, sizeof(guarda_posicao));
              memset(&ultimo_idx, 0, sizeof(ultimo_idx));
              memset(&ultimo_idy, 0, sizeof(ultimo_idy));
              cont_jogadas=0;
              break;
            case 'S':
              cont_pecas = 108;
              break;
            default:
              flag = 1;
          }
        }while (flag);
      }
    }
  }else{
    /*Gera os dados de todos os jogadores */
    while (!getchar ());
    for (int i = 0; i < num_jogadores; i++){
      printf ("Nome do jogador #%d: ", i + 1);
      fgets (jogador[i].nome, 50, stdin);
      jogador[i].pontos = 0;
    }

    //Inicializa o jogo
    while (cont_pecas != 108){
      mostra_tabuleiro (tabuleiro, linha, coluna);
      for (int i=0; i<num_jogadores; i++){
        printf ("Jogador %sPontos: %d\n", jogador[i].nome, jogador[i].pontos);
      }
      printf("Peças jogadas: %d\n", cont_pecas);
      printf ("Tornada de %s", jogador[tornada].nome);
      int flag;
      do{
        printf("\nopcoes: (J)jogar p1 y | (P)passar | (S)sair\n");
        char c;
        scanf(" %c", &c);
        flag = 0;
        switch (c){
          case 'J':
            if (cont_jogadas == 6){
              printf ("Não pode jogar mais\n");
              setbuf(stdin, NULL);
              flag = 1;
              break;
            }
            jogar_uma_peca (&tabuleiro, pecas_disponiveis, jogador, tornada, &linha, &coluna, &cont_jogadas, &cont_pecas, &ultimo_idx, &ultimo_idy, guarda_posicao, cheat);
            break;
          case 'P':
            //incrementamos a pontuação
            jogador[tornada].pontos += acrescenta_pontuacao (tabuleiro, guarda_posicao, cont_jogadas, cont_pecas, ultimo_idx, ultimo_idy, linha, coluna);
            //E vamos à tornada do próximo
            if (tornada == num_jogadores - 1){
              tornada = 0;
            }else{
              tornada++;
            }
            // Zeramos as variáveis de auxílio 
            memset(guarda_posicao, 0, sizeof(guarda_posicao));
            memset(&ultimo_idx, 0, sizeof(ultimo_idx));
            memset(&ultimo_idy, 0, sizeof(ultimo_idy));
            cont_jogadas=0;
            break;
          case 'S':
            cont_pecas=108;
            break;
          default:
            flag = 1;
        }
      }while (flag);
    } 
  }

  jogador[tornada].pontos += acrescenta_pontuacao (tabuleiro , guarda_posicao, cont_jogadas, cont_pecas, ultimo_idx, ultimo_idy, linha, coluna);
  printf("Fim do jogo\n");
  for (int i=0; i<num_jogadores; i++){
    printf("%s%d pontos\n", jogador[i].nome, jogador[i].pontos);
  }
  free(jogador);
  for (int i = 0; i <= linha; i++){
    for (int j = 0; j <= coluna; j++){
      free (tabuleiro[i][j]);
    }
    free (tabuleiro[i]);
  }
  free (tabuleiro);
}


