#include <mikmod.h> /* for MikMod_RegisterAllDrivers, MikMod_RegisterAllLoaders, MikMod_Init, Player_Load, Player_Free and MikMod_strerror */
#include <string.h> /* for strlen, strstr, strcpy, strncpy, strcat and strcmp */
#include <stdlib.h> /* for malloc, free and exit */
#include <stdio.h> /* for printf, fprintf, fopen, fclose and rename */
#include "librenmod.h" /* for init and rename_module */
static char convert_character(char character)
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
static char *create_name(const char *author, const char *title, const char *type)
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
static void destroy_name(char *name)
{
	free(name);
	name = NULL;
}
static char *create_type(const char *modtype)
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
static void destroy_type(char *type)
{
	free(type);
	type = NULL;
}
static char *create_directorypath(const char *filepath)
{
	int index;
	int filepath_len = strlen(filepath);
	int directorypath_len = filepath_len;
	for(index = filepath_len -1; index > -1; index--)
	{
		if(filepath[index] == '/')
			break;
		directorypath_len--;

	}
	char *directorypath = malloc((directorypath_len + 1) * sizeof(char));
	strncpy(directorypath, filepath, directorypath_len);
	directorypath[directorypath_len] = '\0';
	return directorypath;
}
static void destroy_directorypath(char *directrorypath)
{
	free(directrorypath);
	directrorypath = NULL;
}
static char *create_filepath(const char *directorypath, const char *filename)
{
	char *filepath = malloc(strlen(directorypath) + strlen(filename) + 1);
	strcpy(filepath, directorypath);
	strcat(filepath, filename);
	return filepath;
}
static void destroy_filepath(char *filepath)
{
	free(filepath);
	filepath = NULL;
}
static char *create_filename(const char *filepath)
{
	int index;
	int filepath_len = strlen(filepath);
	const char *filepath_pointer = &filepath[0];
	for(index = 0; index < filepath_len; index++)
		if(filepath[index] == '/' && index + 1 < filepath_len)
			filepath_pointer = &filepath[index + 1];
	char *filename = malloc(strlen(filepath_pointer) + 1);
	strcpy(filename, filepath_pointer);
	return filename;
}
static void destroy_filename(char *filename)
{
	free(filename);
	filename = NULL;
}
int rename_module(char *filepath, const char *author, int number, int max)
{
	printf("%d/%d: %s", number, max, filepath);
	MODULE *module;
	module = Player_Load(filepath, 64, 0);
	if(module)
	{
		char *type = create_type(module->modtype);
		if(type == NULL)
		{
			fprintf(stderr, "%s:%d: %s (type is NULL)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
			return -1;
		}
		char *name = create_name(author, module->songname, type);
		if(name == NULL)
		{
			fprintf(stderr, "%s:%d: %s (name is NULL)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
			return -1;
		}
		printf("\n-author=\"%s\"", author);
		printf("\n-title=\"%s\"", module->songname);
		printf("\n-type=\"%s\"", type);
		printf("\n-name=\"%s\"\n", name);
		char *filename = create_filename(filepath);
		if(strcmp(filename, name) == 0)
			printf("[Good name]\n");
		else
		{
			char *directorypath = create_directorypath(filepath);
			char *newfilepath = create_filepath(directorypath, name);
			FILE *fp;
			fp = fopen(newfilepath, "r");
			if(fp != NULL)
			{
				fprintf(stderr, "%s:%d: %s (File exists)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
				fclose(fp);
				return -1;
			}
			if(rename(filepath, newfilepath) == -1)
			{
				fprintf(stderr, "%s:%d: %s (Unable to rename)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
				return -1;
			}
			printf("[Renamed]\n");
			destroy_directorypath(directorypath);
			destroy_filepath(newfilepath);
		}
		Player_Free(module);
		destroy_filename(filename);
		destroy_name(name);
		destroy_type(type);
	}
	else
	{
		printf(" (...)\n");
		fprintf(stderr, "%s:%d: %s (Could not load module, reason: %s)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, MikMod_strerror(MikMod_errno));
	}
	return 0;
}
int init()
{
	/* register all the drivers */
	MikMod_RegisterAllDrivers();
	/* register all the module loaders */
	MikMod_RegisterAllLoaders();
	/* initialize the library */
	if(MikMod_Init(""))
	{
		fprintf(stderr, "%s:%d: %s (Could not initialize sound, reason: %s)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, MikMod_strerror(MikMod_errno));
		return -1;
	}
	return 0;
}
