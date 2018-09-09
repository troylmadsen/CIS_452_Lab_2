#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define INPUT_SIZE 64
#define MAX_ARGS 255
#define ARR_SIZE(x) ( sizeof(x) / sizeof((x)[0]) )

/*
 * Read from STDIN
 * FIXME
 */
void read_input( char **input ) {
	/* Initial buffer setup */
	char *buffer;
	buffer = (char *)malloc( INPUT_SIZE * sizeof( char ) );

	/* Read into buffer */
	fgets( buffer, INPUT_SIZE, stdin );

	*input = buffer;
}

//FIXME
void tokenize( char *input, char ***args ) {
//	char (*tokens)[MAX_ARGS] = NULL;
	char *read;
	int num_read = 0;
	read = strsep( &input, " " );
	while ( read != NULL && num_read < MAX_ARGS) {
		/* Allocate memory for next read */
//		tokens[num_read] = (char **)malloc( sizeof( char * ) );

		/* Store into buffer */
//		*tokens[num_read] = *read;
		*args[num_read] = read;
		printf("%s\n", *args[num_read]);

		/* Increment counter */
		num_read++;

		/* Read next argument */
		//FIXME
		if (read != NULL) {
			read = strsep( &input, " " );
		}
	}

//	args = &tokens;
}

int main() {
	bool running = true;

	char *input;

	char ***args;

	/* Allocate memory */
	args = (char ***)malloc( MAX_ARGS * sizeof( char ** ) );
	for ( int i = 0; i < MAX_ARGS; i++ ) {
		args[i] = (char **)malloc( sizeof( char * ) );
	}

	//FIXME check if args is NULL due to malloc failure

	while ( running ) {
		/* Await input */
		printf( "> ");

		/* Read input */
		read_input( &input );

		/* Get list of arguments */
		tokenize( input, args );

/*
		if ( strcomp( input, "quit") ) {

		}
		else {

		}
*/
	}

	/* Freeing memory */
	free( input );
	free( args );
	/*
	for (int i = 0; i < MAX_ARGS; i++)
		free( args[i] );
	*/
}
