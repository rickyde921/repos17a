/* 
 * File:   main.cpp
 * Author: Ricardo De Leon
 * Purpose: Create a simulation of Texas hold em Poker, along with all the 
 * components real Texas hold em contains
 *
 * Created on October 21, 2015, 12:41 PM
 */

#include <cstdlib>
#include <iostream>
#include <ctime>
#include <iomanip>
#include <cstring>

using namespace std;

int const POSCARDS = 7; //size of array used by structure Player named sevenC

//Im not sure if im allowed to use these here but they are very handy
//these global constants are used to determined what games players have by assigning
//values to possible games, in a similar fashion enumerated numbers would have done
const int SINGLE = 0;
const int ONEPAIR = 3;
const int TWOPAIRS = 6;
const int TRIPPLE = 9;
const int STRAIGHT = 10;
const int FLUSH = 11;
const int FULLHOUSE = 12;
const int POKER = 13;
const int STRAIGHTFLUSH = 21;

struct Player {
    int card1; //holds value of first card
    int card2; //holds value of second card
    bool comp; //Holds the computer's name
    string name;
    int chips; //player chips used to gamble
    int sevenC [POSCARDS]; //all 7 possible cards a player can use to obtain a game
    int totBet; //total betting amount within a game
    int needed; //amount needed to call
    int currBet; //current betting round bet
    string status;
    int game;
};

/*
 * 
 */
void initPlyer(Player &, Player &, Player &, Player &, Player &); //Initializes computer stats

void chosePlayers(int, Player [], int, Player []);

int choseOrder(Player[], int, int[], int, int); //Decides order of game at start

void initDeck(int [], int const);

string faceValue(int); //returns face value of cards

string suit(int); //returns suit of cards

void swap(int[], int, int); //Swaps two positions within an array

int draw(int[], int &); //draws a card from the deck

int findHighest(Player[], int); //Looks for highest card for deciding who is first   

int gameValue(int); //returns a card's game value using deck index

int firstEnd(int, int); //determines new location for ending position in betting phase

void clearScreen(); //clears screen by outputting 50 endl

void deal(Player [], int, int [], int &, int); //deals 2 cards too every player

void initSevenC(Player &p); //initializes 7 possible cards a player can chose from to -10

void addToSeven(Player &, int); //Adds a card to SevenC array

int converter(string); //converts string to number

void newEnd(int, int &, int); //finds a new ending point after someone raises

//makes a player decide if they chose to call/check, fold, or raise
void decide(Player [], int &, int &, int [], int &, int &, int, int [], bool);

void drawField(int, Player [], int, int [], int [], int &); //Draws cards to filed according to rules

void spaces(int); //will output num amount of consecutive spaces

void outoutScreen(Player [], int, int, int[], int[], int, bool);    //Outputs entire poker table

void sort(Player[],int, int);        //sorts values in sevenC for a player

int findMin(Player[], int , int);   //finds smallest card within sevenC array

void swap(int, int, Player[], int );    //swaps two locations in sevenC array

void checkFlush(Player [], int);  //checks if player has flush or straight flush

void checkStraight(Player [], int); //checks for possible straight

void checkPairs(Player[],int);    //checks for four of a kind, triplets, and pairs

void whatGame(Player [], int);    //determines what game and assigns it to players

void op (Player allPlayers[]);

void reInitPlyer(Player &, Player &, Player &, Player &, Player &); //reinitializes all players

