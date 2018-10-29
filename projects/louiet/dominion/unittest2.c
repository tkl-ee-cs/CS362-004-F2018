#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
	

#define NOISY_TEST 0 // set NOISY_TEST to 0 to remove printfs from output
	
int main() {
	int seed = 1000;
	int numPlayer = 3; // number of players
	int p;
	int cardPos; // the position of card
	int trashflag;
	int handCount, played, numPlayed;
	int k[10] = {adventurer, council_room, feast, gardens, mine,remodel, smithy, village, baron, great_hall};
	struct gameState G;
	int maxHandCount = 5;
	int i = 0;
	int suiteflag = 0;

	printf ("TESTING discardCard():\n");
	memset(&G, 23, sizeof(struct gameState)); // clear the game state
	initializeGame(numPlayer, k, seed, &G); // initialize a new game

	for (p = 0; p < numPlayer; p++){ //increment through players
		for(handCount = maxHandCount; handCount > 0; handCount--){ //decrement through hand size (5 - 1)
			for(cardPos = 0; cardPos < handCount; cardPos++){ //increment through hand position
				for(trashflag = 0; trashflag <= 1; trashflag++){
					if(NOISY_TEST == 1){
						printf("P%d w/ card %d of %d, [%d]flg\n",p+1, cardPos+1, handCount, trashflag);
					}
					G.handCount[p] = handCount;
					G.playedCardCount = played;

					for(i = 0; i < handCount; i++){ // fill hand with 1
			        		G.hand[p][i] = 1;
						if(i == (handCount - 1)){
							G.hand[p][i] = 5;
						}
			        	}

					discardCard(cardPos/2, p, &G, trashflag);

					if(trashflag < 1){
					    numPlayed = played + 1;
					}else{
					    numPlayed = played;
					}

					if (NOISY_TEST == 1){
						printf("handCount = %d, expected = %d\n", G.handCount[p], handCount - 1);
						printf("playedCardCount = %d, expected = %d\n", G.playedCardCount, numPlayed);
						if(G.handCount[p] >= 1){
							printf("cardPos has %d, expected 5\n",G.hand[p][cardPos/2]);
						}else{
							printf("cardPos has %d, expected -1\n",G.hand[p][cardPos/2]);
						}
					}

					if(G.handCount[p] == handCount - 1){ // check if card count reduced
						if(NOISY_TEST == 1){
							printf("pass - number of card count\n");
						}
						suiteflag = 0;
					}else{
						if(NOISY_TEST == 1){
							printf("fail - number of card count\n");
						}
						suiteflag = 1;
					}

					if(G.playedCardCount == numPlayed){ // check if playedcard count matches
						if(NOISY_TEST == 1){
							printf("pass - played count\n");
						}
						suiteflag = 0;
					}else{
						if(NOISY_TEST == 1){
							printf("fail - played count\n");
						}
						suiteflag = 1;
					}

					if(G.handCount[p] >= 1){
						if(G.hand[p][cardPos/2] == 5){ // check if card was moved over properly
							if(NOISY_TEST == 1){
								printf("pass - card moved\n");
							}
							suiteflag = 0;
						}else{
							if(NOISY_TEST == 1){
								printf("fail - card moved\n");
							}
							suiteflag = 1;
						}
					}else{
						if(G.hand[p][cardPos/2] == -1){ // check for when last card is used
							if(NOISY_TEST == 1){
								printf("pass - card moved\n");
							}
							suiteflag = 0;
						}else{
							if(NOISY_TEST == 1){
								printf("fail - card moved\n");
							}
							suiteflag = 1;
						}
					}
				}
			}
			played++; // card was played and discarded
		}
	}

	if(suiteflag == 0){
		printf("All tests passed!\n");
	}else{
		printf("discardCard Tests failed!\n");
	}

	return 0;
}
