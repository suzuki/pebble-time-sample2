#include <pebble.h>

static Window *window;
static TextLayer *text_layer;
static Layer *point_layer;
static Layer *rect_layer;
static Layer *image_layer;
static GBitmap *image;

static void select_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Select");
}

static void up_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Up");
}

static void down_click_handler(ClickRecognizerRef recognizer, void *context) {
  text_layer_set_text(text_layer, "Down");
}

static void click_config_provider(void *context) {
  window_single_click_subscribe(BUTTON_ID_SELECT, select_click_handler);
  window_single_click_subscribe(BUTTON_ID_UP, up_click_handler);
  window_single_click_subscribe(BUTTON_ID_DOWN, down_click_handler);
}

static void point_layer_update_callback(Layer *layer, GContext *ctx) {
  GPoint p0 = GPoint(0, 0);
  GPoint p1 = GPoint(100, 100);
  graphics_context_set_stroke_color(ctx, GColorBlack);
  graphics_draw_line(ctx, p0, p1);
}

static void rect_layer_update_callback(Layer *layer, GContext *ctx) {
  GRect bounds = layer_get_bounds(layer);
  GRect frame = GRect(5,5, bounds.size.w, 18 + 2);

  graphics_context_set_text_color(ctx, GColorRed);
  graphics_draw_text(
    ctx,
    "Hello World",
    fonts_get_system_font(FONT_KEY_GOTHIC_18_BOLD),
    frame,
    GTextOverflowModeTrailingEllipsis,
    GTextAlignmentCenter,
    NULL
  );
}

static void image_layer_update_callback(Layer *layer, GContext *ctx) {
  graphics_draw_bitmap_in_rect(ctx, image, layer_get_bounds(layer));
}

static void window_load(Window *window) {
  Layer *window_layer = window_get_root_layer(window);
  GRect bounds = layer_get_bounds(window_layer);

  text_layer = text_layer_create((GRect) { .origin = { 0, 72 }, .size = { bounds.size.w, 20 } });
  text_layer_set_text(text_layer, "Press a button");
  text_layer_set_text_alignment(text_layer, GTextAlignmentCenter);
  layer_add_child(window_layer, text_layer_get_layer(text_layer));

  point_layer = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(point_layer, point_layer_update_callback);
  layer_add_child(window_layer, point_layer);

  rect_layer = layer_create(GRect(0, 0, 144, 168));
  layer_set_update_proc(rect_layer, rect_layer_update_callback);
  layer_add_child(window_layer, rect_layer);

  image = gbitmap_create_with_resource(RESOURCE_ID_ICON_IMAGE);
  image_layer = layer_create(GRect(0, (168 - 60), 60, 60));
  layer_set_update_proc(image_layer, image_layer_update_callback);
  layer_add_child(window_layer, image_layer);
}

static void window_unload(Window *window) {
  text_layer_destroy(text_layer);

  layer_destroy(point_layer);
  layer_destroy(rect_layer);
  layer_destroy(image_layer);
}

static void init(void) {
  window = window_create();
  window_set_click_config_provider(window, click_config_provider);
  window_set_window_handlers(window, (WindowHandlers) {
    .load = window_load,
    .unload = window_unload,
  });
  const bool animated = true;
  window_stack_push(window, animated);
}

static void deinit(void) {
  window_destroy(window);
}

int main(void) {
  init();

  APP_LOG(APP_LOG_LEVEL_DEBUG, "Done initializing, pushed window: %p", window);

  app_event_loop();
  deinit();
}