int main(int argc, char** argv) {

    srand(time(0));

    bool valid = false;
    string rules; //used to decide if program will output rules or play game
    while (valid == false) {
        valid = true;
        cout << "Would you like to hear the rules or are you ready to play?" << endl
                << "1)Hear rules" << endl << "2)Play Game" << endl;
        cin>>rules;
        if(rules.length()!=1){    //if input is longer than one char
            valid = false;
            cout << "*****INVALID RESPONSE*****" << endl
                    << "Please enter 1 or 2 only" << endl;
        }
        else if (rules[0] < '1' || rules[0] > '2'){     //if number not 1 or two
            valid = false;
            cout << "*****INVALID RESPONSE*****" << endl
                    << "Please enter 1 or 2 only" << endl;
        }
        else {  //if valid input
            if(rules[0]=='1'){
                cout<<"Hello and welcome to Texas Hold em!!!"<<endl
                    <<"The rules are as follows. Each individual player will be dealt "
                    <<"2 cards. The game will consist of 4 betting phases. "<<endl
                    <<"The first is right after recieving your 2 cards. After that phase "
                    <<"is over, 3 cards will be dealt on the table. This will initialte "<<endl
                    <<"the second betting phase. After that, a 4th card will be dealt onto the "
                    <<"table, followed by a 3rd betting phase. Finally, a 5th and final card will"<<endl
                    <<"once again be dealth to the table, followed by the 4th and final betting phase. "
                    <<"Once all 5 cards are out, each player may chose from the total 7, 5 cards to "<<endl
                    <<"construct a hand. The winner will be whoever has the better game. The ranking goes as follows"<<endl
                    <<"Straight Flush = 5 consecutive cards of the same suit"<<endl
                    <<"Poker = 4 of the same cards (face value)"<<endl
                    <<"Full House = a triplet(3 of a kind) and a pair"<<endl
                    <<"Flush = 5 cards of the same suit"<<endl
                    <<"Straight = 5 consecutive cards in ascending order"<<endl
                    <<"Triplet = 3 of the same card"<<endl
                    <<"Two Pairs = 2 pairs"<<endl
                    <<"One Pair = 1 pair"<<endl
                    <<"No Game = 5 random cards with no pattern as mentioned above"<<endl<<endl
                    <<"/******NOTED BUGS******/"<<endl
                    <<"Currently, the game is not 100% finished and so it suffers from a few bugs"<<endl
                    <<"1) Currently, going All In will result in the player folding "<<endl
                    <<"2) The user is only allowed to play one round of poker before the program terminates"
                    <<"due to some issues I encountered with variables not being reinitialized"<<endl
                    <<"3) The program no longer outputs user's ending chips, as I ran into an issue where it"
                    <<" was giving the user a random number of chips after calculating winner"<<endl
                    <<"4) if two or more players both manage to achieve the same winning game, the pot is "
                    <<" split into all "<<endl<<"winners, as opposed to traditional poker, in which the highest cards are compared"<<endl
                    <<"4eg) If two players both won with a flush, players would compare higest card until one player"
                    <<" won or all 5 cards were the same resulting in a TRUE tie"<<endl
                    <<"5) After a computer folds, it will still display them as a winner if "
                    <<" they would have had a "<<endl<<"winning game"<<endl
                    <<"6) When playing with only one computer player, the user's status is not displayed"<<endl
                    <<"***********************"<<endl
                    <<"*NOTE* All these bugs are fairly easy to fix and would have been dealt with had I spent more time on this project"<<endl
                    <<"And not focused so much of my time on displaying screen and the logic of what happens when a player "<<endl
                    <<"raises changing the ending position while another player could raise again ect."<<endl
                    <<"My biggest regret was not being able to code the cimputer AI which would have determined what to"<<endl
                    <<"do based on an algorithm dependent on player's current bet, current pot, raised amount, "<<endl
                    <<" along with player stats such as likelihood to bluff and possible future game"<<endl;
                   
                    
            }
            else {
                cout<<"Okay, let us begin"<<endl;
            }
        }
    }
    //Creates empty Players
    Player james;
    Player cynthia;
    Player ricky;
    Player stacy;
    Player user;

    //Initialize Computer Player Statistics
    initPlyer(james, cynthia, ricky, stacy, user);
    string numCom; //Number of computer players desired
    int const totComp = 4; //Represents total number of possible computers

    do {
        valid = true; //Will be used for input validation
        cout << "How many computer players would you like to add?" << endl
                << "Enter a number between 1 and " << totComp << endl;
        ;
        cin>>numCom;
        if (numCom.size() != 1)valid = false; //If input was larger than 1 character
        else if (numCom[0] <= '0' || numCom[0] > '4')valid = false; //Out of given range
    } while (valid == false);

    int compPlay = (numCom[0] - 48);
    cout << compPlay << endl; //Integer equal to # of computers playing
    Player *allPlayers = new Player[compPlay + 1]; //Creates an array of size equal to # of computer 
    //players that will play + the user
    //Creates an array of total computers to ease passing as parameter
    Player totalC[totComp] = {james, cynthia, ricky, stacy};
    chosePlayers(totComp, totalC, compPlay, allPlayers); //Allow the user to decide who will play

    user.name = "You"; //sets user name
    *(allPlayers + compPlay) = user; //sets final value in array to user


    cout << "These are the players that will be playing " << endl;
    for (int i = -0; i < compPlay + 1; i++) { //Displays all players that will be playing
        cout << allPlayers[i].name << endl;
    }

    //Initialize deck
    int const deckSz = 52;
    int deck [deckSz]; //Creates a static array to hold 2 decks
    int capacity = deckSz; //Will be used to keep used and unused cared apart
    initDeck(deck, deckSz);

    int aPSize = compPlay + 1; //equal to size of allPlayers array
    int endPos = choseOrder(allPlayers, aPSize, deck, deckSz, capacity); //current position of player who
    int orgEnd = endPos; //will hold oriinal end position

    int start; //starting position of betting phase
    if (endPos == aPSize - 1)start = 0; //if end player is final element, start is first element
    else start = endPos + 1; //else, start player is element after end player

    cout << "This will be the initial order of the game" << endl;
    int listPs = start;
    for (int i = 0; i < aPSize; i++) {
        if (listPs == aPSize) {
            listPs = 0; //if next element is out of bonds, return to start of array
            cout << allPlayers[listPs].name << endl;
            listPs++; //else increment list position
        } else {
            cout << allPlayers[listPs].name << endl;
            listPs++; //else increment list position
        }
    }
    cout << "Are you ready to begin?" << endl;
    string ready;

    do {
        cout << "Enter 1 to begin game" << endl;
        //Initialize player chips

        valid = true; //will be used for input verification
        cin>>ready;
        if (ready.size() != 1)valid = false;
        else if (ready[0] != '1')valid = false;
    } while (valid == false);

    clearScreen(); //clear screen
    //Deal cards to all players
    deal(allPlayers, aPSize, deck, capacity, start);
    cout << "Start Position after deal is " << start << endl;
    //Initialize pot
    int pot [aPSize - 1];
    int potLoc = 0; //Location of current pot
    int sBlind = 1;
    int bBlind = 2;
    int currBet = bBlind; //current bet amount
    int currPos = start; //current position
    bool finished = false; //if last player finished betting
    int compare; //Will be used to check if bet was raised
    int bRound = 0;
    bool gameEnd = false;
    int openCrd [5] = {-10, -10, -10, -10, -10}; //creates array for cards on field
    bool gameDone = false;

    //do{
        for (int i = bRound; i < 4; i++) { //will loop 4 times for 4 betting rounds
            //opens cards for the field according to the rules
            drawField(bRound, allPlayers, aPSize, openCrd, deck, capacity);

            //execute while betting phase is not yet over
            finished = false;
            do {
                if (currPos == endPos) { //if this is final betting player
                    finished = true;
                    compare = currBet;
                }
                decide(allPlayers, currPos, currBet, pot, potLoc, endPos, aPSize, openCrd, gameEnd);
                if (compare < currBet) {
                    finished = false; //if bet was raised
                    compare = currBet;
                }
            } while (finished == false);

            endPos = orgEnd; //resets end position to original end position
            if (endPos == aPSize - 1)currPos = 0; //if end player is final element, start is first element
            else currPos = endPos + 1; //else, start player is element after end player
            bRound++; //increment betting round
        }

        //ending display
        gameEnd = true;
        outoutScreen(allPlayers, aPSize, currPos, openCrd, pot, potLoc, gameEnd);

        whatGame(allPlayers, aPSize);


//        cout<<"Game of "<<allPlayers[0].name<<" is "<<allPlayers[0].game<<endl;
//        cout<<"Game of "<<allPlayers[1].name<<" is "<<allPlayers[1].game<<endl;
//        cout<<"Game of "<<allPlayers[2].name<<" is "<<allPlayers[2].game<<endl;
//        cout<<"Game of "<<allPlayers[3].name<<" is "<<allPlayers[3].game<<endl;
//        cout<<"Game of "<<allPlayers[4].name<<" is "<<allPlayers[4].game<<endl;

     //declare winner
        int winGame = 0;
        for(int i = 0; i<aPSize; i++){
            //finds best game
            if(allPlayers[i].game > winGame)winGame = allPlayers[i].game;
        }
        int numWin=0;
        int winPos;
        for(int i = 0; i<aPSize; i++){
            if(allPlayers[i].game ==winGame){
                numWin++;
                winPos = i;
            }
        }
        if(numWin == 1){    //if there is only one winner
            cout<<"Congrats "<<allPlayers[winPos].name<<" for winning with a ";
            if(allPlayers[winPos].game == 21)cout<<"Full House"<<endl;
            else if(allPlayers[winPos].game == 13)cout<<"Poker"<<endl;
            else if(allPlayers[winPos].game == 12)cout<<"Flush"<<endl;
            else if(allPlayers[winPos].game == 11)cout<<"Straight"<<endl;
            else if(allPlayers[winPos].game == 9)cout<<"Triplet"<<endl;
            else if(allPlayers[winPos].game == 6)cout<<"Two Pairs"<<endl;
            else if(allPlayers[winPos].game == 3)cout<<"One Pair"<<endl;
            else //will not get executed
            allPlayers[winPos].chips += pot[0];
        }
        else{              //if there is more than one winner
            pot[potLoc] /= numWin;   //divide pot evenly
            cout<<"Congrats "<<endl;
            for (int i = 0; i<aPSize; i++){
                if(allPlayers[i].game == winGame){  //if player is a winner
                    cout<<allPlayers[i].name<<endl;
                    allPlayers[i].chips += pot[potLoc];
                }
                
            }
            if(allPlayers[winPos].game == 21)cout<<"Full House"<<endl;
            else if(allPlayers[winPos].game == 13)cout<<"Poker"<<endl;
            else if(allPlayers[winPos].game == 12)cout<<"Flush"<<endl;
            else if(allPlayers[winPos].game == 11)cout<<"Straight"<<endl;
            else if(allPlayers[winPos].game == 9)cout<<"Triplet"<<endl;
            else if(allPlayers[winPos].game == 6)cout<<"Two Pairs"<<endl;
            else if(allPlayers[winPos].game == 3)cout<<"One Pair"<<endl;
            else {} //will not get executed
        }

     //reinitialize all variables

        pot [0] = 0;
        potLoc = 0; //Location of current pot
        currBet = bBlind; //current bet amount
        currPos = currPos+1; //current position
        finished = false; //if last player finished betting
        compare = 0; //Will be used to check if bet was raised
        bRound = 0;
        gameEnd = false;
        for (int i = 0; i<5; i++){
            openCrd[i] = -10;
        }
        capacity = deckSz;
        reInitPlyer(james, cynthia, ricky, stacy, user);
        
        
        bool play = true;
        string playAgain;
/*Currently needs work*/
//        do{
//            play = true;
//             cout<<allPlayers[aPSize-1].name<<" You now have "<<allPlayers[aPSize-1].chips<<" chips"<<endl
//            <<"What would you like to do?"<<endl
//            <<"1) Play again"<<endl<<"2) Leave"<<endl;
//        cin>>playAgain;
//        if(playAgain.length() !=1){
//            cout<<"Invalid input"<<endl;
//            play = false;
//        }
//        else if (playAgain[0] < '1'|| playAgain[0]>'2'){
//            cout<<"Invalid input"<<endl;
//            play = false;
//        }
//        else {
//            cout<<"Thank you"<<endl;
//        }
//
//        }while(play == false);
//
//       if(playAgain == "2"||allPlayers[aPSize-1].chips < 2){
//           gameDone = true;
//       }         
    //}while(gameDone == false);
    cout<<"Goodbye and thank you for playing Texas hold em"<<endl;
     delete[] allPlayers;
    return 0;
}

