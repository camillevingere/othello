#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <unistd.h>

#define N 9 //Nombre de cases du plateau de jeu + 1 (pour afficher les numéros de cases)

#define GAUCHE 1
#define DROITE 2
#define BAS 3
#define HAUT 4
#define DIAGHAUTDROITE 5
#define DIAGHAUTGAUCHE 6
#define DIAGBASDROITE 7
#define DIAGBASGAUCHE 8

int TOUTESDIRECTIONS[8] = {GAUCHE,DROITE,BAS,HAUT,DIAGHAUTDROITE,DIAGHAUTGAUCHE,DIAGBASDROITE,DIAGBASGAUCHE};

void nouvellepartie(char jeu[N][N]){
  int i = 0;
  int j = 0;

  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      jeu[i][j]= '.';
  for(i=0; i<N; i++){
    jeu[i+1][0]= i+'1';//Affiche les numéros des lignes de la grille
  }
  for(j=0; j<(N-1); j++){ //ATTENTION TROUVER COMMENTAIRE
    jeu[0][j+1]= j+'1';//Affiche les numéros des colonnes de la grille
  }




  jeu[4][4]= 'b';//b pour blanc
  jeu[5][5]= 'b';//n pour noir
  jeu[4][5]= 'n';
  jeu[5][4]= 'n';
  jeu[0][0]= ' ';//pour rien afficher sur la première case
}

void afficheplateau(char jeu[N][N]){
  int i = 0;
  int j = 0;

  for(i=0; i<N; i++){
    for(j=0; j<N; j++){
      printf("%c", jeu[i][j]);
      printf(" ");
    }
  printf("\n");
  }
}

void finjeu(char jeu[N][N],bool* casevide){
  int i = 0;
  int j = 0;

  *casevide = false;

  for(i=0; i<N; i++)
    for(j=0; j<N; j++){
      if(jeu[i][j] == '.'){
	*casevide = true;
      }
    }
}



void indiquejoueur(char joueur){

  switch(joueur){

  case 'b' :
    printf("Joueur blanc joue\n");
    break;
  case 'n' :
    printf("Joueur noir joue\n");
    break;
  }
}

char changejoueur(char joueur){

  char res = ' ';

  if (joueur == 'b'){
    res = 'n';
  }
  else if (joueur == 'n'){
    res = 'b';
  }

  return res;
}

char trouve_adversaire(char joueur){

  char adversaire = '0';

  switch(joueur){

  case 'b' :
    adversaire = 'n';
    break;
  case 'n' :
    adversaire = 'b';
    break;
  }

  return adversaire;
}

void trouve_direction(int* pcolonne, int* pligne, int place){



  switch(place){

  case GAUCHE :

    *pcolonne = -1;
    *pligne = 0;

    break;
  case DROITE :

    *pcolonne = 1;
    *pligne = 0;

    break;
  case BAS :

    *pcolonne = 0;
    *pligne = 1;

    break;
  case HAUT :

    *pcolonne = 0;
    *pligne = -1;

    break;
  case DIAGHAUTGAUCHE :

    *pcolonne = -1;
    *pligne = -1;

    break;
  case DIAGHAUTDROITE :

    *pcolonne = +1;
    *pligne = -1;

    break;
  case DIAGBASGAUCHE :


    *pcolonne = -1;
    *pligne = +1;

    break;
  case DIAGBASDROITE :

    *pcolonne = +1;
    *pligne = +1;

    break;
  }

}

void copie_matrice(char tab1[N][N], char tab2[N][N]){
  int i = 0;
  int j = 0;

  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      tab2[i][j]= tab1[i][j];

}

