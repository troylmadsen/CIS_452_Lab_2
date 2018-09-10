#include <bsd/sys/time.h>
#include <ctype.h>
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#define INPUT_SIZE 256
#define MAX_ARGS INPUT_SIZE

/*
 * A shell emulator that takes in user commands and executes them. Type "quit" to exit the program.
 * @Version 1.0
 * @Author Troy Madsen
 * @Data: 2018/09/10
 */

/*
 * Read in command line input to execute.
 * @param input Pointer to a char * where user input is stored
 */
void read_input( char **input ) {
	/* Initial buffer setup */
	char *buffer;
	int buffer_size = INPUT_SIZE;
	buffer = (char *)malloc( buffer_size * sizeof( char ) );
	if ( buffer == NULL ) {
		perror( "Malloc failure" );
		exit( 1 );
	}

	/* Read into buffer */
	fgets( buffer, INPUT_SIZE, stdin );


	/* Store input for use */
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

	/* Child uasge statistics */
	long t_sec = 0;
	long t_usec = 0;
	long t_nivcsw = 0;

	/* Begin execution commands */
	while ( running ) {
		/* Await input */
		printf( "> " );

		/* Read input */
		read_input( &input );

		/* Get list of arguments */
		tokenize( input, args );

		/* Execute command input */
		if ( strcmp( args[0], "quit" ) == 0 ) {
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
				/* Wait for child to finish */
				waitpid( pid, &status, 0 );

				/* Retrieve child resource usage */
				struct rusage usage;
				getrusage( RUSAGE_CHILDREN, &usage );
				long c_sec = usage.ru_utime.tv_sec - t_sec;
				long c_usec = usage.ru_utime.tv_usec - t_usec;
				long c_nivcsw = usage.ru_nivcsw - t_nivcsw;
				t_sec = usage.ru_utime.tv_sec;
				t_usec = usage.ru_utime.tv_usec;
				t_nivcsw = usage.ru_nivcsw;

				/* Displaying child resource usage */
				printf( "\n" );
				printf( "Child (%d) CPU usage: %ld seconds and %ld microseconds\n", pid, c_sec, c_usec );
				printf( "Child (%d) Involuntary Context Switches: %ld\n", pid, c_nivcsw );
			}
		}


		/* Freeing memory */
		free( input );
	}

	return 0;
}