/*******************************************************************************/



void initPlyer(Player &james, Player &cynthia, Player &ricky, Player &stacy, Player &user) {
    james.comp = true;
    james.name = "James";
    james.card1 = -1;
    james.card2 = -1;
    james.chips = 100;
    initSevenC(james);
    james.totBet = 0;
    james.needed = 0;
    james.currBet = 0;
    james.game = 0;

    cynthia.comp = true;
    cynthia.name = "Cynthia";
    cynthia.card1 = -1;
    cynthia.card2 = -1;
    cynthia.chips = 100;
    initSevenC(cynthia);
    cynthia.totBet = 0;
    cynthia.needed = 0;
    cynthia.currBet = 0;
    cynthia.game = 0;

    ricky.comp = true;
    ricky.card1 = -1;
    ricky.card2 = -1;
    ricky.name = "Ricky";
    ricky.chips = 100;
    initSevenC(ricky);
    ricky.totBet = 0;
    ricky.needed = 0;
    ricky.currBet = 0;
    ricky.game = 0;

    stacy.comp = true;
    stacy.name = "Stacy";
    stacy.card1 = -1;
    stacy.card2 = -1;
    stacy.chips = 100;
    initSevenC(stacy);
    stacy.totBet = 0;
    stacy.needed = 0;
    stacy.currBet = 0;
    stacy.game = 0;

    user.comp = false;
    user.card1 = -1;
    user.card2 = -1;
    user.chips = 100;
    initSevenC(user);
    user.totBet = 0;
    user.needed = 0;
    user.currBet = 0;
    user.game = 0;
}

void chosePlayers(int totComp, Player totalC[], int compPlay, Player allPlayers[]) {
    for (int i = 0; i < compPlay; i++) { //Will loop until you have chosen as many players as compPlay
        cout << "Who would you like to add to the game? (" << compPlay - i << ")remaining" << endl;
        for (int j = 0; j < totComp - i; j++) {//Will loop so all computers left are displayed
            cout << j + 1 << ") " << totalC[j].name << endl;
        }
        string chose; //variable used to chose the computer that will be added to game
        bool valid; //Will be used for input validation
        do {
            valid = true;
            cout << "Please chose a computer player" << endl;
            cin>>chose;
            int choice = chose[0] - 48;
            if (chose.length() != 1)valid = false; //if input is larger than 1 char
            if (chose[0] <= '0' || chose[0] - 48 > totComp - i)valid = false; //out of range
            //             cout<<"max "<<totComp-i<<endl;
            //             cout<<"My choice "<<chose[0]<<endl;
        } while (valid == false);

        allPlayers[i] = totalC[chose[0] - 48 - 1]; //Adds chosen computer to allPlayers array
        totalC[(chose[0] - 48) - 1] = totalC[totComp - 1 - i]; //Replaces chosen name for last element 


    }
    clearScreen();
}

int choseOrder(Player allPlayers[], int size, int deck[], int dSize, int c) {
    cout << "Every body will draw a card. Whoever draws the biggest card will "
            << " obtain the blind last. " << endl
            << "If twp players draw a card of the same value, suits will" << endl
            << "be prioritized by the following order; Spades, Hearts, Clubs, Diamonds" << endl;
    int capacity = 52;
    for (int i = 0; i < size; i++) {
        if (allPlayers[i].card1 == -1) {
            allPlayers[i].card1 = draw(deck, capacity);

        }
    }
    int highest = findHighest(allPlayers, size);

    cout << "Cards Drawn " << endl << "-----------" << endl;
    for (int i = 0; i < size; i++) {
        cout << allPlayers[i].name << ")" << faceValue(allPlayers[i].card1)
                << " " << suit(allPlayers[i].card1) << endl; //Outputs players and cards
    }
    cout << endl;
    int endPos = -1; //Position of ending player in a betting phase
    int winners = 0; //number of winners

    for (int i = 0; i < size; i++) { //checks how many players have the highest card
        if (gameValue(allPlayers[i].card1) == highest) {
            winners++; //a winner found
            endPos = firstEnd(size, i);

        }

    }

    if (winners > 1) { //if more than one winner was found
        int realWin = 53; //will be position of the real winner 
        for (int i = 0; i < size; i++) { //
            if (gameValue(allPlayers[i].card1) == highest) { //if is a winning player
                if (allPlayers[i].card1 < realWin) { //if player card is better than current best
                    realWin = allPlayers[i].card1;
                    endPos = firstEnd(size, i);
                }
            }
        }
    }
    return endPos;
}

