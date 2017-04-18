// Inline.cpp
//
// This file contains routines where functionality is inlined in assembly language
//

#include <windows.h>
#include <stdio.h>

char *gWordList[] = {

	"absorbed",
	"abstracted",
	"advertisement",
	"ants",
	"apparatus",
	"appear",
	"arm",
	"bird",
	"blue",
	"boundless",
	"broad",
	"cause",
	"ceaseless",
	"complete",
	"confuse",
	"cooperative",
	"cover",
	"credit",
	"devilish",
	"dirty",
	"discreet",		// #20, 20 * 4 = 80 == (0x50), 21st word in list, #20
	"ear",
	"eatable",
	"experience",
	"fix",
	"flower",
	"friend",
	"furtive",
	"harm",
	"harmony",
	"heady",
	"heap",
	"ignore",
	"infamous",
	"jittery",
	"language",
	"learn",
	"line",
	"live",
	"maid",
	"melted",
	"memory",
	"nasty",
	"neck",
	"noise",
	"orange",
	"peaceful",
	"pine",
	"piquant",
	"pollution",
	"precede",
	"profit",
	"quiver",
	"quizzical",
	"request",
	"rustic",
	"satisfying",
	"scatter",
	"science",
	"second-hand",
	"shade",
	"sharp",
	"shivering",
	"show",
	"solid",
	"sore",
	"squealing",
	"start",
	"system",
	"terrible",
	"test",
	"throne",
	"tooth",
	"womanly",
	"xylophone",
	"zebra"
};

// searches the gWordList[] for the word passed in
// do a binary search and count the number of checks
// return -1 if the word is not found
int inlineBinarySearch(char *searchWord, int *numSteps)
{
	int elementNumber = -1;

	*numSteps = 0;
	 
	int length = 75;
	int x = strlen(searchWord);


	__asm {

		//mov elementNumber, 4	// puts a 4 in the return value
		//mov edi, numSteps		// how to access numSteps
		//mov[edi], 25

		//xor eax, eax
		//lea edi, gWordList		// this gets the base address of array of character pointers
		//mov esi, [edi]			// address of first word in esi
		//mov al, [esi]			// first letter of first word
		//mov elementNumber, eax	// return 97 which is character 'a'

		//add edi, 0x128			// get to address of 21st word = "discreet"  (word #20 in list, 20 * 4 = 80)
		//mov esi, [edi]			// get address where 21st word is stored in memory
		//mov al, [esi + 1]			// put 2nd character from "discreet" which is an 'i' = 0x69 (105)
		//mov elementNumber, eax

		xor edx, edx

		lea edi, gWordList
		mov esi, [edi]
		mov ecx, esi
		add ecx, 75   //max
		add esi, 37   //mid
		
		LOAD:
		mov eax, [esi]
		mov ebx, [searchWord]
		xor edx, edx

		CMPCHARS:
		cmp ebx, eax
		je INC
		jl LOW
		jg HIGH

		INC:
		lea eax, searchWord
		inc eax
		mov ebx, [eax]
		mov edi, esi
		inc edi
		mov eax, [edi]
		cmp edx, x
		je FOUND
		j CMPCHARS

		HIGH:
		mov eax, ecx  ////move my max to edx
		sub eax, esi	//subtract max - mid
		mov ebx, 2
		div ebx		    // divide eax by 2
		sub esi, eax   // max - eax
		cmp esi, ecx   //compare my max and mid so if the same then I know I am on last word in array to compare
		je LASTCMP
		j LOAD			//otherwise load the new word based on esi...

		LOW:
		sub ecx, 1		//move my max down by 1
		mov eax, esi	//put my mid into eax for div
		mov ebx, 2
		div ebx			//div by 2
		mov esi, eax    //put result back into esi this is my new mid
		cmp ecx, esi   //compare my max and low so if the same then I know I am on last word in array to compare
		je LASTWORD		
		j LOAD			//otherwise load the new word based on esi...

		LASTINC:
		inc ebx 
		inc eax
	    inc edx
		cmp edx, x
		je FOUND

		LASTCMP:
		comp eax, ebx
		je LASTINC

		NOTFOUND:
		mov elementNumber,-1


		FOUND:
		mov elementNumber, [ecx]


	}

	return elementNumber;
} // inlineBinarySearch


void printBytes(char *data, int length)
{
	int x;

	for (x = 0; x < length; x++)
	{
		if ((x & 0xF) == 0) printf("\n");
		printf("%02X ", (unsigned char)data[x]);
	}
	printf("\n\n");

	return;
} // printBytes


void callInLineFunctions()
{
	int x, tmpi;
	char dstA[32], srcA[64];	// no overlap
	char overlap[256], *ptrDstO, *ptrSrcO;
	char word[64];

	// get the length of the word list
	int gListLength = sizeof(gWordList) / sizeof(char *);		// get size of word list

	strcpy(word, "zebra");
	tmpi = inlineBinarySearch(word, &x);
	if (tmpi == -1)
		printf("The word \"%s\" not found!\n\n", word);
	else
		printf("The word \"%s\" was found at element number %d in %d steps.\n\n", word, tmpi, x);

	strcpy(word, "apparatus");
	tmpi = inlineBinarySearch(word, &x);
	if (tmpi == -1)
		printf("The word \"%s\" not found!\n\n", word);
	else
		printf("The word \"%s\" was found at element number %d in %d steps.\n\n", word, tmpi, x);

	strcpy(word, "dude");
	tmpi = inlineBinarySearch(word, &x);
	if (tmpi == -1)
		printf("The word \"%s\" not found!\n\n", word);
	else
		printf("The word \"%s\" was found at element number %d in %d steps.\n\n", word, tmpi, x);

	strcpy(word, "orange");
	tmpi = inlineBinarySearch(word, &x);
	if (tmpi == -1)
		printf("The word \"%s\" not found!\n\n", word);
	else
		printf("The word \"%s\" was found at element number %d in %d steps.\n\n", word, tmpi, x);

	strcpy(word, "livefeed");
	tmpi = inlineBinarySearch(word, &x);
	if (tmpi == -1)
		printf("The word \"%s\" not found!\n\n", word);
	else
		printf("The word \"%s\" was found at element number %d in %d steps.\n\n", word, tmpi, x);

	exit(0);
} // callInLineFunctions


int main(void)
{
	callInLineFunctions();


	return 0;
}