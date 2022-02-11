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
