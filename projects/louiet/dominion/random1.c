#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>
#include <tgmath.h>
#include <time.h>
#define TRUE   1
#define FALSE  0
#define TESTCARD "smithy"
#define TESTRUNS 80
#define TESTRUNS_SIZE (TESTRUNS * MAX_DECK * MAX_PLAYERS * 10)
#define DEBUG 0
#define NUM_CARDS 27

static double randomNumbers[TESTRUNS * MAX_DECK * MAX_PLAYERS * 10];
static int positionInRandomNumbers = 0;

void init_randomNumbers(){
    int i;
    for(i = 0; i < TESTRUNS_SIZE; i++){

    randomNumbers[i] = Random();

    }

}

void dumpVariables(struct gameState * oldG, struct gameState* newG){

    int player, newCards, oldCards;
    printf("Number of Players: Old %d New %d\n", oldG->numPlayers, newG->numPlayers);
    printf("Whose Turn: Old %d New %d \n", oldG->whoseTurn, newG->whoseTurn);
    printf("Phase: Old %d New %d\n", oldG->phase, newG->phase);
    printf("NumActions: Old %d New %d \n", oldG->numActions, newG->numActions);
    printf("Number of Buys: Old %d New %d\n", oldG->numBuys, newG->numBuys);
    printf("Played Card Count: Old %d New %d \n", oldG->playedCardCount, newG->playedCardCount);


    for(player=0; player < oldG->numPlayers; player++) {
        printf("Player %d\n", player);
        printf("Hand Count: Old %d New %d\n", oldG->handCount[player], newG->handCount[player]);
        printf("Deck Count: Old %d New %d \n", oldG->deckCount[player], newG->deckCount[player]);
        printf("Discard Count: Old %d New %d \n", oldG->discardCount[player], newG->discardCount[player]);
    }

    player=oldG->whoseTurn;

    printf("Cards in Hand: Old Player ");

    for(oldCards = 0; oldCards < oldG->handCount[player]; oldCards++){

        printf("%d\t", oldG->hand[player][oldCards]);

    }
    printf("\n");

    printf("Cards in Hand: New Player ");

    for(newCards = 0; newCards < newG->handCount[player]; newCards++){

      printf("%d\t", newG->hand[player][newCards]);
    }
     printf("\n");


    printf("Cards in Deck: Old Player ");

    for(oldCards = 0; oldCards < oldG->deckCount[player]; oldCards++){

        printf("%d\t", oldG->deck[player][oldCards]);

    }
    printf("\n");

    printf("Cards in Deck: New Player ");

    for(newCards = 0; newCards < newG->deckCount[player]; newCards++){

        printf("%d\t", newG->deck[player][newCards]);
    }
    printf("\n");

    printf("Cards in Discard: Old Player ");

    for(oldCards = 0; oldCards < oldG->discardCount[player]; oldCards++){

        printf("%d\t", oldG->discard[player][oldCards]);

    }
    printf("\n");

    printf("Cards in Discards: New Player ");

    for(newCards = 0; newCards < newG->discardCount[player]; newCards++){

        printf("%d\t", newG->discard[player][newCards]);
    }
    printf("\n");

    printf("Played Cards: Old Player ");

    for(oldCards = 0; oldCards < oldG->playedCardCount; oldCards++){

        printf("%d\t", oldG->playedCards[oldCards]);

    }
    printf("\n");

    printf("Played Cards: New Player ");

    for(newCards = 0; newCards < newG->playedCardCount; newCards++){

        printf("%d\t", newG->playedCards[newCards]);
    }
    printf("\n");


}






void assertCustom(int boolean, char * message, struct gameState * oldG, struct gameState* newG){

    if(DEBUG){
        printf("TEST PASSED: %s\n", message );


    }

    if(boolean == FALSE){

        printf("TEST FAILED: %s\n", message );
        dumpVariables(oldG, newG);

    }

}

