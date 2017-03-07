/*
 * statusnotifier - Copyright (C) 2014-2017 Olivier Brunel
 *
 * statusnotifier-compat.h
 * Copyright (C) 2017 Olivier Brunel <jjk@jjacky.com>
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

#ifndef __STATUS_NOTIFIER_COMPAT_H__
#define __STATUS_NOTIFIER_COMPAT_H__

G_BEGIN_DECLS

#define StatusNotifier                      StatusNotifierItem
#define StatusNotifierPrivate               StatusNotifierItemPrivate
#define StatusNotifierClass                 StatusNotifierItemClass

#define TYPE_STATUS_NOTIFIER                STATUS_NOTIFIER_TYPE_ITEM
#define STATUS_NOTIFIER                     STATUS_NOTIFIER_ITEM
#define STATUS_NOTIFIER_CLASS               STATUS_NOTIFIER_ITEM_CLASS
#define IS_STATUS_NOTIFIER                  STATUS_NOTIFIER_IS_ITEM
#define IS_STATUS_NOTIFIER_CLASS            STATUS_NOTIFIER_IS_ITEM_CLASS
#define STATUS_NOTIFIER_GET_CLASS           STATUS_NOTIFIER_ITEM_GET_CLASS

#define status_notifier_get_type            status_notifier_item_get_type

#define status_notifier_new_from_pixbuf     status_notifier_item_new_from_pixbuf
#define status_notifier_new_from_icon_name  status_notifier_item_new_from_icon_name
#define status_notifier_get_id              status_notifier_item_get_id
#define status_notifier_get_category        status_notifier_item_get_category
#define status_notifier_set_from_pixbuf     status_notifier_item_set_from_pixbuf
#define status_notifier_set_from_icon_name  status_notifier_item_set_from_icon_name
#define status_notifier_has_pixbuf          status_notifier_item_has_pixbuf
#define status_notifier_get_pixbuf          status_notifier_item_get_pixbuf
#define status_notifier_get_icon_name       status_notifier_item_get_icon_name
#define status_notifier_set_attention_movie_name \
                                            status_notifier_item_set_attention_movie_name
#define status_notifier_get_attention_movie_name \
                                            status_notifier_item_get_attention_movie_name
#define status_notifier_set_title           status_notifier_item_set_title
#define status_notifier_get_title           status_notifier_item_get_title
#define status_notifier_set_status          status_notifier_item_set_status
#define status_notifier_get_status          status_notifier_item_get_status
#define status_notifier_set_window_id       status_notifier_item_set_window_id
#define status_notifier_get_window_id       status_notifier_item_get_window_id
#define status_notifier_freeze_tooltip      status_notifier_item_freeze_tooltip
#define status_notifier_thaw_tooltip        status_notifier_item_thaw_tooltip
#define status_notifier_set_tooltip         status_notifier_item_set_tooltip
#define status_notifier_set_tooltip_title   status_notifier_item_set_tooltip_title
#define status_notifier_get_tooltip_title   status_notifier_item_get_tooltip_title
#define status_notifier_set_tooltip_body    status_notifier_item_set_tooltip_body
#define status_notifier_get_tooltip_body    status_notifier_item_get_tooltip_body
#define status_notifier_register            status_notifier_item_register
#define status_notifier_get_state           status_notifier_item_get_state

G_END_DECLS

#endif /* __STATUS_NOTIFIER_COMPAT_H__ */
