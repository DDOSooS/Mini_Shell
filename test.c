#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int ft_count_number_len(int number)
{
	int counter;

	counter = 0;
	while (number > 9)
	{	
		number = number / 10;
		counter++;
	}
	counter++;
	return (counter);
}

char *ft_strcat(char *s1, char *s2)
{
	int i;
	int j;

	i = -1;
	j = 0;
	while (s1[++i]);
	while (s2[j])
	{
		s1[i + j] = s2[j];
		j++;
	}
	s1[i + j] = '\0';
	return (s1);
}
int main()
{
	char *s1 = malloc(sizeof(char) * 10);
	// s1[0] ='\0';
	char *s2 = "ddss";
	s1 = ft_strcat(s1, s2);
    printf("===>>> (%s)len(%ld) <<<===\n", s1, strlen(s1));
}