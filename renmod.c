#include <mikmod.h> /* for MikMod_Exit and MikMod_strerror */
#include <stdlib.h> /* for exit */
#include <stdio.h> /* for fprintf */
#include "librenmod.h" /* for init and rename_module */
int main(int argc, char** argv)
{
	/* init */
	if(init() == -1)
	{
		fprintf(stderr, "%s:%d: %s (Could not initialize sound, reason: %s)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, MikMod_strerror(MikMod_errno));
		MikMod_Exit();
		exit(EXIT_FAILURE);
	}
	/* verify the number of arguments */
	if(argc < 3)
	{
		fprintf(stderr, "Usage: %s AUTHOR FILE ...\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	/* rename each module */
	int counter;
	for(counter = 2; counter < argc; counter++)
		if(rename_module(argv[counter], argv[1], counter - 1, argc - 2) == -1)
			fprintf(stderr, "%s:%d: %s (Unable to rename)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
	/* give up */
	MikMod_Exit();
	exit(EXIT_SUCCESS);
}
