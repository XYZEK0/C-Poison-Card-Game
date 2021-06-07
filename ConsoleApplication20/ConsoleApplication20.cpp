#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define maxCauldrons 6
#define maxCardsValue 21
#define maxStringLength 500
#define twoDigitNumber 2
#define ASCII0 48


using namespace std;

struct Game;
struct Card;
struct Player;

enum Color
{
    green,
    blue,
    red,
    violet,
    yellow,
    white,
    black,
    none
};

struct Card
{
    Color color;
    int value = 0;

    Card* next = NULL;
};

struct Game
{
    int activePlayer = 0;
    int numberOfPlayers = 0;
    int cauldrons = 0; //number of cauldrons in game 
    int greens = 0;   // number of green cards
    int gValue = 0;   // value of green cards
    int other = 0;    //number of other colors than green
    int* oValue;       //value of other cards
    int expoldeValue = 0;   //max cauldron value
    int rounds = 0;         //number of played rounds
    int deckSize;

    Player* players;
    Card* deck;
    Card** piles;

};

struct Player
{
    bool hasPointsBeenSummed = false;
    int playerPoints = 0;
    int playerNumber;
    int numberOfCardsStrictColorInDeck[maxCauldrons] = {};
    bool immuneColors[maxCauldrons] = {};
    Card* playerHand = NULL;
    Card* playerDeck = NULL;
};

Game test();
Game createGame();
Card* returnSmallestCardOnHand(Game game);
void menu();
void initalizeDeck(Game& game); //is used to create all cards existing in game
void initializeHands(Game& game); //initializes hands of all players
void addElemToList(Card*& begin, Game game, int index);
void displayInfoAboutHands(Game game);
void displayInfoAboutPlayerCards(int tab[], int amount);
void displayInfoAboutCauldrons(int tab[], int amount);
void displayInfoAboutValueOfGreenCards(int green, int greenCardsValue, bool test);
void checkForNumberCardsOfAllColors(char tab[], int numberOfCardsAllColors[]);
void displayInfoAboutCardsOfAllColors(int tab[]);
void displayColorBasedOnInt(int number);
void checkForValuesOfCardsOfAllColors(int tab[][maxCardsValue], char line[]);
void sortAnArray(int tab[][maxCardsValue]);
void displayIntArraywithout0(int tab[], int size);
void displayValueOfCardsOfAllColors(int valuesOfCardsOfAllColor[][maxCardsValue]);
void UpdateGameParameters(Game& game, int cauldrons, int greens, int greenValue, int other, int* otherValue, int cauldronTreshold, int numPlayers, int numberOfCardsInDeck, char* pileLines[maxStringLength], char* handLines[maxStringLength]);
void checkCorrectnessOfTheGame(int cauldrons[], int handCards[], int handCardsSize, int green, int noGreen, int numberOfCardsAllColors, int valuesOnCauldrons[], int explosionTreshold, bool hasDiffClrsPiles[], int numPlayers, bool wasEverythingOK);
void explodeCauldron(Game& game, int pileNumber);
void deleteList(Card**& cards, int pileNumber);
void endOfRound(Game& game);
void checkAmountOfColorsOnPlayerDeck(Game& game);
void displayEndOfRoundInfo(Game game);
void determineImmunity(Game& game);
void checkPlayerPoints(Game& game);
void checkValuesOnCauldrons(char line[], int index, int tab[]);
void copyLines(char* tab[maxStringLength], Game& game, int cauldrons, bool isForHands);
void makeStep(Game& game);
void addCardToPile(Game& game, int pileNumber);
void removeCard(Card*& ptr, Card*& begin);
void shuffleDeck(Game& game);
void checkCorrectnessOfTheGame(Game game);
void hasDifferentsColorsOnPilesOcuured(Game game, bool* tab);
void checkiFCauldronsShouldExplodeEarlier(Game game, bool tab[]);
void createDeckBasedOnPlayerHands(Game& game);
void clearBoard(Game& game);
void endOfGame(Game game);
void saveGame(Game game);
void writeStringToArrBasedOnColor(Color color, char tab[]);


bool checkIfAllArrElemsAreSame(int tab[], int size);
bool checkifAllSubArraysAreSame(int tab[][maxCardsValue]);
bool findSubString(char line[], char substring[]);
bool checkDifferenceBetweenCardsOnHands(int tab[], int size);
bool checkIfEndOfRound(Game game);
bool searchForAColorOnPile(Game game, int pileNUmber, Color color);
bool checkInfo(Game& game);

int returnIndexOfAnArrayWhenElemIs0(int tab[][maxCardsValue], int i);
int getStringLength(char tab[maxStringLength]);
int returnExplosionTreshold(char line[]);
int returnNumberOfCardsInGame(int size, int cardsInCauldrons[], int cardsInHands[]);
int returnNumberOfCardsInDeck(int greens, int noGreenColors, int numberOfCardsOtherThanGreen);
int returnPileNumber(Game& game);
int returnCauldronForExplosion(Game& game);
int checkWhichPlayerIsImmuneToColor(Color color, Game game);
int checkForValueOfGreenCards(char tab[]);
int checkForGreenCards(char tab[]);
int checkForCardsOtherThanGreen(char line[], bool wasColorFound[]);
int searchForPileWithLeastValue(Game game);

int main()
{
    menu();
    system("pause");
    return 0;
}

void menu()
{
    Game game1;

    cout << "MENU" << endl;
    cout << "------" << endl;
    cout << "0: Rozpocznij nowa gre" << endl;
    cout << "1: Wczytaj stan gry" << endl;
    cout << "Podaj instrukcje: ";
    int instr = 0;
    cin >> instr;
    cout << endl << endl;


    switch (instr)
    {
    case 0:
        cout << "Podaj parametry gry: " << endl; game1 = createGame(); saveGame(game1); initalizeDeck(game1);
        shuffleDeck(game1);
        saveGame(game1);
        initializeHands(game1);
        saveGame(game1);
        break;
    case 1:
        fseek(stdin, 0, SEEK_END);
        if (!checkInfo(game1))
        {
            game1 = createGame();
            initalizeDeck(game1);
            shuffleDeck(game1);
            initializeHands(game1);
        }
        saveGame(game1);
        break;
    default: break;
    }

    saveGame(game1);

    int roundsCounter = 0;
    while (true)
    {

        checkCorrectnessOfTheGame(game1);

        if (checkIfEndOfRound(game1))
        {
            endOfRound(game1);
            displayInfoAboutHands(game1);
            roundsCounter++;
        }
        if (roundsCounter >= 3)
        {
            endOfGame(game1);
            displayInfoAboutHands(game1);
            break;
        }

        displayInfoAboutHands(game1);
        makeStep(game1);
        displayInfoAboutHands(game1);
        int pileNum = returnCauldronForExplosion(game1);


        if (pileNum >= 0)
        {
            explodeCauldron(game1, pileNum);
            deleteList(game1.piles, pileNum);
        }
        if (game1.activePlayer + 1 == game1.numberOfPlayers)
        {
            game1.activePlayer = 0;
        }
        else game1.activePlayer++;

        
        saveGame(game1);
    }

    system("pause");
}

