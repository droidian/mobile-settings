/*
 * Copyright Cedric Bellegarde <cedric.bellegarde@adishatz.org>
 */

#include <stdio.h>
#include <stdarg.h>
#include <glib.h>

#include "utils.h"

void write_to_file (const gchar *filename,
                    const gchar *value)
{
    FILE *file;

    if (!g_file_test (filename, G_FILE_TEST_EXISTS))
        return;

    file = fopen(filename, "w");

    if (file == NULL)
        g_warning ("Can't write to %s", filename);
    else {
        fprintf (file, "%s", value);
        fclose (file);
    }
}

gchar *read_from_file (const gchar *filename)
{
    
   gchar *buffer = NULL;
   int file_size, read_size;
   FILE *file = fopen(filename, "r");

   if (file)
   {
       fseek(file, 0, SEEK_END);
       file_size = ftell(file);
       rewind(file);

       buffer = (char*) malloc(sizeof(char) * (file_size + 1) );

       read_size = fread(buffer, sizeof(char), file_size, file);
       buffer[file_size] = '\0';

       if (file_size != read_size) {
           g_free(buffer);
           buffer = NULL;
       }

       fclose(file);
    }

    return buffer;
}