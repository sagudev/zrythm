/*
 * Copyright (C) 2018-2019 Alexandros Theodotou <alex at zrythm dot org>
 *
 * This file is part of Zrythm
 *
 * Zrythm is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zrythm is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with Zrythm.  If not, see <https://www.gnu.org/licenses/>.
 */

/**
 * \file
 *
 * User Interface utils.
 *
 */

#ifndef __UTILS_UI_H__
#define __UTILS_UI_H__

#include <gtk/gtk.h>

/**
 * @addtogroup widgets
 *
 * @{
 */

typedef struct Position Position;

/**
 * Space on the edges to show resize cursors
 */
#define RESIZE_CURSOR_SPACE 9

/**
 * Drag n Drop related.
 */
/* Gtk target entries */
/* Plugin descriptor, used to instantiate plugins */
#define TARGET_ENTRY_PLUGIN_DESCR "PLUGIN_DESCR"
/* Plugin ID, used to move/copy plugins */
#define TARGET_ENTRY_PLUGIN "PLUGIN"
/* File descriptor */
#define TARGET_ENTRY_FILE_DESCR "FILE_DESCR"

/**
 * Track target entry.
 *
 * This is just the identifier. The
 * TracklistSelections will be used.
 */
#define TARGET_ENTRY_TRACK "TRACK"

/* */
#define TARGET_ENTRY_TL_SELECTIONS \
  "TL_SELECTIONS"

#define GET_ATOM(x) \
  gdk_atom_intern (x, 1)

#define ui_add_widget_tooltip(widget,txt) \
  gtk_widget_set_tooltip_text ( \
    GTK_WIDGET (widget), txt)

#define ui_set_hover_status_bar_signals(w,t) \
  g_signal_connect ( \
    G_OBJECT (w), "enter-notify-event", \
    G_CALLBACK ( \
      ui_on_motion_set_status_bar_text_cb), \
    g_strdup (t)); \
  g_signal_connect ( \
    G_OBJECT(w), "leave-notify-event", \
    G_CALLBACK ( \
      ui_on_motion_set_status_bar_text_cb), \
    g_strdup (t));

/**
 * Shows the notification when idle.
 *
 * This should be called from threads other than GTK main
 * thread.
 */
#define ui_show_notification_idle(msg) \
  char * text = g_strdup (msg); \
  g_message (msg); \
  g_idle_add ((GSourceFunc) ui_show_notification_idle_func, \
              (void *) text)

/**
 * Wrapper to show error message so that no casting
 * of the window is needed on the caller side.
 */
#define ui_show_error_message(win, msg) \
    ui_show_message_full ( \
      GTK_WINDOW (win), \
      GTK_MESSAGE_ERROR, \
      msg);

#define ui_is_widget_revealed(widget) \
  (gtk_widget_get_allocated_height ( \
     GTK_WIDGET (widget)) > 1 || \
   gtk_widget_get_allocated_width ( \
     GTK_WIDGET (widget)) > 1)

/**
 * Used in handlers to get the state mask.
 */
#define UI_GET_STATE_MASK(gesture) \
  GdkEventSequence * _sequence = \
    gtk_gesture_single_get_current_sequence ( \
      GTK_GESTURE_SINGLE (gesture)); \
  const GdkEvent * _event = \
    gtk_gesture_get_last_event ( \
      GTK_GESTURE (gesture), _sequence); \
  GdkModifierType state_mask; \
  gdk_event_get_state (_event, &state_mask)

/**
 * Various cursor states to be shared.
 */
typedef enum UiCursorState
{
  UI_CURSOR_STATE_DEFAULT,
  UI_CURSOR_STATE_RESIZE_L,
  UI_CURSOR_STATE_REPEAT_L,
  UI_CURSOR_STATE_RESIZE_R,
  UI_CURSOR_STATE_REPEAT_R,
  UI_CURSOR_STATE_RESIZE_UP,
} UiCursorState;

/**
 * Various overlay actions to be shared.
 */
