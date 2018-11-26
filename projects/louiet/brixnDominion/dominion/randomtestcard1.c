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
#define TESTNUM 1000


void smithyCheck(int thisPlayer, struct gameState *post, int* fails, int failsize){

	struct gameState pre;
	memcpy(&pre, post, sizeof(struct gameState));


	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int retval = cardEffect(smithy, choice1, choice2, choice3, post, handpos, &bonus); // run card function

	int preHC = pre.handCount[thisPlayer];
	//int preDeC = pre.deckCount[thisPlayer];

	//int newCards = 3;
	//int discarded = 1;

	int val1 = drawCard(thisPlayer, &pre);
	if (val1 < 0) // test for successful draw
		fails[0]++;
	else
		fails[4]++;
	int val2 = drawCard(thisPlayer, &pre);
	if (val2 < 0) // test for successful draw
		fails[0]++;
	else 
		fails[4]++;
	int val3 = drawCard(thisPlayer, &pre);
	if (val3 < 0) // test for successful draw
		fails[0]++;
	else
		fails[4]++;

	int disval = discardCard(0, thisPlayer, &pre, 0);
	if(disval)// test for successful discard
		fails[1]++;
	else
		fails[4]++;

	if (retval)// test for successful Effect
		fails[2]++;
	else
		fails[4]++;

	int postHC = post->handCount[thisPlayer];
	//int postDeC = post->deckCount[thisPlayer];
	if (!(preHC <= postHC - 1)) //hand count check
		fails[3]++;
	else
		fails[4]++;
}


int main(){
	printf("--- Testing Card: %s ---\n", TESTCARD);

	srand((unsigned int)(time(NULL)));
	struct gameState G; 

	int thisPlayer = 0;
	int fails[5] = {0,0,0,0,0};
	
	int i,j;
	for(i=0; i<TESTNUM; i++){ //for random test loop
	
		for (j = 0; j < sizeof(struct gameState); j++)
			((char*)&G)[j] = floor(Random() * 256);
		
		thisPlayer = floor(Random() * MAX_PLAYERS);
		G.deckCount[thisPlayer] = floor(Random() * MAX_DECK);
		G.discardCount[thisPlayer] = floor(Random() * MAX_DECK);
		G.handCount[thisPlayer] = floor(Random() * MAX_HAND);
		G.playedCardCount = floor(Random() * (MAX_DECK - 1));
		G.whoseTurn = thisPlayer;

		smithyCheck(thisPlayer, &G, fails, 5); // call check function
	}

	printf("TOTAL TEST:\n");
	printf("drawCard failed: %d\n", fails[0]);
	printf("discardCard failed: %d\n", fails[1]);
	printf("CardEffect failed: %d\n", fails[2]);
	printf("Hand fails: %d\n", fails[3]);
	printf("\n");
	return 0;
}
