/* history.c
 *
 * Copyright 2022 abb128
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <adwaita.h>
#include "history.h"

static FILE *current_session_file = NULL;
static bool is_paused = false;

static char history_dir[1024] = { 0 };

void history_init(void) {
    const char *data_dir = g_get_user_data_dir();
    snprintf(history_dir, sizeof(history_dir), "%s/livecaptions/history", data_dir);

    // Ensure the history directory exists
    g_mkdir_with_parents(history_dir, 0755);
}

static void history_ensure_session_file(void) {
    if (current_session_file != NULL) {
        return; // File is already open
    }

    char filename[1024];
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    strftime(filename, sizeof(filename), "session_%Y%m%d_%H%M%S.txt", tm_info);

    char filepath[2048];
    snprintf(filepath, sizeof(filepath), "%s/%s", history_dir, filename);

    current_session_file = fopen(filepath, "a");
    if (current_session_file) {
        char time_str[64];
        strftime(time_str, sizeof(time_str), "%F %T", tm_info);
        fprintf(current_session_file, "--- Session started at %s ---\n", time_str);
        fflush(current_session_file);
    }
}

void commit_tokens_to_current_history(const AprilToken *tokens, size_t tokens_count) {
    if (is_paused) {
        return;
    }

    history_ensure_session_file();

    if (current_session_file == NULL) {
        return; // Failed to open file
    }

    for (size_t i = 0; i < tokens_count; i++) {
        const char *token = tokens[i].token;
        if (token) {
            fputs(token, current_session_file);
        }
    }
    fflush(current_session_file);
}

void save_silence_to_history(void) {
    if (is_paused || current_session_file == NULL) {
        return;
    }

    fputs("\n\n", current_session_file);
    fflush(current_session_file);
}

bool history_pause_toggle(void) {
    is_paused = !is_paused;
    return is_paused;
}

void history_stop_session(void) {
    if (current_session_file != NULL) {
        fprintf(current_session_file, "\n--- Session stopped ---\n");
        fclose(current_session_file);
        current_session_file = NULL;
    }
}