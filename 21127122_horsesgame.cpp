#include <iostream>
#include <stdlib.h>
#include <conio.h>
#include <windows.h>
#pragma comment(lib,"Winmm.lib")

using namespace std;

const int MAX = 20;
int boardSize, numberOfHorses, numberOfPlayers, numberOfTurns, numberOfWins[MAX];
char boardChar1[MAX][MAX], boardChar2[MAX][MAX];
bool sound = 1, color = 1;

// Game support function

void input_setting(int& boardSize, int& numberOfPlayers, int& numberOfHorses,
                    char nameOfPlayer[], int& diceType, int pos[]); // Setting before playing game

void setting_color_and_sound(); // Setting color background and turn on / offsound

void roll_dice(int& step, int first, int last); // Roll dice

bool check_move(int pos[], int step, int i); // Check if the current move isaccepted

void move_turn(int step, int pos[], int i, int j, char nameOfPlayer[],
    char horse, bool& check_acceptable); // Process of a moving turn

bool check_win(); // Check if there is a player win or not

void output_not_acceptable();  // Print a line of notification that the currentmove is not accepted

bool replay_game(int& first); // Ask the player to play again or not

void outcome(char nameOfPlayer[], int numberOfTies, int play_times); ; // Printthe number of wins, ties, and total game.

// Interface support function

void makeColor(); // Turn off Dark mode

void fill_board_with_arrow();  // Make arrow of directions

void fill_board_with_space(); // Make blank position

void horizontal_line_edge(); // Make horizontal line 1

void horizontal_line(); // Make horizontal line 2

void vertical_line(bool checkMid, int row); // Make vertical line

void output_board(); // Print the board

// Audio support function

void Sound_Game_1(); // Sound of game

void Sound_Game_2(); // Sound of each step

int main()
{
    int diceType, step, pos[MAX], dem, play_times = 0, numberOfTies = 0, first = 1;
    char nameOfPlayer[MAX], horse_number;
    bool win, check_acceptable = 1;

    setting_color_and_sound();

    system("cls");

    makeColor();

    cout << "WELCOME TO HORSES BOARD GAME!!!\n\n";

    Sound_Game_1();

    while (replay_game(first)) {

        play_times++;
        win = false;

        if (play_times > 1) system("cls");

        input_setting(boardSize, numberOfPlayers,
            numberOfHorses, nameOfPlayer, diceType, pos);
        pos[numberOfHorses * numberOfPlayers] = boardSize * boardSize - 1;

        fill_board_with_arrow();

        boardChar1[0][0] = boardChar2[0][0] = '>';
        if (boardSize % 2 == 0) {
            boardChar1[boardSize - 1][0] = '*';
            boardChar2[boardSize - 1][0] = '*';
        }
        else {
            boardChar1[boardSize - 1][boardSize - 1] = '*';
            boardChar2[boardSize - 1][boardSize - 1] = '*';
        }

        output_board();
        cin.get();
        cout << "Press Enter to continue...";
        cin.get();
        system("cls");

        fill_board_with_space();

        if (boardSize % 2 == 0) {
            boardChar1[boardSize - 1][0] = '*';
            boardChar2[boardSize - 1][0] = '*';
        }
        else {
            boardChar1[boardSize - 1][boardSize - 1] = '*';
            boardChar2[boardSize - 1][boardSize - 1] = '*';
        }

        while (!win && numberOfTurns != 0) {

            horse_number = 49;
            dem = 0;

            for (int i = 0; i < numberOfHorses * numberOfPlayers; i++) {
                check_acceptable = 1;
                cout << "Please Enter to roll dice..." << endl;
                cin.get();
                system("cls");
                cout << "Player " << i % numberOfPlayers + 1 << " roll dice: ";
                roll_dice(step, 1, 3 * diceType);
                cout << step;
                int tmp = i % numberOfPlayers;
                dem++;
                cout << " (horse " << horse_number << ")" << endl;
                move_turn(step, pos, i, tmp, nameOfPlayer, horse_number, check_acceptable);
                
                output_board();

                if (check_acceptable) Sound_Game_2();
                else if (!check_acceptable) output_not_acceptable();

                win = check_win();
                if (win) {
                    numberOfWins[tmp]++;
                    cout << endl << "Player " << nameOfPlayer[tmp] << " is the WINNER!!!\n\n";
                    Sound_Game_1();
                    break;
                }
                if (dem == numberOfPlayers) {
                    horse_number += 1;
                    dem = 0;
                }
            }
            numberOfTurns--;
        }

        if (!win) {
            cout << "\nIt's tie! Nobody is the winner!\n\n";
            Sound_Game_1();
            numberOfTies++;
        }

        outcome(nameOfPlayer, numberOfTies, play_times);

        cout << "Enter to continue...";
        cin.get();
        system("cls");

        if (play_times == 20) { // 20 games limit for players
            system("cls");
            cout << "No more game today!\n";
            cout << "Please go back with your deadline or something else!\n";
            first = 2;
        }
    }

    cout << "\nThanks for playing my game!\n\n";
    Sound_Game_1();

    return 0;
}

