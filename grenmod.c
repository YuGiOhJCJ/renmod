#include <mikmod.h> /* for MikMod_Exit and MikMod_strerror */
#include <stdlib.h> /* for exit */
#include <stdio.h> /* for printf and fprintf */
#include <gtk/gtk.h> /* for GtkWidget */
#include "librenmod.h" /* for init and rename_module */
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
int main(int argc, char *argv[])
{
	/* init */
	gtk_init(&argc, &argv);
	if(init() == -1)
	{
		fprintf(stderr, "%s:%d: %s (Could not initialize sound, reason: %s)\n", __FILE__, __LINE__, __PRETTY_FUNCTION__, MikMod_strerror(MikMod_errno));
		MikMod_Exit();
		exit(EXIT_SUCCESS);
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
	/* entry */
	GtkWidget *entry;
	entry = gtk_entry_new();
	gtk_entry_set_text(GTK_ENTRY(entry), "author");
	gtk_box_pack_start(GTK_BOX(box), entry, FALSE, FALSE, 0);
	/* connect */
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);
	g_signal_connect(G_OBJECT(menu_item_open), "activate", G_CALLBACK(grenmod_callback_menu_item_open), entry);
	g_signal_connect(G_OBJECT(menu_item_quit), "activate", G_CALLBACK(gtk_main_quit), NULL);
	/* main */
	gtk_widget_show_all(window);
	gtk_main();
	/* give up */
	MikMod_Exit();
	exit(EXIT_SUCCESS);
}
