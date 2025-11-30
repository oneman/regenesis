#include <stdio.h>
#include <unistd.h>
#include <xcb/xcb.h>
#include <cairo.h>
#include <cairo-xcb.h>
#include <stdlib.h>

static xcb_visualtype_t *find_visual(xcb_connection_t *c, xcb_visualid_t visual)
{
    xcb_screen_iterator_t screen_iter = xcb_setup_roots_iterator(xcb_get_setup(c));

    for (; screen_iter.rem; xcb_screen_next(&screen_iter)) {
        xcb_depth_iterator_t depth_iter = xcb_screen_allowed_depths_iterator(screen_iter.data);
        for (; depth_iter.rem; xcb_depth_next(&depth_iter)) {
            xcb_visualtype_iterator_t visual_iter = xcb_depth_visuals_iterator(depth_iter.data);
            for (; visual_iter.rem; xcb_visualtype_next(&visual_iter))
                if (visual == visual_iter.data->visual_id)
                    return visual_iter.data;
        }
    }

    return NULL;
}

int main(int argc, char *argv[]) {
    xcb_connection_t *c;
    xcb_screen_t *screen;
    xcb_window_t window;
    uint32_t mask[2];
    xcb_visualtype_t *visual;
    xcb_generic_event_t *event;
    cairo_surface_t *surface;
    cairo_t *cr;

    c = xcb_connect(NULL, NULL);
    if (xcb_connection_has_error(c)) {
        printf("Could not connect to X11 server");
        return 1;
    }

    mask[0] = 1;
    mask[1] = XCB_EVENT_MASK_EXPOSURE       | XCB_EVENT_MASK_BUTTON_PRESS  |
              XCB_EVENT_MASK_BUTTON_RELEASE | XCB_EVENT_MASK_BUTTON_MOTION |
              XCB_EVENT_MASK_ENTER_WINDOW   | XCB_EVENT_MASK_LEAVE_WINDOW  |
              XCB_EVENT_MASK_KEY_PRESS      | XCB_EVENT_MASK_KEY_RELEASE;
    screen = xcb_setup_roots_iterator(xcb_get_setup(c)).data;
    window = xcb_generate_id(c);
    xcb_create_window(c, XCB_COPY_FROM_PARENT, window, screen->root,
            0, 0, 1920, 1080, 0,
            XCB_WINDOW_CLASS_INPUT_OUTPUT,
            screen->root_visual,
            //XCB_CW_OVERRIDE_REDIRECT | XCB_CW_EVENT_MASK,
            XCB_CW_EVENT_MASK,
            mask);
    xcb_map_window(c, window);

    visual = find_visual(c, screen->root_visual);
    if (visual == NULL) {
        printf("Some weird internal error...?!");
        xcb_disconnect(c);
        return 1;
    }
    surface = cairo_xcb_surface_create(c, window, visual, 1920, 1080);
    xcb_flush(c);
    
    cr = cairo_create(surface);
    cairo_set_source_rgb(cr, 0, 1, 0);
    cairo_paint(cr);
    cairo_surface_flush(surface);
int run = 1;
for (;run;) {

    while ((event = xcb_poll_for_event(c))) {
        switch (event->response_type & ~0x80) {
        case XCB_MOTION_NOTIFY:
          printf("butkey motion notif\n");
          break;
        case XCB_ENTER_NOTIFY:
          printf("enter\n");
          break;
        case XCB_LEAVE_NOTIFY:
          printf("leave\n");
          break;          
        case XCB_BUTTON_RELEASE:
          printf("but rel\n");
          run--;
          break;
        case XCB_BUTTON_PRESS:
          printf("but press\n");
  //        popoff++;
          break;
        case XCB_KEY_PRESS:
          printf("key press\n");
//          popoff++;
          break;
        case XCB_KEY_RELEASE:
          printf("key rel\n");
          break;
        case XCB_EXPOSE:
          printf("xpose\n");
            /* Avoid extra redraws by checking if this is
             * the last expose event in the sequence
             */
            if (((xcb_expose_event_t *) event)->count != 0)
                break;
            break;
        default:
          printf("def ev\n");
          break;
        }
        free(event);
        xcb_flush(c);
    }
    usleep(17 * 1000); /* f'vsync this is Xorg */
  }
    
    
    //sleep(3000);
    
    xcb_flush(c);
    xcb_disconnect(c);

  return 0;  
}
