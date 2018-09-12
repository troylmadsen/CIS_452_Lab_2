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

#define BUFFER_SIZE 256

/*
 * A shell emulator that takes in user commands and executes them. Type "quit" to exit the program.
 * @Version 1.0
 * @Author Troy Madsen
 * @Author Tim DeVries
 * @Date: 2018/09/10
 */

/*
 * Read in command line input to execute.
 * @param input Pointer to a char * where user input is stored
 */
void read_input( char **input ) {
	/* Initial buffer setup */
	char *buffer;
	int buffer_size = BUFFER_SIZE;
	buffer = (char *)malloc( buffer_size * sizeof( char ) );
	if ( buffer == NULL ) {
		perror( "Malloc failure" );
		exit( 1 );
	}

	int num_read = 0;
	char c;
	c = getc( stdin );
	while ( c != '\n' ) {
		/* Store character in buffer */
		*(buffer + num_read) = c;

		/* Increment counter */
		num_read++;

		/* Resize buffer if full */
		if ( num_read == buffer_size ) {
			/* Store pointer to old buffer */
			char *temp = buffer;

			/* Double buffer size and reallocate */
			buffer_size =  buffer_size * 2;
			buffer = (char *)malloc( buffer_size * sizeof( char ) );
			if ( buffer == NULL ) {
				perror( "Malloc error" );
				exit( 1 );
			}

			/* Copy old buffer to new buffer */
			for ( int i = 0; i < buffer_size / 2; i++ ) {
				*(buffer + i) = *(temp + i);
			}

			/* Deallocate old buffer */
			free( temp );
		}

		/* Read in next char */
		c = getc( stdin );
	}

	/* Terminate the string with a NULL pointer */
	*(buffer + num_read) = '\0';

	*input = buffer;
}

/*
 * Tokenize the provided input and store the token into the array.
 * @param input Buffer of user input to parse
 * @param args Array of char * to store arguments into
 */
void tokenize( char *input, char ***args ) {
	/* Initial buffer setup */
	char **buffer;
	int buffer_size = BUFFER_SIZE;
	buffer = (char **)malloc( buffer_size * sizeof( char *) );
	if ( buffer == NULL ) {
		perror( "Malloc failure" );
		exit( 1 );
	}

	int num_read = 0;
	char *read;
	read = strsep( &input, " " );
	while ( read != NULL ) {
		/* Store into array */
		*(buffer + num_read) = read;

		/* Increment counter */
		num_read++;

		/* Resize buffer if full */
		if ( num_read == buffer_size ) {
			/* Store pointer to old buffer */
			char **temp = buffer;

			/* Double buffer size and reallocate */
			buffer_size =  buffer_size * 2;
			buffer = (char **)malloc( buffer_size * sizeof( char * ) );
			if ( buffer == NULL ) {
				perror( "Malloc error" );
				exit( 1 );
			}

			/* Copy old buffer to new buffer */
			for ( int i = 0; i < buffer_size / 2; i++ ) {
				*(buffer + i) = *(temp + i);
			}

			/* Deallocate old buffer */
			free( temp );
		}

		/* Read next argument */
		read = strsep( &input, " " );
	}

	/* Ending args with NULL for execvp */
	*(buffer + num_read) = NULL;

	*args = buffer;
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
	char **args = NULL;

	/* Child uasge statistics */
	long t_sec = 0;
	long t_usec = 0;
	long t_nivcsw = 0;

	/* Begin execution commands */
	printf( "Run commands. \"quit\" to exit.\n" );
	while ( running ) {
		/* Await input */
		printf( "> " );

		/* Read input */
		read_input( &input );

		/* Get list of arguments */
		tokenize( input, &args );

		/* Execute command input */
		if ( strcmp( *(args + 0), "quit" ) == 0 ) {
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
				if ( execvp( *(args + 0), args ) < 0 ) {
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
		free( args );
	}

	return 0;
}