void initDeck(int deck[], int const deckSz) {
    for (int i = 0; i < deckSz; i++) { //Fills deck
        deck[i] = i % 52;
    }
}

string faceValue(int deckValue) {
    int faceV = deckValue % 13;
    string card;
    if (faceV == 0) {
        card = "A";
    } else if (faceV > 0 && faceV < 9) { //If card is 2-99
        card = (faceV + 48 + 1);
    } else if (faceV > 8 && faceV < 13) { //if card is 10, Jack, Queen, or King
        if (faceV == 9)card = "10";
        if (faceV == 10)card = "J";
        if (faceV == 11)card = "Q";
        if (faceV == 12)card = "K";
    } else {
        card = "XXX"; //Error
    }
    return card;
}

string suit(int deckValue) {
    string suit;
    if (deckValue / 13 == 0) suit = "S";
    else if (deckValue / 13 == 1) suit = "H";
    else if (deckValue / 13 == 2) suit = "C";
    else if (deckValue / 13 == 3) suit = "D";
    else {
        suit = "X";
    }
    return suit;
}

int draw(int deck[], int &capacity) {

    int pos = rand() % capacity;
    int drawnCard = deck[pos]; //value of randomely chosen card
    swap(deck, pos, capacity - 1);
    capacity--; //decrement capacity to keep two sections apart

    return drawnCard;
}

void swap(int deck[], int pos1, int pos2) {
    int temp = deck[pos1];
    deck[pos1] = deck[pos2];
    deck[pos2] = temp;
}

int findHighest(Player allPlayers[], int size) {
    int highest = -1;
    for (int i = 0; i < size; i++) { //Loops to check all player's cards
        if (gameValue(allPlayers[i].card1) > highest)highest = gameValue(allPlayers[i].card1); //replace if higher
    }
    return highest;
}

int gameValue(int deckValue) {
    int gameV = deckValue % 13;
    if (gameV == 0) { //If card is an A
        gameV = 14; //Ace is worth the most
    } else if (gameV > 0 && gameV < 13) { //If card is between 2-K
        gameV = gameV + 1;
    } else {
        gameV = -1; //Error
    }

    return gameV;

}

int firstEnd(int size, int pos) { //finds position of player that closes the bet round 
    int end = pos; //Position of new ending point
    if (size == 2) { //if only two players (user included) are playing
        if (end == 0)end = 1;
        else if (end == 1)end = 0;
    } else {
        for (int i = 0; i < 2; i++) { //will loop twice, adding 2 to the counter
            if (end == size - 1) { //if you are in the last element in the array
                end = 0; //loop back to begining
            } else {
                end++; //else, increment position
            }
        }
    }
    return end;
}

void clearScreen() {
    for (int i = 0; i < 50; i++) {
        cout << endl;
    }
}

void deal(Player allPlayers[], int size, int deck[], int &capacity, int start) {
    for (int i = 0; i < size; i++) {
        allPlayers[i].card1 = draw(deck, capacity); //draws a random card within unused side of array 
        addToSeven(allPlayers[i], allPlayers[i].card1); //adds drawn card to 7 card array of user
    }
    for (int i = 0; i < size; i++) {
        allPlayers[i].card2 = draw(deck, capacity); //draws a random card within unused side of array 
        addToSeven(allPlayers[i], allPlayers[i].card2); //adds drawn card to 7 card array of user
    }

}

