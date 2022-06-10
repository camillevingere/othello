#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>//Pour fonction rand
#include <stdlib.h>//Pour fonction rand

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

/*Réinitialise le plateau de jeu
 */
void nouvelle_partie(char jeu[N][N]){
  int i = 0;
  int j = 0;

  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      jeu[i][j]= '.';
  for(i=0; i<(N-1); i++){
    jeu[i+1][0]= i+'1';//Affiche les numéros des lignes de la grille
  }
  for(j=0; j<(N-1); j++){
    jeu[0][j+1]= j+'1';//Affiche les numéros des colonnes de la grille
  }




  jeu[4][4]= 'b';//b pour blanc
  jeu[5][5]= 'b';//n pour noir
  jeu[4][5]= 'n';
  jeu[5][4]= 'n';
  jeu[0][0]= ' ';//pour rien afficher sur la première case
}

/*Affiche le plateau de jeu
 */

void affiche_plateau(char jeu[N][N]){
  int i = 0;
  int j = 0;

  //Double for pour parcourir la matrice
  for(i=0; i<N; i++){
    for(j=0; j<N; j++){
      //Affiche le contenu de la case
      printf("%c", jeu[i][j]);
      //Affiche un espace
      printf(" ");
    }
    //Retourne à la ligne à chaque fin de ligne
    printf("\n");
  }
}

/*Parcours le tableau et vérifie si au moins une case est libre
  sinon le jeu devra s'arrêter dans la fonction mode_de_jeu.
*/

void fin_jeu(char jeuvalide[N][N],bool* placedispo){
  
  int i = 0;
  int j = 0;

  *placedispo = false;

  //Double for pour parcourir la matrice
  for(i=0; i<N; i++)
    for(j=0; j<N; j++){
      //Si il y a une case vide on affecte true à casevide
      if(jeuvalide[i][j] == '?'){
	*placedispo = true;
      }
    }
}

/*Indique le joueur en fonction du joueur
 */

void indique_joueur(char joueur){

  switch(joueur){

  case 'b' :
    printf("Joueur blanc joue\n");
    break;
  case 'n' :
    printf("Joueur noir joue\n");
    break;
  }
}

/*Change le joueur en fonction du joueur actuel
 */

char change_joueur(char joueur){

  char res = ' ';

  if (joueur == 'b'){
    res = 'n';
  }
  else if (joueur == 'n'){
    res = 'b';
  }

  return res;
}

/*Trouve l'adversaire en fonction du joueur.
 */

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

/*Apellé par la fonction direction_valide et verif_rempli. Trouve les valeurs des opérateurs pour les opérations à faire sur
  les lignes et colonnes des matrices. Une fois les opérateurs trouvés on peut les utiliser pour parcourir les cases
  d'une matrice depuis une case dans une direction choisie.
*/

