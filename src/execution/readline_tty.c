#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include "../../includes/mshell.h"

int main()
{
    char *line;

    int fd = open("test.txt", O_RDONLY);
    if (fd == -1)
        return (perror("open"), 1);

	while (line = get_next_line(fd))
    {
        printf("%s", line);
        free(line);
    }
    return 0;
}
