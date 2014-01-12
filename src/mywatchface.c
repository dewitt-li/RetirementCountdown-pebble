

// Standard includes
#include "pebble.h"

const int timetoretire=2298427200;
Window *window; 
TextLayer *time_layer; 
TextLayer *countdown_layer;
TextLayer *retired_layer;
char* concat(char *s1, char *s2){
	size_t len1=strlen(s1);
	size_t len2=strlen(s2);
	char *result=malloc(len1+len2+1);
	memcpy(result,s1,len1);
	memcpy(result+len1,s2,len2+1);
	return result;
}

// Called once per second
static void handle_second_tick(struct tm* tick_time, TimeUnits units_changed) {

  static char time_text[60]; // Needs to be static because it's used by the system later.
  static char countdown_text[150];
  strftime(time_text, sizeof(time_text), "%T\n%A\n%D\n", tick_time);
  text_layer_set_text(time_layer, time_text);

  time_t now = time(NULL);
  snprintf(countdown_text,150,"Retire in %d Days \n%02dH %02dM %02dS on \n11/02/2042",(int)((timetoretire-now)/86400+1),23-tick_time->tm_hour,(int)(((timetoretire-now)%3600)/60),(int)(((timetoretire-now)%60)));
  text_layer_set_text(countdown_layer,countdown_text );
    
}


// Handle the start-up of the app
static void do_init(void) {

  // Create our app's base window
  window = window_create();
  window_stack_push(window, true);
  window_set_background_color(window, GColorBlack);

  // Init the text layer used to show the time
  time_layer = text_layer_create(GRect(0, 0, 140 /* width */, 90 /* height */));
  text_layer_set_text_color(time_layer, GColorWhite);
  text_layer_set_background_color(time_layer, GColorClear);
  text_layer_set_font(time_layer, fonts_get_system_font(FONT_KEY_GOTHIC_28_BOLD));
  text_layer_set_text_alignment(time_layer, GTextAlignmentCenter);

  countdown_layer = text_layer_create(GRect(0, 100, 140 /* width */, 110 /* height */));
  text_layer_set_text_color(countdown_layer, GColorWhite);
  text_layer_set_background_color(countdown_layer, GColorClear);
  text_layer_set_font(countdown_layer, fonts_get_system_font(FONT_KEY_GOTHIC_18));
  text_layer_set_text_alignment(countdown_layer, GTextAlignmentCenter);
 
  time_t now = time(NULL);
  struct tm *current_time = localtime(&now);
  handle_second_tick(current_time, SECOND_UNIT);
  tick_timer_service_subscribe(SECOND_UNIT, &handle_second_tick);

  layer_add_child(window_get_root_layer(window), text_layer_get_layer(time_layer));
  layer_add_child(window_get_root_layer(window), text_layer_get_layer(countdown_layer));
}

static void do_deinit(void) {
  text_layer_destroy(time_layer);
  window_destroy(window);
}

int main(void) {
  do_init();
  app_event_loop();
  do_deinit();
}