void input_setting(int& boardSize, int& numberOfPlayers,
    int& numberOfHorses, char nameOfPlayer[], int& diceType, int pos[])
{
    cout << "Board game size (n x n) (3..7): "; cin >> boardSize;
    cout << "Number of players (1..4): "; cin >> numberOfPlayers;
    if (numberOfPlayers == 1) {
        cout << "Name of Player: "; cin >> nameOfPlayer[0];
        cout << "Number of horses (1..4): ";
        cin >> numberOfHorses;
    }
    else {
        for (int i = 0; i < numberOfPlayers; i++) {
            cout << "Name of Player " << i + 1 << ": ";
            cin >> nameOfPlayer[i];
        }
        cout << "Number of horses of each Player (1..4): ";
        cin >> numberOfHorses;
    }

    for (int i = 0; i < numberOfPlayers * numberOfHorses; i++) {
        pos[i] = -1;
    }

    cout << "Which dice type?" << endl;
    cout << "1: 1_3" << endl;
    cout << "2: 1_6" << endl;
    cout << "Type: ";
    cin >> diceType;
    cout << "Number of turns of each horse (1..50): ";
    cin >> numberOfTurns;
    cout << endl;
}

void setting_color_and_sound()
{
    char re;
    cout << "Dark mode? Y(yes) or N(no)\n";
    cout << "Your choice: ";
    cin >> re;
    if (re == 'Y' || re == 'y') color = 0;
    else if (re == 'N' || re == 'n') color = 1;
    else {
        bool check_syntax = 0;
        while (!check_syntax) {
            cout << "*Syntax Error!*" << endl;
            cout << "Your choice: ";
            cin >> re;
            if (re == 'Y' || re == 'y') { color = 0; check_syntax = 1; }
            if (re == 'N' || re == 'n') { color = 1; check_syntax = 1; }
        }
    }

    cout << "Do you want to play sound? Y(yes) or N(no)\n";
    cout << "Your choice: ";
    cin >> re;
    if (re == 'Y' || re == 'y') sound = 1;
    else if (re == 'N' || re == 'n') sound = 0;
    else {
        bool check_syntax = 0;
        while (!check_syntax) {
            cout << "*Syntax Error!*" << endl;
            cout << "Your choice: ";
            cin >> re;
            if (re == 'Y' || re == 'y') { sound = 1; check_syntax = 1; }
            if (re == 'N' || re == 'n') { sound = 0; check_syntax = 1; }
        }
    }
}

void makeColor()
{
    if (color) system("color 3e");
}

void fill_board_with_arrow()
{
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            if (i % 2 == 0) {
                boardChar2[i][j] = '>';
                boardChar1[i][j] = '>';
            }
            else {
                boardChar1[i][j] = '<';
                boardChar2[i][j] = '<';
            }
        }
    }
    for (int i = 0; i < boardSize; i++) {
        if (i % 2 == 0) {
            boardChar1[i][boardSize - 1] = boardChar2[i][boardSize - 1] = 25;
        }
        else boardChar1[i][0] = boardChar2[i][0] = 25;
    }
}

void fill_board_with_space()
{
    for (int i = 0; i < boardSize; i++) {
        for (int j = 0; j < boardSize; j++) {
            boardChar1[i][j] = boardChar2[i][j] = ' ';
        }
    }
}

void horizontal_line_edge()
{
    for (int i = 0; i < boardSize; i++) {
        if (i == 0) {
            cout << " =========";
        }
        else if (i == boardSize - 1) cout << "===========";
        else cout << "==========";
    }
    cout << endl;
}

void horizontal_line()
{
    for (int i = 0; i < boardSize; i++) {
        cout << "||--------";
        if (i == boardSize - 1) {
            cout << "||";
        }
    }
    cout << endl;
}

void vertical_line(bool checkMid, int row)
{
    cout << "||";
    for (int i = 0; i < boardSize; i++) {
        if (checkMid) {
            cout << "   " << boardChar1[row][i] << boardChar2[row][i] << "   ||";
        }
        else {
            cout << "        ||";
        }
    }
    cout << endl;
}

void output_board()
{
    horizontal_line_edge();
    for (int i = 0; i < boardSize; i++) {
        horizontal_line();
        vertical_line(false, i);
        vertical_line(true, i);
        vertical_line(false, i);
        horizontal_line();
    }
    horizontal_line_edge();
}

