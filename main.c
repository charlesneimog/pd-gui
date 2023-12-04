#include <adwaita.h>
#include <cairo.h>

static cairo_surface_t *surface = NULL;

static void clear_Patch(void) {
  cairo_t *cr;
  cr = cairo_create(surface);
  cairo_set_source_rgb(cr, 1, 1, 1);
  cairo_paint(cr);
  cairo_destroy(cr);
}

/* Create a new surface of the appropriate size to store our scribbles */
static void resize_cb(GtkWidget *widget, int width, int height, gpointer data) {
  if (surface) {
    cairo_surface_destroy(surface);
    surface = NULL;
  }

  if (gtk_native_get_surface(gtk_widget_get_native(widget))) {
    surface = gdk_surface_create_similar_surface(
        gtk_native_get_surface(gtk_widget_get_native(widget)),
        CAIRO_CONTENT_COLOR, gtk_widget_get_width(widget),
        gtk_widget_get_height(widget));

    /* Initialize the surface to white */
    clear_Patch();
  }
}

/* Redraw the screen from the surface. Note that the draw
 * callback receives a ready-to-be-used cairo_t that is already
 * clipped to only draw the exposed areas of the widget
 */
static void updatePatchView(GtkDrawingArea *drawing_area, cairo_t *cr,
                            int width, int height, gpointer data) {
  cairo_set_source_surface(cr, surface, 0, 0);
  cairo_paint(cr);
}

/* Draw a rectangle on the surface at the given position */
static void draw_Obj(GtkWidget *widget, double x, double y) {
  cairo_t *cr;
  cr = cairo_create(surface); // Create a cairo object from the surface
  int horizontal = 150;
  int vertical = 50;
  int fontsize = 40;

  // Draw the main rectangle with outline
  cairo_rectangle(cr, x, y, horizontal, vertical);
  cairo_set_source_rgb(cr, 0, 0,
                       0); // Set the outline color (black in this example)
  cairo_set_line_width(cr, 2.0); // Set the line width
  cairo_stroke(cr);

  // Draw a filled black rectangle in the top-left
  cairo_rectangle(cr, x, y, 25, 10);
  cairo_set_source_rgb(cr, 0, 0, 0); // Set the fill color (black)
  cairo_fill(cr);

  // Draw a filled black rectangle in the bottom-left
  cairo_rectangle(cr, x, y + (vertical - 10), 25, 10);
  cairo_set_source_rgb(cr, 0, 0, 0); // Set the fill color (black)
  cairo_fill(cr);

  // draw text in the center of the rectangle
  cairo_select_font_face(cr, "Sans", CAIRO_FONT_SLANT_NORMAL,
                         CAIRO_FONT_WEIGHT_NORMAL);
  cairo_set_font_size(cr, fontsize);
  cairo_move_to(cr, x + 5, y + (fontsize - (int)(fontsize / 8)));
  cairo_set_source_rgb(cr, 0, 0, 0);
  cairo_show_text(cr, "osc~");

  cairo_destroy(cr);             // Destroy the cairo object
  gtk_widget_queue_draw(widget); // Call the draw function (updatePatchView)
}

static double start_x;
static double start_y;

static void drag_begin(GtkGestureDrag *gesture, double x, double y,
                       GtkWidget *area) {
  start_x = x;
  start_y = y;
  draw_Obj(area, x, y);
}

static void pressed(GtkGestureClick *gesture, int n_press, double x, double y,
                    GtkWidget *area) {
  g_print("Right Click\n");
}

static void close_window(void) {
  if (surface)
    cairo_surface_destroy(surface);
}

static const GActionEntry app_actions[] = {
    {"newobj", draw_Obj, NULL, NULL, NULL};
}
;

static void Pd_GUI(GtkApplication *app, gpointer user_data) {
  GtkWidget *window;
  GtkWidget *frame;
  GtkWidget *drawing_area;
  GtkGesture *drag;
  GtkGesture *press;

  window = gtk_application_window_new(app);
  gtk_window_set_title(GTK_WINDOW(window), "Drawing Area");

  g_signal_connect(window, "destroy", G_CALLBACK(close_window), NULL);

  frame = gtk_frame_new(NULL);
  gtk_window_set_child(GTK_WINDOW(window), frame);

  // Patch Area
  drawing_area = gtk_drawing_area_new();
  gtk_widget_set_size_request(drawing_area, 400, 400);
  gtk_frame_set_child(GTK_FRAME(frame),
                      drawing_area); // Add drawing_area to frame

  gtk_drawing_area_set_draw_func(
      GTK_DRAWING_AREA(drawing_area), updatePatchView, NULL,
      NULL); // Every time we call gtk_widget_queue_draw() the updatePatchView()
             // function will be called

  // Resize Patch
  g_signal_connect_after(drawing_area, "resize", G_CALLBACK(resize_cb), NULL);
  // End

  // Left Click
  drag = gtk_gesture_drag_new();
  gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(drag), GDK_BUTTON_PRIMARY);
  gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(drag));
  g_signal_connect(drag, "drag-begin", G_CALLBACK(drag_begin), drawing_area);
  // End

  gtk_application_set_accels_for_action(
      GTK_APPLICATION(app), "app.newobj", // app for now, but should be window
      (const char *[]){"<primary>1", NULL});

  // // Right Click
  // press = gtk_gesture_click_new();
  // gtk_gesture_single_set_button(GTK_GESTURE_SINGLE(press),
  //                               GDK_BUTTON_SECONDARY);
  // gtk_widget_add_controller(drawing_area, GTK_EVENT_CONTROLLER(press));
  // g_signal_connect(press, "pressed", G_CALLBACK(pressed), drawing_area);
  // // End Right Click

  gtk_window_present(GTK_WINDOW(window));
}

// ======================================
int main() {
  g_autoptr(AdwApplication) app = NULL;
  app = adw_application_new("io.github.PureData", G_APPLICATION_DEFAULT_FLAGS);
  g_signal_connect(app, "activate", G_CALLBACK(Pd_GUI), NULL);
  return g_application_run(G_APPLICATION(app), 0, NULL);
}
