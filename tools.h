#ifndef TOOLS_H
#define TOOLS_H

#include "OthelloAI.h"
#include <vector>
#include <string>
std::vector<OthelloAI*> readAIFile(std::string fileName, int Nsize);
void saveAIFile(const std::vector<OthelloAI*>& AIs, std::string fileName, int Nsize);
#endif // TOOLS_H
