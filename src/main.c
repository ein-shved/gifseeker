/* Gif Seeker is a simple tool for gif files seeking.  
 * Copyright (C) 2013  Shvedov Yury
 * 
 * This file is part of Gif Seeker.
 *
 * Gif Seeker is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Gif Seeker is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Devil.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "gifseeker.h"
#include "gtk_interface.h"
#include "../config.h"

#include <gtk/gtk.h>

static const char *
description =
"You can shift within files freely.\n"
"Use arrows Up or Right to switch on next picture in gif.\n"
"Use arrows Down or Left to switch on previous picture in gif.\n"
"Use Return or Mouse button to choose picture randomly.\n"
"Use PageUp to switch on first image of next gif.\n"
"Use PageDown to switch on first image of preveous gif.\n"
"Use key R to switch slidshow mode on/off.\n"
"Use Esc to quit.\n"
"\n"
"Bug report: " PACKAGE_BUGREPORT "\n"
"Thank you for your interest.\n";

int interface_runner (PContext c,
        int *argc, char ***argv, void *user_data)
{
    gboolean version = FALSE;
    GOptionContext *option_context;
    GError *g_error = NULL;
    GOptionEntry option_entries[] = {
        //{"file", 'f', 0, G_OPTION_ARG_FILENAME, &filename, "Gif file to open", "FILE"},
        {"version", 'V', 0, G_OPTION_ARG_NONE, &version, "Show version", NULL},
        { NULL }
    };
    int gif, error = 0, i;

    option_context = g_option_context_new("[FILE...] - " 
            "take random image from gif files.");
    g_option_context_set_description (option_context, description);
    g_option_context_add_main_entries (option_context,
            option_entries, "Application options");
    g_option_context_add_group (option_context, gtk_get_option_group (TRUE));

    if (!g_option_context_parse (option_context, 
                argc, argv, &g_error))
    {
        put_error(1, "option parsing failed: %s\n\n%s", 
                g_error->message,
                g_option_context_get_help(option_context, TRUE, NULL)); 
    }
    
    if (version) {
        printf ("%s\n",PACKAGE_STRING);
    }

    for (i=1; i < *argc; ++i) {
        gif = read_gif (c, (*argv)[i], &error);
        if (!gifptr_correct(gif,c)) {
            put_warning ("Invalid filename '%s'. %s", 
                    (*argv)[i], GifErrorString(error));
        }
    }
    g_option_context_free (option_context);

    return 0;
}

const char * get_help_string (PContext c, void *user_data)
{
    return description;
}

int
main (int argc, char *argv[])
{
    PContext c;
    int error;
    gtkgif_init_data gtkgif_data;

    gtkgif_data.argc = &argc;
    gtkgif_data.argv = &argv;
    gtkgif_data.runner = interface_runner;
    gtkgif_data.get_help = get_help_string;

    c = create_context(gtkgif_init, &gtkgif_data);
    
    free_context (c);
    return 0;
}
