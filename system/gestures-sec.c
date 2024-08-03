/*
 * Copyright Eugenio Paolantonio (g7) <me@medesimo.eu>
 */

#define G_LOG_DOMAIN "gestures-sec"

#include <unistd.h>

#include "gestures.h"
#include "gestures-sec.h"

#include "../common/utils.h"

#define SEC_CMD_LIST  "/sys/devices/virtual/sec/tsp/cmd_list"
#define SEC_CMD       "/sys/devices/virtual/sec/tsp/cmd"
#define PANEL_PRE_SOD "/sys/devices/dsi_panel_driver/pre_sod_mode"

struct _GesturesSec
{
    GObject parent_instance;

    gboolean double_tap_supported;
    gboolean double_tap_enabled;
};

typedef enum {
    GESTURES_SEC_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED = 1,
    GESTURES_SEC_PROP_DOUBLE_TAP_TO_WAKE_ENABLED,
    GESTURES_SEC_PROP_LAST
} GesturesSecProperty;

static void gestures_sec_interface_init (GesturesInterface *iface);

G_DEFINE_TYPE_WITH_CODE (GesturesSec, gestures_sec, G_TYPE_OBJECT,
                            G_IMPLEMENT_INTERFACE (TYPE_GESTURES,
                                gestures_sec_interface_init))

static void
probe_features (GesturesSec *self)
{
    g_autofree gchar *cmd_list = NULL;
    g_autoptr(GError) error = NULL;
    gchar **lines;

    if (!g_file_get_contents (SEC_CMD_LIST, &cmd_list, NULL, &error)) {
        g_warning ("Unable to read SEC command list file: %s", error->message);
        return;
    }

    lines = g_strsplit (cmd_list, "\n", -1);
    for (guint i = 0; i < g_strv_length (lines); i++) {
        if (g_strcmp0 (lines[i], "sod_enable") == 0) {
            self->double_tap_supported = TRUE;
            break;
        }
    }

    g_strfreev (lines);
}

static void
set_double_tap (gboolean double_tap)
{
    g_debug ("Changing double_tap setting: %d", double_tap);
    write_to_file (SEC_CMD, double_tap ? "sod_enable,1" : "sod_enable,0");

    if (access (PANEL_PRE_SOD, F_OK) == 0)
        write_to_file (PANEL_PRE_SOD, double_tap ? "1" : "0");
}

static void
gestures_sec_constructed (GObject *obj)
{
    GesturesSec *self = GESTURES_SEC (obj);

    G_OBJECT_CLASS (gestures_sec_parent_class)->constructed (obj);

    self->double_tap_supported = FALSE;
    self->double_tap_enabled = FALSE;

    probe_features (self);
}

static void
gestures_sec_dispose (GObject *obj)
{
    G_OBJECT_CLASS (gestures_sec_parent_class)->dispose (obj);
}

static void
gestures_sec_set_property (GObject      *obj,
                           uint         property_id,
                           const GValue *value,
                           GParamSpec   *pspec)
{
    GesturesSec *self = GESTURES_SEC (obj);
    gboolean bool_value;

    switch ((GesturesSecProperty) property_id)
        {
        case GESTURES_SEC_PROP_DOUBLE_TAP_TO_WAKE_ENABLED:
            bool_value =  g_value_get_boolean (value);
            set_double_tap (bool_value);
            self->double_tap_enabled = bool_value;
            break;

        case GESTURES_SEC_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED:
        case GESTURES_SEC_PROP_LAST:
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property_id, pspec);
            break;
        }

}

static void
gestures_sec_get_property (GObject    *obj,
                           uint       property_id,
                           GValue     *value,
                           GParamSpec *pspec)
{
    GesturesSec *self = GESTURES_SEC (obj);

    switch ((GesturesSecProperty) property_id)
        {
        case GESTURES_SEC_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED:
            g_value_set_boolean (value, self->double_tap_supported);
            break;

        case GESTURES_SEC_PROP_DOUBLE_TAP_TO_WAKE_ENABLED:
            g_value_set_boolean (value, self->double_tap_enabled);
            break;

        case GESTURES_SEC_PROP_LAST:
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property_id, pspec);
            break;
        }

}

static void
gestures_sec_class_init (GesturesSecClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->constructed  = gestures_sec_constructed;
    object_class->dispose      = gestures_sec_dispose;
    object_class->set_property = gestures_sec_set_property;
    object_class->get_property = gestures_sec_get_property;

    g_object_class_override_property (object_class, GESTURES_SEC_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED,
                                      "double-tap-to-wake-supported");
    g_object_class_override_property (object_class, GESTURES_SEC_PROP_DOUBLE_TAP_TO_WAKE_ENABLED,
                                      "double-tap-to-wake-enabled");
}

static void
gestures_sec_interface_init (GesturesInterface *iface)
{}

static void
gestures_sec_init (GesturesSec *self)
{}

GesturesSec *
gestures_sec_new (void)
{
    return g_object_new (TYPE_GESTURES_SEC, NULL);
}

gboolean
gestures_sec_supported (void)
{
    if (access (SEC_CMD_LIST, F_OK) == 0)
        return TRUE;

    return FALSE;
}
