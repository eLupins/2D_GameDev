#ifndef __SIMPLE_JSON_STRING_H__
#define __SIMPLE_JSON_STRING_H__

/**
 * @brief a basic structure that keeps track of a string and its length
 * Automatically grows to accomodate longer strings
 */
typedef struct SJString_S
{
    char *text; /**<pointer to the character data*/
    int size;   /**<how much data base been allocated*/
    int length;   /**<how much data base been allocated*/
}SJString;

/**
 * @brief make a new empty string
 * @return NULL on error or a new string that must be freed with sj_string_free
 */
SJString *sj_string_new();

/**
 * @brief make a new string with the characters from array s
 * @param s the character array to populate the string with
 * @return NULL on error or a new string that must be freed with sj_string_free
 */
SJString *sj_string_new_text(char *s);

SJString *sj_string_new_integer(int i);

SJString *sj_string_new_float(float i);

SJString *sj_string_new_bool(int i);

/**
 * @brief free an SJString
 * @param string the string to free
 */
void sj_string_free(SJString *string);

/**
 * @brief performs a strcmp on the string with the provided character array
 * @param string the SJString to check
 * @param s the character array to compare with
 * @return <0 id string has a lower value than s, >0 if s has a lower value than string, 0 if they are equivalant
 */
int sj_string_cmp(SJString *string,char *s);

/**
 * @brief set the value of string to s
 * @param string the string to change
 * @param s the character array to change it to
 */
void sj_string_set(SJString *string,char *s);

/**
 * @brief set the value of string to s but only length of s
 * @param string the string to change
 * @param s the character array to change it to
 * @param length the limit on characters to copy from s
 */
void sj_string_set_limit(SJString *string,char *s, long length);

/**
 * @brief append the contents of string2 to string1
 * @param string1 this string will be modified by this operation
 * @param string2 this string will NOT be modified by this operation
 */
void sj_string_concat(SJString *string1,SJString *string2);

/**
 * @brief append the contents of buffer to the string
 * @param string the string to be added to
 * @param buffer a NULL terminated character buffer.  This will not be modified
 */
void sj_string_append(SJString *string,char *buffer);

/**
 * @brief get the text back from the string
 * @param string the string to get
 * @return NULL on error or the character array containing the string text
 */
char *sj_string_get_text(SJString *string);

/**
 * @brief get the integer value if the string is a numer
 * @param string the string to use as an integer
 * @param output [optional] where to store the integer value.  This is untouched if the string was not an integer
 * @return 0 if the string was not an integer or 1 if it was
 */
int sj_string_as_integer(SJString *string,int *output);

/**
 * @brief get the float value if the string is a numer
 * @param string the string to use as a float
 * @param output [optional] where to store the float value.  This is untouched if the string was not a float
 * @return 0 if the string was not an float or 1 if it was
 */
int sj_string_as_float(SJString *string,float *output);

/**
 * @brief get the bool value if the string is a bool
 * @param string the string to use as a bool
 * @param output [optional] where to store the boolean value.  This is untouched if the string was not a bool
 * @return 0 if the string was not an bool or 1 if it was
 */
int sj_string_as_bool(SJString *string,short int *output);


#endif
