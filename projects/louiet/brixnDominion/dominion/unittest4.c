#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
	

#define NOISY_TEST 0 // set NOISY_TEST to 0 to remove printfs from output
	
int main() {
	int seed = 1000;
	int endflag;
	int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
	struct gameState G;
	struct gameState H;
	int i;
	int numPlayer = 3;
	int suiteflag = 0;

	printf ("TESTING isGameOver():\n");

	if(NOISY_TEST == 1){
		printf("Test start game state for end game condition.\n");
	}
	memset(&G, 23, sizeof(struct gameState));   // clear the game state
	initializeGame(numPlayer, k, seed, &G); // initialize a new game
  	memcpy (&H, &G, sizeof(struct gameState)); //memcpy to store initalized state

  	endflag = isGameOver(&G);
	if(endflag == 0){ //check if the game has ended at start, should return 0
		if(NOISY_TEST == 1){
			printf("pass - game start\n");
		}
		suiteflag = 0;
	}else{
		if(NOISY_TEST == 1){
			printf("fail - game start\n");
		}
		suiteflag = 1;
	}

	if(NOISY_TEST == 1){
		printf("Test end game states for end game condition.\n");
	}
	G.supplyCount[province] = 0; // set the supply of province to 0
	endflag = isGameOver(&G);
	if(endflag == 1){ //check if the game has ended, should return 1
		if(NOISY_TEST == 1){
			printf("pass - game end by victory\n");
		}
		suiteflag = 0;
	}else{
		if(NOISY_TEST == 1){
			printf("fail - game end by victory\n");
		}
		suiteflag = 1;
	}

	memcpy (&G, &H, sizeof(struct gameState)); // restore the state with memcpy
	for(i = 0;i < 3; i++){ //set the 3 supplycount to 0
		G.supplyCount[i] = 0;
	}
	endflag = isGameOver(&G);
	if(endflag == 1){ //check if the game has ended, should return 1
		if(NOISY_TEST == 1){
			printf("pass - game end by supply\n");
		}
		suiteflag = 0;
	}else{
		if(NOISY_TEST == 1){
			printf("fail - game end by supply\n");
		}
		suiteflag = 1;
	}

	if(suiteflag == 0){
		printf("All tests passed!\n");
	}else{
		printf("shuffle Tests failed!\n");
	}

	return 0;
}