void decide(Player allPlayers[], int &currPos, int &currBet, int pot[], int &potLoc, int &end, int size, int openCrd[], bool gameEnd) {
    if (allPlayers[currPos].comp == true) { //if player is a computer player
        /* commented out because current AI decision is still broken
        cout << "Comp: " << allPlayers[currPos].name << endl;
        
        allPlayers[currPos].needed = currBet - allPlayers[currPos].currBet; //checks how much is needed to call
        string choice;
        //randomly chooses action
        if(allPlayers[currPos].needed-allPlayers[currPos].currBet <= 10){
            int random = (rand()%10)+1; //gives a number 1-10
            if(random>0 && random < 9){ //80% chance
                choice = "Call";
            }
            else {                      //20% chance
                if(allPlayers[currPos].chips >=20+allPlayers[currPos].needed){
                          choice = "Raise";
                }
                else choice = "Call";
          
            }
        }
        else if (allPlayers[currPos].needed-allPlayers[currPos].currBet <= 50){
            int random = (rand()%10)+1; //gives a number 1-10
            if(random>0 && random < 4){ //30% chance
                choice = "Call";
            }
            else {                      //70% chance
                choice = "Fold";
            }
        }
        else {
            int random = (rand()%10)+1; //gives a number 1-10
            if(random>0 && random < 10){ //90% chance
                choice = "Fold";
            }
            else {                      //10% chance
                choice = "Call";
            }
        }
        //write out steps depending on choice
        if(choice == "Call"){   //if computer chose to call/check
            if(allPlayers[currPos].needed == 0){//if needs to just check
                allPlayers[currPos].status="Check";
            }
            else if (allPlayers[currPos].needed < allPlayers[currPos].chips){
                //computer calls
                    allPlayers[currPos].chips -= allPlayers[currPos].needed; //subtracts call from chips
                    allPlayers[currPos].totBet += allPlayers[currPos].needed; //adds to total bet this game
                    pot[potLoc] += allPlayers[currPos].needed; //adds call/check to pot
                    allPlayers[currPos].currBet += allPlayers[currPos].needed; //adds to current round bet
                    allPlayers[currPos].needed = 0; //reset needed amount to 0
                    allPlayers[currPos].status = "Call";
            }
            //This part of the code is not yet complete. Any attempt to go all in will 
             result in a fold. Normally, player's remaining chips would be added to the 
             current pot while changing position of potPos so that a new element
             within the pot array would recieve further bets that would not correspond to
             this user // 
            else    //if player is going All In
            {
                allPlayers[currPos].needed = allPlayers[currPos].chips - allPlayers[currPos].needed;
                pot[potLoc] += allPlayers[currPos].chips;
                allPlayers[currPos].chips = 0;
                allPlayers[currPos].status="Fold";
            }
        }
        else if (choice == "Raise"){        //if computer chose to raise bet
            //computer calls first
                    allPlayers[currPos].chips -= allPlayers[currPos].needed; //subtracts call from chips
                    allPlayers[currPos].totBet += allPlayers[currPos].needed; //adds to total bet this game
                    pot[potLoc] += allPlayers[currPos].needed; //adds call/check to pot
                    allPlayers[currPos].currBet += allPlayers[currPos].needed; //adds to current round bet
                    allPlayers[currPos].needed = 0; //reset needed amount to 0
                    allPlayers[currPos].status = "Call";
                    
                    int betAmt = 0;
                    int random = rand()%3;
                    if(random == 0){    //33% chance
                        betAmt = 2;
                    }
                    else if (random == 1){//33% chance
                        betAmt = 10;
                    }
                    else {                //33% chance
                        betAmt = 15;
                    }
                    //implement raised bet
                    currBet +=betAmt; //increment current bet for round
                    allPlayers[currPos].chips -= betAmt; //subtracts call from chips
                    allPlayers[currPos].totBet += betAmt; //adds to total bet this game
                    pot[potLoc] += betAmt; //adds call/check to pot
                    allPlayers[currPos].currBet += betAmt; //adds to current round bet                     allPlayers[currPos].needed = 0; //reset needed amount to 0
                    allPlayers[currPos].status = "Raise";
                    newEnd(currPos, end, size); //changes ending position to player before
                    cout << "New end pos is " << end << endl << "Current pos is" << currPos << endl;         
        }
        else if (choice == "Fold"){
            allPlayers[currPos].totBet = 0;
            allPlayers[currPos].currBet = 0;
            allPlayers[currPos].needed = 0;
            allPlayers[currPos].status = "Fold";
        }
        else//will not get executed
       

    */
        //INSTEAD of code above, computer can only check/call or fold if not enough
        allPlayers[currPos].needed = currBet - allPlayers[currPos].currBet;
        if(allPlayers[currPos].needed < allPlayers[currPos].card1){ //if computer has enough to call
            if (allPlayers[currPos].needed == 0){
                allPlayers[currPos].status="Check";
            }
            else {
                allPlayers[currPos].chips -= allPlayers[currPos].needed; //subtracts call from chips
                    allPlayers[currPos].totBet += allPlayers[currPos].needed; //adds to total bet this game
                    pot[potLoc] += allPlayers[currPos].needed; //adds call/check to pot
                    allPlayers[currPos].currBet += allPlayers[currPos].needed; //adds to current round bet
                    allPlayers[currPos].needed = 0; //reset needed amount to 0
                    allPlayers[currPos].status = "Call";
            }
        }
        else {      //computer folds
            allPlayers[currPos].totBet = 0;
                    allPlayers[currPos].currBet = 0;
                    allPlayers[currPos].needed = 0;
                    allPlayers[currPos].status = "Fold";
        }
        if (currPos == size - 1)currPos = 0;
        else currPos++;
    }
    //if player is user, let him determine what to do
    else {
        //if player is user
        outoutScreen(allPlayers, size, currPos, openCrd, pot, potLoc, gameEnd);
        //sets needed amount to call equal to current bet - user's current bet this round
        allPlayers[currPos].needed = currBet - allPlayers[currPos].currBet;
        if (allPlayers[currPos].needed >= allPlayers[currPos].chips) { //If player must go all in to call
              string choice; 
            if (allPlayers[currPos].status != "Fold") {
                bool valid = true;
                do{
                    valid = true;
                    cout<<"What would you like to do?"<<endl
                        <<"1) Fold"<<endl<<"2) All In"<<endl;
                    cin>>choice;
                    if(choice.length()!=1){
                        cout<<"Please enter 1 or 2 only";
                        valid = false;
                    }
                    else if (choice[0]<'1'|| choice[0]>'2'){
                        cout<<"Please enter 1 or two only"<<endl;
                        valid = false;
                    }
                    else {
                        cout<<"Thank you"<<endl;
                    }
                }while(valid == false);
                
            }
            if(choice == "1"){  //if user folded
                allPlayers[currPos].totBet = 0;
                    allPlayers[currPos].currBet = 0;
                    allPlayers[currPos].needed = 0;
                    allPlayers[currPos].status = "Fold";
            }
            /*This part of the code is not yet fully complete. same information as message above*/
            else {      //if user went all in
                allPlayers[currPos].needed = allPlayers[currPos].chips - allPlayers[currPos].needed;
                pot[potLoc] += allPlayers[currPos].chips;
                allPlayers[currPos].chips = 0;
                allPlayers[currPos].status="Fold";
            }
        } 
        else { //if player can check or call
            if (allPlayers[currPos].status != "Fold") { //only execute if player has not folded
                string choice; //will be used to chose what will be done
                bool valid;
                do {
                    //OUTPUT TABLE
                    valid = true; //will be used for input validation
                    cout << "What would you like to do?" << endl;
                    if (allPlayers[currPos].needed == 0) { //if player needs to just check
                        cout << "1)Check" << endl;
                    } else { //if player needs to call
                        cout << "1)Call (" << allPlayers[currPos].needed << ") " << endl;
                    }
                    cout << "2)Fold" << endl << "3)Raise" << endl; //display final options
                    cout<<"Chips ("<<allPlayers[currPos].chips<<")"<<endl;
                    cin>>choice;
                    if (choice.length() != 1)valid = false; //if input is more than 1 char
                    else if (choice[0] < '0' || choice[0] > '3')valid = false; //if bad input
                    else {}
                } while (valid == false);


                //Execute player option
                if (choice[0] == '1') {
                    //if user checked or called
                    allPlayers[currPos].chips -= allPlayers[currPos].needed; //subtracts call from chips
                    allPlayers[currPos].totBet += allPlayers[currPos].needed; //adds to total bet this game
                    pot[potLoc] += allPlayers[currPos].needed; //adds call/check to pot
                    allPlayers[currPos].currBet += allPlayers[currPos].needed; //adds to current round bet
                    allPlayers[currPos].needed = 0; //reset needed amount to 0
                    allPlayers[currPos].status = "Call";
                } else if (choice[0] == '2') {
                     //if user chose to fold
                    allPlayers[currPos].totBet = 0;
                    allPlayers[currPos].currBet = 0;
                    allPlayers[currPos].needed = 0;
                    allPlayers[currPos].status = "Fold";
                } else if (choice[0] == '3') {
                    //if user wants to raise
                    //first makes the player call the current bet
                    allPlayers[currPos].chips -= allPlayers[currPos].needed; //subtracts call from chips
                    allPlayers[currPos].totBet += allPlayers[currPos].needed; //adds to total bet this game
                    pot[potLoc] += allPlayers[currPos].needed; //adds call/check to pot
                    allPlayers[currPos].currBet += allPlayers[currPos].needed; //adds to current round bet
                    allPlayers[currPos].needed = 0; //reset needed amount to 0
                    allPlayers[currPos].status = "Call";
                    //rasing
                    bool valBet = true; //used for input validation
                    string betAmt; //raise amount
                    do {
                        valBet = true;
                        cout << "How much would you like to bet?" << endl;
                        cout << "Enter 0 if you chose to check" << endl;
                        cin>>betAmt;
                        if (converter(betAmt) < 0)valBet = false;
                        else if (converter(betAmt) > allPlayers[currPos].chips) {
                            valBet = false;
                            cout << "Please enter a bet smaller than your chip amount " << allPlayers[currPos].chips << endl;
                        } else { //if proper bet was entered 
                            currBet += converter(betAmt); //increment current bet for round
                            allPlayers[currPos].chips -= converter(betAmt); //subtracts call from chips
                            allPlayers[currPos].totBet += converter(betAmt); //adds to total bet this game
                            pot[potLoc] += converter(betAmt); //adds call/check to pot
                            allPlayers[currPos].currBet += converter(betAmt); //adds to current round bet
                            allPlayers[currPos].needed = 0; //reset needed amount to 0
                            allPlayers[currPos].status = "Raise";
                            newEnd(currPos, end, size); //changes ending position to player before
                          
                        }
                    } while (valBet == false); //while user hasn't entered a valid bet
                }
            }
            if (currPos == size - 1)currPos = 0; //its next user's turn
            else currPos++;
        }//end of if player can check or call
    }//if player is user
}

