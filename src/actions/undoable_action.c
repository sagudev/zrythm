/*
 * Copyright (C) 2019-2021 Alexandros Theodotou <alex at zrythm dot org>
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

#include "audio/engine.h"
#include "actions/arranger_selections.h"
#include "actions/mixer_selections_action.h"
#include "actions/copy_tracks_action.h"
#include "actions/create_tracks_action.h"
#include "actions/delete_tracks_action.h"
#include "actions/edit_tracks_action.h"
#include "actions/move_tracks_action.h"
#include "actions/range_action.h"
#include "actions/transport_action.h"
#include "actions/undoable_action.h"
#include "project.h"
#include "zrythm_app.h"

#include <glib.h>
#include <glib/gi18n.h>

void
undoable_action_init_loaded (
  UndoableAction * self)
{
  /* uppercase, camel case, snake case */
#define INIT_LOADED(uc,sc,cc) \
  case UA_##uc: \
    sc##_action_init_loaded ((cc##Action *) self); \
    break;

  switch (self->type)
    {
    /*INIT_LOADED (CREATE_TRACKS,*/
               /*create_tracks,*/
               /*CreateTracks);*/
    INIT_LOADED (MOVE_TRACKS,
               move_tracks,
               MoveTracks);
    INIT_LOADED (EDIT_TRACKS,
               edit_tracks,
               EditTracks);
    INIT_LOADED (COPY_TRACKS,
               copy_tracks,
               CopyTracks);
    INIT_LOADED (DELETE_TRACKS,
               delete_tracks,
               DeleteTracks);
    INIT_LOADED (CHANNEL_SEND,
               channel_send,
               ChannelSend);
    INIT_LOADED (
      MIXER_SELECTIONS, mixer_selections,
      MixerSelections);
    INIT_LOADED (
      ARRANGER_SELECTIONS, arranger_selections,
      ArrangerSelections);
    INIT_LOADED (
      MIDI_MAPPING, midi_mapping, MidiMapping);
    INIT_LOADED (
      PORT_CONNECTION, port_connection,
      PortConnection);
    INIT_LOADED (PORT, port, Port);
    /*INIT_LOADED (TRANSPORT, transport, Transport);*/
    INIT_LOADED (RANGE, range, Range);
    default:
      break;
    }

#undef INIT_LOADED
}

/**
 * Performs the action.
 *
 * @note Only to be called by undo manager.
 *
 * @return Non-zero if errors occurred.
 */
