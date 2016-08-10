#include <gtk/gtk.h>
void hello(GtkWidget *widget,gpointer *data)
{
	g_printf("button clicked and data = %s\n",(char *)data);
}

void destroy(GtkWidget *widget,gpointer *data)
{
	gtk_main_quit();
}
int main(int argc, char const *argv[])
{
	GtkWidget *window;
	GtkWidget *button;
	gtk_init(&argc,&argv);
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(GTK_OBJECT(window)
		,"destory"	,GTK_SIGNAL_FUNC(destory),NULL);
	gtk_container_border_width(GTK_CONTAINER(window),20);
	button = gtk_button_new_with_label("Hello World");
	g_signal_connect(GTK_OBJECT(button)
		,"clicked"	,GTK_SIGNAL_FUNC(hello),"I an from button");
	gtk_container_add(GTK_AONTAINER(window),button);
	gtk_widget_show(button);
	gtk_widget_show(window);
	gtk_main();
	return 0;
}