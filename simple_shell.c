#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define INPUT_SIZE 256
#define MAX_ARGS 255

/*
 * Read in command line input to execute.
 * @param input Pointer to a char * where user input is stored
 */
void read_input( char **input ) {
	/* Initial buffer setup */
	char *buffer;
	buffer = (char *)malloc( INPUT_SIZE * sizeof( char ) );
	if ( buffer == NULL ) {
		printf( "Malloc failure" );
	}

	/* Read into buffer */
	fgets( buffer, INPUT_SIZE, stdin );

	*input = buffer;
}

/*
 * Tokenize the provided input and store the token into the array.
 * @param input Buffer of user input to parse
 * @param args Array of char * to store arguments into
 */
void tokenize( char *input, char *args[] ) {
	char *read;
	int num_read = 0;
	read = strsep( &input, " " );
	while ( read != NULL && num_read < MAX_ARGS ) {
		/* Store into array */
		args[num_read] = read;

		/* Increment counter */
		num_read++;

		/* Read next argument */
		read = strsep( &input, " " );
	}

	/* Replace ending whitespace */
	if ( num_read > 0 ) {
		char *end = args[ num_read - 1 ] + strlen( args[ num_read - 1 ] ) - 1;
		if ( isspace( (unsigned char)*end ) ) {
			*end = *"\0";
		}
	}
	
	/* Ending args with NULL for execvp */
	args[ num_read < MAX_ARGS ? num_read : MAX_ARGS - 1 ] = NULL;
}

/*
 * A simple shell emulation that takes in user input and executes it.
 */
int main() {
	/* Program state */
	bool running = true;

	/* User input */
	char *input;

	/* List of command line arguments */
	char *args[MAX_ARGS];

	/* Begin execution commands */
	while ( running ) {
		/* Await input */
		printf( "> " );

		/* Read input */
		read_input( &input );

		/* Get list of arguments */
		tokenize( input, args );

		/* Execute command input */
		if ( strcmp( args[0], "exit" ) == 0 ) {
			/* End loop operation */
			running = false;
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
				/* Execute command */
				if ( execvp( args[0], &args[0] ) < 0 ) {
					perror( "Exec failed" );
					exit( 1 );
				}
			}
			else {
				//FIXME Wait for child to finish then retrieve runtime info
				waitpid( pid, &status, 0 );

			}
		}
	}

	/* Freeing memory */
	free( input );

	return 0;
}