void roll_dice(int& step, int first, int last)
{
    step = rand() % (last - first + 1) + first;
}

bool check_move(int pos[], int step, int i)
{
    for (int j = 0; j <= numberOfHorses * numberOfPlayers; j++) {
        if ((pos[j] > pos[i]) && (pos[j] < pos[i] + step)) {
            return false;
            break;
        }
    }
    return true;
}

void move_turn(int step, int pos[], int i, int j,
    char nameOfPlayer[], char horse, bool& check_acceptable)
{
    bool can_move = true, change_pos = 1;
    int x, y;

    x = pos[i] / boardSize;
    if (x % 2 == 0) { y = pos[i] % boardSize; }
    else { y = boardSize - pos[i] % boardSize - 1; }

    can_move = check_move(pos, step, i);

    if (can_move) {
        int row, col;
        row = (pos[i] + step) / boardSize;

        if (row % 2 == 0) { col = (pos[i] + step) % boardSize; }
        else { col = boardSize - (pos[i] + step) % boardSize - 1; }

        if (boardChar1[row][col] == ' ') {
            boardChar1[x][y] = ' ';
            boardChar1[row][col] = nameOfPlayer[j];
            boardChar2[x][y] = ' ';
            boardChar2[row][col] = horse;
        }
        else {
            for (int k = 0; k <= numberOfHorses * numberOfPlayers; k++) {
                if (pos[i] + step == pos[k]) {
                    if (nameOfPlayer[i % numberOfPlayers] != nameOfPlayer[k % numberOfPlayers]
                        || pos[k] == boardSize * boardSize - 1) {
                        pos[k] = -1;
                        boardChar1[x][y] = ' ';
                        boardChar1[row][col] = nameOfPlayer[j];
                        boardChar2[x][y] = ' ';
                        boardChar2[row][col] = horse;
                        break;
                    }
                    else change_pos = 0;
                }
            }
        }

        if (change_pos) pos[i] += step;
    }
    else if (!can_move || !change_pos) check_acceptable = 0;
}

bool check_win()
{
    if (boardSize % 2 != 0) {
        if (boardChar1[boardSize - 1][boardSize - 1] == '*') {
            return false;
        }
        else return true;
    }
    else {
        if (boardChar1[boardSize - 1][0] == '*') {
            return false;
        }
        else return true;
    }
}

void output_not_acceptable()
{
    cout << "\nThis move is not acceptable!\n\n";
}

bool replay_game(int& first)
{
    if (!first) {
        char re;
        cout << "Do you want to play again? Y(yes) or N(no)" << endl;
        cout << "Your choice: ";
        cin >> re;
        if (re == 'Y' || re == 'y') return true;
        else if (re == 'N' || re == 'n') return false;
        else {
            bool check_syntax = 0;
            while (!check_syntax) {
                cout << "*Syntax Error!*" << endl;
                cout << "Your choice: ";
                cin >> re;
                if (re == 'Y' || re == 'y') return true;
                if (re == 'N' || re == 'n') return false;
            }
        }
    }
    else if (first == 2) return false;
    else first = 0;
}

void outcome(char nameOfPlayer[], int numberOfTies, int play_times)
{
    cout << "          ";
    for (int i = 0; i < numberOfPlayers; i++) {
        cout << nameOfPlayer[i] << "     ";
    }
    cout << endl;
    cout << "        ";
    for (int i = 0; i < numberOfPlayers; i++) {
        cout << "====  ";
    }
    cout << endl;
    cout << "Win:   ";
    for (int i = 0; i < numberOfPlayers; i++) {
        if (numberOfWins[i] < 10) {
            cout << "| 0" << numberOfWins[i] << " |";
        }
        else {
            cout << "| " << numberOfWins[i] << " |";
        }
    }
    cout << endl;
    cout << "        ";
    for (int i = 0; i < numberOfPlayers; i++) {
        cout << "====  ";
    }
    cout << endl;
    cout << "        ======";
    cout << endl << "Tie:   ";
    if (numberOfTies < 10) {
        cout << "|  0" << numberOfTies << "  |";
    }
    else {
        cout << "|  " << numberOfTies << "  |";
    }
    cout << endl;
    cout << "        ======\n";
    cout << "        ======\n";
    cout << "Total: ";
    if (play_times < 10) {
        cout << "|  0" << play_times << "  |";
    }
    else {
        cout << "|  " << play_times << "  |";
    }
    cout << endl << "        ======\n";
}

void Sound_Game_1()
{
    if (sound)
        PlaySound(TEXT("HBG_1.wav"), NULL, SND_SYNC);
}

void Sound_Game_2()
{
    if (sound)
        PlaySound(TEXT("Step_1.wav"), NULL, SND_SYNC);
}