Game createGame()
{
    Game game;
    //number of players
    cout << "Number of players: ";
    cin >> game.numberOfPlayers;

    //number of cauldrons
    cout << "Number of cauldrons: ";
    cin >> game.cauldrons;

    //cauldron treshold
    cout << "Cauldrons treshold: ";
    cin >> game.expoldeValue;

    //number of green cards
    cout << "Number of green cards: ";
    cin >> game.greens;

    //value of green cards
    cout << "Value of green cards: ";
    cin >> game.gValue;

    //number of other cards than green
    cout << "Number of cards other than green: ";
    cin >> game.other;

    //creating and initializing all parameters of the Game
    game.oValue = new int[game.other];
    cout << "Give value of cards other than green: ";
    for (int i = 0; i < game.other; i++)
    {
        cin >> game.oValue[i];
    }
    game.deckSize = game.greens + game.cauldrons * game.other;
    game.deck = new Card[game.deckSize];
    game.players = new Player[game.numberOfPlayers];
    game.piles = new Card * [game.cauldrons];

    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        game.players[i].playerNumber = i + 1;
    }
    for (int i = 0; i < game.cauldrons; i++)
    {
        game.piles[i] = NULL;
    }

    return game;
}

void initializeHands(Game& game)
{
    int i = 0, j = 0;
    //initializing playerHands with NULL
    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        game.players[i].playerHand = NULL;
    }

    //dealing cards
    while (j < game.deckSize)
    {
        addElemToList(game.players[i].playerHand, game, j);
        if (i == (game.numberOfPlayers - 1)) i = 0; //
        else i++;
        j++;
    }
}

void addElemToList(Card*& begin, Game game, int index)
{
    //Im adding element to the end of the list

    Card* ptr; //additional pointer used to "reset" the position of the begin pointer
    Card* newEl = new Card;

    ptr = begin;

    if (begin != NULL)
    {
        while (begin->next != NULL)
        {
            begin = begin->next;
        }
        newEl->color = game.deck[index].color;
        newEl->value = game.deck[index].value;
        begin->next = newEl;
        newEl->next = NULL;

        begin = ptr;
    }
    else
    {
        newEl->color = game.deck[index].color;
        newEl->value = game.deck[index].value;
        newEl->next = NULL;
        begin = newEl;
    }

}

void addElemToList(Card*& begin, Color color, int value)
{
    //Im adding element to the end of the list

    Card* ptr = NULL; //additional pointer used to "reset" the position of the begin pointer
    Card* newEl = new Card;

    ptr = begin;

    if (begin != NULL)
    {

        while (begin->next != NULL)
        {
            begin = begin->next;
        }
        newEl->color = color;
        newEl->value = value;
        begin->next = newEl;
        newEl->next = NULL;

        begin = ptr;
    }
    else
    {
        newEl->color = color;
        newEl->value = value;
        newEl->next = NULL;
        begin = newEl;
    }
}

void initalizeDeck(Game& game)
{
    int z = 0, i = 1, j = 0;

    while (j < game.deckSize)
    {
        //initalizing all green cards
        if (j < game.greens)
        {
            game.deck[j].color = green;
            game.deck[j].value = game.gValue;
        }

        //initializing all other cards
        if ((j - game.greens) % game.other == 0 && (j - game.greens) != 0 && (j - game.greens) > 0) i++;
        if (z >= game.other) z = 0;
        if (j >= game.greens)
        {
            game.deck[j].color = (Color)i;
            game.deck[j].value = game.oValue[z];
            z++;
        }
        j++;
    }

}

void displayInfoAboutHands(Game game)
{
    Card* beginHand = NULL;
    Card* beginDeck = NULL;
    Card* beginPile = NULL;
    cout << "active player = " << game.activePlayer + 1 << endl;
    cout << "players number = " << game.numberOfPlayers << endl;
    cout << "explosion threshold = " << game.expoldeValue << endl;
    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        beginHand = game.players[i].playerHand;
        beginDeck = game.players[i].playerDeck;
        cout << game.players[i].playerNumber << " player hand cards: ";

        while (beginHand != NULL)
        {
            cout << beginHand->value << " ";

            displayColorBasedOnInt(beginHand->color);
            beginHand = beginHand->next;
        }
        cout << endl << game.players[i].playerNumber << " player deck cards: ";

        while (beginDeck != NULL)
        {
            cout << beginDeck->value << " ";

            displayColorBasedOnInt(beginDeck->color);
            beginDeck = beginDeck->next;
        }
        cout << endl;
    }

    cout << endl;
    for (int i = 0; i < game.cauldrons; i++)
    {
        beginPile = game.piles[i];
        cout << i + 1 << " pile cards: ";
        while (beginPile != NULL)
        {
            cout << beginPile->value << " ";

            displayColorBasedOnInt(beginPile->color);
            beginPile = beginPile->next;
        }
        cout << endl;
    }
}

