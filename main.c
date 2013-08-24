#include <mikmod.h> /* for MikMod_RegisterAllDrivers, MikMod_RegisterAllLoaders, MikMod_Init, MikMod_Exit, Player_Load and Player_Free */
#include <string.h> /* for strlen */
char convert_character(char character)
{
	if(character >= 32 && character <= 47)
		return '_';
	if(character >= 58 && character <= 64)
		return '_';
	if(character >= 91 && character <= 96)
		return '_';
	if(character >= 123 && character <= 126)
		return '_';
	if(character >= 65 && character <= 90)
		return character + 32;
	return character;
}
char *create_name(char *author, char *title, char *type)
{
	int counter;
	int len_author = strlen(author);
	int len_title = strlen(title);
	int len_type = strlen(type);
	int len = strlen(author) + 1 + strlen(title) + 1 + strlen(type) + 1;
	char *name = malloc(len * sizeof(char));
	if(name == NULL)
	{
		fprintf(stderr, "%s:%d: %s (Failed to allocate memory for the name)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
		exit(EXIT_FAILURE);
	}
	for(counter = 0 ; counter < len_author ; counter++)
		name[counter] = convert_character(author[counter]);
	name[len_author] = '-';
	for(counter = 0 ; counter < len_title ; counter++)
		name[len_author + 1 + counter] = convert_character(title[counter]);
	name[len_author + 1 + len_title] = '.';
	for(counter = 0 ; counter < len_type ; counter++)
		name[len_author + 1 + len_title + 1 + counter] = convert_character(type[counter]);
	name[len - 1] = '\0';
	return name;
}
void destroy_name(char *name)
{
	free(name);
	name = NULL;
}
char *create_type(char *modtype)
{
	if(strstr(modtype, "Screamtracker ") != NULL)
	{
		char *type = malloc(4 * sizeof(char));
		if(type == NULL)
		{
			fprintf(stderr, "%s:%d: %s (Failed to allocate memory for the 's3m' type)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
			exit(EXIT_FAILURE);
		}
		type[0] = 's';
		type[1] = '3';
		type[2] = 'm';
		type[3] = '\0';
		return type;
	}
	else if(strstr(modtype, "Protracker") != NULL || strstr(modtype, "Fasttracker") != NULL)
	{
		char *type = malloc(4 * sizeof(char));
		if(type == NULL)
		{
			fprintf(stderr, "%s:%d: %s (Failed to allocate memory for the 'mod' type)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
			exit(EXIT_FAILURE);
		}
		type[0] = 'm';
		type[1] = 'o';
		type[2] = 'd';
		type[3] = '\0';
		return type;
	}
	else if(strstr(modtype, "ImpulseTracker ") != NULL)
	{
		char *type = malloc(3 * sizeof(char));
		if(type == NULL)
		{
			fprintf(stderr, "%s:%d: %s (Failed to allocate memory for the 'it' type)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
			exit(EXIT_FAILURE);
		}
		type[0] = 'i';
		type[1] = 't';
		type[2] = '\0';
		return type;
	}
	else if(strstr(modtype, "XM format") != NULL)
	{
		char *type = malloc(3 * sizeof(char));
		if(type == NULL)
		{
			fprintf(stderr, "%s:%d: %s (Failed to allocate memory for the 'xm' type)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
			exit(EXIT_FAILURE);
		}
		type[0] = 'x';
		type[1] = 'm';
		type[2] = '\0';
		return type;
	}
	return NULL;
}
void destroy_type(char *type)
{
	free(type);
	type = NULL;
}
int main(int argc, char** argv)
{
	MODULE *module;
	int counter;
	/* register all the drivers */
	MikMod_RegisterAllDrivers();
	/* register all the module loaders */
	MikMod_RegisterAllLoaders();
	/* initialize the library */
	if(MikMod_Init(""))
	{
		fprintf(stderr, "Could not initialize sound, reason: %s\n", MikMod_strerror(MikMod_errno));
		MikMod_Exit();
		exit(EXIT_FAILURE);
	}
	/* verify the number of arguments */
	if(argc < 3)
	{
		fprintf(stderr, "Usage: %s AUTHOR FILE ...\n", argv[0]);
		exit(EXIT_FAILURE);
	}
	for(counter = 2; counter < argc; counter++)
	{
		/* load the module and display the name */
		printf("%d/%d: %s", counter - 1, argc - 2, argv[counter]);
		module = Player_Load(argv[counter], 64, 0);
		if(module)
		{
			char *type = create_type(module->modtype);
			if(type == NULL)
			{
				fprintf(stderr, "type is NULL\n");
				exit(EXIT_FAILURE);
			}
			char *name = create_name(argv[1], module->songname, type);
			if(name == NULL)
			{
				fprintf(stderr, "name is NULL\n");
				exit(EXIT_FAILURE);
			}
			printf("\n-author=\"%s\"", argv[1]);
			printf("\n-title=\"%s\"", module->songname);
			printf("\n-type=\"%s\"", type);
			printf("\n-filename=\"%s\"\n", name);
			if(strcmp(argv[counter], name) == 0)
				printf("[Good name]\n");
			else
			{
				FILE *fp;
				fp = fopen(name, "r");
				if(fp != NULL)
				{
					fprintf(stderr, "%s:%d: %s (File exists)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
					fclose(fp);
					exit(EXIT_FAILURE);
				}
				if(rename(argv[counter], name) == -1)
				{
					fprintf(stderr, "%s:%d: %s (Unable to rename)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
					exit(EXIT_FAILURE);
				}
				printf("[Renamed]\n");
			}
			Player_Free(module);
			destroy_name(name);
			destroy_type(type);
		}
		else
		{
			printf(" (...)\n");
			fprintf(stderr, "%s:%d: %s (Could not load module, reason: %s)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, MikMod_strerror(MikMod_errno));
		}
	}
	/* give up */
	MikMod_Exit();
	exit(EXIT_SUCCESS);
}
