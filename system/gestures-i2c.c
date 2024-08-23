/*
 * Copyright Eugenio Paolantonio (g7) <me@medesimo.eu>
 * Copyright Cedric Bellegarde <cedric.bellegarde@adishatz.org>
 * Copyright Deepak Kumar <notwho53@gmail.com>
 */

#define G_LOG_DOMAIN "gestures-i2c"

#include <unistd.h>
#include <stdio.h>
#include <libudev.h>
#include <limits.h>

#include "gestures.h"
#include "gestures-i2c.h"

#include "../common/utils.h"

static char* find_gesture_mode_node(void) {                                                                                     
    struct udev *udev;                                                                                                          
    struct udev_enumerate *enumerate;                                                                                           
    struct udev_list_entry *devices, *dev_list_entry;                                                                           
    struct udev_device *dev;                                                                                                    
    char *path = NULL;                                                                                                          
    char resolved_path[PATH_MAX];                                                                                               
    const char *syspath;                                                                                                        
    const char *sysattr;                                                                                                        
    const char *attributes[] = {"fts_gesture_mode", "wake_gesture"};                                                            
    int num_attributes = sizeof(attributes) / sizeof(attributes[0]);                                                            
                                                                                                                                
    udev = udev_new();                                                                                                          
    if (!udev) {                                                                                                                
        return NULL;                                                                                                            
    }                                                                                                                           
                                                                                                                                
    enumerate = udev_enumerate_new(udev);                                                                                       
    udev_enumerate_add_match_subsystem(enumerate, "i2c");                                                                       
    udev_enumerate_scan_devices(enumerate);                                                                                     
    devices = udev_enumerate_get_list_entry(enumerate);                                                                         
                                                                                                                                
    udev_list_entry_foreach(dev_list_entry, devices) {                                                                          
        syspath = udev_list_entry_get_name(dev_list_entry);                                                                     
        dev = udev_device_new_from_syspath(udev, syspath);                                                                      
                                                                                                                                
        for (int i = 0; i < num_attributes; i++) {                                                                              
            sysattr = udev_device_get_sysattr_value(dev, attributes[i]);                                                        
            if (sysattr) {                                                                                                      
                if (realpath(syspath, resolved_path) != NULL) {                                                                 
                    size_t full_path_len = strlen(resolved_path) + strlen(attributes[i]) + 2; 
                    path = malloc(full_path_len);                                                                               
                    if (path) {                                                                                                 
                        snprintf(path, full_path_len, "%s/%s", resolved_path, attributes[i]);                                   
                    }                                                                                                           
                    udev_device_unref(dev);                                                                                     
                    goto cleanup;                                                                           
                }                                                                                                               
            }                                                                                                                   
        }                                                                                                                       
                                                                                                                                
        udev_device_unref(dev);                                                                                                 
    }                                                                                                                           
                                                                                                                                
cleanup:                                                                                                                        
    udev_enumerate_unref(enumerate);                                                                                            
    udev_unref(udev);                                                                                                           
                                                                                                                                
    return path;                                                                                                                
}                                                                                                                               

struct _GesturesI2c
{
    GObject parent_instance;

    gboolean double_tap_supported;
    gboolean double_tap_enabled;
    char *touchpanel_dt2w_node;
};

typedef enum {
    GESTURES_I2C_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED = 1,
    GESTURES_I2C_PROP_DOUBLE_TAP_TO_WAKE_ENABLED,
    GESTURES_I2C_PROP_LAST
} GesturesI2cProperty;

static void gestures_i2c_interface_init (GesturesInterface *iface);

G_DEFINE_TYPE_WITH_CODE (GesturesI2c, gestures_i2c, G_TYPE_OBJECT,
                            G_IMPLEMENT_INTERFACE (TYPE_GESTURES,
                                gestures_i2c_interface_init))

static void
set_double_tap (GesturesI2c *self, gboolean double_tap)
{
    g_autofree gchar *value = NULL;

    value = g_strdup_printf ("%d", double_tap);
    g_debug ("Changing double_tap setting: %d", double_tap);

    if (self->touchpanel_dt2w_node) {
        write_to_file (self->touchpanel_dt2w_node, value);
    }
}

static void
gestures_i2c_constructed (GObject *obj)
{
    GesturesI2c *self = GESTURES_I2C (obj);

    G_OBJECT_CLASS (gestures_i2c_parent_class)->constructed (obj);

    self->touchpanel_dt2w_node = find_gesture_mode_node();
    self->double_tap_supported = (self->touchpanel_dt2w_node != NULL);
    self->double_tap_enabled = FALSE;
}

static void
gestures_i2c_dispose (GObject *obj)
{
    GesturesI2c *self = GESTURES_I2C (obj);

    g_free(self->touchpanel_dt2w_node);
    self->touchpanel_dt2w_node = NULL;

    G_OBJECT_CLASS (gestures_i2c_parent_class)->dispose (obj);
}

static void
gestures_i2c_set_property (GObject      *obj,
                              uint         property_id,
                              const GValue *value,
                              GParamSpec   *pspec)
{
    GesturesI2c *self = GESTURES_I2C (obj);
    gboolean bool_value;

    switch ((GesturesI2cProperty) property_id)
        {
        case GESTURES_I2C_PROP_DOUBLE_TAP_TO_WAKE_ENABLED:
            bool_value =  g_value_get_boolean (value);
            set_double_tap (self, bool_value);
            self->double_tap_enabled = bool_value;
            break;

        case GESTURES_I2C_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED:
        case GESTURES_I2C_PROP_LAST:
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property_id, pspec);
            break;
        }

}

static void
gestures_i2c_get_property (GObject    *obj,
                              uint       property_id,
                              GValue     *value,
                              GParamSpec *pspec)
{
    GesturesI2c *self = GESTURES_I2C (obj);

    switch ((GesturesI2cProperty) property_id)
        {
        case GESTURES_I2C_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED:
            g_value_set_boolean (value, self->double_tap_supported);
            break;

        case GESTURES_I2C_PROP_DOUBLE_TAP_TO_WAKE_ENABLED:
            g_value_set_boolean (value, self->double_tap_enabled);
            break;

        case GESTURES_I2C_PROP_LAST:
        default:
            G_OBJECT_WARN_INVALID_PROPERTY_ID (obj, property_id, pspec);
            break;
        }

}

static void
gestures_i2c_class_init (GesturesI2cClass *klass)
{
    GObjectClass *object_class = G_OBJECT_CLASS (klass);

    object_class->constructed  = gestures_i2c_constructed;
    object_class->dispose      = gestures_i2c_dispose;
    object_class->set_property = gestures_i2c_set_property;
    object_class->get_property = gestures_i2c_get_property;

    g_object_class_override_property (object_class, GESTURES_I2C_PROP_DOUBLE_TAP_TO_WAKE_SUPPORTED,
                                                                        "double-tap-to-wake-supported");
    g_object_class_override_property (object_class, GESTURES_I2C_PROP_DOUBLE_TAP_TO_WAKE_ENABLED,
                                                                        "double-tap-to-wake-enabled");
}

static void
gestures_i2c_interface_init (GesturesInterface *iface)
{}

static void
gestures_i2c_init (GesturesI2c *self)
{
    self->touchpanel_dt2w_node = NULL;
}

GesturesI2c *
gestures_i2c_new (void)
{
    return g_object_new (TYPE_GESTURES_I2C, NULL);
}

gboolean
gestures_i2c_supported (void)
{
    char *node_path = find_gesture_mode_node();
    gboolean supported = (node_path != NULL);
    free(node_path);
    return supported;
}
