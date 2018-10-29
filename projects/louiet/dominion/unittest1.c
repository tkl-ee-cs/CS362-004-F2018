#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
	

#define NOISY_TEST 0 // set NOISY_TEST to 0 to remove printfs from output
	
int main() {
	int seed = 1000;
	int numPlayer = 4; // number of players
	int p, toflag, kings;
	int numPlace = 3; // number of supplyPos piles (see below for 4 piles)
	int hand, deck, supply, discard;
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G;
	int suiteflag = 0;
	printf ("TESTING gainCard():\n");

    memset(&G, 23, sizeof(struct gameState)); // clear the game state
    initializeGame(numPlayer, k, seed, &G); // initialize a new game

	for (p = 0; p < numPlayer; p++){ //increment through players
		hand = 5;
		deck = 10;
		discard = 0;
		supply = 3*(treasure_map+1);

		for (kings = 0; kings <= treasure_map; kings++){ //increment through cards
			if (NOISY_TEST == 1){
				printf("Test player %d with card [%d] from all kingdoms.\n",p+1,kings);
			}
			for (toflag = 0; toflag < numPlace; toflag++){ // increment through supplyPos

				G.handCount[p] = hand; // set the number of cards on hand
				G.deckCount[p] = deck; // set the number of cards in deck
				G.discardCount[p] = discard; // set the number of cards in discard
				G.supplyCount[kings] = supply; // set the number of cards in supply

				gainCard(kings, &G, toflag, p); //call gain function
				supply--; //since a card was gained, reduce supply
				if(toflag == 0){ // placement of card in discard
					discard++;
					if (NOISY_TEST == 1){
						printf("discard = %d, expected = %d\n", G.discardCount[p], discard);
						printf("supply = %d, expected = %d\n", G.supplyCount[kings], supply);
					}		

					if(G.discardCount[p] == discard){ //assert discard
						if(NOISY_TEST == 1){
							printf("pass - discard\n");
						}
						suiteflag = 0;
					}else{
						if(NOISY_TEST == 1){
							printf("fail - discard\n");
						}
						suiteflag = 1;
					}

					if(G.supplyCount[kings] == supply){ //assert supply
						if(NOISY_TEST == 1){
							printf("pass - supply\n");
						}
						suiteflag = 0;
					}else{
						if(NOISY_TEST == 1){
							printf("fail - supply\n");
						}
						suiteflag = 1;
					}

				}else if(toflag == 1){ // placement of card in deck
					deck++;
					if (NOISY_TEST == 1){
						printf("deck = %d, expected = %d\n", G.deckCount[p], deck);
						printf("supply = %d, expected = %d\n", G.supplyCount[kings], supply);
					}		

					if(G.deckCount[p] == deck){ //assert deck
						if(NOISY_TEST == 1){
							printf("pass - supply\n");
						}
						suiteflag = 0;
					}else{
						if(NOISY_TEST == 1){
							printf("fail - supply\n");
						}
						suiteflag = 1;
					}


					if(G.supplyCount[kings] == supply){ //assert supply
						if(NOISY_TEST == 1){
							printf("pass - supply\n");
						}
						suiteflag = 0;
					}else{
						if(NOISY_TEST == 1){
							printf("fail - supply\n");
						}
						suiteflag = 1;
					}

				}else if(toflag == 2){ //placement of card in hand
					hand++;
					if (NOISY_TEST == 1){
						printf("hand = %d, expected = %d\n", G.handCount[p], hand);
						printf("supply = %d, expected = %d\n", G.supplyCount[kings], supply);
					}		

					if(G.handCount[p] == hand){ //assert hand
						if(NOISY_TEST == 1){
							printf("pass - supply\n");
						}
						suiteflag = 0;
					}else{
						if(NOISY_TEST == 1){
							printf("fail - supply\n");
						}
						suiteflag = 1;
					}

					if(G.supplyCount[kings] == supply){ //assert supply
						if(NOISY_TEST == 1){
							printf("pass - supply\n");
						}
						suiteflag = 0;
					}else{
						if(NOISY_TEST == 1){
							printf("fail - supply\n");
						}
						suiteflag = 1;
					}
				}else{}
			}
		}
	}
	if(suiteflag == 0){
		printf("All tests passed!\n");
	}else{
		printf("shuffle Tests failed!\n");
	}
	return 0;
}

