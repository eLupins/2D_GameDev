#ifndef __SIMPLE_JSON_ARRAY_H__
#define __SIMPLE_JSON_ARRAY_H__

#include "simple_json_value.h"
#include "simple_json_list.h"
#include "simple_json_string.h"

SJson *sj_array_new();

void sj_array_free(SJson *array);

void sj_array_append(SJson *array,SJson *value);

SJson *sj_array_get_nth(SJson *array,int n);

/**
 * @brief retrieve the nth element in the json array assumed to be a string
 * @param array the array to search through
 * @param n the index of the element to get
 * @return NULL on error (check sj_get_error()) or the SJString value otherwise
 */
SJString *sj_array_get_nth_as_string(SJson *array,int n);

/**
 * @brief get the contents of the array as a formatted json string for output
 * @param array the json array to convert
 * @return a formatted json character array "[*,*,....]"
 */
SJString *sj_array_to_json_string(SJson *array);

#endif
