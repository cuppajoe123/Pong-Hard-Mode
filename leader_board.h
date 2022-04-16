#ifndef LEADER_BOARD_H
#define LEADER_BOARD_H

int save_user_data(char *score, char *username);
char **read_user_data(char *path, int *num_lines); /* returns a pointer to an array of strings, where each string contains a username and a score */
char **sort_user_data(char **user_data, int num_lines);

#endif
