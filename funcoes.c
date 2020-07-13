#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "funcoes.h"

/*Funções de inicialização*/

/*
Essa função preenche uma matriz com todas as peças disponíveis
*/
void preenche_pecas (char pecas_disponiveis[108][3]){
  char letra = 'A';
  int cont = 1;
  for (int i = 0; i < 108; i++){
    pecas_disponiveis[i][0] = letra;
    pecas_disponiveis[i][1] = cont + '0';
    pecas_disponiveis[i][2] = '\0';
    pecas_disponiveis[++i][0] = letra;
    pecas_disponiveis[i][1] = cont + '0';
    pecas_disponiveis[i][2] = '\0';
    pecas_disponiveis[++i][0] = letra;
    pecas_disponiveis[i][1] = cont + '0';
    pecas_disponiveis[i][2] = '\0';
    if (cont == 6){
	  cont = 0;
	  letra = letra + 1;
    }
    cont++;
  }
}

/*Funções do jogo*/

/*
Essa função recebe o tabuleiro e o mostra na tela.
*/
void mostra_tabuleiro (char ***tabuleiro, int linha, int coluna){
  printf ("    ");
  for (int j = 0; j <= coluna; j++){
    if (j>9){
      printf(" %d  ", j);
    }else{
      printf ("  %d  ", j);
    }
  }
  printf ("\n");
  for (int i = 0; i <= linha; i++){
    printf ("     ");
    for (int j = 0; j <= coluna; j++){
	    printf ("-----");
	  }
    printf ("\n");
    if (i>9){
      printf ("%d  ", i);
    }else{
      printf ("%d   ", i);
    } 
    for (int j = 0; j <= coluna; j++){
	    printf ("| %s ", tabuleiro[i][j]);
	  }
    printf ("|\n");
  }
  printf ("     ");
  for (int j = 0; j <= coluna; j++){
    printf ("-----");
  }
  printf ("\n");
}

/*
Essa função pega uma peça verifica se existe no deck do jogador e troca essa peça por uma outra aleatória disponível nas pecas_disponiveis. Ela também devolve essa peça em pecas_disponiveis.
*/

void trocar_uma_peca (char pecas_disponiveis[108][3], car_jogador *jogador, int tornada){
  int i;
  char peca[3];
  while (!getchar());
  fgets (peca, 3, stdin);
  for (i = 0; i < 6; i++){
    if (strcmp (peca, jogador[tornada].deck[i]) == 0){// Se tem uma peça igual a que o usuário digitou
	    break;
	  }
  }
  if (i == 6){// Quer dizer que não há nenhuma peça igual à digitada
    printf ("Digite uma peça válida!!!\n");
    return;
  }
  // A partir daqui há a peça e queremos trocar
  int random;
  do{
    random = rand () % 108;	// Gera 0 a 107
  }while (strcmp (pecas_disponiveis[random], " ") == 0);
  char aux[3];
  strcpy (aux, pecas_disponiveis[random]);
  strcpy (pecas_disponiveis[random], jogador[tornada].deck[i]);
  strcpy (jogador[tornada].deck[i], aux);
}

