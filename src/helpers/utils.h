#ifndef UTILS_H
#define UTILS_H
#include <string>
#include "SDL.h"
#define ENG_UNUSED(x) (void)x;


#define ONE_SECOND 1000
#define ONE_MINUTE 60*ONE_SECOND

void drawCircle(SDL_Renderer* renderer, SDL_Point center, int radius, SDL_Color color);
std::string strLeftPad(const std::string &str, const size_t num, const char paddingChar= ' ');
std::string strRightPad(const std::string &str, const size_t num, const char paddingChar= ' ');
#endif // UTILS_H
