// Include the most common headers from the C standard library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// Include the main libnx system header, for Switch development
#include <switch.h>

int strcheck(char s1[1000], char s2[1000]) {
	int M = strlen(s1);
	int N = strlen(s2);
	/* A loop to slide pat[] one by one */
	for (int i = 0; i <= N - M; i++) { 
		int j;
		/* For current index i, check for pattern match */
		for (j = 0; j < M; j++) 
			if (s2[i + j] != s1[j]) 
				break;
		if (j == M) 
			return i; 
	}
	return -1; 
}

// Main program entrypoint
int main()
{
	// This example uses a text console, as a simple way to output text to the screen.
	// If you want to write a software-rendered graphics application,
	//   take a look at the graphics/simplegfx example, which uses the libnx Framebuffer API instead.
	// If on the other hand you want to write an OpenGL based application,
	//   take a look at the graphics/opengl set of examples, which uses EGL instead.
	consoleInit(NULL);

	// Other initialization goes here. As a demonstration, we print something a stooge would say.
//	printf("'Sup punkalunks");
	printf("Welcome!");
	printf("\nPress X to toggle your EmuMMC!");
	FILE* f = fopen("sdmc:/emummc/emummc.ini", "r");
	char endfile[1000];
	if (f) {
		char mystring[1000];
		while (fgets(mystring, sizeof(mystring), f)) {
			int a = strlen(mystring);
			if (mystring[a-1] == '\n') {
				mystring[a-1] = 0;
				if (mystring[a-2] == '\r')
					mystring[a-2] = 0;
			}
			strcat(mystring, "\n");
			strcat(endfile, mystring);
		}
	} else {
		printf("errno is %d, %s\n", errno, strerror(errno));
	}
	fclose(f);
	int check = strcheck("emummc_enabled = 0", endfile);
	char rep[1000] = "";
	if (check == -1) {
		strcpy(rep, "[emummc]\nemummc_enabled = 0");
		printf("\nYour EmuMMC is currently enabled.");
	} else {
		strcpy(rep, "[emummc]\nemummc_enabled = 1");
		printf("\nYour EmuMMC is currently disabled.");
	}

	// Main loop
	while (appletMainLoop()) {
		// Scan all the inputs. This should be done once for each frame
		hidScanInput();

		// hidKeysDown returns information about which buttons have been
		// just pressed in this frame compared to the previous one
		u64 kDown = hidKeysDown(CONTROLLER_P1_AUTO);

		if (kDown & KEY_PLUS)
			break; // break in order to return to hbmenu

		if (kDown & KEY_A)
			printf("\nHey goober, you pressed A");

		if (kDown & KEY_B)
			printf("\nHey booger, you pressed B");
		
		if (kDown & KEY_X) {
			printf(CONSOLE_ESC(2J));
			printf("Welcome!");
			printf("\nPress X to toggle your EmuMMC!");
			FILE* f = fopen("sdmc:/emummc/emummc.ini", "r");
			char endfile[1000] = "";
			if (f) {
				char mystring[1000];
				while (fgets(mystring, sizeof(mystring), f)) {
					int a = strlen(mystring);
					if (mystring[a-1] == '\n') {
						mystring[a-1] = 0;
						if (mystring[a-2] == '\r')
							mystring[a-2] = 0;
					}
					strcat(mystring, "\n");
					strcat(endfile, mystring);
				}
			} else {
				printf("errno is %d, %s\n", errno, strerror(errno));
			}
			fclose(f);
			int check = strcheck("emummc_enabled = 0", endfile);
			char rep[1000] = "";
			if (check == -1) {
				strcpy(rep, "[emummc]\nemummc_enabled = 0");
				printf("\nYour EmuMMC is currently disabled.");
			} else {
				strcpy(rep, "[emummc]\nemummc_enabled = 1");
				printf("\nYour EmuMMC is currently enabled.");
			}
			char subbuff[1000];
			memcpy(subbuff, &endfile[strlen(rep)], strlen(endfile) - strlen(rep));
			strncpy(endfile, rep, strlen(rep));
			strncpy(&endfile[strlen(rep)], subbuff, strlen(subbuff));
			FILE* fw = fopen("sdmc:/emummc/emummc.ini", "w");
			fputs(endfile, fw);
			fclose(fw);
		}
		// Update the console, sending a new frame to the display
		consoleUpdate(NULL);
	}
	// Deinitialize and clean up resources used by the console (important!)
	consoleExit(NULL);
	return 0;
}
