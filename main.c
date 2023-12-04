#include <adwaita.h>

int main(int argc, char *argv[]) {
  GtkWidget *window, *label;
  adw_init();
  window = gtk_window_new();
  label = gtk_label_new("New GUI for PureData");
  gtk_window_set_title(GTK_WINDOW(window), "PureData");
  gtk_window_set_default_size(GTK_WINDOW(window), 800, 800);

  gtk_window_set_child(GTK_WINDOW(window), label);
  gtk_window_present(GTK_WINDOW(window));

  GtkWidget *box, *bin;
  AdwBreakpoint *breakpoint;

  box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 12);
  gtk_box_append(GTK_BOX(box), gtk_button_new_with_label("Button 1"));

  gtk_widget_set_halign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_valign(box, GTK_ALIGN_CENTER);
  gtk_widget_set_margin_top(box, 12);
  gtk_widget_set_margin_bottom(box, 12);
  gtk_widget_set_margin_start(box, 12);
  gtk_widget_set_margin_end(box, 12);

  while (g_list_model_get_n_items(gtk_window_get_toplevels()) > 0)
    g_main_context_iteration(NULL, TRUE);

  return 0;
}
