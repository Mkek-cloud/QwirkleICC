typedef struct{
  char nome[50];
  char deck[6][3];
  int pontos;
} car_jogador;

/*Funções de inicialização*/
void preenche_pecas (char pecas_disponiveis[108][3]);
/*Funções do jogo*/
void mostra_tabuleiro (char ***tabuleiro, int linha, int coluna);
void trocar_uma_peca (char pecas_disponiveis[108][3], car_jogador * jogador, int tornada);
void jogar_uma_peca (char ****tabuleiro, char pecas_disponiveis[108][3], car_jogador * jogador, int tornada, int *linha, int *coluna, int *cont_jogadas, int *cont_pecas, int *ultimo_idx, int *ultimo_idy, int *guarda_posicao, char cheat);
void altera_tabuleiro (char ****tabuleiro, int *ultimo_idx, int *ultimo_idy, int *linha, int *coluna, int cont_pecas, int cont_jogadas, int *guarda_posicao);
int acrescenta_pontuacao (char ***tabuleiro ,int *guarda_posicao, int cont_jogadas, int cont_pecas, int ultimo_idx, int ultimo_idy, int linha, int coluna);
