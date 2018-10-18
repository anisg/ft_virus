#include "ft_lib.h"

int     str_equal(const char *s1, const char *s2)
{
	int i;

	i = 0;
	if (!s1 || !s2)
		return FALSE;
	while (s1[i] && s2[i]
			&& (s1[i] == s2[i]))
		i++;
	return (s1[i] - s2[i]) == 0;
}

int     scmp(const char *s1, const char *s2)
{
    int i;

    i = 0;
    if (!s1 || !s2)
        return (0);
    while ((unsigned char)s1[i] && (unsigned char)s2[i]
            && ((unsigned char)s1[i] == (unsigned char)s2[i]))
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}

int     sncmp(const char *s1, const char *s2, size_t n)
{
    size_t i;

    i = 0;
    if (!s1 || !s2)
        return (0);
    while (i < n && (unsigned char)s1[i] && (unsigned char)s2[i]
            && ((unsigned char)s1[i] == (unsigned char)s2[i]))
        i++;
    return ((unsigned char)s1[i] - (unsigned char)s2[i]);
}


int     startswith(char *s, char *d)
{
    size_t i;

    if (!s || !d)
        return (FALSE);
    i = 0;
    while (s[i] && d[i] && s[i] == d[i])
    {
        i += 1;
    }
    if (!d[i])
        return (TRUE);
    return (FALSE);
}

int     contains(char *s, char *d)
{
    size_t i;
    size_t n;

    i = 0;
    n = slen(d);
    while (s[i])
    {
        if (sncmp(s + i, d, n) == 0)
            return (TRUE);
        i += 1;
    }
    return (FALSE);
}

void  snbr(size_t nb, char *sn)
{
	int   i;
	int   j;
	char  tmp;

	i = 0;
	if (!nb)
		sn[i++] = '0';
	while (nb)
	{
		sn[i++] = (nb % 10) + '0';
		nb /= 10;
	}
	sn[i] = 0;
	i -= 1;
	j = 0;
	while (j <= i)
	{
		tmp = sn[j];
		sn[j] = sn[i];
		sn[i] = tmp;
		j += 1;
		i -= 1;
	}
}