typedef enum UiOverlayAction
{
  UI_OVERLAY_ACTION_NONE,
  UI_OVERLAY_ACTION_CREATING_RESIZING_R,
  UI_OVERLAY_ACTION_CREATING_MOVING,
  UI_OVERLAY_ACTION_RESIZING_L,
  UI_OVERLAY_ACTION_RESIZING_L_LOOP,
  UI_OVERLAY_ACTION_RESIZING_R,
  UI_OVERLAY_ACTION_RESIZING_R_LOOP,
  UI_OVERLAY_ACTION_RESIZING_UP,
  UI_OVERLAY_ACTION_STRETCHING_L,
  UI_OVERLAY_ACTION_STRETCHING_R,

  UI_OVERLAY_ACTION_AUDITIONING,

  /** Auto-filling in edit mode. */
  UI_OVERLAY_ACTION_AUTOFILLING,

  /** Erasing. */
  UI_OVERLAY_ACTION_ERASING,

  /**
   * To be set in drag_start.
   */
  UI_OVERLAY_ACTION_STARTING_MOVING,
  UI_OVERLAY_ACTION_STARTING_MOVING_COPY,
  UI_OVERLAY_ACTION_STARTING_MOVING_LINK,
  UI_OVERLAY_ACTION_MOVING,
  UI_OVERLAY_ACTION_MOVING_COPY,
  UI_OVERLAY_ACTION_MOVING_LINK,
  UI_OVERLAY_ACTION_STARTING_CHANGING_CURVE,
  UI_OVERLAY_ACTION_CHANGING_CURVE,

  /**
   * To be set in drag_start.
   *
   * Useful to check if nothing was clicked.
   */
  UI_OVERLAY_ACTION_STARTING_SELECTION,
  UI_OVERLAY_ACTION_SELECTING,

  /** Like selecting but it auto deletes whatever
   * touches the selection. */
  UI_OVERLAY_ACTION_STARTING_DELETE_SELECTION,
  UI_OVERLAY_ACTION_DELETE_SELECTING,

  UI_OVERLAY_ACTION_STARTING_RAMP,
  UI_OVERLAY_ACTION_RAMPING,
  UI_OVERLAY_ACTION_CUTTING,
} UiOverlayAction;

#define ui_set_pointer_cursor(widget) \
  ui_set_cursor_from_icon_name ( \
    GTK_WIDGET (widget), \
    "z-edit-select", \
    5, 4);

#define ui_set_pencil_cursor(widget) \
  ui_set_cursor_from_icon_name ( \
    GTK_WIDGET (widget), \
    "z-editor", \
    3, 6);

#define ui_set_cut_clip_cursor(widget) \
  ui_set_cursor_from_icon_name ( \
    GTK_WIDGET (widget), \
    "cut-clip", \
    9, 6);

#define ui_set_eraser_cursor(widget) \
  ui_set_cursor_from_icon_name ( \
    GTK_WIDGET (widget), \
    "z-draw-eraser", \
    3, 6);

#define ui_set_line_cursor(widget) \
  ui_set_cursor_from_icon_name ( \
    GTK_WIDGET (widget), \
    "z-draw-line", \
    3, 6);

#define ui_set_speaker_cursor(widget) \
  ui_set_cursor_from_icon_name ( \
    GTK_WIDGET (widget), \
    "z-audio-speakers-symbolic", \
    3, 6);

#define ui_set_hand_cursor(widget) \
  ui_set_cursor_from_icon_name ( \
    GTK_WIDGET (widget), \
    "z-labplot-transform-move", \
    10, 10);

#define ui_set_left_resize_cursor(widget) \
  ui_set_cursor_from_icon_name ( \
    GTK_WIDGET (widget), \
    "left-resize", \
    0, 10);

#define ui_set_left_resize_loop_cursor(widget) \
  ui_set_cursor_from_icon_name ( \
    GTK_WIDGET (widget), \
    "left-resize-loop", \
    0, 10);

#define ui_set_right_resize_cursor(widget) \
  ui_set_cursor_from_icon_name ( \
    GTK_WIDGET (widget), \
    "right-resize", \
    15, 10);

#define ui_set_right_resize_loop_cursor(widget) \
  ui_set_cursor_from_icon_name ( \
    GTK_WIDGET (widget), \
    "right-resize-loop", \
    15, 10);

/**
 * Sets cursor from icon name.
 */
void
ui_set_cursor_from_icon_name (
  GtkWidget *  widget,
  const char * name,
  int          offset_x,
  int          offset_y);

/**
 * Sets cursor from standard cursor name.
 */
void
ui_set_cursor_from_name (GtkWidget * widget, char * name);

gboolean
ui_on_motion_set_status_bar_text_cb (
  GtkWidget * widget,
  GdkEvent *  event,
  char *      text);

/**
 * Shows a popup message of the given type with the
 * given message.
 */
