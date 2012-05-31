/*
 *
 *  Embedded Linux library
 *
 *  Copyright (C) 2011-2012  Intel Corporation. All rights reserved.
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License version 2.1 as published by the Free Software Foundation.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

#ifndef __ELL_SETTINGS_H
#define __ELL_SETTINGS_H

#include <stdbool.h>
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct l_settings;

typedef void (*l_settings_debug_cb_t) (const char *str, void *user_data);
typedef void (*l_settings_destroy_cb_t) (void *user_data);

struct l_settings *l_settings_new(void);
void l_settings_free(struct l_settings *settings);

bool l_settings_load_from_data(struct l_settings *settings,
						const char *data, size_t len);
char *l_settings_to_data(struct l_settings *settings, size_t *len);

bool l_settings_load_from_file(struct l_settings *settings,
					const char *filename);

bool l_settings_set_debug(struct l_settings *settings,
				l_settings_debug_cb_t callback,
				void *user_data,
				l_settings_destroy_cb_t destroy);

char **l_settings_get_groups(struct l_settings *settings);
char **l_settings_get_keys(struct l_settings *settings, const char *group_name);

bool l_settings_has_group(struct l_settings *settings, const char *group_name);
bool l_settings_has_key(struct l_settings *settings, const char *group_name,
			const char *key);

const char *l_settings_get_value(struct l_settings *settings,
					const char *group_name,
					const char *key);
bool l_settings_set_value(struct l_settings *settings, const char *group_name,
				const char *key, const char *value);

bool l_settings_get_bool(struct l_settings *settings, const char *group_name,
				const char *key, bool *out);
bool l_settings_set_bool(struct l_settings *settings, const char *group_name,
				const char *key, bool in);

bool l_settings_get_int(struct l_settings *settings, const char *group_name,
				const char *key, int *out);
bool l_settings_set_int(struct l_settings *settings, const char *group_name,
				const char *key, int in);

bool l_settings_get_uint(struct l_settings *settings, const char *group_name,
				const char *key, unsigned int *out);
bool l_settings_set_uint(struct l_settings *settings, const char *group_name,
				const char *key, unsigned int in);

bool l_settings_get_int64(struct l_settings *settings, const char *group_name,
				const char *key, int64_t *out);
bool l_settings_set_int64(struct l_settings *settings, const char *group_name,
				const char *key, int64_t in);

bool l_settings_get_uint64(struct l_settings *settings, const char *group_name,
				const char *key, uint64_t *out);
bool l_settings_set_uint64(struct l_settings *settings, const char *group_name,
				const char *key, uint64_t in);

char *l_settings_get_string(struct l_settings *settings, const char *group_name,
				const char *key);
bool l_settings_set_string(struct l_settings *settings, const char *group_name,
				const char *key, const char *value);

char **l_settings_get_string_list(struct l_settings *settings,
					const char *group_name,
					const char *key, char delimiter);
bool l_settings_set_string_list(struct l_settings *settings,
					const char *group_name,
					const char *key, char **list,
					char delimiter);

bool l_settings_get_double(struct l_settings *settings, const char *group_name,
				const char *key, double *out);
bool l_settings_set_double(struct l_settings *settings, const char *group_name,
				const char *key, double in);

bool l_settings_get_float(struct l_settings *settings, const char *group_name,
				const char *key, float *out);
bool l_settings_set_float(struct l_settings *settings, const char *group_name,
				const char *key, float in);

bool l_settings_remove_key(struct l_settings *settings, const char *group_name,
				const char *key);
bool l_settings_remove_group(struct l_settings *settings,
				const char *group_name);
#ifdef __cplusplus
}
#endif

#endif /* __ELL_SETTINGS_H */