void direction_valide(char joueur, char jeu[N][N],char jeuvalide[N][N], int ligne, int colonne, int DIRECTIONSVALIDES[8],int j){
  int h=0;
  int i=1;
  int pcolonne = 0;
  int pligne = 0;
  char adversaire = '0';

adversaire = trouve_adversaire(joueur);

for(h=0;h<7;h++){

trouve_direction(&pcolonne,&pligne,TOUTESDIRECTIONS[h]);
if(jeu[ligne+j*pligne][colonne+j*pcolonne] == adversaire){

i=1;

while( (colonne+i*pcolonne > 1 && colonne+i*pcolonne < 8) && (ligne+i*pligne > 0 && ligne+i*pligne < 9) ){
  //Tant qu'on atteint pas un côté du plateau ET qu'on ne sors pas du plateau

    if(jeu[ligne+i*pligne][colonne+i*pcolonne] == joueur){
          //Si la case est libre
          if(jeuvalide[ligne-pligne][colonne-pcolonne] == '.'){
              jeuvalide[ligne-pligne][colonne-pcolonne] = '?';
            }
              DIRECTIONSVALIDES[h] = TOUTESDIRECTIONS[h];
      	      //printf("Ligne : %d Colonne : %d\n",ligne,colonne);
      	      i = 10;//On sors de la boucle car le move est bien valide
        }
        i++;
      }
    }
  }
}

void trouve_position_valide(char joueur,char jeu[N][N],char jeuvalide[N][N], int DIRECTIONSVALIDES[8]){

  int colonne = 0;
  int ligne = 0;
  int i = 1;
  int h =0;
  int pcolonne = 0;
  int pligne = 0;
  char adversaire = '0';

  adversaire = trouve_adversaire(joueur);



  copie_matrice(jeu,jeuvalide);

  for(ligne=0; ligne<N; ligne++){
    for(colonne=0; colonne<N; colonne++){
      if(jeu[ligne][colonne] == adversaire){
          direction_valide(joueur,jeu,jeuvalide, ligne, colonne, DIRECTIONSVALIDES,0);
        }
	  }
	}
}


void verif_rempli(int ligne, int colonne, char joueur,char jeu[N][N],char jeuvalide[N][N],int DIRECTIONSVALIDES[8]){

  char adversaire = '0';
  int i = 0;
  int j = 0;
  int h = 0;
  int pcolonne = 0;
  int pligne = 0;

  adversaire = trouve_adversaire(joueur);

  for(i=0;i<7;i++){
    DIRECTIONSVALIDES[i]=0;
  }
  direction_valide(joueur,jeu,jeuvalide, ligne, colonne, DIRECTIONSVALIDES,1);

  i=1;
  for(j=0;j<7;j++){

    if(DIRECTIONSVALIDES[j] != 0){

	  trouve_direction(&pcolonne,&pligne,TOUTESDIRECTIONS[j]);
                while(jeu[ligne+h*pligne][colonne+h*pcolonne] != joueur){//Tant que on rencontre pas un joueur sur la case ou Tant que on est pas passés à la case d'après
                    jeu[ligne+h*pligne][colonne+h*pcolonne]=joueur;
                    h++;
                }
      }
	}

}

void indique_gagnant(char jeu[N][N],int* b, int* n){
  *b=0;
  *n=0;
  int i=0;
  int j=0;
  for(i=0; i<N; i++)
    for(j=0; j<N; j++){
      if(jeu[i][j] == 'b'){
        *b=*b+1;
      }
      else if(jeu[i][j] == 'n'){
        *n=*n+1;
      }
    }
}

void bot_facile(char jeu[N][N], char jeuvalide[N][N], char joueur, int* ligne, int* colonne){

  int move = 0;
  int movechoisi = 0;
  int i =0;
  int j = 0;

  for(i=0; i<N; i++)
    for(j=0; j<N; j++){
      if(jeuvalide[i][j] == '?'){
        move++;
      }
    }

  movechoisi = rand(1,move);
  move = 0;

  for(i=0; i<N; i++)
    for(j=0; j<N; j++){
      if(jeuvalide[i][j] == '?'){
        move++;
        if(move == movechoisi){
          jeu[i][j] = joueur;
        }
      }
    }

}