//___________________
bool checkInfo(Game& game)
{

    FILE* file;
    file = fopen("savegame.txt", "r");
    if (file == NULL)
    {
        cout << "File ERROR!";
        return false;
    }
    else
    {
        //checking if file is empty
        int size = 0;
        fseek(file, 0, SEEK_END);
        size = ftell(file);

        if (0 == size)
        {
            cout << "File is empty you must create game manually\n";
            cout << "-------------------------------------------\n";
            return false;
        }
        fseek(file, 0, SEEK_SET);
    }


    int activePlayer = 0;
    char line[maxStringLength];

    fgets(line, sizeof(line), file);


    //getting active playerInfo
    for (int i = 0; i < getStringLength(line); i++)
    {
        if (isdigit(line[i])) activePlayer = (int)line[i] - ASCII0;
    }
    game.activePlayer = activePlayer - 1;
    fgets(line, sizeof(line), file);


    int numberOfCardsInDeck = 0;
    int explosionTreshhold = 0;
    int greenCardsValue = 0;
    int noGreen = 0; //number of colors other than green
    int green = 0; //number of green cards
    int numPlayers = 0; // number of players
    bool wasEverythingOK = true;
    bool hasDifferentGreenCardsValuesOccured = false;
    bool* hasDifferentColorsOccuredOnPiles;
    bool wasColorFound[maxCauldrons] = {};
    int valuesOfCardsOfAllColor[maxCauldrons][maxCardsValue] = {};
    int numberCardsOfColors[maxCauldrons] = {}; //number Of cards of Colors other than green
    int cauldronsCheck[maxCauldrons] = {};

    //checking second line for info about number of players
    //_____________________________________________________
    for (int i = 0; i < getStringLength(line); i++)
    {
        if (isdigit(line[i]))
        {
            numPlayers = (int)line[i] - ASCII0;
        }
    }
    fgets(line, sizeof(line), file);


    if (returnExplosionTreshold(line) != NULL)
    {
        explosionTreshhold = returnExplosionTreshold(line);
    }

    int* cardAmounts = new int[2 * numPlayers]; // 

    char** allHandLines = new char* [2 * numPlayers];
    for (int i = 0; i < 2 * numPlayers; i++)
    {
        allHandLines[i] = new char[maxStringLength];
    }
    //checking other lines about amount of cards
    //__________________________________________
    for (int i = 0; i < 2 * numPlayers; i++)
    {

        cardAmounts[i] = 0;

        fgets(line, sizeof(line), file);
        strcpy(allHandLines[i], line);
        checkForValuesOfCardsOfAllColors(valuesOfCardsOfAllColor, line);
        green += checkForGreenCards(line);
        noGreen += checkForCardsOtherThanGreen(line, wasColorFound);


        if (greenCardsValue == 0)
            greenCardsValue = checkForValueOfGreenCards(line);
        else if (greenCardsValue != checkForValueOfGreenCards(line) && checkForValueOfGreenCards(line) != 0) hasDifferentGreenCardsValuesOccured = true;

        checkForNumberCardsOfAllColors(line, numberCardsOfColors);

        for (int j = 0; j < getStringLength(line); j++)
        {
            if (isdigit(line[j]))
            {
                if (!isdigit(line[j + 1]))
                {
                    cardAmounts[i]++;
                }
            }
        }
        cardAmounts[i]--;
    }


    int* valuesOnCauldrons = new int[noGreen];
    hasDifferentColorsOccuredOnPiles = new bool[noGreen];

    char** pileLines = new char* [noGreen];
    for (int i = 0; i < noGreen; i++)
    {
        pileLines[i] = new char[maxStringLength];
    }
    //chcecking for amount of cards in cauldrons
    //__________________________________________
    for (int i = 0; i < noGreen; i++)
    {
        valuesOnCauldrons[i] = 0;

        hasDifferentColorsOccuredOnPiles[i] = false;
        fgets(line, sizeof(line), file);
        strcpy(pileLines[i], line);
        checkForValuesOfCardsOfAllColors(valuesOfCardsOfAllColor, line);
        green += checkForGreenCards(line);

        if (greenCardsValue == 0)
            greenCardsValue = checkForValueOfGreenCards(line);
        else if (greenCardsValue != checkForValueOfGreenCards(line) && checkForValueOfGreenCards(line) != 0) hasDifferentGreenCardsValuesOccured = true;

        checkForNumberCardsOfAllColors(line, numberCardsOfColors);
        checkValuesOnCauldrons(line, i, valuesOnCauldrons);

        for (int i = 0; i < maxCauldrons; i++)
            wasColorFound[i] = false;

        if (checkForCardsOtherThanGreen(line, wasColorFound) >= 2) hasDifferentColorsOccuredOnPiles[i] = true;


        for (int j = 1; j < getStringLength(line); j++)
        {
            if (isdigit(line[j]))
            {
                if (!isdigit(line[j + 1]))
                {
                    cauldronsCheck[i]++;
                }
            }
        }
    }

    cout << endl << endl;

    numberOfCardsInDeck = returnNumberOfCardsInDeck(green, noGreen, numberCardsOfColors[0]);

    //displayInfoAboutPlayerCards(cardAmounts, numPlayers);
    //displayInfoAboutCauldrons(cauldronsCheck, noGreen);
    //displayInfoAboutValueOfGreenCards(green, greenCardsValue, hasDifferentGreenCardsValuesOccured);
    //displayInfoAboutCardsOfAllColors(numberCardsOfColors);
    //displayValueOfCardsOfAllColors(valuesOfCardsOfAllColor);
    checkCorrectnessOfTheGame(cauldronsCheck, cardAmounts, 2 * numPlayers, green, noGreen, numberCardsOfColors[0], valuesOnCauldrons, explosionTreshhold, hasDifferentColorsOccuredOnPiles, numPlayers, wasEverythingOK);


    if (wasEverythingOK)
        UpdateGameParameters(game, noGreen, green, greenCardsValue, numberCardsOfColors[0], valuesOfCardsOfAllColor[0], explosionTreshhold, numPlayers, numberOfCardsInDeck, pileLines, allHandLines);

    fclose(file);
    return true;
}

void checkForValuesOfCardsOfAllColors(int tab[][maxCardsValue], char line[])
{
    char temp;
    char number[twoDigitNumber];
    int numInt = 0;
    //Im searching for a value and checking which color it represents, than writing it to an 2D array where columns represent numbers and lines represent colors
    for (int i = 0; i < getStringLength(line); i++)
    {
        if (isdigit(line[i]) && i != 0 && !isdigit(line[i - 1]))
        {
            if (isdigit(line[i + 1]))
            {
                number[0] = line[i];
                number[1] = line[i + 1];
                numInt = atoi(number);

                if (line[i + 3] == 'b')
                {
                    if (line[i + 5] == 'a') temp = 'l';
                    else temp = 'b';
                }
                else
                    temp = line[i + 3];
            }
            else if (line[i + 2] == 'b')
            {

                numInt = (int)line[i] - ASCII0;

                if (line[i + 4] == 'a') temp = 'l';
                else temp = 'b';
            }
            else
            {
                numInt = (int)line[i] - ASCII0;

                temp = line[i + 2];
            }

            switch (temp)
            {
            case 'b': tab[0][returnIndexOfAnArrayWhenElemIs0(tab, 0)] = numInt; break;
            case 'r': tab[1][returnIndexOfAnArrayWhenElemIs0(tab, 1)] = numInt; break;
            case 'v': tab[2][returnIndexOfAnArrayWhenElemIs0(tab, 2)] = numInt; break;
            case 'y': tab[3][returnIndexOfAnArrayWhenElemIs0(tab, 3)] = numInt; break;
            case 'w': tab[4][returnIndexOfAnArrayWhenElemIs0(tab, 4)] = numInt; break;
            case 'l': tab[5][returnIndexOfAnArrayWhenElemIs0(tab, 5)] = numInt; break;
            default: break;
            }

        }
    }
}

