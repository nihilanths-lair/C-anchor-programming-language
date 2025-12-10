// #1 Лексический анализатор (Токенизатор) / Lexical analyzer (Tokenizer)
#pragma once

#ifndef LEXICAL_ANALYZER_H
#define LEXICAL_ANALYZER_H

#define LIMIT_SIZE_LEXICAL_ANALYZER 0xFFFF
extern unsigned char lexical_analyzer_pattern[LIMIT_SIZE_LEXICAL_ANALYZER];

typedef struct {
    char token;
    char rule;
} LexicalAnalyzer;

#endif // LEXICAL_ANALYZER_H