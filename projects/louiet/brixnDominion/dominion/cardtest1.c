#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Smithy"
#define NOISY_TEST 1 // set NOISY_TEST to 0 to remove printfs from output

int assertFunc(int testassert, char * testmsg){

	if(testassert == 1){
		if(NOISY_TEST == 1){
			printf("pass - %s\n", testmsg);
		}
		return 0;
 	}else if(testassert == 0){
		if(NOISY_TEST == 1){
			printf("fail - %s\n", testmsg);
		}
		return 1;
	}else{
		return 0;
	}
}

int main() {
	int newCards = 0;
	int discarded = 1;
	//int xtraCoins = 0;
	//int shuffledCards = 0;
	//int extraBuy = 0;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, council_room};
	struct gameState G, test;

	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;

	initializeGame(numPlayers, k, seed, &G); // initialize a game state
	memcpy(&test,&G,sizeof(struct gameState)); // copy state to compare

	int retval = cardEffect(smithy, choice1, choice2, choice3, &test, handpos, &bonus);

	printf("--- Testing Card: %s ---\n", TESTCARD);
	newCards = 3; // draw 3 cards (effect)
	discarded = 1; // card discarded

	int t0 = assertFunc(retval == 0, "card used"); //successful return from CardEffect
	int t1 = assertFunc(test.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, " three cards");
	//printf("%d|%d\n",test.handCount[thisPlayer],G.handCount[thisPlayer] + newCards - discarded);
	int t2 = assertFunc(test.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards, " three less in deck");
	int t3 = assertFunc(test.coins == G.coins, "coins"); // number of coins unchanged by smithy
	int t4 = assertFunc(test.whoseTurn == G.whoseTurn, "player turn"); // player turn not changed
	int t5 = assertFunc(test.numActions == G.numActions, "actions"); //actions not affected
	int t6 = assertFunc(test.numBuys == G.numBuys, "buy phase" ); //number of busy remain the same
	int t7 = assertFunc(test.playedCardCount == G.playedCardCount + discarded, "card used"); //card discarded after use
	//printf("%d|%d\n",test.playedCardCount, G.playedCardCount + discarded);
	int t8 = assertFunc(G.numPlayers == test.numPlayers, "num players"); 
	int t9 = assertFunc(G.handCount[thisPlayer+1] == test.handCount[thisPlayer+1], "opp.player hand unaffected"); 
	int ta = assertFunc(G.deckCount[thisPlayer+1] == test.deckCount[thisPlayer+1], "opp.player deck unaffected"); 
	int tb = assertFunc(G.discardCount[thisPlayer+1] == test.discardCount[thisPlayer+1], "opp.player discard unaffected"); 

	int results = t0 + t1 + t2 + t3 + t4 + t5 + t6 + t7 + t8 + t9 + ta + tb;
	if(results == 0){
		printf("pass - all test\n");
	}else{
		printf("fail - all test\n");
	}
	return 0;
}