void initSevenC(Player &player) {
    for (int i = 0; i < POSCARDS; i++) {
        player.sevenC[i] = -10; //sets all values in array to -10
    }

}

void addToSeven(Player &player, int value) {
    bool found = false;
    int i = 0;
    while (!found) { //loop until an empty slot is found
        if (player.sevenC[i] == -10) { //if card slot is empty
            player.sevenC[i] = value;
            found = true;
        } else i++; //else, increment i 
    }
}

int converter(string num) {

    int placeHolder;
    int addOn;
    int multiplier = 1;
    int returnThis = 0;
    ;

    for (int i = 0; i < num.size(); i++) {
        char zerois48 = num[num.size() - 1 - i];
        int number = zerois48;

        if (number == 49) {
            placeHolder = 1;
        }
        else if (number == 50) {
            placeHolder = 2;
        }
        else if (number == 51) {
            placeHolder = 3;
        }
        else if (number == 52) {
            placeHolder = 4;
        }
        else if (number == 53) {
            placeHolder = 5;
        }
        else if (number == 54) {
            placeHolder = 6;
        }
        else if (number == 55) {
            placeHolder = 7;
        }
        else if (number == 56) {
            placeHolder = 8;
        }
        else if (number == 57) {
            placeHolder = 9;
        }
        else if (number == 48) {
            placeHolder = 0;
        }
        else {
            placeHolder = -2;
        }

        if (i > 0) {
            multiplier = multiplier * 10;
        }

        if (placeHolder > -1) {
            addOn = placeHolder * multiplier;
            returnThis = returnThis + addOn;
        }
        else {
            returnThis = -1;
        }



    }

    return returnThis;
}

void newEnd(int currPos, int &end, int size) {

    if (currPos == 0) { //if you are in the last element in the array
        currPos = size - 1; //loop back to begining
    } else {
        currPos--; //else, increment position
    }
    end = currPos; //
}

void drawField(int bRound, Player allPlayers[], int size, int openCrd[], int deck[], int &capacity) {
    if (bRound == 0) { //if first round
        //Draw NO Cards
    } else if (bRound == 1) { //if second round
        //Draw three cards
        for (int i = 0; i < 3; i++) { //add three cards
            openCrd[i] = draw(deck, capacity);
            for (int j = 0; j < size; j++) { //will loop for as many players currently playing
                addToSeven(allPlayers[j], openCrd[i]); //gives players cards on field 
            }
        }
    } else if (bRound == 2) { //if third round
        openCrd[3] = draw(deck, capacity);
        for (int j = 0; j < size; j++) { //will loop for as many players currently playing
            addToSeven(allPlayers[j], openCrd[3]); //gives players cards on field 
        }

    } else if (bRound == 3) { //if fourth and final round
        openCrd[4] = draw(deck, capacity);
        for (int j = 0; j < size; j++) { //will loop for as many players currently playing
            addToSeven(allPlayers[j], openCrd[4]); //gives players cards on field 
        }
    } else cout << "Error with drawField (bRound" << endl;

}

