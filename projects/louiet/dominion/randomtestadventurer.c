#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define TESTCARD "Adventurer"
#define TESTNUM 1000
#define ALLCARDS treasure_map

void adventCheck(int thisPlayer, struct gameState *post, int* fails, int failsize, int numTreasures){

	struct gameState pre;
//	int tempHand[MAX_HAND][2];
	memcpy(&pre, post, sizeof(struct gameState));

//	int postTreC = int preTreC = 0;
//	int numDrawn = int TreDrawn = 0;

	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int retval = cardEffect(adventurer, choice1, choice2, choice3, post, handpos, &bonus); // run card function

	int postHC = post->handCount[thisPlayer];
	int preHC = pre.handCount[thisPlayer];
	int postDeC = post->deckCount[thisPlayer];
	int preDeC = pre.deckCount[thisPlayer];
	int postPlayed = post->playedCardCount;
	int prePlayed = pre.playedCardCount;
	int postDiC = post->discardCount[thisPlayer];
	int preDiC = pre.discardCount[thisPlayer];


	if(retval == 0)
		fails[0]++;
	else
		fails[1]++;

	if(preHC + 2 - 1 == postHC) // two treasurers added, adventurer discarded
		fails[0]++;
	else
		fails[2]++;

	if(prePlayed + 1 == postPlayed) //treasure is played
		fails[0]++;
	else
		fails[3]++;

	if(preDeC - 2 >= postDeC) // check drew two treasure
		fails[0]++;
	else if(preDiC -1 >= postDeC) // check drew at least one from shuffled discard
		fails[0]++;
	else
		fails[4]++;

	if(preDeC - 2 + preDiC + 1 == postDeC + postDiC) //two treasures taken from deck+discard, adventurer discarded
		fails[0]++;
	else
		fails[5]++;
}

int main(){
	printf("--- Testing Card: %s ---\n", TESTCARD);

	srand((unsigned int)(time(NULL)));
	struct gameState G;

	int thisPlayer = 0;
	int fails[6] = {0,0,0,0,0,0};


	int i,j,l;
	int numTreasures;
	for(i=0; i<TESTNUM; i++){ //for random test loop

		for (j = 0; j < sizeof(struct gameState); j++)
			((char*)&G)[j] = floor(Random() * 256);

		thisPlayer = floor(Random() * MAX_PLAYERS);
		G.deckCount[thisPlayer] = floor(Random() * MAX_DECK);
		G.discardCount[thisPlayer] = floor(Random() * MAX_DECK);
		G.handCount[thisPlayer] = floor(Random() * MAX_HAND);
		G.playedCardCount = floor(Random() * (MAX_DECK - 1));
		G.whoseTurn = thisPlayer;

		numTreasures = 0;
		while(numTreasures<2){
			for (l = 0; l < G.deckCount[thisPlayer]; l++){ //create and shuffle deck
				G.deck[thisPlayer][l] = floor(Random() * ALLCARDS);
				if(G.deck[thisPlayer][l] >= copper && G.deck[thisPlayer][l] <= gold) 
					numTreasures++; //count number of treasure in case
			}
			for (l = 0; l < G.discardCount[thisPlayer]; l++){ //create and shuffle deck
				G.discard[thisPlayer][l] = floor(Random() * ALLCARDS);
				if(G.discard[thisPlayer][l] >= copper && G.discard[thisPlayer][l] <= gold) 
					numTreasures++; //count number of treasure in case
			}
		}

		adventCheck(thisPlayer, &G, fails, 6, numTreasures); // call check function
	}

	int allfails = fails[1] + fails[2] + fails[3] + fails[4] + fails[5]; 

	printf("TOTAL TEST FAILED: %d\n", allfails);
	printf("cardEffect failed: %d\n", fails[1]);
	printf("Hand fails: %d\n", fails[2]);
	printf("PlayedCard failed: %d\n", fails[3]);
	printf("Deck fails: %d\n", fails[4]);
	printf("Handling fails: %d\n", fails[5]);
	printf("\n");
	return 0;
}

