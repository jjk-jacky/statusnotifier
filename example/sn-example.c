/*
 * statusnotifier - Copyright (C) 2014-2017 Olivier Brunel
 *
 * sn-example.c
 * Copyright (C) 2014-2017 Olivier Brunel <jjk@jjacky.com>
 *
 * This file is part of statusnotifier.
 *
 * statusnotifier is free software: you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the Free
 * Software Foundation, either version 3 of the License, or (at your option) any
 * later version.
 *
 * statusnotifier is distributed in the hope that it will be useful, but WITHOUT
 * ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.
 * See the GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along with
 * statusnotifier. If not, see http://www.gnu.org/licenses/
 */

#include "config.h"

#include <glib.h>
#include <gtk/gtk.h>
#include <statusnotifier.h>
#include <string.h>

#define streq(s1, s2)           (((s1) == NULL && (s2) == NULL) ? 1 \
        : ((s1) == NULL || (s2) == NULL) ? 0 : strcmp  ((s1), (s2)) == 0)

#define EXAMPLE_ERROR           g_quark_from_static_string ("Example error")
enum rc
{
    RC_OK = 0,
    RC_CMDLINE
};

struct config
{
    StatusNotifierCategory category;
    StatusNotifierStatus status;
    gchar *title;
    struct {
        gboolean has_pixbuf;
        union {
            gchar *icon_name;
            GdkPixbuf *pixbuf;
        };
    } icon[_NB_STATUS_NOTIFIER_ICONS];
    gchar *tooltip_title;
    gchar *tooltip_body;
    gboolean item_is_menu;
#ifdef USE_DBUSMENU
    gboolean menu;
#endif
};

static void
set_status (GObject *item, StatusNotifierItem *sn)
{
    status_notifier_item_set_status (sn, GPOINTER_TO_UINT (g_object_get_data (item, "sn-status")));
}

static void
set_status_activation_trigger (gpointer item, gpointer sn)
{
    g_signal_connect (item, "activate", (GCallback) set_status, sn);
}

static GtkMenu *
create_menu (StatusNotifierItem *sn, GMainLoop *loop)
{
    GtkMenu *menu;
    GtkMenu *submenu;
    GtkWidget *item;
    guint i;
    StatusNotifierStatus status;
    GSList *group = NULL;

    menu = (GtkMenu *) gtk_menu_new ();
    submenu = (GtkMenu *) gtk_menu_new ();

    g_object_get (sn, "status", &status, NULL);

    i = 0;

    item = gtk_radio_menu_item_new_with_label (group, "Passive");
    group = gtk_radio_menu_item_get_group ((GtkRadioMenuItem *) item);
    if (status == STATUS_NOTIFIER_STATUS_PASSIVE)
        gtk_check_menu_item_set_active ((GtkCheckMenuItem *) item, TRUE);
    g_object_set_data ((GObject *) item,
            "sn-status", GUINT_TO_POINTER (STATUS_NOTIFIER_STATUS_PASSIVE));
    gtk_widget_show (item);
    gtk_menu_attach (submenu, item, 0, 1, i, i + 1);
    ++i;
    item = gtk_radio_menu_item_new_with_label (group, "Active");
    group = gtk_radio_menu_item_get_group ((GtkRadioMenuItem *) item);
    if (status == STATUS_NOTIFIER_STATUS_ACTIVE)
        gtk_check_menu_item_set_active ((GtkCheckMenuItem *) item, TRUE);
    g_object_set_data ((GObject *) item,
            "sn-status", GUINT_TO_POINTER (STATUS_NOTIFIER_STATUS_ACTIVE));
    gtk_widget_show (item);
    gtk_menu_attach (submenu, item, 0, 1, i, i + 1);
    ++i;
    item = gtk_radio_menu_item_new_with_label (group, "Needs attention");
    group = gtk_radio_menu_item_get_group ((GtkRadioMenuItem *) item);
    if (status == STATUS_NOTIFIER_STATUS_NEEDS_ATTENTION)
        gtk_check_menu_item_set_active ((GtkCheckMenuItem *) item, TRUE);
    g_object_set_data ((GObject *) item,
            "sn-status", GUINT_TO_POINTER (STATUS_NOTIFIER_STATUS_NEEDS_ATTENTION));
    gtk_widget_show (item);
    gtk_menu_attach (submenu, item, 0, 1, i, i + 1);
    ++i;

    g_slist_foreach (group, &set_status_activation_trigger, sn);

    i = 0;
    item = gtk_menu_item_new_with_label ("Status");
    gtk_menu_item_set_submenu ((GtkMenuItem *) item, (GtkWidget *) submenu);
    gtk_widget_show (item);
    gtk_menu_attach (menu, item, 0, 1, i, i + 1);
    ++i;
    item = gtk_menu_item_new_with_label ("Exit");
    g_signal_connect_swapped (item, "activate", (GCallback) g_main_loop_quit, loop);
    gtk_widget_show (item);
    gtk_menu_attach (menu, item, 0, 1, i, i + 1);
    ++i;

    return menu;
}