int returnIndexOfAnArrayWhenElemIs0(int tab[][maxCardsValue], int x)
{
    //function is used to check where should next element be saved
    for (int i = 0; i < maxCardsValue; i++)
    {
        if (tab[x][i] == 0) return i;
    }
    return 0;
}

void sortAnArray(int tab[][maxCardsValue])
{
    for (int k = 0; k < maxCardsValue; k++)
        for (int i = 0; i < maxCauldrons; i++)
            for (int j = 0; j < maxCardsValue; j++)
            {
                if (tab[i][j] != 0 && tab[i][j + 1] != 0 && tab[i][j] > tab[i][j + 1])
                    swap(tab[i][j], tab[i][j + 1]);
            }

}

bool checkIfAllArrElemsAreSame(int tab[], int size)
{
    int first = tab[0];
    for (int i = 0; i < size; i++)
    {
        if (tab[i] != 0)
            if (tab[i] != first) return false;
    }
    return true;
}

void checkForNumberCardsOfAllColors(char line[], int numberCardsOfColors[])
{
    char temp = ' ';
    //checking how many cards of specific color other than green are in game
    for (int i = 0; i < getStringLength(line); i++)
    {
        if (isdigit(line[i]) && i != 0 && !isdigit(line[i - 1]))
        {
            if (isdigit(line[i + 1]))
            {
                if (line[i + 3] == 'b')
                {
                    if (line[i + 5] == 'a') temp = 'l';
                    else temp = 'b';
                }
                else temp = line[i + 3];

            }
            else if (line[i + 2] == 'b')
            {
                if (line[i + 4] == 'a') temp = 'l';
                else temp = 'b';
            }
            else temp = line[i + 2];

            switch (temp)
            {
            case 'b': numberCardsOfColors[0]++; break;
            case 'r': numberCardsOfColors[1]++; break;
            case 'v': numberCardsOfColors[2]++; break;
            case 'y': numberCardsOfColors[3]++; break;
            case 'w': numberCardsOfColors[4]++; break;
            case 'l': numberCardsOfColors[5]++; break;
            }
        }
    }
}

int checkForGreenCards(char line[])
{
    //checks number of green cards
    int numberOfGreens = 0;
    for (int i = 0; i < getStringLength(line); i++)
    {
        if (line[i] == 'g')
            if (line[i + 1] == 'r')
                if (line[i + 2] == 'e')
                    numberOfGreens++;
    }
    return numberOfGreens;
}

int checkForValueOfGreenCards(char line[])
{
    int value = 0;
    for (int i = 0; i < getStringLength(line); i++)
    {
        if (isdigit(line[i]))
        {
            if (line[i + 2] == 'g')
            {
                return (int)line[i] - ASCII0;
            }
            else if (line[i + 1] == '0')
                if (line[i + 3] == 'g')
                {
                    return 10;
                }

        }
    }
    return value;
}

int checkForCardsOtherThanGreen(char line[], bool wasColorFound[])
{
    //checking number of colors other than green
    int numberOfDifferentcolors = 0;

    char blue[] = { 'b','l','u','e','\0' };
    char red[] = { 'r','e','d','\0' };
    char violet[] = { 'v','i','o','l','e','t','\0' };
    char yellow[] = { 'y','e','l','l','o','w','\0' };
    char white[] = { 'w','h','i','t','e','\0' };
    char black[] = { 'b','l','a','c','k','\0' };

    if (findSubString(line, blue) && !wasColorFound[0])
    {
        wasColorFound[0] = true;
        numberOfDifferentcolors++;
    }
    if (findSubString(line, red) && !wasColorFound[1])
    {
        wasColorFound[1] = true;
        numberOfDifferentcolors++;
    }
    if (findSubString(line, violet) && !wasColorFound[2])
    {
        wasColorFound[2] = true;
        numberOfDifferentcolors++;
    }
    if (findSubString(line, yellow) && !wasColorFound[3])
    {
        wasColorFound[3] = true;
        numberOfDifferentcolors++;
    }
    if (findSubString(line, white) && !wasColorFound[4])
    {
        wasColorFound[4] = true;
        numberOfDifferentcolors++;
    }
    if (findSubString(line, black) && !wasColorFound[5])
    {
        wasColorFound[5] = true;
        numberOfDifferentcolors++;
    }


    return numberOfDifferentcolors;

}

int returnExplosionTreshold(char line[])
{
    //checking line for explosion value and casting it to int
    char number[twoDigitNumber + 1];
    for (int i = 0; i < getStringLength(line); i++)
    {
        if (isdigit(line[i]))
        {
            if (isdigit(line[i + 1]))
            {
                if (isdigit(line[i + 2]))
                {
                    number[0] = line[i];
                    number[1] = line[i + 1];
                    number[2] = line[i + 2];
                    return atoi(number);
                }
                else
                {
                    number[0] = line[i];
                    number[1] = line[i + 1];
                    return atoi(number);
                }
            }
            else return  ((int)line[i] - ASCII0);
        }
    }
    return NULL;
}

void displayInfoAboutPlayerCards(int tab[], int amount)
{
    cout << endl << endl;
    for (int i = 0, j = 0; i < 2 * amount; i++)
    {
        if ((i + 1) % 2)
            cout << j + 1 << " player has " << tab[i] << " cards on hand" << endl;
        else
        {
            cout << j + 1 << " player has " << tab[i] << " cards in front of him" << endl;
            j++;
        }
    }
}

void displayInfoAboutCauldrons(int tab[], int amount)
{
    for (int i = 0; i < amount; i++)
    {
        cout << "there are " << tab[i] << " cards on " << i + 1 << " pile" << endl;
    }
}

void displayInfoAboutValueOfGreenCards(int green, int greenCardsValue, bool test)
{
    if (test)
    {
        cout << "Different green cards values occurred";
    }
    else if (green != 0)
    {
        cout << "Found " << green << " green cards, all with " << greenCardsValue << " value";
    }
    else cout << "Green cards does not exist";
}

void displayInfoAboutCardsOfAllColors(int tab[])
{
    bool wasDisplayed = false;
    for (int i = 0; i < maxCauldrons; i++)
    {
        int first = tab[0];
        if (tab[i] != 0)
        {
            if (!checkIfAllArrElemsAreSame(tab, maxCauldrons))
            {
                if (!wasDisplayed)
                {
                    cout << "At least two colors with a different number of cards were found: " << endl;
                    wasDisplayed = true;
                }
                displayColorBasedOnInt(i + 1);
                cout << "cards are " << tab[i] << endl;
            }
            else
            {
                cout << "The number cards of all colors is equal: " << first;
                break;
            }
        }
        else break;
    }
}

