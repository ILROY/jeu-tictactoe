#include <iostream>
#include <cstdlib> // Pour rand() et srand()
#include <ctime>   // Pour time()
using namespace std;

const int SIZE = 3; // Taille du plateau
char board[SIZE][SIZE]; // Le plateau de jeu
char currentPlayer; // Le joueur actuel
char opponent; // L'adversaire
int aiLevel; // Niveau de l'IA
bool twoPlayers; // Indique si le jeu est en mode 2 joueurs

// Statistiques
int xWins = 0;
int oWins = 0;
int totalGames = 0;

// Fonction pour initialiser le plateau
void initializeBoard() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = ' ';
}

// Fonction pour afficher le plateau en ASCII
void displayBoard() {
    cout << "   0   1   2\n";
    cout << "  -----------\n";
    for (int i = 0; i < SIZE; i++) {
        cout << i << " | ";
        for (int j = 0; j < SIZE; j++) {
            cout << board[i][j] << " | ";
        }
        cout << "\n  -----------\n";
    }
}

// Fonction pour vérifier si un joueur a gagné
bool checkWin(char player) {
    for (int i = 0; i < SIZE; i++)
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))
            return true;
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player))
        return true;
    return false;
}

// Fonction pour vérifier si le plateau est plein
bool isBoardFull() {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == ' ')
                return false;
    return true;
}

// Fonction Minimax
int minimax(int depth, bool isMaximizing) {
    if (checkWin(currentPlayer)) return 10 - depth;  // Le joueur a gagné
    if (checkWin(opponent)) return depth - 10;       // L'adversaire a gagné
    if (isBoardFull()) return 0;                      // Match nul

    if (isMaximizing) {
        int bestScore = -1000;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = currentPlayer;
                    int score = minimax(depth + 1, false);
                    board[i][j] = ' ';
                    bestScore = max(score, bestScore);
                }
            }
        }
        return bestScore;
    } else {
        int bestScore = 1000;
        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = opponent;
                    int score = minimax(depth + 1, true);
                    board[i][j] = ' ';
                    bestScore = min(score, bestScore);
                }
            }
        }
        return bestScore;
    }
}

// Fonction pour que l'IA joue le meilleur coup
void playAI() {
    if (aiLevel == 1) { // Niveau facile (coups aléatoires)
        int row, col;
        do {
            row = rand() % SIZE;
            col = rand() % SIZE;
        } while (board[row][col] != ' '); // Cherche une case vide
        board[row][col] = currentPlayer;
    } else { // Niveau moyen/difficile (Minimax)
        int bestScore = -1000;
        int bestRow = -1;
        int bestCol = -1;

        for (int i = 0; i < SIZE; i++) {
            for (int j = 0; j < SIZE; j++) {
                if (board[i][j] == ' ') {
                    board[i][j] = currentPlayer;
                    int score = minimax(0, false);
                    board[i][j] = ' ';
                    if (score > bestScore) {
                        bestScore = score;
                        bestRow = i;
                        bestCol = j;
                    }
                }
            }
        }
        board[bestRow][bestCol] = currentPlayer;
    }
}

// Fonction pour afficher les statistiques
void displayStats() {
    cout << "\n--- Statistiques ---\n";
    cout << "Total de parties jouées : " << totalGames << endl;
    cout << "Victoires de X : " << xWins << endl;
    cout << "Victoires de O : " << oWins << endl;
    cout << "---------------------\n";
}

int main() {
    srand(static_cast<unsigned int>(time(0))); // Initialiser le générateur de nombres aléatoires

    while (true) {
        initializeBoard();
        totalGames++; // Incrémenter le nombre total de parties

        // Choix du mode de jeu
        int gameMode;
        cout << "Choisissez le mode de jeu (1 pour 2 joueurs, 2 pour 1 joueur contre IA) : ";
        cin >> gameMode;
        twoPlayers = (gameMode == 1);

        cout << "Choisissez qui commence (1 pour Joueur X, 2 pour Joueur O) : ";
        int firstPlayer;
        cin >> firstPlayer;
        currentPlayer = (firstPlayer == 1) ? 'X' : 'O';
        opponent = (firstPlayer == 1) ? 'O' : 'X';

        if (!twoPlayers) { // Si le mode n'est pas 2 joueurs, demande le niveau de l'IA
            cout << "Choisissez le niveau de l'IA (1 pour Facile, 2 pour Moyenne/Difficile) : ";
            cin >> aiLevel;
        }

        while (true) {
            displayBoard();

            // Tour du joueur
            if (currentPlayer == 'X' || (currentPlayer == 'O' && !twoPlayers)) {
                int row, col;
                while (true) {
                    cout << "Joueur " << currentPlayer << ", entrez la ligne et la colonne (ex: 1 1): ";
                    cin >> row >> col;

                    // Vérifier si l'entrée est valide
                    if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && board[row][col] == ' ') {
                        board[row][col] = currentPlayer;
                        break; // Sortir de la boucle si le coup est valide
                    } else {
                        cout << "Mouvement invalide. Essayez encore.\n";
                    }
                }
            } 
            
            if (checkWin(currentPlayer)) { // Vérifier après le coup du joueur
                displayBoard();
                cout << "Le joueur " << currentPlayer << " a gagné !\n";
                if (currentPlayer == 'X') xWins++;
                else oWins++;
                break;
            }

            if (isBoardFull()) { // Vérifier si le plateau est plein
                displayBoard();
                cout << "Match nul !\n";
                break;
            }

            if (!twoPlayers) { // Tour de l'IA seulement si ce n'est pas en mode 2 joueurs
                cout << "L'IA joue...\n";
                playAI();

                if (checkWin(currentPlayer)) { // Vérifier après le coup de l'IA
                    displayBoard();
                    cout << "L'IA a gagné !\n";
                    oWins++;
                    break;
                }

                if (isBoardFull()) { // Vérifier si le plateau est plein
                    displayBoard();
                    cout << "Match nul !\n";
                    break;
                }
            }
            
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X'; // Changer de joueur
        }

        displayStats(); // Afficher les statistiques après chaque partie

        char playAgain;
        cout << "Voulez-vous jouer à nouveau ? (o/n) : ";
        cin >> playAgain;
        if (playAgain != 'o' && playAgain != 'O') {
            break; // Quitter le jeu
        }
    }

    return 0;
}