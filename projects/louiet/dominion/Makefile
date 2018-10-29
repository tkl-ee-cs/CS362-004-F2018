CFLAGS= -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

testUpdateCoins: testUpdateCoins.c dominion.o rngs.o
	gcc -o testUpdateCoins -g  testUpdateCoins.c dominion.o rngs.o $(CFLAGS)

testGainCard: unittest1.c dominion.o rngs.o
	gcc -o unittest1 -g  unittest1.c dominion.o rngs.o $(CFLAGS)

testDiscardCard: unittest2.c dominion.o rngs.o
	gcc -o unittest2 -g  unittest2.c dominion.o rngs.o $(CFLAGS)

ShuffleTest: unittest3.c dominion.o rngs.o
	gcc -o unittest3 -g  unittest3.c dominion.o rngs.o $(CFLAGS)

testisGameOver: unittest4.c dominion.o rngs.o
	gcc -o unittest4 -g  unittest4.c dominion.o rngs.o $(CFLAGS)

cardtest1: cardtest1.c dominion.o rngs.o
	gcc -o cardtest1 -g  cardtest1.c dominion.o rngs.o $(CFLAGS)

cardtest2: cardtest2.c dominion.o rngs.o
	gcc -o cardtest2 -g  cardtest2.c dominion.o rngs.o $(CFLAGS)

cardtest3: cardtest3.c dominion.o rngs.o
	gcc -o cardtest3 -g  cardtest3.c dominion.o rngs.o $(CFLAGS)

cardtest4: cardtest4.c dominion.o rngs.o
	gcc -o cardtest4 -g  cardtest4.c dominion.o rngs.o $(CFLAGS)

unittestresults: testGainCard testDiscardCard ShuffleTest testisGameOver dominion.o rngs.o cardtest1 cardtest2 cardtest3 cardtest4
	echo "Results from running unit tests:" > unittestresults.out
	echo "unittest1.c:" >> unittestresults.out
	./unittest1 >> unittestresults.out
	gcov -b unittest1.c >> unittestresults.out
#	cat unittest1.c.gcov >> unittestresults.out
	echo "testDiscardCard.c:" >> unittestresults.out
	./unittest2 >> unittestresults.out
	gcov -b unittest2.c >> unittestresults.out
#	cat unittest2.c.gcov >> unittestresults.out
	echo "ShuffleTest.c:" >> unittestresults.out
	./unittest3 >> unittestresults.out
	gcov -b unittest3.c >> unittestresults.out
#	cat unittest3.c.gcov >> unittestresults.out
	echo "testisGameOver.c:" >> unittestresults.out
	./unittest4 >> unittestresults.out
	gcov -b unittest4.c >> unittestresults.out
#	cat unittest4.c.gcov >> unittestresults.out
	echo >> unittestresults.out
	echo "Results from running card tests:" >> unittestresults.out
	echo "cardtest1.c:" >> unittestresults.out
	./cardtest1 >> unittestresults.out
	gcov -b cardtest1.c >> unittestresults.out
#	cat cardtest1.c.gcov >> unittestresults.out
	echo "cardtest2.c:" >> unittestresults.out
	./cardtest2 >> unittestresults.out
	gcov -b cardtest2.c >> unittestresults.out
#	cat cardtest2.c.gcov >> unittestresults.out
	echo "cardtest3.c:" >> unittestresults.out
	./cardtest3 >> unittestresults.out
	gcov -b cardtest3.c >> unittestresults.out
#	cat cardtest3.c.gcov >> unittestresults.out
	echo "cardtest4.c:" >> unittestresults.out
	./cardtest4 >> unittestresults.out
	gcov -b cardtest4.c >> unittestresults.out
#	cat cardtest4.c.gcov >> unittestresults.out

playdom: dominion.o playdom.c
	gcc -o playdom playdom.c -g dominion.o rngs.o $(CFLAGS)
#To run playdom you need to entere: ./playdom <any integer number> like ./playdom 10*/
testDrawCard: testDrawCard.c dominion.o rngs.o
	gcc  -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

badTestDrawCard: badTestDrawCard.c dominion.o rngs.o
	gcc -o badTestDrawCard -g  badTestDrawCard.c dominion.o rngs.o $(CFLAGS)

testBuyCard: testDrawCard.c dominion.o rngs.o
	gcc -o testDrawCard -g  testDrawCard.c dominion.o rngs.o $(CFLAGS)

testAll: dominion.o testSuite.c
	gcc -o testSuite testSuite.c -g  dominion.o rngs.o $(CFLAGS)

interface.o: interface.h interface.c
	gcc -c interface.c -g  $(CFLAGS)

testme: testme.c
	gcc -o testme testme.c

runtests: testDrawCard 
	./testDrawCard &> unittestresult.out
	gcov dominion.c >> unittestresult.out
	cat dominion.c.gcov >> unittestresult.out

player: player.c interface.o
	gcc -o player player.c -g  dominion.o rngs.o interface.o $(CFLAGS)

all: playdom player 

cleanall: tests clean

tests:
	rm -f unittest1 unittest2 unittest3 unittest4 testDiscardCard testGainCard ShuffleTest testisGameOver cardtest1 cardtest2 cardtest3 cardtest4
clean:
	rm -f *.o playdom.exe playdom player player.exe  *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