void outoutScreen(Player allPlayers[], int size, int currPos, int fCards[], int pot[], int potPos, bool endGame) {
    clearScreen();
    //outputs Player 1 name
    spaces(18);
    cout << left << setw(8) << allPlayers[0].name;
    spaces(18);
    cout << endl;
    //outputs Player1 cards
    spaces(18);
    if (allPlayers[0].comp == false) { //if is user
        cout << right << setw(2) << faceValue(allPlayers[0].card1) << setw(2) << suit(allPlayers[0].card1)
                << left << setw(2) << faceValue(allPlayers[0].card2) << setw(2) << suit(allPlayers[0].card2);
    } else { //if is computer
        if (allPlayers[0].status != "Fold") {
            if (endGame == false)cout << " ?  " << " ?  "; //hide cards
            else { //unless its end of game
                cout << right << setw(2) << faceValue(allPlayers[0].card1)
                        << left << setw(2) << suit(allPlayers[0].card1)
                        << right << setw(2) << faceValue(allPlayers[0].card2)
                        << left << setw(2) << suit(allPlayers[0].card2);
            }
        } else spaces(8);
    }
    spaces(18);
    cout << endl;
    //outputs player1 status
    spaces(18);
    cout << left << setw(8) << allPlayers[0].status;
    spaces(18);
    cout << endl;
    //output player's entry to table
    if (allPlayers[0].status != "Fold") { //if player still in game
        spaces(18);
        cout << left << setw(8) << allPlayers[0].currBet;
        spaces(18);
    } else spaces(8); //if player folded
    cout << endl;
    //output Player 5 and Player 2 names
    if (size == 5) { //if there is a fifth player
        cout << left << setw(8) << allPlayers[4].name;
    } else { //if there is no 5th player
        spaces(8); //leave blank
    }
    spaces(28);
    cout << right << setw(8) << allPlayers[1].name;
    cout << endl;
    //outputs Player 5 cards, field cards, and player 2 cards
    if (size == 5) { //if there is a fifth player
        if (allPlayers[4].comp == false) { //if is user
            cout << right << setw(2) << faceValue(allPlayers[4].card1) << left << setw(2) << suit(allPlayers[4].card1);
            cout << right << setw(2) << faceValue(allPlayers[4].card2) << left << setw(2) << suit(allPlayers[4].card2);
        } else {
            if (allPlayers[4].status != "Fold") {
                if (endGame == false)cout << " ?  " << " ?  "; //hide cards
                else { //unless its end of game
                    cout << right << setw(2) << faceValue(allPlayers[4].card1)
                            << left << setw(2) << suit(allPlayers[4].card1)
                            << right << setw(2) << faceValue(allPlayers[4].card2)
                            << left << setw(2) << suit(allPlayers[4].card2);
                }
            } else spaces(8);
        }
    } else { //if there is no 5th player
        spaces(8); //leave blank
    }
    //output cards in field
    spaces(4);
    for (int i = 0; i < 5; i++) { //will loop 5 times
        if (fCards[i] == -10) { //if card is empty
            cout << " ?  ";
        } else { //if card has a value
            cout << right << setw(2) << faceValue(fCards[i]);
            cout << left << setw(2) << suit(fCards[i]);
        }
    }
    spaces(4);
    //output player 2 cards
    if (allPlayers[1].comp == false) { //if is user
        cout << right << setw(2) << faceValue(allPlayers[1].card1) << left << setw(2) << suit(allPlayers[1].card1);
        cout << right << setw(2) << faceValue(allPlayers[1].card2) << left << setw(2) << suit(allPlayers[1].card2);
    } else { //if is computer
        if (allPlayers[1].status != "Fold") {
            if (endGame == false)cout << " ?  " << " ?  "; //hide cards
            else { //unless its end of game
                cout << right << setw(2) << faceValue(allPlayers[1].card1)
                        << left << setw(2) << suit(allPlayers[1].card1)
                        << right << setw(2) << faceValue(allPlayers[1].card2)
                        << left << setw(2) << suit(allPlayers[1].card2);
            }
        } else spaces(8);
    }
    cout << endl;
    //output Player 5 and Player 2 status
    if (size == 5) { //if there is a player 5
        cout << left << setw(8) << allPlayers[4].status;
    } else { //if there is no player 5
        spaces(8);
    }
    spaces(28);
    //output player 2 status
    cout << right << setw(8) << allPlayers[2].status;
    cout << endl;
    //outputs player 5 and player 2 bet amount
    if (size == 5) { //if there is a player 5
        //output player's entry to table
        if (allPlayers[4].status != "Fold") { //if player still in game
            cout << left << setw(8) << allPlayers[4].currBet;
        } else spaces(8); //if player folded
    } else spaces(8); //if there is no player 5
    spaces(28);
    //output player's entry to table
    if (allPlayers[1].status != "Fold") { //if player still in game
        cout << right << setw(8) << allPlayers[0].currBet;
    } else spaces(8); //if player folded
    cout << endl;
    //outputs Player 4 and Player 3 names
    spaces(8);
    if (size >= 4) { //if there is a player 4
        cout << left << setw(8) << allPlayers[3].name;
    } else spaces(8); //if there is no player 4
    spaces(12);
    //output player 3 name
    if (size >= 3) { //if there is a player 3
        cout << left << setw(8) << allPlayers[2].name;
    } else spaces(8); //if there is no player 3
    spaces(8);
    cout << endl;

    //output player 4 and player 3 cards
    spaces(8);
    if (size >= 4) { //if there is a player 4
        if (allPlayers[3].comp == false) { //if is user
            cout << right << setw(2) << faceValue(allPlayers[3].card1) << left << setw(2) << suit(allPlayers[3].card1);
            cout << right << setw(2) << faceValue(allPlayers[3].card2) << left << setw(2) << suit(allPlayers[3].card2);
        } else {
            if (allPlayers[3].status != "Fold") {
                if (endGame == false)cout << " ?  " << " ?  "; //hide cards
                else { //unless its end of game
                    cout << right << setw(2) << faceValue(allPlayers[3].card1)
                            << left << setw(2) << suit(allPlayers[3].card1)
                            << right << setw(2) << faceValue(allPlayers[3].card2)
                            << left << setw(2) << suit(allPlayers[3].card2);
                }
            } else spaces(8);
        }
    } else spaces(8); //if there is no player 4
    spaces(12);
    if (size >= 3) { //if there is a player 3
        if (allPlayers[2].comp == false) { //if is user
            cout << right << setw(2) << faceValue(allPlayers[2].card1) << left << setw(2) << suit(allPlayers[2].card1);
            cout << right << setw(2) << faceValue(allPlayers[2].card2) << left << setw(2) << suit(allPlayers[2].card2);
        } else {
            if (allPlayers[2].status != "Fold") {
                if (endGame == false)cout << " ?  " << " ?  "; //hide cards
                else { //unless its end of game
                    cout << right << setw(2) << faceValue(allPlayers[2].card1)
                            << left << setw(2) << suit(allPlayers[2].card1)
                            << right << setw(2) << faceValue(allPlayers[2].card2)
                            << left << setw(2) << suit(allPlayers[2].card2);
                }
            } else spaces(8);
        }
    } else spaces(8); //if there is no player 3
    spaces(8);
    cout << endl;
    //output player 4 and player 3 status
    spaces(8);
    if (size >= 4) { //if there is a player 4
        cout << left << setw(8) << allPlayers[3].status;
    } else spaces(8); //if there is no player 4
    spaces(12);
    if (size >= 3) { //if there is a player 3
        cout << right << setw(8) << allPlayers[2].status;
    } else spaces(8); //if there is no player 3
    spaces(8);
    cout << endl;
    //output player 4 and player 3 bet amount
    spaces(8);
    if (size >= 4) { //if there is a player 4
        if (allPlayers[3].status != "Fold") { //if player is still in game
            cout << left << setw(8) << allPlayers[3].currBet;
        } else spaces(8); //if player folded
    } else spaces(8); //if there is no player 4


    spaces(12);
    if (size >= 3) { //if there is a plyer 3
        if (allPlayers[2].status != "Fold") { //if player is still in game
            cout << right << setw(8) << allPlayers[2].currBet;
        } else spaces(8); //if player folded
    } else spaces(8); //if there is no player 3
    cout << endl;
}

void spaces(int num) {
    for (int i = 0; i < num; i++) {
        cout << " ";
    }
}

void whatGame(Player allPlayers[], int aPSize) { //Assigns game made by all players
    //sort all possible 7 cards within all players
    for (int i = 0; i < aPSize; i++) {
        sort(allPlayers, i, aPSize);
    }
    //check for flush/straight flush
    for (int i = 0; i < aPSize; i++) {
        checkFlush(allPlayers, i);
    }
    //check for straight
    for (int i = 0; i < aPSize; i++) {
        if(allPlayers[i].game <11){ //only execute if game is not Flush or Straight flush
            checkStraight(allPlayers, i);
        }
        
    }
    //check for pairs, triplets, four of a kind
    for (int i = 0; i < aPSize; i++) {
        if(allPlayers[i].game < 10){        //only execute if game is not flush, straight, or straigh flush
            checkPairs(allPlayers, i);
        }
        
    }
}