void displayColorBasedOnInt(int number)
{
    switch (number)
    {
    case 0: cout << "green" << " "; break;
    case 1: cout << "blue" << " "; break;
    case 2: cout << "red" << " "; break;
    case 3: cout << "violet" << " "; break;
    case 4: cout << "yellow" << " "; break;
    case 5: cout << "white" << " "; break;
    case 6: cout << "black" << " "; break;
    default: break;
    }
}

void displayIntArraywithout0(int tab[], int size)
{
    for (int i = 0; i < size; i++)
    {
        if (tab[i] != 0)
            cout << tab[i] << " ";
    }
}

bool checkifAllSubArraysAreSame(int tab[][maxCardsValue])
{
    for (int i = 0; i < maxCardsValue; i++)
    {
        for (int j = 0; j < maxCauldrons; j++)
        {
            if (tab[j][0] == 0)continue;
            int temp = tab[0][i];
            if (tab[j][i] != temp) return false;
        }
    }
    return true;
}

int getStringLength(char tab[maxStringLength])
{
    int counter = 0;
    while (tab[counter] != '\0')
    {
        counter++;
    }

    return counter;
}

bool findSubString(char line[], char substring[])
{
    int counter = 0;
    for (int i = 0; i < getStringLength(line); i++)
    {
        for (int j = 0; j < getStringLength(substring); j++)
        {

            if (line[i + j] == substring[j]) counter++;

        }
        if (counter == getStringLength(substring)) return true;
        else counter = 0;
    }
    return false;
}

void displayValueOfCardsOfAllColors(int valuesOfCardsOfAllColor[][maxCardsValue])
{
    sortAnArray(valuesOfCardsOfAllColor);

    if (checkifAllSubArraysAreSame(valuesOfCardsOfAllColor))
    {
        cout << "The values of cards of all colors are identical: " << endl;
        displayIntArraywithout0(valuesOfCardsOfAllColor[0], maxCardsValue);
    }
    else
    {
        cout << "The values of cards of all colors are not identical: " << endl;
        for (int i = 0; i < maxCauldrons; i++)
        {
            if (valuesOfCardsOfAllColor[i][0] != 0)
            {
                displayColorBasedOnInt(i + 1);
                cout << "cards values: ";
                displayIntArraywithout0(valuesOfCardsOfAllColor[i], maxCardsValue);
                cout << endl;
            }
        }
    }
}

void checkCorrectnessOfTheGame(Game game)
{
    bool wasEverythingOk = true;

    int* tab = new int[game.numberOfPlayers];
    bool* shouldCauldronsExplodeEarlier = new bool[game.cauldrons];
    bool* hasDiffClrOccuredOnPiles = new bool[game.cauldrons];
    for (int i = 0; i < game.cauldrons; i++)
    {
        shouldCauldronsExplodeEarlier[i] = false;
        hasDiffClrOccuredOnPiles[i] = false;
    }

    Card* begin = NULL;
    hasDifferentsColorsOnPilesOcuured(game, hasDiffClrOccuredOnPiles);
    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        tab[i] = 0;
        begin = game.players[i].playerHand;

        while (begin != NULL)
        {
            tab[i]++;
            begin = begin->next;
        }

    }

    if (!checkDifferenceBetweenCardsOnHands(tab, game.numberOfPlayers))
    {
        cout << "The number of players cards on hand is wrong" << endl;
        wasEverythingOk = false;
    }

    for (int i = 0; i < game.cauldrons; i++)
    {
        if (hasDiffClrOccuredOnPiles[i])
        {
            cout << "Two different colors were found on the " << i + 1 << " pile" << endl;
            wasEverythingOk = false;
        }
    }

    for (int i = 0; i < game.cauldrons; i++)
    {
        if (shouldCauldronsExplodeEarlier[i])
        {
            cout << "Pile number " << i + 1 << " should explode earlier" << endl;
            wasEverythingOk = false;
        }
    }

    if (wasEverythingOk) cout << "Current state of the game is ok" << endl;
}

void checkCorrectnessOfTheGame(int cauldrons[], int handCards[], int handCardsSize, int green, int noGreen, int numberOfCardsAllColors, int valuesOnCauldrons[], int explosionTreshold, bool hasDiffClrsPiles[], int numPlayers, bool wasEverythingOK)
{

    int inGameCards = returnNumberOfCardsInGame(handCardsSize, cauldrons, handCards);
    int allCards = returnNumberOfCardsInDeck(green, noGreen, numberOfCardsAllColors);


    if (!checkDifferenceBetweenCardsOnHands(handCards, handCardsSize))
    {
        wasEverythingOK = false;
        cout << "The number of players cards on hand is wrong" << endl;
    }

    for (int i = 0; i < noGreen; i++)
        if (hasDiffClrsPiles[i])
        {
            cout << "Two different colors were found on the " << i + 1 << " pile" << endl;
            wasEverythingOK = false;
        }


    for (int i = 0; i < noGreen; i++)
        if (valuesOnCauldrons[i] > explosionTreshold)
        {
            cout << "Pile number " << i + 1 << " should explode earlier" << endl;
            wasEverythingOK = false;
        }

    if (wasEverythingOK) cout << "Current state of the game is ok" << endl;

}

int returnNumberOfCardsInGame(int size, int cardsInCauldrons[], int cardsInHands[])
{
    int allCards = 0;
    for (int i = 0; i < size; i++)
    {
        allCards += cardsInHands[i];
    }
    for (int i = 0; i < maxCauldrons; i++)
    {
        allCards += cardsInCauldrons[i];
    }
    return allCards;
}

int returnNumberOfCardsInDeck(int greens, int noGreenColors, int numberOfCardsOtherThanGreen)
{
    return (greens + noGreenColors * numberOfCardsOtherThanGreen);
}

void UpdateGameParameters(Game& game, int cauldrons, int greens, int greenValue, int other, int* otherValue, int cauldronTreshold, int numPlayers, int numberOfCardsInDeck, char* pileLines[maxStringLength], char* handLines[maxStringLength])
{
    game.cauldrons = cauldrons;
    game.greens = greens;
    game.gValue = greenValue;
    game.other = other;
    game.expoldeValue = cauldronTreshold;
    game.numberOfPlayers = numPlayers;
    game.deckSize = numberOfCardsInDeck;

    game.players = new Player[game.numberOfPlayers];
    game.oValue = new int[game.other];
    game.deck = new Card[game.deckSize];
    game.piles = new Card * [game.cauldrons];

    for (int i = 0; i < game.cauldrons; i++)
    {
        game.piles[i] = NULL;
    }
    for (int i = 0; i < numPlayers; i++)
    {
        game.players[i].playerDeck = NULL;
    }
    for (int i = 0; i < game.other; i++)
    {
        game.oValue[i] = otherValue[i];
    }
    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        game.players[i].playerNumber = i + 1;
    }
    copyLines(pileLines, game, cauldrons, false);

    copyLines(handLines, game, 2 * numPlayers, true);

    createDeckBasedOnPlayerHands(game);
}

