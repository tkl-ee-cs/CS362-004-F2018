#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define TESTCARD "Great Hall"
#define TESTNUM 1000


void ghallCheck(int thisPlayer, struct gameState *post, int* fails, int failsize){

	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));

	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int retval = cardEffect(great_hall, choice1, choice2, choice3, post, handpos, &bonus); // run card function

	int postHC = post->handCount[thisPlayer];
	int preHC = pre.handCount[thisPlayer];
	int postDeC = post->deckCount[thisPlayer];
	int preDeC = pre.deckCount[thisPlayer];
	int postDiC = post->playedCardCount;
	int preDiC = pre.playedCardCount;
	int preNuAc = pre.numActions;
	int postNuAc = post->numActions;

	if(retval == 0)
		fails[0]++;
	else
		fails[2]++;

	if (preHC == postHC)
		fails[0]++;
	else
		fails[1]++;

        if (preDeC - 1 == postDeC)
                fails[0]++;
	else if (preDeC == 0 && postDeC == pre.discardCount[thisPlayer] - 1)
		fails[0]++;
        else
                fails[4]++;

        if ( preDiC + 1 == postDiC)
                fails[0]++;
        else
                fails[3]++;

	if (preNuAc + 1  == postNuAc)
		fails[0]++;
	else
		fails[5]++;

}

int main(){
	printf("--- Testing Card: %s ---\n", TESTCARD);

	srand((unsigned int)(time(NULL)));
	struct gameState G;

	int thisPlayer = 0;
	int numActions = 0;

	int fails[6] = {0,0,0,0,0,0};


	int i,j;
	for(i=0; i<TESTNUM; i++){ //for random test loop

		for (j = 0; j < sizeof(struct gameState); j++)
			((char*)&G)[j] = floor(Random() * 256);

		thisPlayer = floor(Random() * MAX_PLAYERS);
		G.deckCount[thisPlayer] = floor(Random() * MAX_DECK);
		G.discardCount[thisPlayer] = floor(Random() * MAX_DECK);
		G.handCount[thisPlayer] = floor(Random() * MAX_HAND);
		G.playedCardCount = floor(Random() * (MAX_DECK - 1));
		G. numActions = numActions;
		G.whoseTurn = thisPlayer;

		ghallCheck(thisPlayer, &G, fails, 5); // call check function
	}
	int allfails = fails[1] + fails[2] + fails[3] + fails[4];

	printf("TOTAL TEST FAILED: %d\n", allfails);
	printf("Hand failed: %d\n", fails[1]);
	printf("cardEffect failed: %d\n", fails[2]);
	printf("discardCard failed: %d\n", fails[3]);
	printf("Deck fails: %d\n", fails[4]);
	printf("numAction fails: %d\n", fails[5]);
	printf("\n");
	return 0;
}

