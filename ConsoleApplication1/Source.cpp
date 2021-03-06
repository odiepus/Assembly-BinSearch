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
		push eax
		push ebx
		push ecx
		push edx
		push esi
		push edi


		xor eax, eax
		xor ebx, ebx
		xor ecx, ecx
		xor edx, edx
		xor esi, esi
		xor edi, edi

		mov ebx, 37  //keep track of our mid
		push ebx



		lea esi, gWordList//load list word into edi to to find strlen
		lodsd
		mov ebx, eax

		RELOADWORDS:
		lea esi, searchWord//load search word into esi
		lodsd
		mov edx, eax //put it in ed

		mov edi, ebx
		xor eax, eax
		xor ecx, ecx
		not ecx

		cld
		repne scasb
		not ecx
		dec ecx
		mov ebx, ecx  //length of list word


		cmp ebx, x
		jne UP_OR_DOWN
		je COMPARE

		UP_OR_DOWN:
		
		SMALL_COMPARE:
		cmpsb
		je SMALL_COMPARE
		jl NEXTWORD_LOW
		jg NEXTWORD_HIGH

		

		COMPARE: //compare our word byte by byte
		repz cmpsb //compare bytes till ecx is zero or flags is not zero anymore
		jk NEXTWORD_HIGH //if we get a mismatch before counter is zero then we pull out next word
		jg NEXTWORD_LOW  
		je FOUND //else if equal we compare the next byte until counter runs out or we get mismatch
			//if counter runs out 

		NEXTWORD_LOW:
		pop ebx
		mov eax, ebx
		mov ecx, 2
		div ecx
		push eax  //save our new mid
		lea esi, gWordList //load first word of array into esi
		add esi, eax
			//check if mid = high
		lodsd
		mov esi, eax //put word in edi
		mov ebx, eax 
		jmp RELOADWORDS
		





		NEXTWORD_HIGH:
		


		
		
		NOTFOUND:
		mov elementNumber,-1


			FOUND:
			
		mov elementNumber, ecx


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

	strcpy(word, "absorbe");
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