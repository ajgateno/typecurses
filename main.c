#include <ctype.h>
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TEXT_SIZE 5000
#define TEXT_PATH "text.txt"

typedef struct {

  int is_done;
  char *text;
  int progress;
  int size;
  int num_words;
  int start_time_seconds;

} game_t;

void get_text(char *text);
void start_game(game_t *game, char *text);
void game_update(game_t *game);
void game_render(game_t game);
void game_over_render(game_t game);

int main(void)
{
  char text[MAX_TEXT_SIZE];
  get_text(text);

  game_t game;
  start_game(&game, text);

  game_render(game);

  while (!game.is_done) {
    game_update(&game);
    game_render(game);
  }

  game_over_render(game);

  return 0;
}

void get_text(char *text)
{
  FILE *file = fopen(TEXT_PATH, "r");

  if (file == NULL) {
    fprintf(stderr, "Error opening file\n");
    exit(1);
  }

  int i = 0;
  char ch;
  while ((ch = fgetc(file)) != EOF) {
    if (ch != '\n') {
      text[i] = ch;
      i++;
    }
  }

  text[i] = '\0';
}

void start_game(game_t *game, char *text)
{
  initscr();

  game->is_done = 0;
  game->text = text;
  game->progress = 0;
  game->size = strlen(text);
  game->start_time_seconds = time(NULL);

  int num_words = 0;
  num_words += !isspace((unsigned char) game->text[0]);
  for (int i = 0; i < game->size - 1; i++) {
    if (isspace(game->text[i]) && !isspace(game->text[i + 1])) num_words++;
  }

  game->num_words = num_words;
}

void game_update(game_t *game)
{
  char ch = getch();
  if (ch == game->text[game->progress]) game->progress++;
  game->is_done = (game->progress == game->size);
}

void game_render(game_t game)
{
  clear();

  attron(A_BOLD | A_UNDERLINE);
  for (int i = 0; i < game.progress; i++) {
    printw("%c", game.text[i]);
  }

  attroff(A_BOLD | A_UNDERLINE);
  for (int i = game.progress; i < game.size; i++) {
    printw("%c", game.text[i]);
  }

  printw("\n");

  refresh();
}

void game_over_render(game_t game)
{
  int end_time_seconds = time(NULL);
  float time_elapsed_seconds = end_time_seconds - game.start_time_seconds;
  float words_per_minute = 60.0 * game.num_words / time_elapsed_seconds;

  printw("\n");
  printw("Done! Your speed: %.0f WPM", words_per_minute);
  refresh();
  getch();
  endwin();
}

