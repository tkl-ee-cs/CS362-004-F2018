#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
	

#define NOISY_TEST 0 // set NOISY_TEST to 0 to remove printfs from output
	
int main() {
	int i, j, m;
	int seed = 1000;
	int numPlayer = 3;
	int p;
	int tmp = 0;
	int deckCount;
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	int suiteflag = 0;
	int tmpdeck[10] = {0,1,2,3,4,5,6,7,8,9};
	int maxDeckCount = 10;

	struct gameState G;

	printf ("TESTING Shuffle():\n");

	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &G); // initialize a new game

	for (p = 0; p < numPlayer; p++){

		if (NOISY_TEST == 1){
			printf("Test deck for player%d.\n", p+1);
		}

		m = shuffle(p, &G);
		if(m == 0){ //assert that it returns sucessfully
			if(NOISY_TEST == 1){
				printf("pass - shuffle return\n");
			}
			suiteflag = 0;
		}else{
			if(NOISY_TEST == 1){
				printf("fail - shuffle return\n");
			}
			suiteflag = 1;
		}

		for (deckCount = 1; deckCount <= maxDeckCount; deckCount++){
			if (NOISY_TEST == 1){
				printf("Test player%d with deck of size %d.\n", p+1, deckCount);
			}
			G.deckCount[p] = deckCount;

			for(i = 0; i < maxDeckCount; i++){
				G.deck[p][i] = tmpdeck[i];
			}

			m = shuffle(p,&G);

			for(i = 0; i < maxDeckCount; i++){ // check through whole deck
				tmp = 0;
				for(j = 0; j < maxDeckCount; j++){ //inner loop to check for values
					if(G.deck[p][j] == tmpdeck[i]){
						tmp++; //check how many times a card is present
					}
				} 
				if(tmp > 1){
					if (NOISY_TEST == 1){
						printf("fail - shuffle move, dupe card");
					}
					suiteflag = 1;
				}else if(tmp == 0){
					if (NOISY_TEST == 1){
						printf("fail - shuffle move, card lost");
					}
					suiteflag = 1;
				}else if(tmp == 1){
					if (NOISY_TEST == 1){
						printf("pass - shuffle move");
					}
				}
			}
		}

		if (NOISY_TEST == 1){
			printf("Empty deck evaluated for player%d.\n", p+1);
		}
		G.deckCount[p] = 0; // make the deck empty
		assert(shuffle(p,&G) == -1); 


		m = shuffle(p, &G); 
		if(m == -1){ //assert when that it returns handles empty hand
			if(NOISY_TEST == 1){
				printf("pass - empty caught\n");
			}
			suiteflag = 0;
		}else{
			if(NOISY_TEST == 1){
				printf("fail - empty caught\n");
			}
			suiteflag = 1;
		}
	}
	if(suiteflag == 0){
		printf("All tests passed!\n");
	}else{
		printf("shuffle Tests failed!\n");
	}

	return 0;
}