void assertGameState(int player, struct gameState * oldG, struct gameState * newG){
    int failed = FALSE;
    if(oldG->handCount[player] !=   newG->handCount[player]){
        printf("TEST FAILED: Hand Count for non-action player Changed\n");
        failed = TRUE;
    }
    if(oldG->numPlayers != newG->numPlayers){
        printf("TEST FAILED: Number of Players in Game Changed\n");
        failed = TRUE;
    }
    if(oldG->deckCount[player] !=   newG->deckCount[player]){
        printf("TEST FAILED: Deck Count for non-action player Changed\n");
        failed = TRUE;
    }
    int i;
    for(i = curse; i < NUM_CARDS; i++){
        if(oldG->supplyCount[i] != newG->supplyCount[i]){
            printf("TEST FAILED Card %d Supply Count Changed\n", i);
           failed = TRUE;
        }
        if(oldG->embargoTokens[i] != newG->embargoTokens[i]){
            printf("TEST FAILED Embargo Token on Card %d Changed\n", i);
            failed = TRUE;

        }
    }


    if(failed){
      dumpVariables(oldG, newG);
    }
    if(DEBUG && !failed){
        printf("GAME STATE INVARIANT TEST PASSED\n");
        dumpVariables(oldG, newG);
    }

}

int randomInt(int intMax){
   int random = (int)(intMax * randomNumbers[positionInRandomNumbers++]);
    if(DEBUG){printf("Random Number: %d\t", random);}
    return random;
}


int main() {
    srand((unsigned int)(time(NULL)));


    int newCards = 0;
    int discarded = 1;
    int xtraCoins = 0;
    int shuffledCards = 0;
    int numBuys = 0;
    int numActions =0;
    int minimumHandSize = 5;

    int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
    int seed = 1000;
    int numPlayers = 3;
    int thisPlayer = 0;
    struct gameState G, testG;
    int k[10] = {adventurer, embargo, village, minion, mine, cutpurse,
                 sea_hag, tribute, smithy, council_room};

    int i;

    init_randomNumbers();

    for(i = 0; i < TESTRUNS; i++) {
        printf("----------------- Test Number %d, Card: %s (ONLY FAILURES PRINTED)----------------\n", i,  TESTCARD);
         numPlayers = randomInt(numPlayers) + 2;
         initializeGame(numPlayers, k, seed, &G);

        int player, positionToAddCard, testDeckSize;

         testDeckSize = randomInt(MAX_DECK);

         for (player = 0; player < numPlayers; player++) {

             G.deckCount[player] = randomInt(testDeckSize);
             testDeckSize -= G.deckCount[player];

             for (positionToAddCard = 0; positionToAddCard < G.deckCount[player]; positionToAddCard++) {

                 G.deck[player][positionToAddCard] = randomInt(NUM_CARDS);


             }
         }


         for (player = 0; player < numPlayers; player++) {

             G.handCount[player] = randomInt(testDeckSize);
             testDeckSize -= G.handCount[player];


             for (positionToAddCard = 0; positionToAddCard < G.handCount[player]; positionToAddCard++) {

                 G.hand[player][positionToAddCard] = randomInt(NUM_CARDS);


             }

         }


         for (player = 0; player < numPlayers; player++) {

             G.discardCount[player] = testDeckSize;

             for (positionToAddCard = 0; positionToAddCard < G.discardCount[player]; positionToAddCard++) {

                 G.discard[player][positionToAddCard] = randomInt(NUM_CARDS);


             }
         }

        if(handpos < G.handCount[player]){
            G.hand[thisPlayer][handpos] = smithy;
        }
        else{
            G.hand[thisPlayer][0] = smithy;
        }


         memcpy(&testG, &G, sizeof(struct gameState));


         newCards = 3;
         xtraCoins = 0;
         shuffledCards = 0;
         if (testG.deckCount[thisPlayer] <= newCards) {

             shuffledCards = testG.discardCount[thisPlayer];

         }


         cardEffect(smithy, choice1, choice2, choice3, &testG, handpos, &bonus);


         assertCustom(testG.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, "Receives 3 cards", &G,
                      &testG);
         assertCustom(testG.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards + shuffledCards,
                      "Deck has 3 less Cards", &G, &testG);
         assertCustom(testG.coins == G.coins + xtraCoins, "No extra coins received", &G, &testG);
         assertCustom(testG.whoseTurn == G.whoseTurn, "Same Players Turn", &G, &testG);
         assertCustom(testG.numActions == G.numActions, "Number of actions", &G, &testG);
         assertCustom(testG.numBuys == G.numBuys, "Number of Buys", &G, &testG);
         assertCustom(testG.playedCardCount == G.playedCardCount + discarded, "Number of Cards Discarded", &G, &testG);
         assertGameState(thisPlayer + 1, &G, &testG);

    }

    return 0;
}


