#include <stdio.h>
#include <stdlib.h>

#include <string.h>
#include <time.h>

#define TO_MIN(x,y) (double)(((x)-(y)) / 60.0)
#define CALC_WPM(x, y) ((x) / 5 / (y))

#define MAX_SIZE 5000 
#define MAX_DICTLINES 236000 
#define DICT_FILE "/usr/share/dict/words"
FILE* dict;
char* dict_mem;

typedef struct {
	unsigned int max_words;
	char words_buffer[MAX_SIZE];
} typing_test;

typing_test test;

void play(void)
{
	size_t word_buff_size = strlen(test.words_buffer), user_buff_size;
	time_t start, finish;

	char user_buffer[word_buff_size+1] = {};
	int typos = 0, i = 0;

	time(&start);
	__builtin_printf("%s\n\nType it!\n\n", test.words_buffer);
	scanf("%[a-zA-Z ]", user_buffer);
//	strcpy(user_buffer, test.words_buffer);
	time(&finish);

	user_buffer[word_buff_size] = '\0';

	for(; i != word_buff_size; i++) if(test.words_buffer[i] != user_buffer[i]) typos++;

	printf("\n\nYou typed this test at %.1f WPM with %d typos\n", 
			CALC_WPM(word_buff_size, TO_MIN(finish,start)) ,
			typos);
}

void getrandword(void)
{
	char word[64];
	int i = rand() % MAX_DICTLINES + 1, x = 0;

	for(; i != 0; x++) if(*(dict_mem+x) == '\n') i--;
	for(i = 0; *(dict_mem+x) != '\n'; x++, i++) word[i] = *(dict_mem+x);
	word[i++] = ' ';
	word[i] = '\0';

	if(strlen(test.words_buffer) + strlen(word) >= MAX_SIZE) exit(1);
	strcat(test.words_buffer, word);
}

void copydict(void)
{
	long filesize;

	fseek(dict, 0, SEEK_END);
	filesize = ftell(dict);
	rewind(dict);
	if(filesize <= 0) exit(1);

	dict_mem = (char*)malloc(filesize+1);
	if(!dict_mem)
	{
		perror("malloc");
		fclose(dict);
		exit(1);
	}

	fread(dict_mem, 1, filesize, dict);
	dict_mem[filesize] = '\0';

	fclose(dict);

	for(; test.max_words != 0; test.max_words--) getrandword();
	test.words_buffer[strlen(test.words_buffer)-1] = '\0';

	free(dict_mem);
}

int readdict(void)
{
	dict = fopen(DICT_FILE, "r");
	if(dict == NULL)

	{
		__builtin_printf("Can't read dict file, make sure DICT_FILE exists");
		return 0;
	}

	copydict();
	play();

	return 1;
}

void maketest(void)
{
	__builtin_printf("%d random words will be generated\n\n", test.max_words);
}

int main(int argc, char *argv[])
{
	srand(time(NULL));

	if(argc < 2)
	{
		__builtin_printf("You need to specify a positive number for how many words to generate!");
			return 1;
	}
	test.max_words = atoi(argv[1]);
	maketest();

	if(!readdict()) exit(1);
	return 0;
}