/*
Essa função pega uma peça e uma posição verifica se são válidos de acordo com a existência da peça e da posição e depois verifica se são válidos para as regras do jogo.
Para a regra do jogo temos que :
  .A partir da primeira jogada do jogador:
   -Para jogar uma nova peça essa peça tem q ser da (mesma cor da antiga e formato diferente) ou (cor diferente e formato igual)
  
  .A partir da segunda jogada do jogador:
    -O jogador formou uma linha(horizontal ou vertical), próximas jogadas devem estar nessa linha 

  .Para cada jogada devemos verificar uma linha (horizontal e vertical)
    -Horizontal/Vertical: Se na linha (até um espaço) tem algum elemento idêntico ao colocado não podemos permitir
    Se na linha tiver um elemento com cor diferente e formato igual não podemos permitir
    Se na linha tiver um elemento com cor igual e formato diferente não podemos permitir


  //As verificações a seguir só valem para quando não for o primeiro round do jogo
  .O usuário jogou em algum lugar que está coberto por espaços:
   -O jogador jogou nas bordas 
   -Ao redor do local jogado temos somente espaços
   (devemos tomar cuidado com erros de segmentação nas margens)
  Obs: A explicação do porque de ****tabuleiro está no site https://stackoverflow.com/questions/13431108/changing-address-contained-by-pointer-using-function
  Note que (*tabuleiro)[i][j] pois devemos considerar a precedência
*/
void jogar_uma_peca (char ****tabuleiro, char pecas_disponiveis[108][3], car_jogador * jogador, int tornada, int *linha, int *coluna, int *cont_jogadas, int *cont_pecas, int *ultimo_idx, int *ultimo_idy, int *guarda_posicao, char cheat){
  int i=0, j=0, idx, idy;
  char peca[3];
  while(!getchar());
  fgets (peca, 3, stdin);
  scanf(" %d %d", &idx, &idy);

  /*Verifica se a peça existe no deck e se a posição é válida */
  if (cheat == 'N'){
    for (i = 0; i < 6; i++){
      if (strcmp (peca, jogador[tornada].deck[i]) == 0){// Se tem uma peça igual a que o usuário digitou no deck
	      break;
	    }
    }
  }

  if ((i == 6)||(j == 108)){// Quer dizer que não há nenhuma peça igual à digitada 
    printf ("Digite uma peça válida!!! Regra 0\n");
    return;
  }else if (idx < 0 || idy < 0 || idx > *linha || idy > *coluna || strcmp ((*tabuleiro)[idx][idy], "  ") != 0){// Quer dizer que a posição é inválida
    printf ("Digite uma posição válida!!! Regra 1\n");
    return;
  }

  /* A partir daqui a posição e a peça existe devemos agora verificar a possibilidade de acordo com as regras do jogo */

  if (*cont_jogadas >= 1){
    if ( !( 
    ( (peca[1] == (*tabuleiro)[*ultimo_idx][*ultimo_idy][1]) &&  
    (peca[0] != (*tabuleiro)[*ultimo_idx][*ultimo_idy][0]) ) ||
    ( (peca[1] != (*tabuleiro)[*ultimo_idx][*ultimo_idy][1]) &&
    (peca[0] == (*tabuleiro)[*ultimo_idx][*ultimo_idy][0]) ) ) )
    {// Se tem (mesma cor da antiga e formato diferente) ou (cor diferente e formato igual) então ele está correto se não faça o seguinte
      printf ("Digite uma posição válida!!! Regra 2\n");
      return;
    }
    if ( (idx != guarda_posicao[0]) && (idy !=guarda_posicao[1]) ){// A peça deve ter pelo menos a linha ou a coluna em comum com a ultima primeira peça jogada
      printf("Digite uma posição válida!!! Regra 3\n");
      return;
    }
    if (*cont_jogadas>=2){//Se ele formou uma linha
      if (guarda_posicao[0] == guarda_posicao[2]){//Linha na horizontal
        if (idx != guarda_posicao[0]){
          printf("Digite uma posição válida!!! Regra 4\n");
          return;
        }
      }else if (guarda_posicao[1] == guarda_posicao[3]){
        if (idy != guarda_posicao[1]){
          printf("digite uma posição válida!!! Regra 5\n");
          return;
        }
      }
    }
  }
  /*Para cada jogada devemos verificar uma linha (até um espaço) vertical e horizontal
  //Se na linha (até um espaço) tem algum elemento idêntico ao colocado não podemos permitir
  Se na linha tiver um elemento com cor diferente e formato diferente não podemos permitir*/
  for (i = idy+1; i <= *coluna; i++){// Andando para direita
	  if (strcmp ((*tabuleiro)[idx][i], "  ") == 0){
	    break;
	  }
	  if ( (strcmp((*tabuleiro)[idx][i], peca) == 0) || 
      (((*tabuleiro)[idx][i][1] != peca[1]) && ((*tabuleiro)[idx][i][0] != peca[0])) )
    {
      printf ("Digite uma posição válida!!! Regra 6\n");
      return;
	  }
	}
  for (i = idy-1; i >= 0; i--){// Andando para esquerda
    if (strcmp ((*tabuleiro)[idx][i], "  ") == 0){
      break;
    }
    if ( (strcmp((*tabuleiro)[idx][i], peca) == 0) || 
      (((*tabuleiro)[idx][i][1] != peca[1]) && ((*tabuleiro)[idx][i][0] != peca[0])) )
    {
      printf ("Digite uma posição válida!!! Regra 7\n");
      return;
	  }
	}
	
  /*Verticalmente */
  for (i = idx+1; i <= *linha; i++){// Andando para baixo
    if (strcmp ((*tabuleiro)[i][idy], "  ") == 0){
      break;
    }
    if ( (strcmp((*tabuleiro)[i][idy], peca) == 0) || 
      (((*tabuleiro)[i][idy][1] != peca[1]) && ((*tabuleiro)[i][idy][0] != peca[0])) )
    {
      printf ("Digite uma posição válida!!! Regra 8\n");
      return;
    }
	}
  for (i = idx-1; i >= 0; i--){// Andando para cima
    if (strcmp ((*tabuleiro)[i][idy], "  ") == 0){
      break;
    }
    if ( (strcmp((*tabuleiro)[i][idy], peca) == 0) || 
      (((*tabuleiro)[i][idy][1] != peca[1]) && ((*tabuleiro)[i][idy][0] != peca[0])) )
    {
      printf ("Digite uma posição válida!!! Regra 9\n");
      return;
    }
  }//Agora resta checar as bordas

  if (*cont_pecas != 0){// Não é a primeira jogada do jogo
    if ( 
    ((idx == 0) && (idy == 0))      || 
    ((idx == 0) && (idy == *coluna)) || 
    ((idx == *linha) && (idy == 0))  || 
    ((idx == *linha) && (idy == *coluna)) )
    {// As bordas são inválidas
      printf ("Digite uma posição válida!!! Regra 10\n");
      return;
	  }// A partir daqui sabemos que não está nas bordas
    /*tabuleiro[idx+1][idy];// inferior
    tabuleiro[idx-1][idy];// superior
    tabuleiro[idx][idy-1];// esquerda
    tabuleiro[idx][idy+1];// Direita */
    if (idx - 1 < 0){	//É na margem superior
      if (
      (strcmp ((*tabuleiro)[idx][idy + 1], "  ") == 0) && 
      (strcmp ((*tabuleiro)[idx][idy - 1], "  ") == 0) && 
      (strcmp ((*tabuleiro)[idx + 1][idy], "  ") == 0) )
      {
        printf ("Digite uma posição válida!!! Regra 11\n");
        return;
      }
	  }else if (idx + 1 > *linha){// É na margem inferior
      if (
      (strcmp ((*tabuleiro)[idx][idy + 1], "  ") == 0) && 
      (strcmp ((*tabuleiro)[idx][idy - 1], "  ") == 0) && 
      (strcmp ((*tabuleiro)[idx - 1][idy], "  ") == 0) )
      {
        printf ("Digite uma posição válida!!! Regra 12\n");
        return;
      }
	  }else if (idy + 1 > *coluna){// É na margem à direita
      if (
      (strcmp ((*tabuleiro)[idx + 1][idy], "  ") == 0) && 
      (strcmp ((*tabuleiro)[idx - 1][idy], "  ") == 0) && 
      (strcmp ((*tabuleiro)[idx][idy - 1], "  ") == 0) )
      {
        printf ("Digite uma posição válida!!! Regra 13\n");
        return;
      }
	  }else if (idy - 1 < 0){// É na margem à esquerda
      if (
      (strcmp ((*tabuleiro)[idx][idy + 1], "  ") == 0) && 
      (strcmp ((*tabuleiro)[idx + 1][idy], "  ") == 0) && 
      (strcmp ((*tabuleiro)[idx - 1][idy], "  ") == 0) )
      {
        printf ("Digite uma posição válida!!! Regra 14\n");
        return;
      }
	  }//A partir daqui sabemos que o elemento pedido não está em nenhuma das margens nem nas bordas
    else if (
    (strcmp ((*tabuleiro)[idx + 1][idy], "  ") == 0) && 
    (strcmp ((*tabuleiro)[idx - 1][idy], "  ") == 0) && 
    (strcmp ((*tabuleiro)[idx][idy - 1], "  ") == 0) && 
    (strcmp ((*tabuleiro)[idx][idy + 1], "  ") == 0) )
    {// A posição pega está cercada por espaços vazios nos 4 lados
      printf ("Digite uma posição válida!!! Regra 15\n");
      return;
	  }
	
  }
  //A partir daqui a peça e posição é válida para cheat == 'N'
  strcpy ((*tabuleiro)[idx][idy], peca);
  if (cheat == 'N'){
    for (i=0; i<6; i++){
      if (strcmp(jogador[tornada].deck[i], peca) == 0){// É a peça que queremos retirar do deck
        break;
      }
    }
    strcpy (jogador[tornada].deck[i], " ");
  }else{
    for(i = 0; i<108; i++){// Deixamos essa comparaçao ao final para aproveitar o i
      if (strcmp(peca, pecas_disponiveis[i]) == 0){
        break;
      }
    }
    if (i == 108){
      printf("Digite uma peça válida!!! Regra 0\n");
    }
    strcpy(pecas_disponiveis[i], " ");
  }

  *ultimo_idx = idx;
  *ultimo_idy = idy;

  altera_tabuleiro (tabuleiro, ultimo_idx, ultimo_idy, linha, coluna, *cont_pecas, *cont_jogadas, guarda_posicao);
  *cont_jogadas += 1;
  *cont_pecas += 1;
}