/*
#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define TESTCARD "Smithy"
#define TESTNUM 100


void smithyCheck(int thisPlayer, struct gameState *post, int* fails, int failsize){

	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int r = cardEffect(smithy, choice1, choice2, choice3, post, handpos, &bonus); // run card function

	int s = drawCard(thisPlayer, &pre);
	int t = drawCard(thisPlayer, &pre);
	int u = drawCard(thisPlayer, &pre);	
	int v = discardCard(0, thisPlayer, &pre, 0);

	int postHC = post->handCount[thisPlayer];
	int preHC = pre.handCount[thisPlayer];
	int postDC = post->deckCount[thisPlayer];
	int preDC = pre.deckCount[thisPlayer];

	if (s == -1 && pre.deckCount[thisPlayer] != 0)
		fails[0]++;

	if (t == -1 && pre.deckCount[thisPlayer] != 0)
		fails[0]++;

	if (u == -1 && pre.deckCount[thisPlayer] != 0)
		fails[0]++;
	
	if (!(r == 0 && v == 0))
	{
		if(r)
			fails[1]++;

		if (v)
			fails[2]++;
	}
	
	if (!(postHC == preHC && postDC == preDC))
		fails[3]++;
}


int main(){
	printf("--- Testing Card: %s ---\n", TESTCARD);

	
	//int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room}; 
	srand((unsigned int)(time(NULL)));
	struct gameState G; 
	//int seed = 1000;

	int thisPlayer = 0;
	//int newCards = 0; // set static variables of game
	//int discarded = 1;
	//int xtraCoins = 0;
	//int shuffledCards = 0;
	//int numBuys = 0;
	//int numActions =0;
	//int minimumHandSize = 5;
 
	//int newCards = = 3; // draw 3 cards (effect)
	//int discarded = 1; // card discarded
	int fails[4] = {0,0,0,0};
	
	int i,j;
	for(i=0; i<TESTNUM; i++){ //for random test loop
	
		for (j = 0; j < sizeof(struct gameState); j++)
			((char*)&G)[i] = floor(Random() * 256);

		//initializeGame(MAX_PLAYERS, k, seed, &G);

		thisPlayer = floor(Random() * MAX_PLAYERS);
		G.deckCount[thisPlayer] = floor(Random() * MAX_DECK);
		G.discardCount[thisPlayer] = floor(Random() * MAX_DECK);
		G.handCount[thisPlayer] = floor(Random() * MAX_HAND);
		G.playedCardCount = floor(Random() * (MAX_DECK - 1));
		G.whoseTurn = thisPlayer;

		smithyCheck(thisPlayer, &G, fails, 4); // call check function
	}

	int allfails = fails[0] + fails[1] + fails[2] + fails[3]; // gather results

	if(!allfails){ //display results
		printf("all passed\n");
	}else{
		printf("TOTAL TEST FAILED: %d\n", allfails);
		printf("drawCard failed: %d\n", fails[0]);
		printf("cardEffect failed: %d\n", fails[1]);
		printf("discardCard failed: %d\n", fails[2]);
		printf("Hand/Deck fails: %d\n", fails[3]);
	}

	return 0;
}
*/
