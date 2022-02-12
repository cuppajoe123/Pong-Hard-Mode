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

int linecount(FILE *fp)
{
    int count = 0;
    for (char c = getc(fp); c != EOF; c = getc(fp)) {
        if (c == '\n')
            count++;
    }
    return count;
}


char **read_user_data(char *path)
{
    FILE *fp = fopen(path, "r");
    if (fp == NULL) {
        printf("Error: file pointer is null.\n");
        exit(1);
    }

    int num_lines = linecount(fp);
    size_t len = 80;
    char **users = malloc(num_lines * sizeof(char *));
    if (users == NULL) {
        printf("Allocation error: malloc\n");
        exit(1);
    }
    for (int i = 0; i < num_lines; i++) {
        users[i] = malloc(80 * sizeof(char));
        if (users[i] == NULL) {
            printf("Allocation error: malloc\n");
            exit(1);
        }
    }
    fseek(fp, 0, SEEK_SET);

    for (int i = 0; i < num_lines; i++)
        getline(&users[i], &len, fp);

    return users;
}