void trouve_direction(int* pcolonne, int* pligne, int direction){
  


  switch(direction){

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

/*Copie la matrice tab1 dans la matrice tab2
 */

void copie_matrice(char tab1[N][N], char tab2[N][N]){
  
  int i = 0;
  int j = 0;

  for(i=0; i<N; i++)
    for(j=0; j<N; j++)
      tab2[i][j]= tab1[i][j];

}

/*Apellé par la fonction trouve_position_valide et verif_rempli. Cherche les directions valides pour une case donnée
  et rempli les cases valides dans jeuvalide avec des '?' pour indiquer au joueur où il peut jouer.
*/

void direction_valide(char joueur, char jeu[N][N],char jeuvalide[N][N], int ligne, int colonne, int DIRECTIONSVALIDES[8],int j){
  int h=0;
  int i=1;
  int pcolonne = 0;
  int pligne = 0;
  char adversaire = '0';
  
  //Trouve l'adversaire en fonction du joueur actuel
  adversaire = trouve_adversaire(joueur);

  //For pour parcourir toutes les directions autour d'une case
  for(h=0;h<7;h++){

    //Trouve les valeurs des opérateurs pour les opérations à faire sur
    //Les lignes et colonnes des matrices selon une direction
    trouve_direction(&pcolonne,&pligne,TOUTESDIRECTIONS[h]);
    
    /*Explication du facteur j : Dans la fonction trouve_position_valide on s'arrête déjà sur l'adversaire
      dans le parcours du plateau donc on donne j = 0 et on rentre dans le if. Dans la fonction verif_rempli
      on est sur une case vide donc on veut vérifier si la case d'APRES, dans une direction donnée,
      est bien l'adversaire (pour que ce soit un "moov" valide) donc j = 1 pour avoir la position d'après.
    */

    //Si sur la case en question il y a l'adversaire
    if(jeu[ligne+j*pligne][colonne+j*pcolonne] == adversaire){

      i=1;
      
      //Tant qu'on atteint pas un côté du plateau ET qu'on ne sors pas du plateau
      while( (colonne+i*pcolonne > 1 && colonne+i*pcolonne < 8) && (ligne+i*pligne > 0 && ligne+i*pligne < 9) ){
	
	//Si on rencontre un joueur 
	if(jeu[ligne+i*pligne][colonne+i*pcolonne] == joueur){
          //Si la case est libre alors le move est valide
          if(jeuvalide[ligne-pligne][colonne-pcolonne] == '.'){
	    jeuvalide[ligne-pligne][colonne-pcolonne] = '?';
	  }
	  //On retient la direction valide pour pas la chercher une
	  //nouvelle fois quand on va devoir remplir les cases
	  DIRECTIONSVALIDES[h] = TOUTESDIRECTIONS[h];
	  
	  i = 10;//On sors de la boucle car le move est bien valide
        }
        i++;
      }
    }
  }
}

/*Apellé par la fonction mode_de_jeu. Copie le plateau de jeu dans une nouvelle matrice et rajoute un '?'
  sur chaque case valide.
*/

void trouve_position_valide(char joueur,char jeu[N][N],char jeuvalide[N][N], int DIRECTIONSVALIDES[8]){

  int colonne = 0;
  int ligne = 0;
  char adversaire = '0';

  //Trouve l'adversaire en fonction du joueur actuel
  adversaire = trouve_adversaire(joueur);

  //Copie la matrice jeu dans la matrice valide
  copie_matrice(jeu,jeuvalide);

  //Double for pour parcourir la matrice
  for(ligne=0; ligne<N; ligne++){
    for(colonne=0; colonne<N; colonne++){
      //Si on rencontre un adversaire sur la case
      if(jeu[ligne][colonne] == adversaire){
	//Cherche les directions valides à remplir sur la case choisie et place les '?' sur les cases valides
	direction_valide(joueur,jeu,jeuvalide, ligne, colonne, DIRECTIONSVALIDES,0);
      }
    }
  }
}

/*Apellé par la fonction mode_de_jeu et bot_difficile. Rempli le plateau pour une case donnée en vérifiant
  les bonnes directions au préalable.
*/

void verif_rempli(int ligne, int colonne, char joueur,char jeu[N][N],char jeuvalide[N][N],int DIRECTIONSVALIDES[8]){

  int i = 0;
  int h = 0;
  int pcolonne = 0;
  int pligne = 0;
  char adversaire = '0';

  adversaire = trouve_adversaire(joueur);
  
  //On remet toutes les valeurs de DIRECTIONSVALIDES à 0
  for(i=0;i<7;i++){
    DIRECTIONSVALIDES[i]=0;
  }

  //Cherche les directions valides à remplir sur la case choisie
  direction_valide(joueur,jeu,jeuvalide, ligne, colonne, DIRECTIONSVALIDES,1);

  //For pour parcourir le tableau DIRECTIONSVALIDES
  for(i=0;i<7;i++){

    //Si la valeur de la case DIRECTIONSVALIDES n'est pas égale à 0
    //C'est à dire si la direction est valide
    if(DIRECTIONSVALIDES[i] != 0){

      //Trouve les valeurs des opérateurs pour les opérations à faire sur
      //Les lignes et colonnes des matrices selon une direction
      trouve_direction(&pcolonne,&pligne,TOUTESDIRECTIONS[i]);
      h = 0;
      //Tant que on rencontre pas un joueur sur la case et que les cases à remplir sont vides ou contiennent l'adversaire on remplit avec la couleur du joueur actuel
      while( (jeu[ligne+h*pligne][colonne+h*pcolonne] != joueur  || h == 0)
	    && (jeu[ligne+h*pligne][colonne+h*pcolonne] == '.' || jeu[ligne+h*pligne][colonne+h*pcolonne] == adversaire || jeu[ligne+h*pligne][colonne+h*pcolonne] == joueur)){
	jeu[ligne+h*pligne][colonne+h*pcolonne]=joueur;
	h++;//On incrémente h++ pour passer à la case suivante
      }
    }
  }

}

/*Apellé par la fonction mode_de_jeu et bot_difficile. Parcours le plateau (matrice) et incrémente le compteur
  adéquat suivant la couleur des cases.
*/

void cherche_score(char jeu[N][N],int* scoreblanc,int* scorenoir){

  int i=0;
  int j=0;

  *scoreblanc = 0;//Remise à 0 car il va être recompté en totalité
  *scorenoir = 0;//Remise à 0 car il va être recompté en totalité

  //Double for pour parcourir la matrice
  for(i=0; i<N; i++){
    for(j=0; j<N; j++){
      //Si la case est blanche
      if(jeu[i][j] == 'b'){
        *scoreblanc = *scoreblanc + 1;
      }
      //Sinon si la case est noire
      else if(jeu[i][j] == 'n'){
        *scorenoir = *scorenoir + 1;
      }
    }
  }
}

/*Apellé par la fonction mode_de_jeu. Bot qui choisi une case valide aléatoirement. Simple affichage du score
  des pions noirs et blancs.
*/

void affiche_score(int scoreblanc,int scorenoir){
  
  printf("\n");
  printf("\n");
    
  printf("SCORE\n");
    
  printf("Blanc : %d \n", scoreblanc);
  printf("Noir : %d \n", scorenoir);
    
  printf("\n");
  printf("\n");
  
}

/*Apellé par la fonction mode_de_jeu. Bot qui choisi une case valide aléatoirement. Il compte premièrement le nombre de case valide (=move) 
  puis génère un nombre aléatoire entre 1 et move pour choisir la case sur laquelle il va jouer.
*/

void bot_facile(char jeu[N][N], char jeuvalide[N][N], char joueur, int* ligne, int* colonne){

  int move = 0;
  int movechoisi = 0;
  int i =0;
  int j = 0;

  //Double for pour parcourir la matrice
  for(i=0; i<N; i++)
    for(j=0; j<N; j++){
      //Compte le nombre de cases valides quand on en croise une
      if(jeuvalide[i][j] == '?'){
        move++;
      }
    }

  //Génération d'un nombre aléatoire entre 1 et move
  movechoisi = rand()%(1-move) +1;
  move = 0;

  //Jeu sur la nème case valide choisie aléatoirement par l'algorithme précédent
  for(i=0; i<N; i++)
    for(j=0; j<N; j++){
      if(jeuvalide[i][j] == '?'){
        move++;
	//On retient la ligne et la colonne quand on atteint la case choisie
        if(move == movechoisi){
	  *ligne = i;
	  *colonne = j;
        }
      }
    }

}

/*Apellé par la fonction mode_de_jeu. Bot qui choisi la case qui rapportera le plus de points. Grâce à la matrice jeuvalide
  cette fonction test chaque case valide pour déterminer celle qui rapporte le plus de points.
*/

void bot_difficile(char jeu[N][N], char jeuvalide[N][N], char joueur, int* ligne, int* colonne){

  int i =0;
  int j = 0;
  int mem = 0;
  int pligne = 0;
  int pcolonne = 0;
  int scoreblanc = 0;
  int scorenoir = 0;
  
  int DIRECTIONSVALIDES[8] = {0};

  char jeutest[N][N];

  //Double for pour parcourir chaque ligne et chaque colonne de la matrice utilisée (ici jeuvalide)
  for(i=0; i<N; i++)
    for(j=0; j<N; j++){
      
      //Si en parcourant le plateau on trouve un placement valide
      if(jeuvalide[i][j] == '?'){
	
	mem = scorenoir;//On mémorise l'ancienne valeur de scorenoir dans mem
	copie_matrice(jeu,jeutest);//Copie la matrice jeu dans jeutest
	verif_rempli(*ligne,*colonne,joueur,jeutest,jeuvalide,DIRECTIONSVALIDES);//Rempli les cases du plateau en sachant déjà les directions valides pour remplir les bonnes cases
	cherche_score(jeutest,&scoreblanc,&scorenoir);//On cherche la nouvelle valeur de scorenoir
	
	//Si le placement du pion sur cette case donne plus de point que la case valide précédente
	if(scorenoir > mem){
	  pligne = i;//On retient la ligne
	  pcolonne = j;//On retient la colonne
	}
      }
    }

  *ligne = pligne;
  *colonne = pcolonne;

}

/*Apellé par la fonction mode_de_jeu. Vérifie si les cases rentrées par l'utilisateur sont légales et valides
 */

void demande_verif_placement(char jeuvalide[N][N],int* ligne, int* colonne){
    
  printf("Quel numéro de ligne ? \n");
  scanf("%d",ligne);//Pas de & car ligne est un pointer
  printf("Quel numéro de colonne ? \n");
  scanf(" %d",colonne);//Pas de & car colonne est un pointer
    
  //Si la case rentrée n'est pas un move valide ou si la case est hors du plateau
  if(jeuvalide[*ligne][*colonne] != '?' || *ligne > 8 || *ligne < 1 || *colonne > 8 || *colonne < 1 ){
    //Tant que la case rentrée n'est pas un move valide
    while(jeuvalide[*ligne][*colonne] != '?'){
      printf("Veuillez choisir une case valide\n");
      printf("Quel numéro de ligne ? \n");
      scanf("%d",ligne);
      printf("Quel numéro de colonne ? \n");
      scanf(" %d",colonne);
    }
  }
}


/*Appellé par la fonction main. Fonction qui interprète le choix du mode de jeu choisi par l'utilisateur en éxécutant le code adéquat.
 */

void mode_de_jeu(int modedejeu,int niveauIA){

  
  char jeu[N][N];//Matrice du plateau de jeu
  char jeuvalide[N][N];//Matrice du plateau de jeu avec les indications de placement valide sur les cases
  int DIRECTIONSVALIDES[8] = {0};//Retient les directions valides après l'éxécution de trouve_position_valide
  
  char joueur = 'b';
  bool endgame = true;
  int ligne = 0;
  int colonne = 0;
  int scorenoir = 0;
  int scoreblanc = 0;

  //Switch avec le mode de jeu choisi dans le main
  switch(modedejeu){

    //Choix numéro 1 : Le mode 1V1 contre une autre personne
  case 1 :
    

    nouvelle_partie(jeu);//Réinitialise le plateau de jeu

    //Tant que le plateau n'est pas plein OU que aucun pion ne puisse plus être joué
    while(endgame==true){

      indique_joueur(joueur);

      trouve_position_valide(joueur,jeu,jeuvalide, DIRECTIONSVALIDES);//Trouve les positions valides sur le plateau pour le joueur qui est en train de jouer

      printf("Vous pouvez jouer sur les cases où il y a des points d'interrogation\n");
      affiche_plateau(jeuvalide);//Affiche le plateau avec les placements valides
        
      demande_verif_placement(jeuvalide,&ligne,&colonne);//Vérifie si les cases rentrées sont valides

      verif_rempli(ligne,colonne,joueur,jeu,jeuvalide,DIRECTIONSVALIDES);//Rempli les cases du plateau en sachant déjà les directions valides pour remplir les bonnes cases

      cherche_score(jeu,&scoreblanc,&scorenoir);
      affiche_score(scoreblanc,scorenoir);
      trouve_position_valide(joueur,jeu,jeuvalide, DIRECTIONSVALIDES);//RELANCE pour savoir si il y a au moins un placement valide
      fin_jeu(jeuvalide,&endgame);//Vérifie si il y au moins une case vide et un moov valide (pour savoir si le jeu est terminé ou non)
      joueur = change_joueur(joueur);

    }
    printf("Le jeu est terminé, celui qui a le score le plus élevé a gagné\n");
    break;
  case 2 :

    nouvelle_partie(jeu);//Réinitialise le plateau de jeu

    while(endgame==true){

      //Si c'est au joueur blanc de jouer alors on laisse l'utilisateur choisir sa case
      
      if(joueur == 'b'){
	
	indique_joueur(joueur);

	trouve_position_valide(joueur,jeu,jeuvalide, DIRECTIONSVALIDES);//Trouve les positions valides sur le plateau pour le joueur qui est en train de jouer

	printf("Vous pouvez jouer sur les cases où il y a des points d'interrogation\n");
	affiche_plateau(jeuvalide);//Affiche le plateau avec les placements valides

	demande_verif_placement(jeuvalide,&ligne,&colonne);//Vérifie si les cases rentrées sont valides
	
      }
	
      //Si c'est au joueur noir de jouer alors on laisse l'IA choisir une case
	
      else if(joueur == 'n') {
	
	indique_joueur(joueur);
	trouve_position_valide(joueur,jeu,jeuvalide, DIRECTIONSVALIDES);//Trouve les positions valides sur le plateau pour le joueur qui est en train de jouer
	
	if(niveauIA == 1){
	  bot_facile(jeu,jeuvalide,joueur,&ligne,&colonne);
	}
	else if(niveauIA == 2){
	  bot_difficile(jeu,jeuvalide,joueur,&ligne,&colonne);
	}
	
      }

      verif_rempli(ligne,colonne,joueur,jeu,jeuvalide,DIRECTIONSVALIDES);//Rempli les cases du plateau en sachant déjà les directions valides pour remplir les bonnes cases
	
      affiche_plateau(jeu);
      cherche_score(jeu,&scoreblanc,&scorenoir);
      affiche_score(scoreblanc,scorenoir);
      trouve_position_valide(joueur,jeu,jeuvalide, DIRECTIONSVALIDES);//RELANCE pour savoir si il y a au moins un placement valide
      fin_jeu(jeuvalide,&endgame);//Vérifie si il y au moins une case vide et un moov valide (pour savoir si le jeu est terminé ou non)
      joueur = change_joueur(joueur);

    }
    printf("Le jeu est terminé, celui qui a le score le plus élevé a gagné\n");
    break;
  }
  
}

int main(void)
{
  int modedejeu = 0;
  int niveauIA = 0;

  printf("Mode Humain : Tapez 1 \n");
  printf("Mode IA : Tapez 2 \n");
  scanf("%d",&modedejeu);

  if(modedejeu == 2){
    printf("Mode Facile : Tapez 1 \n");
    printf("Mode Difficile : Tapez 2 \n");
    scanf("%d",&niveauIA);
  }

  mode_de_jeu(modedejeu,niveauIA);//Lance le code adapté à chaque mode de jeu

  
}
