
#include <gtk/gtk.h>
static int count = 0;
/* callback */
void button_clicked(GtkWidget *widget, gpointer data)
{
	printf(“%s pressed %d time(s)\n”, data, ++count);
}
int main( int argc, char *argv[] )
{
	GtkWidget *window, *button;
	gtk_init (&argc, &argv);
	window = 
		gtk_window_new(GTK_WINDOW_TOPLEVEL);
	button = gtk_button_new_with_label ("Hello World!");
	gtk_container_add (GTK_CONTAINER(window), 
			button);
	g_signal_connect ( button, "clicked",
			G_CALLBACK (button_clicked), “Button1”);
	gtk_widget_show (button);
	gtk_widget_show (window);
	gtk_main();
	return (0);
}