void checkValuesOnCauldrons(char line[], int index, int tab[])
{
    char number[twoDigitNumber];
    int numInt;
    for (int i = 0; i < getStringLength(line); i++)
        if (isdigit(line[i]) && i != 0 && !isdigit(line[i - 1]))
        {
            if (isdigit(line[i + 1]))
            {
                number[0] = line[i];
                number[1] = line[i + 1];
                numInt = atoi(number);
            }
            else
            {
                numInt = (int)line[i] - ASCII0;
            }

            tab[index] += numInt;
        }
}

bool checkDifferenceBetweenCardsOnHands(int tab[], int size)
{
    for (int i = 0; i < size; i += 2)
    {
        for (int j = 0; j < size; j += 2)
        {
            if ((tab[i] - tab[j]) >= 2 || (tab[i] - tab[j]) <= -2) return false;
        }
    }

    return true;
}

void copyLines(char* tab[maxStringLength], Game& game, int size, bool isForHands)
{
    int playerHandIndex = 0;
    int playerDeckIndex = 0;
    char temp;
    char number[twoDigitNumber];
    int numInt = 0;

    for (int j = 0; j < size; j++)
    {
        for (int i = 0; i < getStringLength(tab[j]); i++)
        {
            if (isdigit(tab[j][i]) && i != 0 && !isdigit(tab[j][i - 1]))
            {
                if (isdigit(tab[j][i + 1]))
                {
                    number[0] = tab[j][i];
                    number[1] = tab[j][i + 1];
                    numInt = atoi(number);

                    if (tab[j][i + 3] == 'b')
                    {
                        if (tab[j][i + 5] == 'a') temp = 'l';
                        else temp = 'b';
                    }
                    else
                        temp = tab[j][i + 3];
                }
                else if (tab[j][i + 2] == 'b')
                {

                    numInt = (int)tab[j][i] - ASCII0;

                    if (tab[j][i + 4] == 'a') temp = 'l';
                    else temp = 'b';
                }
                else
                {
                    numInt = (int)tab[j][i] - ASCII0;

                    temp = tab[j][i + 2];
                }
                if (isForHands)
                {
                    if (j % 2 == 0)
                    {
                        switch (temp)
                        {
                        case 'b': addElemToList(game.players[playerHandIndex].playerHand, blue, numInt);  break;
                        case 'r': addElemToList(game.players[playerHandIndex].playerHand, red, numInt); break;
                        case 'v': addElemToList(game.players[playerHandIndex].playerHand, violet, numInt); break;
                        case 'y': addElemToList(game.players[playerHandIndex].playerHand, yellow, numInt); break;
                        case 'w': addElemToList(game.players[playerHandIndex].playerHand, white, numInt); break;
                        case 'l': addElemToList(game.players[playerHandIndex].playerHand, black, numInt); break;
                        case 'g': addElemToList(game.players[playerHandIndex].playerHand, green, numInt); break;
                        default: break;
                        }
                    }
                    else
                    {
                        switch (temp)
                        {
                        case 'b': addElemToList(game.players[playerDeckIndex].playerDeck, blue, numInt);  break;
                        case 'r': addElemToList(game.players[playerDeckIndex].playerDeck, red, numInt); break;
                        case 'v': addElemToList(game.players[playerDeckIndex].playerDeck, violet, numInt); break;
                        case 'y': addElemToList(game.players[playerDeckIndex].playerDeck, yellow, numInt); break;
                        case 'w': addElemToList(game.players[playerDeckIndex].playerDeck, white, numInt); break;
                        case 'l': addElemToList(game.players[playerDeckIndex].playerDeck, black, numInt); break;
                        case 'g': addElemToList(game.players[playerDeckIndex].playerDeck, green, numInt); break;
                        default: break;
                        }
                    }
                }
                else
                {
                    switch (temp)
                    {
                    case 'b': addElemToList(game.piles[j], blue, numInt);  break;
                    case 'r': addElemToList(game.piles[j], red, numInt); break;
                    case 'v': addElemToList(game.piles[j], violet, numInt); break;
                    case 'y': addElemToList(game.piles[j], yellow, numInt); break;
                    case 'w': addElemToList(game.piles[j], white, numInt); break;
                    case 'l': addElemToList(game.piles[j], black, numInt); break;
                    case 'g': addElemToList(game.piles[j], green, numInt); break;
                    default: break;
                    }
                }
            }
        }
        if (j % 2 == 0) playerHandIndex++;
        else playerDeckIndex++;
    }

}

void makeStep(Game& game)
{
    Card* card = returnSmallestCardOnHand(game);
    Card* begin = game.players[game.activePlayer].playerHand;
    if (game.players[game.activePlayer].playerHand != NULL)
    {
        addCardToPile(game, returnPileNumber(game));
        removeCard(card, begin);
        game.players[game.activePlayer].playerHand = begin;
    }
    else return;
}

void removeCard(Card*& ptr, Card*& begin)
{
    Card* temp2 = NULL;
    Card* temp = NULL;

    temp = ptr;
    //ostatni element
    if (temp->next == NULL)
    {
        temp2 = ptr;
        temp = begin;

        if (ptr == begin)
        {
            delete begin;
            begin = NULL;
            return;
        }
        while (temp->next != ptr)
        {
            temp = temp->next;
        }
        temp->next = NULL;

        delete temp2;

    }
    //pierwszy element
    else if (temp == begin)
    {
        ptr = ptr->next;
        begin = ptr;
        temp->next = NULL;
        delete temp;
    }
    //srodkowy element
    else
    {
        temp = begin;
        temp2 = ptr;
        while (temp->next != ptr)
        {
            temp = temp->next;
        }
        temp->next = temp2->next;
        temp2->next = NULL;

        delete temp2;
    }

}

int returnCauldronForExplosion(Game& game)
{
    int summary = 0;

    Card* begin = NULL;
    for (int i = 0; i < game.cauldrons; i++)
    {
        begin = game.piles[i];
        while (begin != NULL)
        {
            summary += begin->value;
            if (summary >= game.expoldeValue) return i;
            begin = begin->next;
        }
        summary = 0;
    }
    return -1;
}