int main(void)
{
  char jeu[N][N];
  char jeuvalide[N][N];
  int DIRECTIONSVALIDES[8] = {0};
  bool endgame = false;
  int ligne = 0;
  int colonne = 0;
  char joueur = 'b';
  int scoreblanc = 0;
  int scorenoir=0;
  int i=0;

  nouvellepartie(jeu);//Réinitialise le plateau de jeu
  afficheplateau(jeu);//Affiche le plateau de jeu
  finjeu(jeu,&endgame);//Vérifie si il y au moins une case vide (pour savoir si le jeu est terminé ou non)

  /*printf("%d", casevide);
    printf("\n");*/

  while(endgame==true){

    indiquejoueur(joueur);

    trouve_position_valide(joueur,jeu,jeuvalide, DIRECTIONSVALIDES);

    printf("Vous pouvez jouer sur les cases où il y a des points d'interrogation\n");
    afficheplateau(jeuvalide);


    printf("Quel numéro de ligne ? \n");
    scanf("%d",&ligne);
    printf("Quel numéro de colonne ? \n");
    scanf(" %d",&colonne);

    //Si la case rentrée n'est pas un move valide ou si la case est hors du plateau
    if(jeuvalide[ligne][colonne] != '?' || ligne > 8 || ligne < 1 || colonne > 8 || colonne < 1 ){
      //Tant que la case rentrée n'est pas un move valide
      while(jeuvalide[ligne][colonne] != '?'){
        printf("Veuillez choisir une case valide\n");
        printf("Quel numéro de ligne ? \n");
        scanf("%d",&ligne);
        printf("Quel numéro de colonne ? \n");
        scanf(" %d",&colonne);
      }
    }

    verif_rempli(ligne,colonne,joueur,jeu,jeuvalide,DIRECTIONSVALIDES);

    indique_gagnant(jeu,&scoreblanc,&scorenoir);
    printf("Blanc : %d \n", scoreblanc);
    printf("Noir : %d \n", scorenoir);

    afficheplateau(jeu);//Affiche le plateau de jeu
    finjeu(jeu,&endgame);//Vérifie si il y au moins une case vide (pour savoir si le jeu est terminé ou non)
    joueur = changejoueur(joueur);

  }
}



/* if(possible != true){
       printf("Veuillez choisir une case valide\n");

       printf("Quel numéro de ligne ? \n");
       scanf("%d",&ligne);
       printf("Quel numéro de colonne ? \n");
       scanf(" %d",&colonne);
 }*/
