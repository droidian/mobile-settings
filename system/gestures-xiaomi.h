/*
 * Copyright Eugenio Paolantonio (g7) <me@medesimo.eu>
 */

#ifndef GESTURESXIAOMI_H
#define GESTURESXIAOMI_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_GESTURES_XIAOMI gestures_xiaomi_get_type ()
G_DECLARE_FINAL_TYPE (GesturesXiaomi, gestures_xiaomi, GESTURES, XIAOMI, GObject)

GesturesXiaomi *gestures_xiaomi_new (void);
gboolean gestures_xiaomi_supported (void);

G_END_DECLS

#endif /* GESTURESXIAOMI_H */