/*
Essa função aumenta o tamanho do tabuleiro. Ela recebe a última jogada e quantas linhas e colunas tem o tabuleiro antigo.

-Se essa for a primeira jogada do jogo devemos fazer linha+2 coluna+2 e para o posição já ocupada o indice (i)(j) vai para (i+1)(j+1)

1.Se a posição colocada for i=0 j=qualquer; (margem superior sem bordas)
  Acrescentamos linha. E copiamos o indice (i)(j) do tabuleiro antigo para o (i+1)(j) do novo tabuleiro
2.Senão se a posição colocada for i=qualquer j=0; (margem esquerda sem bordas)
  Acrescentamos coluna. E copiamos o indice (i)(j) do tabuleiro antigo para o (i)(j+1) do novo tabuleiro
3.Senão se a posição colocada for i=linha j=qualquer; (margem inferior sem bordas)
  Acrescentamos linha. E copiamos o indice (i)(j) do tabuleiro antigo para o (i)(j) do novo tabuleiro
4.Senão se a posição colocada for i=qualquer j=coluna; (margem direita sem bordas)
  Acrescentamos coluna. E copiamo o indice (i)(j) do tabuleiro antigo para o (i)(j) do novo tabuleiro
Obs: A explicação para ****tabuleiro está em https://stackoverflow.com/questions/13431108/changing-address-contained-by-pointer-using-function
note que usamos (*tabuleiro)[][] devido a ordem de precedência
*/
void altera_tabuleiro (char ****tabuleiro, int *ultimo_idx, int *ultimo_idy, int *linha, int *coluna, int cont_pecas, int cont_jogadas, int *guarda_posicao){
  if (cont_pecas == 0){// É a primeira jogada do jogo
    *linha = *linha + 2;
    *coluna = *coluna + 2;

    char ***aux_tabuleiro;
    aux_tabuleiro = (char ***) malloc (sizeof (char **) * (*linha+1));
    
    if (aux_tabuleiro == NULL){
	    printf ("ERRO");
	    exit (1);
	  }
    for (int i = 0; i <= *linha; i++){
	    aux_tabuleiro[i] = (char **) malloc (sizeof (char *) * (*coluna+1));
	    if (aux_tabuleiro[i] == NULL){
	      printf ("ERRO");
	      exit (1);
	    }
	    for (int j = 0; j <= *coluna; j++){
        aux_tabuleiro[i][j] = (char *) malloc (sizeof (char) * 3);
        if (aux_tabuleiro[i][j] == NULL){
          printf ("ERRO");
          exit (1);
		    }
		    strcpy (aux_tabuleiro[i][j], "  ");
	    }
	  }
    strcpy (aux_tabuleiro[*ultimo_idx + 1][*ultimo_idy + 1], (*tabuleiro)[*ultimo_idx][*ultimo_idy]);

    for (int i = 0; i <= (*linha-2); i++){
	    for (int j = 0; j <= (*coluna-2); j++){
	      free ((*tabuleiro)[i][j]);
	    }
	    free ((*tabuleiro)[i]);
	  }
    free(*tabuleiro);

    *tabuleiro = aux_tabuleiro;
    *ultimo_idx += 1;
    *ultimo_idy += 1;
    //Colocando o novo índice em guarda_posicao
    guarda_posicao[0] = (*ultimo_idx);
    guarda_posicao[1] = (*ultimo_idy);
  }else if (*ultimo_idx == 0){// Margem superior
    *linha = *linha + 1;
    char ***aux_tabuleiro;
    aux_tabuleiro = (char ***) malloc (sizeof (char **) * (*linha+1));

    if (aux_tabuleiro == NULL){
	    printf ("ERRO");
	    exit (1);
	  }
    for (int i = 0; i <= *linha; i++){
	    aux_tabuleiro[i] = (char **) malloc (sizeof (char *) * (*coluna+1));
	    if (aux_tabuleiro[i] == NULL){
	      printf ("ERRO");
	      exit (1);
	    }
	    for (int j = 0; j <= *coluna; j++){
        aux_tabuleiro[i][j] = (char *) malloc (sizeof (char) * 3);
        if (aux_tabuleiro[i][j] == NULL){
          printf ("ERRO");
          exit (1);
		    }
		    strcpy (aux_tabuleiro[i][j], "  ");
	    }
	  }
    /*Passando os dados de tabuleiro para aux_tabuleiro da forma correta */
    for (int i = 0; i <= (*linha - 1); i++){
      for (int j = 0; j <= *coluna; j++){
        if (strcmp ((*tabuleiro)[i][j], "  ") != 0){// É uma peça
          strcpy (aux_tabuleiro[i + 1][j], (*tabuleiro)[i][j]);
        }
	    }
	  }
    for (int i = 0; i <= (*linha-1); i++){
	    for (int j = 0; j <= *coluna; j++){
	      free ((*tabuleiro)[i][j]);
	    }
	    free ((*tabuleiro)[i]);
	  }

    free(*tabuleiro);
    *tabuleiro = aux_tabuleiro;
    *ultimo_idx += 1;
    // Alterando e colocando índices das posições em guarda_posição
    // Está dentro desse if se cont_jogadas>0 (Ele já jogou uma peça) então o último indice de guarda_posicao com valor é 2*cont_jogadas-1 
    // Devemos alterar os idx
    for (int i = 0; i <= 2*cont_jogadas-1; i+=2){
      guarda_posicao[i] += 1;
    }
    guarda_posicao[2*cont_jogadas] = (*ultimo_idx);
    guarda_posicao[2*cont_jogadas+1] = (*ultimo_idy);
  
  }
  else if (*ultimo_idy == 0){// Margem esquerda
    *coluna = *coluna + 1;
    char ***aux_tabuleiro;
    aux_tabuleiro = (char ***) malloc (sizeof (char **) * (*linha+1));

    if (aux_tabuleiro == NULL){
	    printf ("ERRO");
	    exit (1);
	  }
    for (int i = 0; i <= *linha; i++){
      aux_tabuleiro[i] = (char **) malloc (sizeof (char *) * (*coluna+1));
      if (aux_tabuleiro[i] == NULL){
        printf ("ERRO");
        exit (1);
      }
      for (int j = 0; j <= *coluna; j++){
        aux_tabuleiro[i][j] = (char *) malloc (sizeof (char) * 3);
        if (aux_tabuleiro[i][j] == NULL){
          printf ("ERRO");
          exit (1);
        }
        strcpy (aux_tabuleiro[i][j], "  ");
	    }
	  }
    /*Passando os dados de tabuleiro para aux_tabuleiro da forma correta */
    for (int i = 0; i <= *linha; i++){
	    for (int j = 0; j <= (*coluna - 1); j++){
	      if (strcmp ((*tabuleiro)[i][j], "  ") != 0){// É uma peça
		      strcpy (aux_tabuleiro[i][j + 1], (*tabuleiro)[i][j]);
	      }
	    }
	  }
    for (int i = 0; i <= *linha; i++){
	    for (int j = 0; j <= (*coluna - 1); j++){
	      free ((*tabuleiro)[i][j]);
	    }
	    free ((*tabuleiro)[i]);
	  }

    free(*tabuleiro);
    *tabuleiro = aux_tabuleiro;
    *ultimo_idy += 1;
    // Devemos alterar somente os idy
    for (int i = 1; i <= 2*cont_jogadas-1; i+=2){// Altera
      guarda_posicao[i] += 1;
    }
    guarda_posicao[2*cont_jogadas] = (*ultimo_idx);
    guarda_posicao[2*cont_jogadas+1] = (*ultimo_idy);
    
  }
  else if (*ultimo_idx == *linha){// Margem inferior
    *linha = *linha + 1;
    char ***aux_tabuleiro;
    aux_tabuleiro = (char ***) malloc (sizeof (char **) * (*linha+1));

    if (aux_tabuleiro == NULL){
	    printf ("ERRO");
	    exit (1);
	  }
    for (int i = 0; i <= *linha; i++){
      aux_tabuleiro[i] = (char **) malloc (sizeof (char *) * (*coluna+1));
      if (aux_tabuleiro[i] == NULL){
        printf ("ERRO");
        exit (1);
      }
      for (int j = 0; j <= *coluna; j++){
        aux_tabuleiro[i][j] = (char *) malloc (sizeof (char) * 3);
        if (aux_tabuleiro[i][j] == NULL){
          printf ("ERRO");
          exit (1);
        }
        strcpy (aux_tabuleiro[i][j], "  ");
	    }
    }
    /*Passando os dados de tabuleiro para aux_tabuleiro da forma correta */
    for (int i = 0; i <= (*linha-1); i++){
      for (int j = 0; j <= *coluna; j++){
        if (strcmp ((*tabuleiro)[i][j], "  ") != 0){// É uma peça
          strcpy (aux_tabuleiro[i][j], (*tabuleiro)[i][j]);
        }
      }
    }
    for (int i = 0; i <= (*linha-1); i++){
      for (int j = 0; j <= (*coluna - 1); j++){
        free ((*tabuleiro)[i][j]);
      }
      free ((*tabuleiro)[i]);
    }
    free(*tabuleiro);
    
    *tabuleiro = aux_tabuleiro;

    guarda_posicao[2*cont_jogadas] = (*ultimo_idx);
    guarda_posicao[2*cont_jogadas+1] = (*ultimo_idy);
  
  }else if (*ultimo_idy == *coluna){// Margem direita
    *coluna = *coluna + 1;
    char ***aux_tabuleiro;
    aux_tabuleiro = (char ***) malloc (sizeof (char **) * (*linha+1));

    if (aux_tabuleiro == NULL){
	    printf ("ERRO");
	    exit (1);
	  }
    for (int i = 0; i <= *linha; i++){
      aux_tabuleiro[i] = (char **) malloc (sizeof (char *) * (*coluna+1));
      if (aux_tabuleiro[i] == NULL){
        printf ("ERRO");
        exit (1);
      }
      for (int j = 0; j <= *coluna; j++){
        aux_tabuleiro[i][j] = (char *) malloc (sizeof (char) * 3);
        if (aux_tabuleiro[i][j] == NULL){
          printf ("ERRO");
          exit (1);
        }
        strcpy (aux_tabuleiro[i][j], "  ");
	    }
    }
    /*Passando os dados de tabuleiro para aux_tabuleiro da forma correta */
    for (int i = 0; i <= *linha; i++){
      for (int j = 0; j <= (*coluna-1); j++){
        if (strcmp ((*tabuleiro)[i][j], "  ") != 0){// É uma peça
          strcpy (aux_tabuleiro[i][j], (*tabuleiro)[i][j]);
        }
      }
    }
    for (int i = 0; i <= *linha; i++){
      for (int j = 0; j <= (*coluna - 1); j++){
        free ((*tabuleiro)[i][j]);
      }
      free ((*tabuleiro)[i]);
    }
    free(*tabuleiro);

    *tabuleiro = aux_tabuleiro;
    guarda_posicao[2*cont_jogadas] = (*ultimo_idx);
    guarda_posicao[2*cont_jogadas+1] = (*ultimo_idy);
  }
  //Caso a posição não altere o tabuleiro
  guarda_posicao[2*cont_jogadas] = (*ultimo_idx);
  guarda_posicao[2*cont_jogadas+1] = (*ultimo_idy);
}