static gboolean
sn_menu (StatusNotifierItem *sn, gint x, gint y, GMainLoop *loop)
{
    static GtkMenu *menu = NULL;
    if (!menu)
    {
        menu = create_menu (sn, loop);
        g_object_ref_sink (menu);
    }

    gtk_menu_popup (menu, NULL, NULL, NULL, NULL, 0, gtk_get_current_event_time ());
    return TRUE;
}

static gboolean
sn_activate (GMainLoop *loop)
{
    g_main_loop_quit (loop);
    return TRUE;
}

static void
sn_reg_failed (StatusNotifierItem *sn, GError *error, GMainLoop *loop)
{
    fprintf (stderr, "Failed to create status notifier: %s\n", error->message);
    g_main_loop_quit (loop);
}

static gboolean
cmdline_category (const gchar   *option,
                  const gchar   *value,
                  struct config *cfg,
                  GError       **error)
{
    if (streq (value, "app"))
        cfg->category = STATUS_NOTIFIER_CATEGORY_APPLICATION_STATUS;
    else if (streq (value, "comm"))
        cfg->category = STATUS_NOTIFIER_CATEGORY_COMMUNICATIONS;
    else if (streq (value, "system"))
        cfg->category = STATUS_NOTIFIER_CATEGORY_SYSTEM_SERVICES;
    else if (streq (value, "hardware"))
        cfg->category = STATUS_NOTIFIER_CATEGORY_HARDWARE;
    else
    {
        g_set_error (error, EXAMPLE_ERROR, RC_CMDLINE,
                "Invalid category: '%s'; Must be 'app', 'comm', 'system' or 'hardware'",
                value);
        return FALSE;
    }

    return TRUE;
}

static gboolean
cmdline_status (const gchar     *option,
                const gchar     *value,
                struct config   *cfg,
                GError         **error)
{
    if (streq (value, "active"))
        cfg->status = STATUS_NOTIFIER_STATUS_ACTIVE;
    else if (streq (value, "passive"))
        cfg->status = STATUS_NOTIFIER_STATUS_PASSIVE;
    else if (streq (value, "attention"))
        cfg->status = STATUS_NOTIFIER_STATUS_NEEDS_ATTENTION;
    else
    {
        g_set_error (error, EXAMPLE_ERROR, RC_CMDLINE,
                "Invalid status: '%s'; Must be 'passive', 'active' or 'attention'",
                value);
        return FALSE;
    }

    return TRUE;
}

static void
free_icon (struct config *cfg, StatusNotifierIcon icon)
{
    if (cfg->icon[icon].has_pixbuf)
    {
        if (cfg->icon[icon].pixbuf)
            g_object_unref (cfg->icon[icon].pixbuf);
    }
    else
        g_free (cfg->icon[icon].icon_name);
    cfg->icon[icon].has_pixbuf = FALSE;
    cfg->icon[icon].icon_name = NULL;
}

