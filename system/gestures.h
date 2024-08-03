/*
 * Copyright Eugenio Paolantonio (g7) <me@medesimo.eu>
 */

#ifndef GESTURES_H
#define GESTURES_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_GESTURES gestures_get_type ()
G_DECLARE_INTERFACE (Gestures, gestures, GESTURES, IFACE, GObject)

struct _GesturesInterface
{
  GTypeInterface parent_iface;
};

G_END_DECLS

#endif /* GESTURES_H */
