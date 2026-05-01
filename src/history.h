/* history.h
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

#ifndef LCAP_HISTORY_H
#define LCAP_HISTORY_H

#include <stddef.h>
#include <stdbool.h>
#include <april_api.h>

// Initialize history tracking
void history_init(void);

// Every time finalized, commit to list of history_entry
void commit_tokens_to_current_history(const AprilToken *tokens,
                                      size_t tokens_count);

// Puts an empty entry into history meaning silence
void save_silence_to_history(void);

// Pause or Resume writing to history
bool history_pause_toggle(void);

// Finalize and stop current session log
void history_stop_session(void);

#endif