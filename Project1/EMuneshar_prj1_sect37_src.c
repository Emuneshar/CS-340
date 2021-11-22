// Eshwar Muneshar
// Filename: Emuneshar_prj1_sect37.c
// Compile command: gcc Emuneshar_prj1_sect37.c -o Emuneshar_prj1_sect37.exe
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#define SIZ 128


int main(int argc, const char* argv[]) {
	int f1, f2;
	int n;
	
	// checks if we right number of arguments?
	if (argc != 3) {
		const char msgxxxx[] = "Wrong number of command line arguments";
		write(STDOUT_FILENO, "Wrong number of command line arguments", sizeof(msgxxxx) - 1);
		return 1;
	}
	
	
	// Can we access thge source file?
	if ((f1 = open(argv[1], O_RDONLY, 0)) == -1) {
		const char msgxxx[] = "Can't open %s \n";
		write(STDOUT_FILENO, "Can't create %s \n", sizeof(msgxxx) - 1);
		return 2;
	}

	
	// Can we create the target file?
	if ((f2 = creat(argv[2], 0644)) == -1) {
		const char msgxx[] = "Can't create %s \n";
		write(STDOUT_FILENO, "Can't create %s \n", sizeof(msgxx) - 1);
		return 3;
	}
	
	// Array for Pipe ends
	int fd[2];
	pid_t pid;    

	// Checks for a pipe error, if there is a pipe error we will display this to the screen
	if (pipe(fd) == -1) {
		const char msgx[] = "Can't write to screen";
		write(STDOUT_FILENO, "Pipe Failed", sizeof(msgx) - 1);
		return 1;
	}

	pid = fork();

	
	// Runs if there is a fork error, pid would be less than 0
	if (pid < 0) { 
		const char msgex[] = "Fork Failed";
		write(STDOUT_FILENO, "Fork Failed", sizeof(msgex) - 1);
		return 1;
	}

	
	// Parent Process
	if (pid > 0) { 
		// we now need to close the read side of the pipe for Parent Process
		close(fd[0]); 
		char buf[SIZ]; 
	
		// here we have a while loop that will read from f1 in chunks of 128 and then will write that into the pipe.
		const char msg[] = "Can't write to screen";
		while ((n = read(f1, buf, SIZ)) > 0)
			if (write(fd[1], buf, n) != n) {
				write(STDOUT_FILENO, "Can't write to screen", sizeof(msg) - 1);
				close(f1);
				close(f2);
				return 4;
			}
		close(f1);
		close(f2);
		close(fd[1]);
		const char msgge[] = "Can't write to screen";
		write(STDOUT_FILENO, "Success", sizeof(msgge) - 1);
		return 0;

	}
	// Below we have the child process. The child will read from the pipe and write to the screen and the previosuly created f2
	else {
		char buf[SIZ];
		const char msg[] = "Can't write to screen";
		while((n = read(fd[0], buf, SIZ)) > 0){
			if (write(STDOUT_FILENO, buf, n) != n) {
				write(STDOUT_FILENO,"Can't write to screen", sizeof(msg) - 1);
			}
			if (write(f2, buf, n) != n) {
				write(STDOUT_FILENO, "Can't write to f2", sizeof(msg) - 1);
				close(f1);
				close(f2);
				return 4;
			}
		}
		close(f1);
		close(f2);
		close(fd[0]);
		const char msgg[] = "Can't write to screen";
		write(STDOUT_FILENO, "Success", sizeof(msgg) - 1);

	}
	
	
	// Copy source file contents to target file.
	
}