// }



	  /*if( ((pcolonne < 0) && (pligne == 0))
	      || ((pcolonne == 0) && (pligne < 0))
	      || ((pcolonne < 0) && (pligne < 0))
	      || ((pcolonne < 0) && (pligne > 0))){*/


  /*void verifplacement(int ligne, int colonne, char joueur,char jeu[N][N]){

  char adversaire ='0';

  int i=colonne;
  bool possible = false;

  switch(joueur){

  case 'b' :
    adversaire = 'n';
    break;
  case 'n' :
    adversaire = 'b';
    break;
  }

    if(jeu[ligne][colonne] == '.'){
      if( (ligne!=1 && colonne!=1)//Si on est pas dans un coin du plateau
	  && (ligne!=1 && colonne!=8)
	  && (ligne!=8 && colonne!=1)
	  && (ligne!=8 && colonne!=8)){

	if(jeu[ligne][colonne-1]== adversaire){

	  possible = false;

	  for(i=colonne; i>=1;i--){
	    if(jeu[ligne][i] == joueur){
	      possible = true;
	    }
	    else if(jeu[ligne][i] == '.'){
	      possible = false;
	      i = 0;
	    }
	  }

	  i = 0;

	  if(possible == true){

	    while(jeu[ligne][colonne-i] != joueur){
	      jeu[ligne][colonne-i]=joueur;
	      i++;
	    }

	  }
	}
	if(jeu[ligne][colonne+1]==adversaire){//PAS DE ELSE IF car toutes les possibilités sont possibles en même temps donc on doit pas exclure cette possibilité

	  possible = false;

	  for(i=colonne; i<=8;i++){
	    if(jeu[ligne][i] == joueur){
	      possible = true;
	    }
	    else if(jeu[ligne][i] == '.'){
	      possible = false;
	      i = 0;
	    }
	  }

	  i = 0;

	  if(possible == true){

	    while(jeu[ligne][colonne+i] != joueur){
	      jeu[ligne][colonne+i]=joueur;
	      i++;
	    }

	  }
	}
	if(jeu[ligne-1][colonne]==adversaire && jeu[ligne-2][colonne] != '.'){

	  possible = false;

	  for(i=ligne; i>=1;i--){
	    if(jeu[i][colonne] == joueur){
	      possible = true;
	    }
	    else if(jeu[i][colonne] == '.'){
	      possible = false;
	      i = 0;
	    }
	  }

	  i = 0;

	  if(possible == true){

	    while(jeu[ligne][colonne+i] != jou	  /*if( ((pcolonne < 0) && (pligne == 0))
	      || ((pcolonne == 0) && (pligne < 0))
	      || ((pcolonne < 0) && (pligne < 0))
	      || ((pcolonne < 0) && (pligne > 0))){*/


  /*void verifplacement(int ligne, int colonne, char joueur,char jeu[N][N]){

  char adversaire ='0';

  int i=colonne;
  bool possible = false;

  switch(joueur){

  case 'b' :
    adversaire = 'n';
    break;
  case 'n' :
    adversaire = 'b';
    break;
  }

    if(jeu[ligne][colonne] == '.'){
      if( (ligne!=1 && colonne!=1)//Si on est pas dans un coin du plateau
	  && (ligne!=1 && colonne!=8)
	  && (ligne!=8 && colonne!=1)
	  && (ligne!=8 && colonne!=8)){

	if(jeu[ligne][colonne-1]== adversaire){

	  possible = false;

	  for(i=colonne; i>=1;i--){
	    if(jeu[ligne][i] == joueur){
	      possible = true;
	    }
	    else if(jeu[ligne][i] == '.'){
	      possible = false;
	      i = 0;
	    }
	  }

	  i = 0;

	  if(possible == true){

	    while(jeu[ligne][colonne-i] != joueur){
	      jeu[ligne][colonne-i]=joueur;
	      i++;
	    }

	  }
	}

  char jeuvalide[N][N];
	if(jeu[ligne][colonne+1]==adversaire){//PAS DE ELSE IF car toutes les possibilités sont possibles en même temps donc on doit pas exclure cette possibilité

	  possible = false;

	  for(i=colonne; i<=8;i++){
	    if(jeu[ligne][i] == joueur){
	      possible = true;
	    }
	    else if(jeu[ligne][i] == '.'){
	      possible = false;
	      i = 0;
	    }
	  }

	  i = 0;

	  if(possible == true){

	    while(jeu[ligne][colonne+i] != joueur){
	      jeu[ligne][colonne+i]=joueur;
	      i++;
	    }

	  }
	}
	if(jeu[ligne-1][colonne]==adversaire && jeu[ligne-2][colonne] != '.'){

	  possible = false;

	  for(i=ligne; i>=1;i--){
	    if(jeu[i][colonne] == joueur){
	      possible = true;
	    }
	    else if(jeu[i][colonne] == '.'){
	      possible = false;
	      i = 0;
	    }
	  }

	  i = 0;

	  if(possible == true){

	    while(jeu[ligne][colonne+i] != joueur){
	      jeu[ligne][colonne+i]=joueur;
	      i++;
	    }

	  }
	}
	if(jeu[ligne+1][colonne]==adversaire && jeu[ligne+2][colonne] != '.'){
	  int i=0;
	  while(jeu[ligne+i][colonne] != joueur){
	    jeu[ligne+i][colonne]=joueur;
	    i++;
	  }
	}
	if(jeu[ligne-1][colonne-1]==adversaire && jeu[ligne-2][colonne-2] != '.'){
	  int i=0;
	  while(jeu[ligne-i][colonne-i] != joueur){
	    jeu[ligne-i][colonne-i]=joueur;
	    i++;
	  }
	}
	if(jeu[ligne-1][colonne+1]==adversaire && jeu[ligne-2][colonne+2] != '.'){
	  int i=0;
	  while(jeu[ligne-i][colonne+i] != joueur){
	    jeu[ligne-i][colonne+i]=joueur;
	    i++;
	  }
	}

	if(jeu[ligne+1][colonne-1]==adversaire && jeu[ligne+2][colonne-2] != '.'){
	  int i=0;
	  while(jeu[ligne+i][colonne-i] != joueur){
	    jeu[ligne+i][colonne-i]=joueur;
	    i++;
	  }
	}

	if(jeu[ligne+1][colonne+1]==adversaire && jeu[ligne+2][colonne+2] != '.'){
	  int i=0;
	  while(jeu[ligne+i][colonne+i] != joueur){
	    jeu[ligne+i][colonne+i]=joueur;
	    i++;
	  }
	}

      }
    }
    }


if(place == "gauche"){//Pas de switch car place est une chaine de caractères

    pcolonne = -1;
    pligne = 0;
  }
  else if(place == "droite"){

    pcolonne = 1;
    pligne = 0;

  }
  else if(place == "bas"){

    pcolonne = 0;
    pligne = 1;
  }
  else if(place == "haut"){

    pcolonne = 0;
    pligne = -1;
  }
  else if(place == "diaghautgauche"){

    pcolonne = -1;
    pligne = -1;
  }
  else if(place == "diaghautdroite"){

    pcolonne = +1;
    pligne = -1;

  }
  else if(place == "diagbasgauche"){

    pcolonne = -1;
    pligne = +1;

  }
  else if(strcmp(place,diagbasdroite)){//place == "diagbasdroite"){

    pcolonne = +1;
    pligne = +1;
  }*/

  /*eur){
	      jeu[ligne][colonne+i]=joueur;
	      i++;
	    }

	  }
	}
	if(jeu[ligne+1][colonne]==adversaire && jeu[ligne+2][colonne] != '.'){
	  int i=0;
	  while(jeu[ligne+i][colonne] != joueur){
	    jeu[ligne+i][colonne]=joueur;
	    i++;
	  }
	}
	if(jeu[ligne-1][colonne-1]==adversaire && jeu[ligne-2][colonne-2] != '.'){
	  int i=0;
	  while(jeu[ligne-i][colonne-i] != joueur){
	    jeu[ligne-i][colonne-i]=joueur;
	    i++;
	  }
	}
	if(jeu[ligne-1][colonne+1]==adversaire && jeu[ligne-2][colonne+2] != '.'){
	  int i=0;
	  while(jeu[ligne-i][colonne+i] != joueur){
	    jeu[ligne-i][colonne+i]=joueur;
	    i++;
	  }
	}

	if(jeu[ligne+1][colonne-1]==adversaire && jeu[ligne+2][colonne-2] != '.'){
	  int i=0;
	  while(jeu[ligne+i][colonne-i] != joueur){
	    jeu[ligne+i][colonne-i]=joueur;
	    i++;
	  }
	}

	if(jeu[ligne+1][colonne+1]==adversaire && jeu[ligne+2][colonne+2] != '.'){
	  int i=0;
	  while(jeu[ligne+i][colonne+i] != joueur){
	    jeu[ligne+i][colonne+i]=joueur;
	    i++;
	  }
	}

      }
    }
    }


if(place == "gauche"){//Pas de switch car place est une chaine de caractères

    pcolonne = -1;
    pligne = 0;
  }
  else if(place == "droite"){

    pcolonne = 1;
    pligne = 0;

  }
  else if(place == "bas"){

    pcolonne = 0;
    pligne = 1;
  }
  else if(place == "haut"){

    pcolonne = 0;
    pligne = -1;
  }
  else if(place == "diaghautgauche"){

    pcolonne = -1;
    pligne = -1;
  }
  else if(place == "diaghautdroite"){

    pcolonne = +1;
    pligne = -1;

  }
  else if(place == "diagbasgauche"){

    pcolonne = -1;
    pligne = +1;

  }
  else if(strcmp(place,diagbasdroite)){//place == "diagbasdroite"){

    pcolonne = +1;
    pligne = +1;
  }*/
