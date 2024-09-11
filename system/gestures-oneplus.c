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
#define TOUCHPANEL_SWIPE_UP_NODE "/proc/touchpanel/up_swipe_enable"
#define TOUCHPANEL_SWIPE_DOWN_NODE "/proc/touchpanel/down_swipe_enable"
#define TOUCHPANEL_SWIPE_LEFT_NODE "/proc/touchpanel/left_swipe_enable"
#define TOUCHPANEL_SWIPE_RIGHT_NODE "/proc/touchpanel/right_swipe_enable"

struct _GesturesOnePlus
{
    GObject parent_instance;

    gboolean double_tap_supported;
    gboolean double_tap_enabled;
    gboolean swipe_up_supported;
    gboolean swipe_up_enabled;
    gboolean swipe_down_supported;
    gboolean swipe_down_enabled;
    gboolean swipe_left_supported;
    gboolean swipe_left_enabled;
    gboolean swipe_right_supported;
    gboolean swipe_right_enabled;
};

typedef enum {
    GESTURES_ONEPLUS_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED = 1,
    GESTURES_ONEPLUS_PROP_DOUBLE_TAP_TO_WAKE_ENABLED,
    GESTURES_ONEPLUS_PROP_SWIPE_UP_TO_WAKE_SUPPORTED,
    GESTURES_ONEPLUS_PROP_SWIPE_UP_TO_WAKE_ENABLED,
    GESTURES_ONEPLUS_PROP_SWIPE_DOWN_TO_WAKE_SUPPORTED,
    GESTURES_ONEPLUS_PROP_SWIPE_DOWN_TO_WAKE_ENABLED,
    GESTURES_ONEPLUS_PROP_SWIPE_LEFT_TO_WAKE_SUPPORTED,
    GESTURES_ONEPLUS_PROP_SWIPE_LEFT_TO_WAKE_ENABLED,
    GESTURES_ONEPLUS_PROP_SWIPE_RIGHT_TO_WAKE_SUPPORTED,
    GESTURES_ONEPLUS_PROP_SWIPE_RIGHT_TO_WAKE_ENABLED,
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
set_swipe_up (gboolean swipe_up)
{
    g_autofree gchar *value = NULL;

    value = g_strdup_printf ("%d", swipe_up);
    g_debug ("Changing swipe_up setting: %d", swipe_up);

    write_to_file (TOUCHPANEL_SWIPE_UP_NODE, value);
}

static void
set_swipe_down (gboolean swipe_down)
{
    g_autofree gchar *value = NULL;

    value = g_strdup_printf ("%d", swipe_down);
    g_debug ("Changing swipe_down setting: %d", swipe_down);

    write_to_file (TOUCHPANEL_SWIPE_DOWN_NODE, value);
}

static void
set_swipe_left (gboolean swipe_left)
{
    g_autofree gchar *value = NULL;

    value = g_strdup_printf ("%d", swipe_left);
    g_debug ("Changing swipe_left setting: %d", swipe_left);

    write_to_file (TOUCHPANEL_SWIPE_LEFT_NODE, value);
}

static void
set_swipe_right (gboolean swipe_right)
{
    g_autofree gchar *value = NULL;

    value = g_strdup_printf ("%d", swipe_right);
    g_debug ("Changing swipe_right setting: %d", swipe_right);

    write_to_file (TOUCHPANEL_SWIPE_RIGHT_NODE, value);
}

static void
gestures_oneplus_constructed (GObject *obj)
{
    GesturesOnePlus *self = GESTURES_ONEPLUS (obj);

    G_OBJECT_CLASS (gestures_oneplus_parent_class)->constructed (obj);

    self->double_tap_supported = (access (TOUCHPANEL_DT2W_NODE, F_OK) == 0);
    self->double_tap_enabled = FALSE;
    self->swipe_up_supported = (access (TOUCHPANEL_SWIPE_UP_NODE, F_OK) == 0);
    self->swipe_up_enabled = FALSE;
    self->swipe_down_supported = (access (TOUCHPANEL_SWIPE_DOWN_NODE, F_OK) == 0);
    self->swipe_down_enabled = FALSE;
    self->swipe_left_supported = (access (TOUCHPANEL_SWIPE_LEFT_NODE, F_OK) == 0);
    self->swipe_left_enabled = FALSE;
    self->swipe_right_supported = (access (TOUCHPANEL_SWIPE_RIGHT_NODE, F_OK) == 0);
    self->swipe_right_enabled = FALSE;
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

        case GESTURES_ONEPLUS_PROP_SWIPE_UP_TO_WAKE_ENABLED:
            bool_value =  g_value_get_boolean (value);
            set_swipe_up (bool_value);
            self->swipe_up_enabled = bool_value;
            break;

        case GESTURES_ONEPLUS_PROP_SWIPE_DOWN_TO_WAKE_ENABLED:
            bool_value =  g_value_get_boolean (value);
            set_swipe_down (bool_value);
            self->swipe_down_enabled = bool_value;
            break;

        case GESTURES_ONEPLUS_PROP_SWIPE_LEFT_TO_WAKE_ENABLED:
            bool_value =  g_value_get_boolean (value);
            set_swipe_left (bool_value);
            self->swipe_left_enabled = bool_value;
            break;

        case GESTURES_ONEPLUS_PROP_SWIPE_RIGHT_TO_WAKE_ENABLED:
            bool_value =  g_value_get_boolean (value);
            set_swipe_right (bool_value);
            self->swipe_right_enabled = bool_value;
            break;

        case GESTURES_ONEPLUS_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED:
        case GESTURES_ONEPLUS_PROP_SWIPE_UP_TO_WAKE_SUPPORTED:
        case GESTURES_ONEPLUS_PROP_SWIPE_DOWN_TO_WAKE_SUPPORTED:
        case GESTURES_ONEPLUS_PROP_SWIPE_LEFT_TO_WAKE_SUPPORTED:
        case GESTURES_ONEPLUS_PROP_SWIPE_RIGHT_TO_WAKE_SUPPORTED:
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

        case GESTURES_ONEPLUS_PROP_SWIPE_UP_TO_WAKE_SUPPORTED:
            g_value_set_boolean (value, self->swipe_up_supported);
            break;

        case GESTURES_ONEPLUS_PROP_SWIPE_UP_TO_WAKE_ENABLED:
            g_value_set_boolean (value, self->swipe_up_enabled);
            break;

        case GESTURES_ONEPLUS_PROP_SWIPE_DOWN_TO_WAKE_SUPPORTED:
            g_value_set_boolean (value, self->swipe_down_supported);
            break;

        case GESTURES_ONEPLUS_PROP_SWIPE_DOWN_TO_WAKE_ENABLED:
            g_value_set_boolean (value, self->swipe_down_enabled);
            break;

        case GESTURES_ONEPLUS_PROP_SWIPE_LEFT_TO_WAKE_SUPPORTED:
            g_value_set_boolean (value, self->swipe_left_supported);
            break;

        case GESTURES_ONEPLUS_PROP_SWIPE_LEFT_TO_WAKE_ENABLED:
            g_value_set_boolean (value, self->swipe_left_enabled);
            break;

        case GESTURES_ONEPLUS_PROP_SWIPE_RIGHT_TO_WAKE_SUPPORTED:
            g_value_set_boolean (value, self->swipe_right_supported);
            break;

        case GESTURES_ONEPLUS_PROP_SWIPE_RIGHT_TO_WAKE_ENABLED:
            g_value_set_boolean (value, self->swipe_right_enabled);
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
    g_object_class_override_property (object_class, GESTURES_ONEPLUS_PROP_SWIPE_UP_TO_WAKE_SUPPORTED,
                                                                        "swipe-up-to-wake-supported");
    g_object_class_override_property (object_class, GESTURES_ONEPLUS_PROP_SWIPE_UP_TO_WAKE_ENABLED,
                                                                        "swipe-up-to-wake-enabled");
    g_object_class_override_property (object_class, GESTURES_ONEPLUS_PROP_SWIPE_DOWN_TO_WAKE_SUPPORTED,
                                                                        "swipe-down-to-wake-supported");
    g_object_class_override_property (object_class, GESTURES_ONEPLUS_PROP_SWIPE_DOWN_TO_WAKE_ENABLED,
                                                                        "swipe-down-to-wake-enabled");
    g_object_class_override_property (object_class, GESTURES_ONEPLUS_PROP_SWIPE_LEFT_TO_WAKE_SUPPORTED,
                                                                        "swipe-left-to-wake-supported");
    g_object_class_override_property (object_class, GESTURES_ONEPLUS_PROP_SWIPE_LEFT_TO_WAKE_ENABLED,
                                                                        "swipe-left-to-wake-enabled");
    g_object_class_override_property (object_class, GESTURES_ONEPLUS_PROP_SWIPE_RIGHT_TO_WAKE_SUPPORTED,
                                                                        "swipe-right-to-wake-supported");
    g_object_class_override_property (object_class, GESTURES_ONEPLUS_PROP_SWIPE_RIGHT_TO_WAKE_ENABLED,
                                                                        "swipe-right-to-wake-enabled");
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
    // Should we return true even if a gesture is missing?
    if (
            access (TOUCHPANEL_DT2W_NODE, F_OK) == 0 ||
            access(TOUCHPANEL_SWIPE_UP_NODE, F_OK) == 0 ||
            access(TOUCHPANEL_SWIPE_DOWN_NODE, F_OK) == 0 ||
            access(TOUCHPANEL_SWIPE_LEFT_NODE, F_OK) == 0 ||
            access(TOUCHPANEL_SWIPE_RIGHT_NODE, F_OK) == 0
       )
        return TRUE;

    return FALSE;
}
