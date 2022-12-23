#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>

// Global variables
GtkWidget *window;
GtkWidget *drawing_area;
GtkWidget *entry;
GtkWidget *label;
int array[100];
int size;
int i, j, temp;

// Function to draw the graph on the drawing area
static gboolean draw_graph(GtkWidget *widget, cairo_t *cr, gpointer data)
{
	// Set the background color
	cairo_set_source_rgb(cr, 0.0, 0.0, 0.0);
	cairo_paint(cr);

	// Set the color for the graph
	cairo_set_source_rgb(cr, 1.0, 0.0, 0.0);

	// Draw the graph
	for (i = 0; i < size; i++)
	{
		cairo_rectangle(cr, i * 10, 0, 10, array[i] * 10);
		cairo_fill(cr);
	}

	return FALSE;
}

// Function to sort the array using insertion sort
static void insertion_sort()
{
	for (i = 1; i < size; i++)
	{
		temp = array[i];
		j = i - 1;

		while (j >= 0 && array[j] > temp)
		{
			array[j + 1] = array[j];
			j--;
		}

		array[j + 1] = temp;

		// Redraw the graph after each iteration
		gtk_widget_queue_draw(drawing_area);
	}
}

// Function to get the input from the user
static void get_input()
{
	const gchar *input;

	// Get the input from the entry widget
	input = gtk_entry_get_text(GTK_ENTRY(entry));

	// Convert the input string to an array of integers
	size = 0;
	for (i = 0; input[i] != '\0'; i++)
	{
		if (input[i] != ' ')
		{
			array[size] = input[i] - '0';
			size++;
		}
	}

	// Sort the array using insertion sort
	insertion_sort();

	// Set the label to show the sorted array
	gchar *output = g_strdup_printf("Sorted array: ");
	for (i = 0; i < size; i++)
	{
		gchar *temp = g_strdup_printf("%d ", array[i]);
		output = g_strconcat(output, temp, NULL);
		g_free(temp);
	}
	gtk_label_set_text(GTK_LABEL(label), output);
	g_free(output);
}

// Main function
int main(int argc, char *argv[])
{
	// Initialize GTK
	gtk_init(&argc, &argv);

	// Create the window
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_window_set_title(GTK_WINDOW(window), "Insertion Sort Visualization");
	gtk_window_set_default_size(GTK_WINDOW(window), 500, 500);
	g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL);

	// Create the drawing area
	drawing_area = gtk_drawing_area_new();
	gtk_widget_set_size_request(drawing_area, 500, 400);
	g_signal_connect(G_OBJECT(drawing_area), "draw", G_CALLBACK(draw_graph), NULL);

	// Create the entry widget
	entry = gtk_entry_new();
	gtk_entry_set_max_length(GTK_ENTRY(entry), 100);

	// Create the label
	label = gtk_label_new("");

	// Create the button
	GtkWidget *button = gtk_button_new_with_label("Sort");
	g_signal_connect(button, "clicked", G_CALLBACK(get_input), NULL);

	// Create the layout
	GtkWidget *layout = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_box_pack_start(GTK_BOX(layout), drawing_area, TRUE, TRUE, 0);
	gtk_box_pack_start(GTK_BOX(layout), entry, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(layout), button, FALSE, FALSE, 0);
	gtk_box_pack_start(GTK_BOX(layout), label, FALSE, FALSE, 0);

	// Add the layout to the window
	gtk_container_add(GTK_CONTAINER(window), layout);

	// Show the window
	gtk_widget_show_all(window);

	// Start the main loop
	gtk_main();

	return 0;
}
