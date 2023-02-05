#include "main.h"

char	*strjoin(char const *s1, char const *s2)
{
	char	*str;

	if (!s1 || !s2)
		return (NULL);
	if (!(str = (char*)malloc(sizeof(char) *
					(strlen(s1) + strlen(s2) + 1))))
		return (NULL);
	strcpy(str, s1);
	strcat(str, s2);
	return (str);
}
