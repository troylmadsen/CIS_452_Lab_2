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
void tokenize( char *input, char *args[] ) {
	char *read;
	int num_read = 0;
	read = strsep( &input, " " );
	while ( read != NULL && num_read < MAX_ARGS) {
		/* Store into array */
		args[num_read] = read;

		/* Increment counter */
		num_read++;

		/* Read next argument */
		read = strsep( &input, " " );
	}
}

int main() {
	/* Program state */
	bool running = true;

	/* User input */
	char *input;

	/* List of command line arguments */
	char *args[MAX_ARGS];

	/* Allocate memory */
	/*
	for ( int i = 0; i < MAX_ARGS; i++ ) {
		args[i] = (char *)malloc( sizeof( char * ) );
		if ( args[i] == NULL ) {
			perror( "malloc error" );
			for ( int j = 0; j < i; j++ ) {
				free( args[j] );
			}
		}
	}
	*/

	/* Begin execution commands */
	while ( running ) {
		/* Await input */
		printf( "> ");

		/* Read input */
		read_input( &input );

		/* Get list of arguments */
		tokenize( input, args );

		/* Execute command input */
		if ( strcmp( args[0], "quit\n" ) == 0 ) {
			/* End loop operation */
			running = false;
		}
		else {
			/* Fork child and run command */
//			pid_t pid;
//			int status;
//		     	pid = fork();
/*			if ( pid < 0 ) {
				perror( "fork failure" );
				exit( 1 );
			} */
//			else if ( pid == 0 ) {
				//FIXME print child info and execute command
//			}
//			else {
				//FIXME Wait for child to finish then retrieve runtime info
//				waitpid( pid, &status, 0 );

//			}
		}
	}

	/* Freeing memory */
	free( input );
	/*
	for ( int i = 0; i < MAX_ARGS; i++ ) {
		free( args[i] );
	}
	*/
}
