#ifndef COLOR_H
#define COLOR_H


#define RESET    "\033[0m"
#define RED      "\x1B[31m"
#define GREEN    "\x1B[32m"
#define YELLOW   "\x1B[33m"
#define BLUE     "\x1B[34m"
#define MAGENTA  "\x1B[35m"
#define CYAN     "\x1B[36m"
#define WHITE    "\x1B[37m"
#define DRAW_TEXT(COLOR, TEXT) COLOR TEXT RESET

#endif // COLOR_H