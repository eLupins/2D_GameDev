#ifndef __SIMPLE_JSON_ERROR_H__
#define __SIMPLE_JSON_ERROR_H__

#ifndef MIN
#define MIN(a,b)          (a<=b?a:b)
#endif

#ifndef MAX
#define MAX(a,b)          (a>=b?a:b)
#endif

#ifndef false
#define false 0
#endif
#ifndef true
#define true 1
#endif
/**
 * @brief get the last reported error for simple json
 * @return a character array with the error message
*/
char *sj_get_error();

/**
 * @brief used internally to se the error message
 * @param error the error message to set
 * @param <vargs> variable arguments for the string
 */
void sj_set_error(char *error,...);

#endif