void checkFlush(Player allPlayers[], int loc) {
    int best = 0;
    int count = 0;
    string bestS;

    for (int i = 0; i < POSCARDS; i++) {
        if (suit(allPlayers[loc].sevenC[i]) == "H") {
            count++;
        }
    }
    if (count > best) {//if # of suits counted is better than last #, replace
        best = count;
        bestS = "H";
    }
    count = 0; //reset counter
    for (int i = 0; i < POSCARDS; i++) {
        if (suit(allPlayers[loc].sevenC[i]) == "S") {
            count++;
        }
    }
    if (count > best){//if # of suits counted is better than last #, replace
        best = count;
        bestS = "S";
    }
    count = 0; //reset counter
    for (int i = 0; i < POSCARDS; i++) {
        if (suit(allPlayers[loc].sevenC[i]) == "C") {
            count++;
        }
    }
    if (count > best){//if # of suits counted is better than last #, replace
        best = count;
        bestS = "C";
    }
    count = 0; //reset counter
    for (int i = 0; i < POSCARDS; i++) {
        if (suit(allPlayers[loc].sevenC[i]) == "D") {
            count++;
        }
    }
    if (count > best){//if # of suits counted is better than last #, replace
        best = count;
        bestS = "D";
    }
    count = 0; //reset counter

    if(best >= 5){      //if user has Flush
        allPlayers[loc].game = FLUSH;
        //cout<<"P L A Y E R H A S F L U S H "<<endl;
    
    
        //check for straight flush
        int gap = 4;    
        int sCount = 0;
        for (int i = 0; i<POSCARDS-1; i++){
            if(suit(allPlayers[loc].sevenC[i]) == bestS){   //if card is of flush suit
               //if next card is within range of straight of first card
                if (gameValue(allPlayers[loc].sevenC[i+1])- gameValue(allPlayers[loc].sevenC[i])<= gap){
                    sCount++;
                    gap -= gameValue(allPlayers[loc].sevenC[i+1])- gameValue(allPlayers[loc].sevenC[i]);
                }
            } 
        }
        if(sCount>= 5){     //if a straight was found within flush
            allPlayers[loc].game += STRAIGHT;
        }
    }
}

void sort(Player allPlayers[],int loc, int size){
   int start = 0;
   int minLoc;
   for (int i = 0; i<POSCARDS; i++){    //sort all cards within sevenC
       minLoc = findMin(allPlayers, start, loc);
       swap(start, minLoc, allPlayers, loc);
       start++;
   }
   

   
}

int findMin(Player allPlayers[], int start, int loc){
    int min = 99999;
    int minLoc = -1;
    for (int i = start; i<POSCARDS; i++){
        if(gameValue(allPlayers[loc].sevenC[i]) < min){
            min = gameValue(allPlayers[loc].sevenC[i]);
            minLoc = i;
        }
    }
    return minLoc;
}

void swap(int pos1, int pos2, Player allPlayers[], int loc){
    int temp = allPlayers[loc].sevenC[pos1];
    allPlayers[loc].sevenC[pos1] = allPlayers[loc].sevenC[pos2];
    allPlayers[loc].sevenC[pos2] = temp;
}

void checkStraight(Player allPlayers[], int loc){
    int gap = 4;
    int sCount = 0;
    int starting;
    for(int j = 0; j<POSCARDS-4; j++){      //loops 3 times, after no straight would be possible 
        starting = j;       //location of initial compared value
        for (int i = j; i<POSCARDS-1; i++){ //loops 6, 5, then 4 times
             //checks possible straights only
            //if next card is one value higher than first card and also not the same value
            if (gameValue(allPlayers[loc].sevenC[i+1])- gameValue(allPlayers[loc].sevenC[i])<= gap
                && gameValue(allPlayers[loc].sevenC[i+1])!= gameValue(allPlayers[loc].sevenC[i])
                && (i+1)-starting<5){
                        sCount++;
                        gap -= gameValue(allPlayers[loc].sevenC[i+1])- gameValue(allPlayers[loc].sevenC[i]);
                    }
        }
        if(sCount>= 4){     //if straight was found
                allPlayers[loc].game = STRAIGHT;
                //cout<<"U S E R H A S S T R A I G H T "<<endl;
                cout<<allPlayers[loc].name<<endl<<" count = "<<sCount<<endl;
            }
            gap = 4;        //reset
            sCount = 0;     //reset
    }
    
}

void checkPairs(Player allPlayers[],int loc){
    int pairs = 0;
    int tripple = 0;
    int poker = 0;
    int counter = 1;    //will be used to count look alikes
    int start = 0;      //used to avoid recounting a triplet as a pair
    int i = 1;          //used to check next and next next card
    while (start < POSCARDS-1){
        //if cards are the same
        if(gameValue(allPlayers[loc].sevenC[start]) == gameValue(allPlayers[loc].sevenC[start+i])){
            counter++;  //cards are the same
            i++;
        }
        else {      //if cards are NOT the same
            if(counter == 4)poker++;    //if 4 alike cards are found
            if (counter == 3)tripple++; //if 3 alike cards are found
            if(counter==2)pairs++;        //if 2 alike cards are found
            start += counter;           //places at new starting point
            counter = 1;                //reset
            i = 1;                      //reset
                    
        }
    }
    if(poker==1){       //if 4 of a kind were found
        allPlayers[loc].game = POKER;
    }
    else if (tripple >= 1){     //if at least one triplet was found
        if(pairs>=1){           //if also found at least one pair
            allPlayers[loc].game = FULLHOUSE;
        }
        else {                  //if only one (or two triplet found)
            allPlayers[loc].game = TRIPPLE;
        }
    }
    else if (pairs >= 1)  {     //if at least one pair was found
        if(pairs >=2){          //if at least 2 pairs were found
            allPlayers[loc].game = TWOPAIRS;
        }
        else {                  //if only one pair was found
            allPlayers[loc].game = ONEPAIR;
        }
    }
    else                        //if no card was alike the other
    {
        //player has no game
    }
}

void op (Player allPlayers[]){
    for (int i = 0; i<5; i++){
        for (int j = 0; j<POSCARDS; j++){
            cout<<allPlayers[i].name<<" "<<j+1<<" "<<gameValue(allPlayers[i].sevenC[j])
                    <<suit(allPlayers[i].sevenC[j])<<endl;
        }
    }
}

void reInitPlyer(Player &james, Player &cynthia, Player &ricky, Player &stacy, Player &user) {
    james.card1 = -1;
    james.card2 = -1;
    initSevenC(james);
    james.totBet = 0;
    james.needed = 0;
    james.currBet = 0;
    james.game = 0;
    james.status = "";

    cynthia.card1 = -1;
    cynthia.card2 = -1;
    initSevenC(cynthia);
    cynthia.totBet = 0;
    cynthia.needed = 0;
    cynthia.currBet = 0;
    cynthia.game = 0;
    cynthia.status = "";

    ricky.card1 = -1;
    ricky.card2 = -1;
    initSevenC(ricky);
    ricky.totBet = 0;
    ricky.needed = 0;
    ricky.currBet = 0;
    ricky.game = 0;
    ricky.status = "";

    stacy.card1 = -1;
    stacy.card2 = -1;
    initSevenC(stacy);
    stacy.totBet = 0;
    stacy.needed = 0;
    stacy.currBet = 0;
    stacy.game = 0;
    stacy.status = "";

    user.card1 = -1;
    user.card2 = -1;
    initSevenC(user);
    user.totBet = 0;
    user.needed = 0;
    user.currBet = 0;
    user.game = 0;
    user.status = "";
}
