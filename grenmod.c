#include <mikmod.h> /* for MikMod_Exit and MikMod_strerror */
#include <stdlib.h> /* for exit */
#include <stdio.h> /* for printf and fprintf */
#include <gtk/gtk.h> /* for GtkWidget */
#include "librenmod.h" /* for init and rename_module */
#define GRENMOD_FILE "grenmod.conf"
#define GRENMOD_GROUP "grenmod"
#define GRENMOD_KEY "author"
#define GRENMOD_VALUE "author"
typedef struct grenmod_struct_quit
{
	gchar *path;
	GKeyFile *key_file;
	GtkWidget *entry;
} grenmod_struct_quit_t;
static void grenmod_callback_menu_item_open(GtkWidget *widget, gpointer data)
{
	GtkWidget *entry = (GtkWidget *)data;
	/* file chooser dialog */
	GtkWidget *file_chooser_dialog;
	file_chooser_dialog = gtk_file_chooser_dialog_new("Open", NULL, GTK_FILE_CHOOSER_ACTION_OPEN, GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL, GTK_STOCK_OPEN, GTK_RESPONSE_ACCEPT, NULL);
	gtk_file_chooser_set_select_multiple(GTK_FILE_CHOOSER(file_chooser_dialog), TRUE);
	/* rename each module */
	if(gtk_dialog_run(GTK_DIALOG(file_chooser_dialog)) == GTK_RESPONSE_ACCEPT)
	{
		GSList *filenames;
		filenames = gtk_file_chooser_get_filenames(GTK_FILE_CHOOSER(file_chooser_dialog));
		GSList *filename = filenames;
		int len = g_slist_length(filenames);
		int counter;
		for(counter = 1; counter <= len; counter++)
		{
			if(rename_module((char *)filename->data, gtk_entry_get_text(GTK_ENTRY(entry)), counter, len) == -1)
				fprintf(stderr, "%s:%d: %s (Unable to rename)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__);
			filename = g_slist_next(filename);
		}
		g_slist_free(filenames);
	}
	gtk_widget_destroy(file_chooser_dialog);
}
static void grenmod_callback_quit(GtkWidget *widget, gpointer data)
{
	/* save */
	grenmod_struct_quit_t *quit = (grenmod_struct_quit_t*)data;
	g_key_file_set_value(quit->key_file, GRENMOD_GROUP, GRENMOD_KEY, gtk_entry_get_text(GTK_ENTRY(quit->entry)));
	gchar *key_file_data = g_key_file_to_data(quit->key_file, NULL, NULL);
	g_file_set_contents(quit->path, key_file_data, -1, NULL);
	g_free (key_file_data);
	/* terminate the GTK+ main loop */
	gtk_main_quit();
}
int main(int argc, char *argv[])
{
	/* init */
	gtk_init(&argc, &argv);
	if(init() == -1)
	{
		fprintf(stderr, "%s:%d: %s (Could not initialize sound, reason: %s)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, MikMod_strerror(MikMod_errno));
		MikMod_Exit();
		exit(EXIT_FAILURE);
	}
	/* window */
	GtkWidget *window;
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "renmod");
	gtk_widget_show(window);
	/* box */
	GtkWidget *box;
	box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 0);
	gtk_container_add(GTK_CONTAINER(window), box);
	/* menubar */
	GtkWidget *menubar;
	menubar = gtk_menu_bar_new();
	gtk_box_pack_start(GTK_BOX(box), menubar, FALSE, FALSE, 0);
	/* menu file */
	GtkWidget *menu_file;
	menu_file = gtk_menu_new();
	/* menu item file */
	GtkWidget *menu_item_file;
	menu_item_file = gtk_menu_item_new_with_label("File");
	gtk_menu_shell_append(GTK_MENU_SHELL(menubar), menu_item_file);
	gtk_menu_item_set_submenu(GTK_MENU_ITEM(menu_item_file), menu_file);
	/* menu item open */
	GtkWidget *menu_item_open;
	menu_item_open = gtk_menu_item_new_with_label("Open");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_file), menu_item_open);
	/* menu item quit */
	GtkWidget *menu_item_quit;
	menu_item_quit = gtk_menu_item_new_with_label("Quit");
	gtk_menu_shell_append(GTK_MENU_SHELL(menu_file), menu_item_quit);
	/* author */
	gchar *author;
	author = NULL;
	/* path */
	gchar *path = g_strdup_printf("%s%c%s", g_get_user_config_dir(), G_DIR_SEPARATOR, GRENMOD_FILE);
	/* key file */
	GKeyFile *key_file;
	key_file = g_key_file_new();
	if(g_key_file_load_from_file(key_file, path, G_KEY_FILE_NONE, NULL) == TRUE)
		author = g_key_file_get_value(key_file, GRENMOD_GROUP, GRENMOD_KEY, NULL);
	/* entry */
	GtkWidget *entry;
	entry = gtk_entry_new();
	if(author == NULL)
		gtk_entry_set_text(GTK_ENTRY(entry), GRENMOD_VALUE);
	else
		gtk_entry_set_text(GTK_ENTRY(entry), author);
	gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);
	/* struct quit */
	grenmod_struct_quit_t quit;
	quit.path = path;
	quit.key_file = key_file;
	quit.entry = entry;
	/* connect */
	g_signal_connect(window, "destroy", G_CALLBACK(grenmod_callback_quit), key_file);
	g_signal_connect(G_OBJECT(menu_item_open), "activate", G_CALLBACK(grenmod_callback_menu_item_open), entry);
	g_signal_connect(G_OBJECT(menu_item_quit), "activate", G_CALLBACK(grenmod_callback_quit), &quit);
	/* main */
	gtk_widget_show_all(window);
	gtk_main();
	/* free */
	g_free(path);
	g_key_file_free(key_file);
	/* give up */
	MikMod_Exit();
	exit(EXIT_SUCCESS);
}