int
undoable_action_do (UndoableAction * self)
{
#if 0
  g_debug ("waiting for port operation lock...");
  zix_sem_wait (&AUDIO_ENGINE->port_operation_lock);
  g_debug ("lock acquired");
#endif

  /* stop engine and give it some time to stop
   * running */
  int run_before =
    g_atomic_int_get (&AUDIO_ENGINE->run);
  g_atomic_int_set (&AUDIO_ENGINE->run, 0);
  while (g_atomic_int_get (
           &AUDIO_ENGINE->cycle_running))
    {
      g_usleep (100);
    }

  int ret = 0;

  /* uppercase, camel case, snake case */
#define DO_ACTION(uc,sc,cc) \
  case UA_##uc: \
    { \
      char * str = \
        undoable_action_stringize (self); \
      g_message ( \
        "[DOING ACTION]: " #uc " (%s)", str); \
      ret = sc##_action_do ((cc##Action *) self); \
      g_message ("[DONE]: " #uc " (%s)", str); \
      g_free (str); \
    } \
    break;

  switch (self->type)
    {
    DO_ACTION (CREATE_TRACKS,
               create_tracks,
               CreateTracks);
    DO_ACTION (MOVE_TRACKS,
               move_tracks,
               MoveTracks);
    DO_ACTION (EDIT_TRACKS,
               edit_tracks,
               EditTracks);
    DO_ACTION (COPY_TRACKS,
               copy_tracks,
               CopyTracks);
    DO_ACTION (DELETE_TRACKS,
               delete_tracks,
               DeleteTracks);
    DO_ACTION (CHANNEL_SEND,
               channel_send,
               ChannelSend);
    DO_ACTION (
      MIXER_SELECTIONS, mixer_selections,
      MixerSelections);
    DO_ACTION (
      ARRANGER_SELECTIONS, arranger_selections,
      ArrangerSelections);
    DO_ACTION (TRANSPORT, transport, Transport);
    DO_ACTION (RANGE, range, Range);
    DO_ACTION (
      PORT_CONNECTION, port_connection,
      PortConnection);
    DO_ACTION (PORT, port, Port);
    DO_ACTION (
      MIDI_MAPPING, midi_mapping, MidiMapping);
    default:
      g_warn_if_reached ();
      ret = -1;
    }

#undef DO_ACTION

#if 0
  g_debug ("releasing lock...");
  zix_sem_post (&AUDIO_ENGINE->port_operation_lock);
  g_debug ("lock released");
#endif

  /* restart engine */
  g_atomic_int_set (
    &AUDIO_ENGINE->run, (guint) run_before);

  return ret;
}

/**
 * Undoes the action.
 *
 * Note: only to be called by undo manager.
 */
int
undoable_action_undo (UndoableAction * self)
{
  /*zix_sem_wait (&AUDIO_ENGINE->port_operation_lock);*/

  /* stop engine and give it some time to stop
   * running */
  int run_before =
    g_atomic_int_get (&AUDIO_ENGINE->run);
  g_atomic_int_set (&AUDIO_ENGINE->run, 0);
  while (g_atomic_int_get (
           &AUDIO_ENGINE->cycle_running))
    {
      g_usleep (100);
    }

  int ret = 0;

/* uppercase, camel case, snake case */
#define UNDO_ACTION(uc,sc,cc) \
  case UA_##uc: \
    { \
      char * str = \
        undoable_action_stringize (self); \
      g_message ( \
        "[UNDOING ACTION]: " #uc " (%s)", str); \
      ret = \
        sc##_action_undo ((cc##Action *) self); \
      g_message ( \
        "[UNDONE]: " #uc " (%s)", str); \
      g_free (str); \
    } \
    break;

  switch (self->type)
    {
    UNDO_ACTION (CREATE_TRACKS,
               create_tracks,
               CreateTracks);
    UNDO_ACTION (MOVE_TRACKS,
               move_tracks,
               MoveTracks);
    UNDO_ACTION (EDIT_TRACKS,
               edit_tracks,
               EditTracks);
    UNDO_ACTION (COPY_TRACKS,
               copy_tracks,
               CopyTracks);
    UNDO_ACTION (DELETE_TRACKS,
               delete_tracks,
               DeleteTracks);
    UNDO_ACTION (CHANNEL_SEND,
               channel_send,
               ChannelSend);
    UNDO_ACTION (
      MIXER_SELECTIONS, mixer_selections,
      MixerSelections);
    UNDO_ACTION (
      ARRANGER_SELECTIONS, arranger_selections,
      ArrangerSelections);
    UNDO_ACTION (
      MIDI_MAPPING, midi_mapping, MidiMapping);
    UNDO_ACTION (
      PORT_CONNECTION, port_connection,
      PortConnection);
    UNDO_ACTION (PORT, port, Port);
    UNDO_ACTION (TRANSPORT, transport, Transport);
    UNDO_ACTION (RANGE, range, Range);
    default:
      g_warn_if_reached ();
      ret = -1;
    }

#undef UNDO_ACTION

  /*zix_sem_post (&AUDIO_ENGINE->port_operation_lock);*/

  /* restart engine */
  g_atomic_int_set (
    &AUDIO_ENGINE->run, (guint) run_before);

  return ret;
}

void
undoable_action_free (UndoableAction * self)
{
/* uppercase, camel case, snake case */
#define FREE_ACTION(uc,sc,cc) \
  case UA_##uc: \
    sc##_action_free ((cc##Action *) self); \
    break;

  switch (self->type)
    {
    FREE_ACTION (CREATE_TRACKS,
               create_tracks,
               CreateTracks);
    FREE_ACTION (MOVE_TRACKS,
               move_tracks,
               MoveTracks);
    FREE_ACTION (EDIT_TRACKS,
               edit_tracks,
               EditTracks);
    FREE_ACTION (COPY_TRACKS,
               copy_tracks,
               CopyTracks);
    FREE_ACTION (DELETE_TRACKS,
               delete_tracks,
               DeleteTracks);
    FREE_ACTION (CHANNEL_SEND,
               channel_send,
               ChannelSend);
    FREE_ACTION (
      MIXER_SELECTIONS, mixer_selections,
      MixerSelections);
    FREE_ACTION (
      ARRANGER_SELECTIONS, arranger_selections,
      ArrangerSelections);
    FREE_ACTION (
      MIDI_MAPPING, midi_mapping, MidiMapping);
    FREE_ACTION (
      PORT_CONNECTION, port_connection,
      PortConnection);
    FREE_ACTION (PORT, port, Port);
    FREE_ACTION (TRANSPORT, transport, Transport);
    FREE_ACTION (RANGE, range, Range);
    default:
      g_warn_if_reached ();
      break;
    }

#undef FREE_ACTION
}

/**
 * Stringizes the action to be used in Undo/Redo
 * buttons.
 *
 * The string MUST be free'd using g_free().
 */
char *
undoable_action_stringize (
  UndoableAction * ua)
{
#define STRINGIZE_UA(caps,cc,sc) \
  case UA_##caps: \
    return sc##_action_stringize ( \
      (cc##Action *) ua);

  switch (ua->type)
    {
    STRINGIZE_UA (CREATE_TRACKS,
                  CreateTracks,
                  create_tracks);
    STRINGIZE_UA (MOVE_TRACKS,
                  MoveTracks,
                  move_tracks);
    STRINGIZE_UA (EDIT_TRACKS,
                  EditTracks,
                  edit_tracks);
    STRINGIZE_UA (COPY_TRACKS,
                  CopyTracks,
                  copy_tracks);
    STRINGIZE_UA (DELETE_TRACKS,
                  DeleteTracks,
                  delete_tracks);
    STRINGIZE_UA (CHANNEL_SEND,
               ChannelSend,
               channel_send);
    STRINGIZE_UA (
      MIXER_SELECTIONS, MixerSelections,
      mixer_selections);
    STRINGIZE_UA (
      ARRANGER_SELECTIONS, ArrangerSelections,
      arranger_selections);
    STRINGIZE_UA (
      MIDI_MAPPING, MidiMapping, midi_mapping);
    STRINGIZE_UA (
      PORT_CONNECTION, PortConnection,
      port_connection);
    STRINGIZE_UA (PORT, Port, port);
    STRINGIZE_UA (TRANSPORT, Transport, transport);
    STRINGIZE_UA (RANGE, Range, range);
    default:
      g_return_val_if_reached (
        g_strdup (""));
    }

#undef STRINGIZE_UA
}
