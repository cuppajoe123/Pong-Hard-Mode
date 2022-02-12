#ifndef LEADERBOARD_H
#define LEADERBOARD_H

int save_user_data(char *score, char *username);
char **read_user_data(char *path); /* returns a pointer to an array of strings, where each string contains a username and a score */

#endif
