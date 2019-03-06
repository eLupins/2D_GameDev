#ifndef __SIMPLE_JSON_VALUE_H__
#define __SIMPLE_JSON_VALUE_H__

#include "simple_json_list.h"
#include "simple_json_string.h"

typedef enum
{
    SJVT_NULL,
    SJVT_Object,
    SJVT_Array,
    SJVT_String
}SJValueTypes;

/**
 * @brief this is the abstract container structure for all json data
 * This structure may be an object, array, string, null, boolean value, integer or float
 */
typedef struct SJson_S
{
    SJValueTypes sjtype;    /**<internal tracking of the type.  DO NOT TOUCH*/
    struct
    {
        SJList *array;      /**<an array or values or an array of pairs*/
        SJString *string;   /**<the string if this is a string type*/
    }v;                     /**<union of possible values*/
    SJString *(*get_string)(struct SJson_S *json);  /**<pointer to the function to convert this into json string*/
    void  (*json_free)(struct SJson_S *json);       /**<pointer to the function to free this json*/
    struct SJson_S *(*copy)(struct SJson_S *json);  /**<pointer to the function to copy this json*/
}SJson;

/**
 * @brief allocate a new empty json object
 * @return NULL on error or a new json object
 */
SJson *sj_new();

/*forward declaration, documented in simple_json.h*/
void sj_free(SJson *sjs);

/**
 * @brief make a jason value object out of a string
 * @param string the string to convert
 * @note the original string is put into the SJson object and is no longer owned by you do not free it!
 * @return NULL on error or the newly allocated and set SJson object
 */
SJson *sj_string_to_value(SJString *string);

/**
 * @brief get the contents of the string back formatted and escaped for json
 * @param string the json string to conver
 * @return NULL on error or the converted string
 */
SJString *sj_string_to_json_string(SJson *string);

/**
 * @brief convert the json value into a json string
 * @param json the value to convert
 * @return NULL on error or the json string
 */
SJString *sj_value_to_json_string(SJson *json);

#endif