static gboolean
cmdline_icon_name (const gchar      *option,
                   const gchar      *value,
                   struct config    *cfg,
                   GError          **error)
{
    StatusNotifierIcon icon;

    if (streq (option, "-i") || streq (option, "--icon"))
        icon = STATUS_NOTIFIER_ICON;
    else if (streq (option, "-a") || streq (option, "--attention-icon"))
        icon = STATUS_NOTIFIER_ATTENTION_ICON;
    else if (streq (option, "-o") || streq (option, "--overlay-icon"))
        icon = STATUS_NOTIFIER_OVERLAY_ICON;
    else /* if (streq (option, "-l") || streq (option, "--tooltip-icon")) */
        icon = STATUS_NOTIFIER_TOOLTIP_ICON;

    free_icon (cfg, icon);
    cfg->icon[icon].icon_name = g_strdup (value);
    return TRUE;
}

static gboolean
cmdline_icon_pixbuf (const gchar    *option,
                     const gchar    *value,
                     struct config  *cfg,
                     GError        **error)
{
    StatusNotifierIcon icon;
    GdkPixbuf *pixbuf;

    if (streq (option, "-I") || streq (option, "--pixbuf"))
        icon = STATUS_NOTIFIER_ICON;
    else if (streq (option, "-A") || streq (option, "--attention-pixbuf"))
        icon = STATUS_NOTIFIER_ATTENTION_ICON;
    else if (streq (option, "-O") || streq (option, "--overlay-pixbuf"))
        icon = STATUS_NOTIFIER_OVERLAY_ICON;
    else /* if (streq (option, "-L") || streq (option, "--tooltip-pixbuf")) */
        icon = STATUS_NOTIFIER_TOOLTIP_ICON;

    pixbuf = gdk_pixbuf_new_from_file (value, error);
    if (!pixbuf)
    {
        g_prefix_error (error, "Failed to load pixbuf from '%s': ", value);
        return FALSE;
    }

    free_icon (cfg, icon);
    cfg->icon[icon].pixbuf = pixbuf;
    cfg->icon[icon].has_pixbuf = TRUE;
    return TRUE;
}

static gboolean
parse_cmdline (struct config *cfg, gint *argc, gchar **argv[], GError **error)
{
    GOptionContext *context;
    GOptionEntry entries[] =
    {
        { "category",           'c',    0, G_OPTION_ARG_CALLBACK,   cmdline_category,
            "Set the item's category;\n\t\t"
                "CATEGORY must be app, comm, system or hardware", "CATEGORY" },
        { "title",              't',    0, G_OPTION_ARG_STRING,     &cfg->title,
            "Set TITLE as the item's title", "TITLE" },
        { "status",             's',    0, G_OPTION_ARG_CALLBACK,   cmdline_status,
            "Set the item's status;\n\t\t"
                "STATUS must be active, passive or attention", "STATUS" },
        { "icon",               'i',    0, G_OPTION_ARG_CALLBACK,   cmdline_icon_name,
            "Use NAME as icon name for (main) icon", "NAME" },
        { "pixbuf",             'I',    0, G_OPTION_ARG_CALLBACK,   cmdline_icon_pixbuf,
            "Load FILE as icon for (main) icon", "FILE" },
        { "attention-icon",     'a',    0, G_OPTION_ARG_CALLBACK,   cmdline_icon_name,
            "Use NAME as icon name for attention icon", "NAME" },
        { "attention-pixbuf",   'A',    0, G_OPTION_ARG_CALLBACK,   cmdline_icon_pixbuf,
            "Load FILE as icon for attention icon", "FILE" },
        { "overlay-icon",       'o',    0, G_OPTION_ARG_CALLBACK,   cmdline_icon_name,
            "Use NAME as icon name for overlay icon", "NAME" },
        { "overlay-pixbuf",     'O',    0, G_OPTION_ARG_CALLBACK,   cmdline_icon_pixbuf,
            "Load FILE as icon for overlay icon", "FILE" },
        { "tooltip-icon",       'l',    0, G_OPTION_ARG_CALLBACK,   cmdline_icon_name,
            "Use NAME as icon name for tooltip icon", "NAME" },
        { "tooltip-pixbuf",     'L',    0, G_OPTION_ARG_CALLBACK,   cmdline_icon_pixbuf,
            "Load FILE as icon for tooltip icon", "FILE" },
        { "tooltip",            'T',    0, G_OPTION_ARG_STRING,     &cfg->tooltip_title,
            "Set TITLE as title of the item's tooltip", "TITLE" },
        { "tooltip-body",       'b',    0, G_OPTION_ARG_STRING,     &cfg->tooltip_body,
            "Set TEXT as body of the item's tooltip", "TEXT" },
        { "item-is-menu",       'M',    0, G_OPTION_ARG_NONE,       &cfg->item_is_menu,
            "Whether the item only supports context menu or not", NULL},
#ifdef USE_DBUSMENU
        { "dbus-menu",          'm',    0, G_OPTION_ARG_NONE,       &cfg->menu,
            "Whether menu should be exposed via dbusmenu or not", NULL },
#endif
        { NULL }
    };
    GOptionGroup *group;

    context = g_option_context_new ("");
    group = g_option_group_new ("example", "example", "app options", cfg, NULL);
    g_option_group_add_entries (group, entries);
    g_option_context_set_main_group (context, group);
    if (!g_option_context_parse (context, argc, argv, error))
        return FALSE;

    return TRUE;
}

