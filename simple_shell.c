#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define INPUT_SIZE 256
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
void tokenize( char *input, char **args[] ) {
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
//		printf("%s ", *args[num_read]);

		/* Increment counter */
		num_read++;

		/* Read next argument */
		//FIXME
		read = strsep( &input, " " );
		/*
		if (read != NULL) {
			read = strsep( &input, " " );
		}
		*/
	}

//	args = &tokens;
}

int main() {
	bool running = true;

	char *input;

	char **args[MAX_ARGS];

	/* Allocate memory */
	//args = (char ***)malloc( MAX_ARGS * sizeof( char ** ) );
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

//		printf("x%sx\n", *args[1]);
		printf("x%sx\n", *args[0]);
		printf("%d\n", strcmp( *args[0], "quit\n" ) );
		if ( strcmp( *args[0], "quit\n" ) == 0 ) {
			/* End loop operation */
			running = false;
			printf("%s\n", *args[0]);
		}
		else {
			/* Fork child and run command */
			pid_t pid;
			int status;
		     	pid = fork();
			if ( pid < 0 ) {
				perror( "Fork failure" );
				exit( 1 );
			}
			else if ( pid == 0 ) {
				//FIXME print child info and execute command
			}
			else {
				//FIXME Wait for child to finish then retrieve runtime info
				waitpid( pid, &status, 0 );

			}
		}
	}

	/* Freeing memory */
	free( input );
	free( args );
	/*
	for (int i = 0; i < MAX_ARGS; i++)
		free( args[i] );
	*/
}