void
ui_show_message_full (
  GtkWindow * parent_window,
  GtkMessageType type,
  const char * message);

/**
 * Returns if the child is hit or not by the
 * coordinates in parent.
 *
 * @param check_x Check x-axis for match.
 * @param check_y Check y-axis for match.
 * @param x x in parent space.
 * @param y y in parent space.
 * @param x_padding Padding to add to the x
 *   of the object when checking if hit.
 *   The bigger the padding the more space the
 *   child will have to get hit.
 * @param y_padding Padding to add to the y
 *   of the object when checking if hit.
 *   The bigger the padding the more space the
 *   child will have to get hit.
 */
static inline int
ui_is_child_hit (
  GtkWidget * parent,
  GtkWidget *    child,
  const int            check_x,
  const int            check_y,
  const double         x,
  const double         y,
  const double         x_padding,
  const double         y_padding)
{
  GtkAllocation allocation;
  gtk_widget_get_allocation (
    child,
    &allocation);

  gint wx, wy;
  gtk_widget_translate_coordinates (
    GTK_WIDGET (parent),
    child,
    x, y, &wx, &wy);

  //g_message ("wx wy %d %d", wx, wy);

  /* if hit */
  if ((!check_x ||
        (wx >= - x_padding &&
         wx <= allocation.width + x_padding)) &&
      (!check_y ||
        (wy >= - y_padding &&
         wy <= allocation.height + y_padding)))
    {
      return 1;
    }
  return 0;
}

/**
 * Returns the matching hit child, or NULL.
 */
GtkWidget *
ui_get_hit_child (
  GtkContainer * parent,
  double         x, ///< x in parent space
  double         y, ///< y in parent space
  GType          type); ///< type to look for

/**
 * Converts from pixels to position.
 *
 * @param has_padding Whether the given px contain
 *   padding.
 */
void
ui_px_to_pos_timeline (
  double     px,
  Position * pos,
  int        has_padding);

/**
 * Converts from pixels to frames.
 *
 * Returns the frames.
 *
 * @param has_padding Whether then given px contains
 *   padding.
 */
long
ui_px_to_frames_timeline (
  double px,
  int    has_padding);

/**
 * Converts from pixels to frames.
 *
 * Returns the frames.
 *
 * @param has_padding Whether then given px contains
 *   padding.
 */
long
ui_px_to_frames_editor (
  double px,
  int    has_padding);

/**
 * Converts position to px, optionally adding the ruler
 * padding.
 */
int
ui_pos_to_px_timeline (
  Position * pos,
  int        use_padding);

/**
 * Converts position to px, optionally adding the ruler
 * padding.
 */
int
ui_pos_to_px_editor (
  Position * pos,
  int        use_padding);

/**
 * Converts from pixels to position.
 */
void
ui_px_to_pos_editor (double               px,
           Position *        pos,
           int               has_padding); ///< whether the given px contain padding


/**
 * Shows a notification in the revealer.
 */
void
ui_show_notification (const char * msg);

/**
 * Show notification from non-GTK threads.
 *
 * This should be used internally. Use the
 * ui_show_notification_idle macro instead.
 */
int
ui_show_notification_idle_func (char * msg);

/**
 * Returns the modifier type (state mask) from the
 * given gesture.
 */
void
ui_get_modifier_type_from_gesture (
  GtkGestureSingle * gesture,
  GdkModifierType *  state_mask); ///< return value

/**
 * Sets up a combo box to have a selection of
 * languages.
 */
void
ui_setup_language_combo_box (
  GtkComboBox * language);

/**
 * Sets up an audio backends combo box.
 */
void
ui_setup_audio_backends_combo_box (
  GtkComboBox * cb);

/**
 * Sets up a MIDI backends combo box.
 */
void
ui_setup_midi_backends_combo_box (
  GtkComboBox * cb);

/**
 * Sets up a pan algorithm combo box.
 */
void
ui_setup_pan_algo_combo_box (
  GtkComboBox * cb);

/**
 * Sets up a pan law combo box.
 */
void
ui_setup_pan_law_combo_box (
  GtkComboBox * cb);

/**
 * Returns the contrasting color (variation of
 * black or white) based on if the given color is
 * dark enough or not.
 *
 * @param src The source color.
 * @param dest The desination color to write to.
 */
void
ui_get_contrast_text_color (
  GdkRGBA * src,
  GdkRGBA * dest);

/**
 * @}
 */

#endif
