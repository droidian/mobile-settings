/*
 * Copyright Eugenio Paolantonio (g7) <me@medesimo.eu>
 * Copyright Deepak Kumar <notwho53@gmail.com>
 */

#ifndef GESTURESI2C_H
#define GESTURESI2C_H

#include <glib.h>
#include <glib-object.h>

G_BEGIN_DECLS

#define TYPE_GESTURES_I2C gestures_i2c_get_type ()
G_DECLARE_FINAL_TYPE (GesturesI2c, gestures_i2c, GESTURES, I2C, GObject)

GesturesI2c *gestures_i2c_new (void);
gboolean gestures_i2c_supported (void);

G_END_DECLS

#endif /* GESTURESI2C_H */
