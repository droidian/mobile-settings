/*
 * Copyright Eugenio Paolantonio (g7) <me@medesimo.eu>
 */

#ifndef GESTURESSEC_H
#define GESTURESSEC_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_GESTURES_SEC gestures_sec_get_type ()
G_DECLARE_FINAL_TYPE (GesturesSec, gestures_sec, GESTURES, SEC, GObject)

GesturesSec *gestures_sec_new (void);
gboolean gestures_sec_supported (void);

G_END_DECLS

#endif /* GESTURESSEC_H */
