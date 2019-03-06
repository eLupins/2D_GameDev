#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "simple_json.h"
#include "simple_json_object.h"
#include "simple_json_array.h"
#include "simple_json_string.h"
#include "simple_json_error.h"

extern int __SJ_DEBUG;

/**
 * @brief structure keeps track of the buffer being parsed and the position last used
 */
typedef struct
{
    char *buffer;   /**<the buffer being parsed*/
    char *position; /**<the current position being parsed*/
    char *end;      /**<the last position to prevent over seek*/
}jsParse;

SJson *sj_parse_object(jsParse *parse);
SJson *sj_parse_array(jsParse *parse);
SJString *sj_parse_string(jsParse *parse);

int overseek_check_fail(jsParse *parse)
{
    if (!parse)return 1;
    if (parse->position >= parse->end)
    {
        sj_set_error("sj_parse: we have parsed too far");
        if (__SJ_DEBUG) printf("we have parsed too far");
        return 1;
    }
    return 0;
}

char *get_next_unescaped_char(char *buffer, char target)
{
    if (!buffer)return NULL;
    if (buffer[0] == target)return buffer;
    while ((buffer[0] != '\0')&&(buffer[1] != '\0'))
    {
        if ((buffer[0] != '\\')&&(buffer[1] == target))
        {
            return ++buffer;
        }
        buffer++;
    }
    return NULL;
}

char *get_next_relevant_char(char *buffer)
{
    if (!buffer)return NULL;
    while (buffer[0] != '\0')
    {
        switch (buffer[0])
        {
            case '\n':
            case '\r':
            case '\t':
            case ' ':
//                printf("chomping white space (%c:%i)\n",*buffer,*buffer);
                buffer++;
                break;
            default:
  //              printf("character (%c:%i) checks out\n",*buffer,*buffer);
                return buffer;
        }
    }
    return NULL;
}


SJString *sj_parse_string(jsParse *parse)
{
    char *p,*p1,*p2,*p3;
    size_t str_length;
    SJString *string;
    int quoted = 0;
    if (!parse)return NULL;
    // validate we are an object
    if (overseek_check_fail(parse))return NULL;
    
    string = sj_string_new();
    if (!string)
    {
        return NULL;
    }
    if (*parse->position == '"')
    {
        //chomp quote character
        quoted = 1;
        parse->position++;
    }
    if (quoted)
    {
        p = get_next_unescaped_char(parse->position, '"');
    }
    else
    {
        p = get_next_unescaped_char(parse->position, ',');
        p1 = get_next_unescaped_char(parse->position, ']');
        p2 = get_next_unescaped_char(parse->position, '}');
        p3 = get_next_unescaped_char(parse->position, ':');
        if (p != NULL)
        {
            if (p2 != NULL)p = MIN(p,p2);
        }
        else p = p2;
        if (p1 != NULL)
        {
            if (p3 != NULL)p1 = MIN(p1,p3);
        }
        else p1 = p3;
        if (p != NULL)
        {
            if (p1 != NULL)p = MIN(p,p1);
        }
        else p = p1;
    }
    if (p == NULL)
    {
        sj_set_error("sj_parse_string: no end delimeter");
        sj_string_free(string);
        return NULL;
    }
    str_length = p - parse->position;
    if (str_length <= 0)
    {
        sj_set_error("sj_parse_string: string is a zero or negative length\nerror parsing string at: %s",parse->position);
        sj_string_free(string);
        return NULL;
    }
    sj_string_set_limit(string,parse->position,str_length);
    parse->position = p + quoted;
    return string;
}

SJson *sj_parse_value(jsParse *parse)
{
    SJString *string;
    if (overseek_check_fail(parse))return NULL;
    parse->position = get_next_relevant_char(parse->position);
    if (strncmp(parse->position,"null",4)==0)
    {
        switch (parse->position[4])
        {
            case ',':// NULL value in an array
            case ']':// NULL value as the last value for an array
            case '}':// NULL value as the last value for an object
                parse->position+=4;
                return sj_null_new();
        }
    }
    switch (*parse->position)
    {
        case '{':
            return sj_parse_object(parse);
        case '[':
            return sj_parse_array(parse);
    }
    string = sj_parse_string(parse);
    if (string == NULL)return NULL;
    return sj_string_to_value(string);
}

SJson *sj_parse_array(jsParse *parse)
{
    SJson *json = NULL;
    SJson *value = NULL;
    if (!parse)return NULL;
    // validate we are an object
    if (*parse->position != '[')
    {
        sj_set_error("sj_parse_array: expected first character to be a [\n");
        return NULL;
    }

    // allocate working space
    json = sj_array_new();
    if (!json)return NULL;

    //chomp first character
    parse->position++;
    do
    {
        value = sj_parse_value(parse);
        if (value == NULL)
        {
            sj_set_error("sj_parse_array : --=== array value failed to parse! ===--\n");
            sj_array_free(json);
            return NULL;
        }
        
        sj_array_append(json,value);
        
        parse->position = get_next_relevant_char(parse->position);
        if (*parse->position == ',')
        {
            parse->position++;
        }
    }
    while((*parse->position != ']') && (parse->position < parse->end));
    parse->position++;
    return json;
}

SJson *sj_parse_object(jsParse *parse)
{
    SJson *json = NULL;
    SJson *value = NULL;
    SJString *key;
    if (!parse)return NULL;
    // validate we are an object
    
    if (*parse->position != '{')
    {
        sj_set_error("sj_parse_object: expected first character to be a {\n");
        return NULL;
    }

    // allocate working space
    json = sj_object_new();
    if (!json)return NULL;

    //chomp first character
    parse->position++;
    do
    {
        parse->position = get_next_relevant_char(parse->position);
        key = sj_parse_string(parse);
        if (!key)
        {
            sj_set_error("sj_parse_object: failed to parse key\n");
            sj_object_free(json);
            sj_string_free(key);
            return NULL;
        }
        parse->position = get_next_relevant_char(parse->position);
        if (*parse->position != ':')
        {
            sj_set_error("sj_parse_object: no colon (:) delimeter for object\n");
            sj_object_free(json);
            sj_string_free(key);
            return NULL;
        }
        parse->position++;
        value = sj_parse_value(parse);

        if (value == NULL)
        {
            sj_set_error("sj_parse_object: --=== object value failed to parse! ===--");
            sj_string_free(key);
            sj_object_free(json);
            return NULL;
        }
        sj_object_insert(json,key->text,value);
        sj_string_free(key);
        parse->position = get_next_relevant_char(parse->position);
        
        if (*parse->position == ',')
        {
            parse->position++;
        }
    }
    while((*parse->position != '}') && (parse->position < parse->end));
    parse->position++;
    return json;
}

SJson *sj_parse_buffer(char *string,unsigned long length)
{
    SJson *json = NULL;
    static jsParse parse;
    if (!string)
    {
        sj_set_error("sj_parse_buffer: no string provided");
        return NULL;
    }
    parse.buffer = string;
    parse.position = strchr(string, '{');
    parse.end = &string[length -1];
    json = sj_parse_object(&parse);
    return json;
}
