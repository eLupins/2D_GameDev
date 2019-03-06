#ifndef __SIMPLE_JSON_OBJECT_H__
#define __SIMPLE_JSON_OBJECT_H__

#include "simple_json_value.h"

/**
 * @brief free a previously allocated json object
 * @param object the object to free
 */
void sj_object_free(SJson *object);

void sj_object_insert(SJson *object,char *key,SJson *value);

/**
 * @brief get the json object back as a formatted json string
 * @param object the object to convert
 * @return NULL on error, or a string encapsulated ith {}
 */
SJString *sj_object_to_json_string(SJson *object);


#endif