void explodeCauldron(Game& game, int pileNumber)
{
    Card* begin = game.piles[pileNumber];
    while (begin != NULL)
    {
        addElemToList(game.players[game.activePlayer].playerDeck, begin->color, begin->value);
        begin = begin->next;
    }
}

void deleteList(Card**& cards, int pileNumber)
{
    Card* begin = NULL;
    while (cards[pileNumber] != NULL)
    {
        begin = cards[pileNumber]->next;
        delete cards[pileNumber];
        cards[pileNumber] = begin;
    }
}

void deleteList(Card*& first)
{
    Card* begin = NULL;
    while (first != NULL)
    {
        begin = first->next;
        delete first;
        first = begin;
    }
}

void endOfRound(Game& game)
{
    checkAmountOfColorsOnPlayerDeck(game);
    determineImmunity(game);
    checkPlayerPoints(game);
    clearBoard(game);
    initializeHands(game);
}

int returnHighestIndex(int tab[], int size)
{
    int biggest = 0;
    for (int i = 0; i < size; i++)
    {
        if (tab[i] > tab[biggest]) biggest = i;
    }
    return biggest;
}

void displayEndOfRoundInfo(Game game)
{
    for (int i = 0; i < game.cauldrons + 1; i++)
    {
        if (checkWhichPlayerIsImmuneToColor((Color)i, game) > -1)
        {
            cout << "Na kolor "; displayColorBasedOnInt(i); cout << "odporny jest gracz ";  cout << checkWhichPlayerIsImmuneToColor((Color)i, game) + 1;
            cout << endl;
        }
    }

    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        cout << "Wynik gracza " << i + 1 << " = " << game.players[i].playerPoints << endl;
    }

    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        for (int j = 0; j < game.cauldrons; j++)
        {
            game.players[i].immuneColors[j] = false;
        }
    }
    system("pause");
}

void checkAmountOfColorsOnPlayerDeck(Game& game)
{
    Card* begin = NULL;
    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        begin = game.players[i].playerDeck;
        while (begin != NULL)
        {
            switch (begin->color)
            {
            case blue: game.players[i].numberOfCardsStrictColorInDeck[0]++; break;
            case red: game.players[i].numberOfCardsStrictColorInDeck[1]++; break;
            case violet: game.players[i].numberOfCardsStrictColorInDeck[2]++; break;
            case yellow: game.players[i].numberOfCardsStrictColorInDeck[3]++; break;
            case white: game.players[i].numberOfCardsStrictColorInDeck[4]++; break;
            case black: game.players[i].numberOfCardsStrictColorInDeck[5]++; break;
            default: break;
            }
            begin = begin->next;
        }
    }

}

void determineImmunity(Game& game)
{
    bool duplicate = false;
    for (int i = 0; i < game.cauldrons; i++)
    {
        int max = 0;

        for (int j = 1, k = 0; j < game.numberOfPlayers; j++, k++)
        {
            if (game.players[j].numberOfCardsStrictColorInDeck[i] > game.players[max].numberOfCardsStrictColorInDeck[i]) max = j;

            if (game.players[max].numberOfCardsStrictColorInDeck[i] == game.players[k].numberOfCardsStrictColorInDeck[i] && k != max) duplicate = true;
        }

        if (!duplicate)
            game.players[max].immuneColors[i] = true;
        duplicate = false;

    }
}

void checkPlayerPoints(Game& game)
{
    Card* begin;

    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        begin = game.players[i].playerDeck;
        while (begin != NULL)
        {
            if (begin->color == green) game.players[i].playerPoints += 2;
            else if (!game.players[i].immuneColors[begin->color - 1]) game.players[i].playerPoints++;
            begin = begin->next;
        }
    }

    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        for (int j = 0; j < game.cauldrons; j++)
        {
            game.players[i].numberOfCardsStrictColorInDeck[j] = 0;
        }
    }

    displayEndOfRoundInfo(game);
}

int checkWhichPlayerIsImmuneToColor(Color color, Game game)
{
    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        if (game.players[i].immuneColors[color - 1]) return i;
    }
    return -1;
}

void shuffleDeck(Game& game)
{
    srand(time(NULL));
    for (int i = 0; i < game.deckSize; i++)
    {
        int x = rand() % (game.deckSize - 1);
        int y = rand() % (game.deckSize - 1);

        Card temp = game.deck[x];
        game.deck[x] = game.deck[y];
        game.deck[y] = temp;
    }
}

Game test()
{
    srand(time(NULL));
    Game game;
    //number of players

    game.numberOfPlayers = 20;

    //number of cauldrons

    game.cauldrons = rand() % 3 + 3;

    //cauldron treshold

    game.expoldeValue = rand() % 30 + 10;

    //number of green cards

    game.greens = rand() % 4 + 3;

    //value of green cards

    game.gValue = rand() % 3 + 4;

    //number of other cards than green

    game.other = rand() % 10 + 6;


    game.oValue = new int[game.other];

    for (int i = 0; i < game.other; i++)
    {
        game.oValue[i] = rand() % 10 + 2;
    }
    game.deckSize = game.greens + game.cauldrons * game.other;
    game.deck = new Card[game.deckSize];
    game.players = new Player[game.numberOfPlayers];
    game.piles = new Card * [game.cauldrons];

    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        game.players[i].playerNumber = i + 1;
        game.players[i].playerHand = NULL;
    }
    for (int i = 0; i < game.cauldrons; i++)
    {
        game.piles[i] = NULL;
    }

    return game;
}

bool checkIfEndOfRound(Game game)
{
    int counter = 0;
    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        if (game.players[i].playerHand == NULL) counter++;
    }
    if (counter == game.numberOfPlayers) return true;

    return false;
}

void hasDifferentsColorsOnPilesOcuured(Game game, bool* tab)
{
    Color prevColor = none;
    Card* begin = NULL;
    for (int i = 0; i < game.cauldrons; i++)
    {
        prevColor = none;
        begin = game.piles[i];
        while (begin != NULL)
        {
            if (begin->color != green)
            {
                if (prevColor == none) prevColor = begin->color;
                else if (prevColor != begin->color)
                {
                    tab[i] = true;
                    break;
                }
            }
            begin = begin->next;
        }
    }
}

void checkiFCauldronsShouldExplodeEarlier(Game game, bool tab[])
{
    Card* begin = NULL;
    for (int i = 0; i < game.cauldrons; i++)
    {
        int sum = 0;
        begin = game.piles[i];
        while (begin != NULL)
        {
            sum += begin->value;
            begin = begin->next;
        }
        if (sum >= game.expoldeValue) tab[i] = true;
    }

}

