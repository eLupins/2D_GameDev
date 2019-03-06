#ifndef __SIMPLE_JSON_PARSE_H__
#define __SIMPLE_JSON_PARSE_H__

/**
 * @brief given a string, create a JSON object from it
 * @param string the strong to convert.  NO OP if this is NULL
 * @param length the amount of characters in the string
 * @return NULL on error or a JSson object otherwise
 */
SJson *sj_parse_buffer(char *string,unsigned long length);


#endif
