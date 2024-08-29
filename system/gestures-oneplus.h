/*
 * Copyright Eugenio Paolantonio (g7) <me@medesimo.eu>
 * Copyright Chris Mustola (Bettehem) <chris.mustola@gmail.com>
 */

#ifndef GESTURESONEPLUS_H
#define GESTURESONEPLUS_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_GESTURES_ONEPLUS gestures_oneplus_get_type ()
G_DECLARE_FINAL_TYPE (GesturesOnePlus, gestures_oneplus, GESTURES, ONEPLUS, GObject)

GesturesOnePlus *gestures_oneplus_new (void);
gboolean gestures_oneplus_supported (void);

G_END_DECLS

#endif /* GESTURESONEPLUS_H */