int returnPileNumber(Game& game)
{
    Card* smallest = returnSmallestCardOnHand(game);


    if (smallest->color == green)
    {
        return searchForPileWithLeastValue(game);
    }
    for (int i = 0; i < game.cauldrons; i++)
    {
        if (searchForAColorOnPile(game, i, smallest->color)) return i;
    }
    for (int i = 0; i < game.cauldrons; i++)
    {
        if (game.piles[i] == NULL) return i;
    }
    return searchForPileWithLeastValue(game);
}

int returnPileValue(Game game, int index)
{
    if (game.piles[index] == NULL) return 0;

    int sum = 0;
    Card* begin = NULL;
    begin = game.piles[index];
    while (begin != NULL)
    {
        sum += begin->value;
        begin = begin->next;
    }

    return sum;
}

Card* returnSmallestCardOnHand(Game game)
{
    Card* begin = game.players[game.activePlayer].playerHand;
    Card* smallest = NULL;
    smallest = game.players[game.activePlayer].playerHand;
    while (begin != NULL)
    {
        if (begin->value < smallest->value) smallest = begin;
        begin = begin->next;
    }

    return smallest;
}

bool searchForAColorOnPile(Game game, int pileNUmber, Color color)
{
    Card* begin = NULL;
    int counter = 0;
    int numOfgreens = 0;
    begin = game.piles[pileNUmber];
    while (begin != NULL)
    {
        if (begin->color == color) return true;
        else if (begin->color == green) numOfgreens++;
        begin = begin->next;
    }
    begin = game.piles[pileNUmber];
    while (begin != NULL)
    {
        counter++;
        begin = begin->next;
    }

    if (numOfgreens == counter && numOfgreens != 0) return true;

    return false;
}

int searchForPileWithLeastValue(Game game)
{
    int smallestValue = returnPileValue(game, 0);
    int smallestIndex = 0;
    for (int i = 0; i < game.cauldrons; i++)
    {
        if (game.piles[i] == NULL) return i;
        if (returnPileValue(game, i) < smallestValue)
        {
            smallestValue = returnPileValue(game, i);
            smallestIndex = i;
        }
    }

    return smallestIndex;
}

void addCardToPile(Game& game, int pileNumber)
{
    Card* card = game.piles[pileNumber];

    if (game.players[game.activePlayer].playerHand != NULL)
        addElemToList(game.piles[pileNumber], returnSmallestCardOnHand(game)->color, returnSmallestCardOnHand(game)->value);

}

void createDeckBasedOnPlayerHands(Game& game)
{
    int d = 0;
    int size = 0;
    Card* begin = NULL;
    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        begin = game.players[i].playerHand;
        while (begin != NULL)
        {
            game.deck[d] = *begin;
            d++;
            begin = begin->next;
        }
        begin = game.players[i].playerDeck;
        while (begin != NULL)
        {
            game.deck[d] = *begin;
            d++;
            begin = begin->next;
        }
    }

    for (int i = 0; i < game.cauldrons; i++)
    {
        begin = game.piles[i];
        while (begin != NULL)
        {
            game.deck[d] = *begin;
            d++;
            begin = begin->next;
        }
    }

}

void clearBoard(Game& game)
{
    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        deleteList(game.players[i].playerHand);
        deleteList(game.players[i].playerDeck);
    }
    for (int i = 0; i < game.cauldrons; i++)
    {
        deleteList(game.piles[i]);
    }
}

void endOfGame(Game game)
{
    cout << endl << endl << "---------------------------------------------" << endl;
    cout << "Game has ended" << endl;

    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        cout << "Wynik gracza " << i + 1 << " = " << game.players[i].playerPoints << endl;
    }

}

void saveGame(Game game)
{
    FILE* file;

    //arr used to write string to the file
    char input[2 * maxStringLength];

    file = fopen("savegame.txt", "w");

    if (file == NULL)
    {
        cout << "File Error!";
        return;
    }

    snprintf(input, sizeof(input), "active player = %d\n", game.activePlayer);
    fputs(input, file);
    snprintf(input, sizeof(input), "players number = %d\n", game.numberOfPlayers);
    fputs(input, file);
    snprintf(input, sizeof(input), "explosion threshold = %d\n", game.expoldeValue);
    fputs(input, file);

    Card* beginHand = NULL;
    Card* beginDeck = NULL;
    Card* beginPile = NULL;

    //saving playerHand and Deck info
    for (int i = 0; i < game.numberOfPlayers; i++)
    {
        beginHand = game.players[i].playerHand;
        beginDeck = game.players[i].playerDeck;
        snprintf(input, sizeof(input), "%d player hand cards:", i + 1);

        while (beginHand != NULL)
        {
            char tempCol[20];
            char tempVal[10];

            writeStringToArrBasedOnColor(beginHand->color, tempCol);
            snprintf(tempVal, sizeof(tempVal), " %d ", beginHand->value);

            strcat(input, tempVal);
            strcat(input, tempCol);

            beginHand = beginHand->next;
        }

        fputs(input, file);
        fputs("\n", file);

        snprintf(input, sizeof(input), "%d player deck cards:", i + 1);
        while (beginDeck != NULL)
        {
            char tempCol[20];
            char tempVal[10];

            writeStringToArrBasedOnColor(beginDeck->color, tempCol);
            snprintf(tempVal, sizeof(tempVal), " %d ", beginDeck->value);

            strcat(input, tempVal);
            strcat(input, tempCol);
            beginDeck = beginDeck->next;
        }
        fputs(input, file);
        fputs("\n", file);
    }


    //saving cauldronsInfo
    for (int i = 0; i < game.cauldrons; i++)
    {
        beginPile = game.piles[i];
        snprintf(input, sizeof(input), "%d pile cards:", i + 1);
        while (beginPile != NULL)
        {
            char tempCol[20];
            char tempVal[10];

            writeStringToArrBasedOnColor(beginPile->color, tempCol);
            snprintf(tempVal, sizeof(tempVal), " %d ", beginPile->value);

            strcat(input, tempVal);
            strcat(input, tempCol);

            beginPile = beginPile->next;
        }
        fputs(input, file);
        fputs("\n", file);
    }

    fclose(file);
}

void writeStringToArrBasedOnColor(Color color, char tab[])
{

    switch (color)
    {
    case green: strcpy(tab, "green"); break;
    case blue: strcpy(tab, "blue"); break;
    case red: strcpy(tab, "red"); break;
    case violet: strcpy(tab, "violet"); break;
    case yellow: strcpy(tab, "yellow"); break;
    case white: strcpy(tab, "white"); break;
    case black: strcpy(tab, "black"); break;
    default: break;
    }
}