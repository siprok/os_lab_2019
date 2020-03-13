#include "revert_string.h"
#include <string.h>

void RevertString(char *str)
{
	char buf;
	size_t str_length = strlen(str);
	size_t half_length = str_length >> 1;
	for(int i = 0; i < half_length; i++)
	{
		buf = str[i];
		str[i] = str[str_length - 1 - i];
		str[str_length - 1 - i] = buf;
	}
}

