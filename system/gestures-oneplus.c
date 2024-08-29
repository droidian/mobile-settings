/*
 * Copyright Eugenio Paolantonio (g7) <me@medesimo.eu>
 * Copyright Cedric Bellegarde <cedric.bellegarde@adishatz.org>
 * Copyright Chris Mustola (Bettehem) <chris.mustola@gmail.com>
 */

#define G_LOG_DOMAIN "gestures-oneplus"

#include <unistd.h>

#include "gestures.h"
#include "gestures-oneplus.h"

#include "../common/utils.h"

#define TOUCHPANEL_DT2W_NODE "/proc/touchpanel/double_tap_enable"

struct _GesturesOnePlus
{
    GObject parent_instance;

    gboolean double_tap_supported;
    gboolean double_tap_enabled;
};

typedef enum {
    GESTURES_ONEPLUS_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED = 1,
    GESTURES_ONEPLUS_PROP_DOUBLE_TAP_TO_WAKE_ENABLED,
    GESTURES_ONEPLUS_PROP_LAST
} GesturesOnePlusProperty;

static void gestures_oneplus_interface_init (GesturesInterface *iface);

G_DEFINE_TYPE_WITH_CODE (GesturesOnePlus, gestures_oneplus, G_TYPE_OBJECT,
                            G_IMPLEMENT_INTERFACE (TYPE_GESTURES,
                                gestures_oneplus_interface_init))

static void
set_double_tap (gboolean double_tap)
{
    g_autofree gchar *value = NULL;

    value = g_strdup_printf ("%d", double_tap);
    g_debug ("Changing double_tap setting: %d", double_tap);

    write_to_file (TOUCHPANEL_DT2W_NODE, value);
}

static void
gestures_oneplus_constructed (GObject *obj)
{
    GesturesOnePlus *self = GESTURES_ONEPLUS (obj);

    G_OBJECT_CLASS (gestures_oneplus_parent_class)->constructed (obj);

    self->double_tap_supported = (access (TOUCHPANEL_DT2W_NODE, F_OK) == 0);
    self->double_tap_enabled = FALSE;
}

static void
gestures_oneplus_dispose (GObject *obj)
{
    G_OBJECT_CLASS (gestures_oneplus_parent_class)->dispose (obj);
}

static void
gestures_oneplus_set_property (GObject      *obj,
                              uint         property_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
    GesturesOnePlus *self = GESTURES_ONEPLUS (obj);
    gboolean bool_value;

    switch ((GesturesOnePlusProperty) property_id)
        {
        case GESTURES_ONEPLUS_PROP_DOUBLE_TAP_TO_WAKE_ENABLED:
            bool_value =  g_value_get_boolean (value);
            set_double_tap (bool_value);
            self->double_tap_enabled = bool_value;
            break;

        case GESTURES_ONEPLUS_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED:
        case GESTURES_ONEPLUS_PROP_LAST:
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property_id, pspec);
            break;
        }

}

static void
gestures_oneplus_get_property (GObject    *obj,
                              uint       property_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
    GesturesOnePlus *self = GESTURES_ONEPLUS (obj);

    switch ((GesturesOnePlusProperty) property_id)
        {
        case GESTURES_ONEPLUS_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED:
            g_value_set_boolean (value, self->double_tap_supported);
            break;

        case GESTURES_ONEPLUS_PROP_DOUBLE_TAP_TO_WAKE_ENABLED:
            g_value_set_boolean (value, self->double_tap_enabled);
            break;

        case GESTURES_ONEPLUS_PROP_LAST:
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property_id, pspec);
            break;
        }

}

static void
gestures_oneplus_class_init (GesturesOnePlusClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->constructed  = gestures_oneplus_constructed;
    object_class->dispose      = gestures_oneplus_dispose;
    object_class->set_property = gestures_oneplus_set_property;
    object_class->get_property = gestures_oneplus_get_property;

    g_object_class_override_property (object_class, GESTURES_ONEPLUS_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED,
                                                                        "double-tap-to-wake-supported");
    g_object_class_override_property (object_class, GESTURES_ONEPLUS_PROP_DOUBLE_TAP_TO_WAKE_ENABLED,
                                                                        "double-tap-to-wake-enabled");
}

static void
gestures_oneplus_interface_init (GesturesInterface *iface)
{}

static void
gestures_oneplus_init (GesturesOnePlus *self)
{}

GesturesOnePlus *
gestures_oneplus_new (void)
{
    return g_object_new (TYPE_GESTURES_ONEPLUS, NULL);
}

gboolean
gestures_oneplus_supported (void)
{
    if (access (TOUCHPANEL_DT2W_NODE, F_OK) == 0)
        return TRUE;

    // TODO: do other checks?
    return FALSE;
}
