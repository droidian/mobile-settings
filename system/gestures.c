/*
 * Copyright Eugenio Paolantonio (g7) <me@medesimo.eu>
 */

#define G_LOG_DOMAIN "gestures-interface"

#include "gestures.h"

G_DEFINE_INTERFACE (Gestures, gestures, G_TYPE_OBJECT)

static void
gestures_default_init (GesturesInterface *iface)
{
    g_object_interface_install_property (iface,
                                         g_param_spec_boolean ("double-tap-to-wake-supported",
                                                               "double-tap-to-wake-supported",
                                                               "Whether the device supports double tap to wake",
                                                               FALSE,
                                                               G_PARAM_READABLE));
    g_object_interface_install_property (iface,
                                         g_param_spec_boolean ("double-tap-to-wake-enabled",
                                                               "double-tap-to-wake-enabled",
                                                               "Whether double tap to wake is enabled",
                                                               FALSE,
                                                               G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_interface_install_property (iface,
                                         g_param_spec_boolean ("swipe-up-to-wake-supported",
                                                               "swipe-up-to-wake-supported",
                                                               "Whether the device supports swipe up to wake",
                                                               FALSE,
                                                               G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_interface_install_property (iface,
                                         g_param_spec_boolean ("swipe-up-to-wake-enabled",
                                                               "swipe-up-to-wake-enabled",
                                                               "Whether swipe up to wake is enabled",
                                                               FALSE,
                                                               G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_interface_install_property (iface,
                                         g_param_spec_boolean ("swipe-down-to-wake-supported",
                                                               "swipe-down-to-wake-supported",
                                                               "Whether the device supports swipe down to wake",
                                                               FALSE,
                                                               G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_interface_install_property (iface,
                                         g_param_spec_boolean ("swipe-down-to-wake-enabled",
                                                               "swipe-down-to-wake-enabled",
                                                               "Whether swipe down to wake is enabled",
                                                               FALSE,
                                                               G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_interface_install_property (iface,
                                         g_param_spec_boolean ("swipe-left-to-wake-supported",
                                                               "swipe-left-to-wake-supported",
                                                               "Whether the device supports swipe left to wake",
                                                               FALSE,
                                                               G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_interface_install_property (iface,
                                         g_param_spec_boolean ("swipe-left-to-wake-enabled",
                                                               "swipe-left-to-wake-enabled",
                                                               "Whether swipe left to wake is enabled",
                                                               FALSE,
                                                               G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_interface_install_property (iface,
                                         g_param_spec_boolean ("swipe-right-to-wake-supported",
                                                               "swipe-right-to-wake-supported",
                                                               "Whether the device supports swipe right to wake",
                                                               FALSE,
                                                               G_PARAM_READABLE | G_PARAM_WRITABLE));
    g_object_interface_install_property (iface,
                                         g_param_spec_boolean ("swipe-right-to-wake-enabled",
                                                               "swipe-right-to-wake-enabled",
                                                               "Whether swipe right to wake is enabled",
                                                               FALSE,
                                                               G_PARAM_READABLE | G_PARAM_WRITABLE));
}