gint
main (gint argc, gchar *argv[])
{
    GError *err = NULL;
    GMainLoop *loop;
    StatusNotifierItem *sn;
    struct config cfg = { 0, };
    const gchar *prop_name_from_icon[_NB_STATUS_NOTIFIER_ICONS] = {
        "main-icon-name",
        "attention-icon-name",
        "overlay-icon-name",
        "tooltip-icon-name"
    };
    const gchar *prop_pixbuf_from_icon[_NB_STATUS_NOTIFIER_ICONS] = {
        "main-icon-pixbuf",
        "attention-icon-pixbuf",
        "overlay-icon-pixbuf",
        "tooltip-icon-pixbuf"
    };
    guint i;

    gtk_init (&argc, &argv);
    if (!parse_cmdline (&cfg, &argc, &argv, &err))
    {
        fputs (err->message, stderr);
        fputc ('\n', stderr);
        g_clear_error (&err);
        return RC_CMDLINE;
    }

    loop = g_main_loop_new (NULL, TRUE);
    sn = g_object_new (STATUS_NOTIFIER_TYPE_ITEM,
            "id",               "sn-example",
            "category",         cfg.category,
            "status",           cfg.status,
            "title",            (cfg.title) ? cfg.title : "Example",
            "item-is-menu",     cfg.item_is_menu,
            NULL);
    for (i = 0; i < _NB_STATUS_NOTIFIER_ICONS; ++i)
    {
        if (cfg.icon[i].has_pixbuf)
            g_object_set (sn, prop_pixbuf_from_icon[i], cfg.icon[i].pixbuf, NULL);
        else if (cfg.icon[i].icon_name)
            g_object_set (sn, prop_name_from_icon[i], cfg.icon[i].icon_name, NULL);
    }
    if (cfg.tooltip_title)
        status_notifier_item_set_tooltip_title (sn, cfg.tooltip_title);
    if (cfg.tooltip_body)
        status_notifier_item_set_tooltip_body (sn, cfg.tooltip_body);

#ifdef USE_DBUSMENU
    if (cfg.menu)
        status_notifier_item_set_context_menu (sn, (GtkWidget *) create_menu(sn, loop));
    else
#endif
        g_signal_connect (sn, "context-menu", (GCallback) sn_menu, loop);

    g_signal_connect (sn, "registration-failed", (GCallback) sn_reg_failed, loop);
    g_signal_connect_swapped (sn, "activate", (GCallback) sn_activate, loop);
    status_notifier_item_register (sn);
    g_main_loop_run (loop);

    g_object_unref (sn);
    return RC_OK;
}
