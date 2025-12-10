// #2 Синтаксический анализатор (Парсер) / Syntactic analyzer (Parser)
#pragma once

#ifndef SYNTACTIC_ANALYZER_H
#define SYNTACTIC_ANALYZER_H

#define LIMIT_SIZE_SYNTACTIC_ANALYZER 0xFFFF
extern unsigned char syntactic_analyzer_pattern[LIMIT_SIZE_SYNTACTIC_ANALYZER];

typedef struct {
    char construction;
    char rule;
} SyntacticAnalyzer;

#endif // SYNTACTIC_ANALYZER_H