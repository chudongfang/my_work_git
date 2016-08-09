#include <gtk/gtk.h>
void click_button(GtkWidget *widget,gpointer *data)
{
	g_print("%s: ",(char *)data);
	if(GTK_TOGGLE_BUTTON(widget)->active)
		g_print("stat is active\n");
	else 
		g_print("stat is not active\n");
}

void destroy(GtkWidget *widget,gpointer *data)
{
	gtk_main_quit();
}

int main(int argc, char  *argv[])
{
	GtkWidget *window;
	GtkWidget *box;
	GSList    *group;
	GtkWidget *check, *radio;

	gtk_init(&argc,&argv);
	
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(GTK_OBJECT(window),"destroy",GTK_SIGNAL_FUNC(destroy),NULL);
	gtk_container_border_width(GTK_CONTAINER(window),50);
	
	box = gtk_vbox_new(FALSE ,0);
	gtk_container_add(GTK_CONTAINER(window),box);
	
	check = gtk_check_button_new_with_label("coffee");
	g_signal_connect(GTK_OBJECT(check),"clicked",GTK_SIGNAL_FUNC(click_button),"check_button1");
	gtk_box_pack_start(GTK_BOX(box),check,TRUE,TRUE,0);
	gtk_widget_show(check);

	check = gtk_check_button_new_with_label("tea");
	g_signal_connect(GTK_OBJECT(check),"clicked",GTK_SIGNAL_FUNC(click_button),"check_button2");
	gtk_box_pack_start(GTK_BOX(box),check,TRUE,TRUE,0);
	gtk_widget_show(check);


	radio = gtk_radio_button_new_with_label(NULL,"A");
	g_signal_connect(GTK_OBJECT(radio),"clicked",GTK_SIGNAL_FUNC(click_button),"a");
	gtk_box_pack_start(GTK_BOX(box),radio,TRUE,TRUE,0);
	gtk_widget_show(radio);

	group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio));
	radio = gtk_radio_button_new_with_label(group,"B");
	g_signal_connect(GTK_OBJECT(radio),"clicked",GTK_SIGNAL_FUNC(click_button),"b");
	gtk_box_pack_start(GTK_BOX(box),radio,TRUE,TRUE,0);
	gtk_widget_show(radio);
	
	/*group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio));
	radio = gtk_radio_button_new_with_label(NULL,"C");
	g_signal_connect(GTK_OBJECT(radio),"clicked",GTK_SIGNAL_FUNC(click_button),"c");
	//gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(radio),TRUE);

	gtk_box_pack_start(GTK_BOX(box),radio,TRUE,TRUE,0);
	gtk_widget_show(radio);*/

	group = gtk_radio_button_group(GTK_RADIO_BUTTON(radio));
	radio = gtk_radio_button_new_with_label(group,"C");
	g_signal_connect(GTK_OBJECT(radio),"clicked",GTK_SIGNAL_FUNC(click_button),"c");
	gtk_box_pack_start(GTK_BOX(box),radio,TRUE,TRUE,0);
	gtk_widget_show(radio);
	


	gtk_widget_show(box);
	gtk_widget_show(window);


	gtk_main();
	return 0;
}




