#include "dominion.h"
#include "dominion_helpers.h"
#include <string.h>
#include <stdio.h>
#include <assert.h>
#include "rngs.h"
#include <stdlib.h>

#define TESTCARD "Adventurer"
#define NOISY_TEST 0 // set NOISY_TEST to 0 to remove printfs from output

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
	int unused = 0;
	int k[10] = {adventurer, embargo, village, minion, mine, cutpurse, sea_hag, tribute, smithy, great_hall};
	struct gameState G, test;

	int handpos = 0, choice1 = 0, choice2 = 0, choice3 = 0, bonus = 0;
	int seed = 1000;
	int numPlayers = 2;
	int thisPlayer = 0;

	initializeGame(numPlayers, k, seed, &G); // initialize a game state
	memcpy(&test,&G,sizeof(struct gameState)); // copy state to compare

	int retval = cardEffect(adventurer, choice1, choice2, choice3, &test, handpos, &bonus);

	printf("--- Testing Card: %s ---\n", TESTCARD);
	if(NOISY_TEST == 1){
		printf("---- Test: [top]copper|silver|village|...[bottom] ----\n");
	}
	//printf("%d,%d,%d | ",test.deck[thisPlayer][0],test.deck[thisPlayer][1],test.deck[thisPlayer][2]);
	test.deck[thisPlayer][0] = copper;
	test.deck[thisPlayer][1] = silver;
	test.deck[thisPlayer][2] = village;

	newCards = 2; // 2 cards (copper,silver)
	discarded = 1; // cards discarded (effect)
	unused = 0; //village should not be reached
	int a0 = assertFunc(retval == 0, "card used"); //successful return from CardEffect
	int a1 = assertFunc(test.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, "hand +1");
	//printf("%d v %d\n",test.handCount[thisPlayer], G.handCount[thisPlayer] + newCards - discarded);
	int a2 = assertFunc(test.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards, " deck -2");
	int a3 = assertFunc(test.coins == G.coins, "coins"); // number of coins unchanged by smithy
	int a4 = assertFunc(test.whoseTurn == G.whoseTurn, "player turn unchanged"); // player turn not changed
	int a5 = assertFunc(test.numActions == G.numActions, "actions"); //actions unchanged
	int a6 = assertFunc(test.numBuys == G.numBuys, "buy phase" ); //number of buy remain the same
	int a7 = assertFunc(test.playedCardCount == G.playedCardCount + discarded, "card used"); //card discarded after use
	//printf("%d v %d\n",test.playedCardCount, G.playedCardCount + discarded);
	int case1 = a0 + a1 + a2 + a3 + a4 + a5 + a6 + a7;

	if(NOISY_TEST == 1){
		printf("---- Test: [top]silver|mine|gold|...[bottom] ----\n");
	}
	test.deck[thisPlayer][0] = silver;
	test.deck[thisPlayer][1] = mine;
	test.deck[thisPlayer][2] = gold;
	newCards = 3; // 3 cards (silver,mine,gold)
	discarded = 1; // cards discarded (effect)
	unused++; //mine not used
	int b0 = assertFunc(retval == 0, "card used"); //successful return from CardEffect
	int b1 = assertFunc(test.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, " +1 hand");
	int b2 = assertFunc(test.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards, " deck -3");
	//printf("%d v %d\n",test.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards);
	int b3 = assertFunc(test.coins == G.coins, "coins"); // number of coins unchanged by smithy
	int b4 = assertFunc(test.whoseTurn == G.whoseTurn, "player turn unchanged"); // player turn not changed
	int b5 = assertFunc(test.numActions == G.numActions, "actions"); //actions + 2
	int b6 = assertFunc(test.numBuys == G.numBuys, "buy phase" ); //number of buy remain the same
	int b7 = assertFunc(test.playedCardCount == G.playedCardCount + discarded, "card used"); //card discarded after use
	//printf("%d v %d\n",test.playedCardCount, G.playedCardCount + discarded);
	int case2 = b0 + b1 + b2 + b3 + b4 + b5 + b6 + b7;

	if(NOISY_TEST == 1){
		printf("---- Test: [top]smithy|gold|copper|...[bottom] ----\n");
	}
	test.deck[thisPlayer][0] = smithy;
	test.deck[thisPlayer][1] = gold;
	test.deck[thisPlayer][2] = copper;
	newCards = 3; // 2 cards (copper,silver)
	discarded = 1; // cards discarded (effect)
	unused = 1; //smithy not used
	int c0 = assertFunc(retval == 0, "card used"); //successful return from CardEffect
	int c1 = assertFunc(test.handCount[thisPlayer] == G.handCount[thisPlayer] + newCards - discarded, " +1 card");
	int c2 = assertFunc(test.deckCount[thisPlayer] == G.deckCount[thisPlayer] - newCards, " three less in deck");
	//printf("%d v %d\n",test.deckCount[thisPlayer], G.deckCount[thisPlayer] - newCards);
	int c3 = assertFunc(test.coins == G.coins, "coins"); // number of coins unchanged by smithy
	int c4 = assertFunc(test.whoseTurn == G.whoseTurn, "player turn unchanged"); // player turn not changed
	int c5 = assertFunc(test.numActions == G.numActions, "actions"); //actions + 2
	int c6 = assertFunc(test.numBuys == G.numBuys, "buy phase" ); //number of buy remain the same
	int c7 = assertFunc(test.playedCardCount == G.playedCardCount + discarded, "card used"); //card discarded after use
	//printf("%d v %d\n",test.playedCardCount, G.playedCardCount + discarded);
	int case3 = c0 + c1 + c2 + c3 + c4 + c5 + c6 + c7;
	if(NOISY_TEST == 1){
		printf("Test: Other\n");
	}
	int t1 = assertFunc(G.numPlayers == test.numPlayers, "num players"); 
	int t2 = assertFunc(G.handCount[thisPlayer+1] == test.handCount[thisPlayer+1], "opp.player hand unaffected"); 
	int t3 = assertFunc(G.deckCount[thisPlayer+1] == test.deckCount[thisPlayer+1], "opp.player deck unaffected"); 
	int t4 = assertFunc(G.discardCount[thisPlayer+1] == test.discardCount[thisPlayer+1], "opp.player discard unaffected"); 
	int case0 = t1 + t2 + t3 + t4;

	int results = case1 + case2 + case3 + case0;
	if(results == 0){
		printf("pass - all test\n");
	}else{
		printf("fail - not all test pass\n");
	}
	return 0;
}