/*
Essa função irá receber quem é o jogador, qual é a peça, e qual foi a posição colocada.

Ao fim da jogada do jogador teremos que ou ele só colocou uma peça ou ele formou uma linha horizontal/vertical

.Caso jogou só uma peça:
  -Partindo da peça percorremos verticalmente/horizontalmente para cada peça encontrada incrementamos um ponto
.Casa haja formação de linha:
  -Primeiro identificamos se a linha é horizontal/vertical
  .Caso horizontal:
    -Partimos do primeiro elemento contamos quantas peças há para direita
    -Sem contar o elemento fazemos o mesmo para esquerda
    -Para cada elemento do grupo fazemos uma verificação na vertical (partindo deles) incrementando ponto a cada elemento encontrado
  .Caso vertical:
    -Partimos do primeiro elemento contamos quantas peças há para cima
    -Sem contar o elemento fazemos o mesmo para baixo
    -Para cada elemento do grupo fazemos uma verificação na horizontal (partindo deles) incrementando ponto a cada elemento encontrado
*/
int acrescenta_pontuacao (char ***tabuleiro ,int *guarda_posicao, int cont_jogadas, int cont_pecas, int ultimo_idx, int ultimo_idy, int linha, int coluna){
  int i, j, pontuacao=0, cont;
  //cont guarda quantas vezes o ciclo rodou
  if (cont_pecas==1){//Primeira rodada é a unica q não forma linha
    pontuacao=1;
  }else if (cont_jogadas == 1){//O jogador só jogou uma peça
    //ultimo_idx/y guardam a posição dessa peça que o jogador jogou
    pontuacao++;//Primeiro ponto da linha horizontal a direita
    //Verifica a direita
    for (i=ultimo_idy+1, cont=1; i<=coluna; i++){
      if(strcmp(tabuleiro[ultimo_idx][i], "  ") != 0){
        pontuacao++;
        cont++;
      }else{//Encontrou um espaço
        break;
      }
    }
    if(i==ultimo_idy+1) pontuacao--;
    //Verifica a esquerda
    pontuacao++; // Primeiro ponto da linha horizontal a esquerda
    for (i=ultimo_idy-1; i>=0; i--){
      if(strcmp(tabuleiro[ultimo_idx][i], "  ") != 0){
        pontuacao++;
        cont++;
      }else{
        break;
      }
    }
    if (i==ultimo_idy-1) pontuacao--;
    if (cont==6) pontuacao+=6;
    //Verifica inferiormente
    pontuacao++;//A primeira peça para a vertical inferior (Considerando que haja linha)
    for (i=ultimo_idx+1, cont=1; i<=linha; i++){
      if (strcmp(tabuleiro[i][ultimo_idy], "  ") != 0){
        pontuacao++;
        cont++;
      }else{
        break;
      }
    }
    if (i==ultimo_idx+1) pontuacao--;
    //Verifica superiormente
    pontuacao++;//A primeira peça para veritical superior (Considerando que há linha)
    for (i=ultimo_idx-1; i>=0; i--){
      if (strcmp(tabuleiro[i][ultimo_idy], "  ") != 0){
        pontuacao++;
        cont++;
      }else{
        break;
      }
    }
    if (i==ultimo_idx-1) pontuacao--;
    if (cont == 6) pontuacao += 6;
  }else if (cont_jogadas>1){//O jogador formou uma linha
    if (guarda_posicao[0] == guarda_posicao[2]){// Se ele formou uma linha na horizontal
      //2*cont_jogadas-1 é o maior índice de guarda_posicao
      //Primeiro Verificamos a linha horizontal
      //Para a direita
      for(i=guarda_posicao[1], cont=0; i<=coluna; i++){// Considera a primeira peça
        if (strcmp(tabuleiro[guarda_posicao[0]][i], "  ") != 0){
          pontuacao++;
          cont++;
        }else{
          break;
        }
      }
      //Para a esquerda
      for(i=guarda_posicao[1]-1; i>=0; i--){// Não considera
        if (strcmp(tabuleiro[guarda_posicao[0]][i], "  ") != 0){
          pontuacao++;
          cont++;
        }else{
          break;
        }
      }
      if (cont == 6) pontuacao += 6;
      //Para cada peça em guarda_posicao temos índice x igual a guarda_posicao[0] e temos que verificar as linhas que formam na vertical
      for (i=1; i<=2*cont_jogadas-1; i+=2){//Pega um guarda_posicao[i] representando coluna
        //Verifica inferiormente (Só devemos contar se tiver um elemento no índice tabuleiro[guarda_posicao[0]+1][guarda_posicao[i]])
        for (j=guarda_posicao[0], cont=0; j<=linha; j++){
          if (strcmp(tabuleiro[j][guarda_posicao[i]], "  ") != 0){
            pontuacao++;
            cont++;
          }else{
            break;
          }
        }//Caso saiu do laço com j = guarda_posicao[0]+1 quer dizer que não há elemento fazemos então
        if (j==guarda_posicao[0]+1){
          pontuacao--;
          cont--;
        }
        //Verifica superiormente
        for (j=guarda_posicao[0]; j>=0; j--){
          if (strcmp(tabuleiro[j][guarda_posicao[i]], "  ") != 0){
            pontuacao++;
            cont++;
          }else{
            break;
          }
        }
        if (j==guarda_posicao[0]-1){
          pontuacao--;
          cont--;
        }
        if (cont == 6) pontuacao += 6;
      }
    }else if (guarda_posicao[1] == guarda_posicao[3]){//Se ele formou uma linha na vertical
      //Primeiro Verificamos a linha vertical
      //Inferior
      for(i=guarda_posicao[0], cont=0; i<=linha; i++){
        if(strcmp(tabuleiro[i][guarda_posicao[1]], "  ") != 0){
          pontuacao++;
          cont++;
        }else{
          break;
        }
      }
      //Superior
      for(i=guarda_posicao[0]-1; i>=0; i--){
        if(strcmp(tabuleiro[i][guarda_posicao[1]], "  ") != 0){
          pontuacao++;
          cont++;
        }else{
          break;
        }
      }
      if (cont == 6) pontuacao += 6;
      //Para cada peça em guarda_posicao temos índice y igual a guarda_posicao[1]
      for (i=0; i<=2*cont_jogadas-1; i+=2){//Pega um guarda_posicao[i] representando linha
        //Verifica a direita
        for (j=guarda_posicao[1], cont=0; j<=coluna; j++){
          if (strcmp(tabuleiro[guarda_posicao[i]][j], "  ") != 0){
            pontuacao++;
            cont++;
          }else{
            break;
          }
        }
        if(j==guarda_posicao[1]+1){
          pontuacao--;
          cont--;
        }
        //Verifica a esquerda
        for (j=guarda_posicao[1]; j>=0; j--){
          if (strcmp(tabuleiro[guarda_posicao[i]][j], "  ") != 0){
            pontuacao++;
            cont++;
          }else{
            break;
          }
        }
        if(j==guarda_posicao[1]-1){
          pontuacao--;
          cont--;
        }
        if (cont == 6) pontuacao += 6;
      } 
    }
  }
  return pontuacao;
}
