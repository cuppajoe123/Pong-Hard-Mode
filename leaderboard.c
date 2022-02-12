#include "graphics.h"
#include "leaderboard.h"

int save_user_data(char *score, char *username)
{
    /* concatenates a space and then the username, then a newline to score. Score will then contain the entire leaderboard entry for that user */
    strncat(score, " ", strlen(" ") + strlen(score));
    strncat(score, username, strlen(username) + strlen(score));
    strncat(score, "\n", strlen("\n") + strlen(score));


    FILE *fp = fopen("resources/data.txt", "a");
    if (fwrite(score, sizeof(char), strlen(score), fp) != strlen(score))
        printf("Error writing to file\n");

    return 0;
}

static int linecount(char *path)
{
    FILE *fp = fopen(path, "r");

    if (fp == NULL) {
        printf("Error: file pointer is null.\n");
        exit(1);
    }

    int count = 0;
    for (char c = getc(fp); c != EOF; c = getc(fp)) {
        if (c == '\n')
            count++;
    }
    fclose(fp);
    return count;
}


char **read_user_data(char *path, int *num_lines)
{
    /* reads each line of path into a char *array, and stores length of path in num_lines to be used later */
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: file pointer is null.\n");
        exit(1);
    }

    *num_lines = linecount(path);
    size_t len = 80;
    char **user_data = malloc(*num_lines * sizeof(char *));
    if (user_data == NULL) {
        printf("Allocation error: malloc\n");
        exit(1);
    }
    for (int i = 0; i < *num_lines; i++) {
        user_data[i] = malloc(80 * sizeof(char));
        if (user_data[i] == NULL) {
            printf("Allocation error: malloc\n");
            exit(1);
        }
    }

    for (int i = 0; i < *num_lines; i++)
        getline(&user_data[i], &len, fp);

    fclose(fp);

    return user_data;
}

static int cmpscore(const void *p1, const void *p2)
{
    int score1, score2;
    sscanf(*(char **)p1, "%d", &score1);
    sscanf(*(char **)p2, "%d", &score2);
    if (score1 == score2)
        return 0;
    if (score1 < score2)
        return -1;
    else
        return 1;
}

char **sort_user_data(char **user_data, int num_lines)
{
    /* uses sscanf to parse out scores and then rearranges strings based on that */
    /* get num_lines from previous read_user_data */
    int length = num_lines;
    qsort(user_data, length, sizeof(char *), cmpscore);

    return user_data;
}
