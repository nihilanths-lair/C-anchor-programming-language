enum
{
    // Идентификация токенов для лексера и парсера (лексического/синтаксического анализа и синтеза)
    TOKEN__NUMERIC_LITERAL,       // ЧИСЛОВОЙ ЛИТЕРАЛ
    TOKEN__CHARACTER_LITERAL,     // СИМВОЛЬНЫЙ ЛИТЕРАЛ
    TOKEN__STRING_LITERAL,        // СТРОКОВЫЙ ЛИТЕРАЛ
    TOKEN__LEFT_SIDED_ASSIGNMENT, // ЛЕВОСТОРОННЕЕ ПРИСВАИВАНИЕ
    TOKEN__IDENTIFIER,            // ИДЕНТИФИКАТОР
    TOKEN__LABEL_IDENTIFIER,      // ИДЕНТИФИКАТОР МЕТКИ
    TOKEN__END_OF_LABEL,          // КОНЕЦ МЕТКИ
    TOKEN__SPACE_SEPARATOR,       // РАЗДЕЛИТЕЛЬ ПРОСТРАНСТВА
    TOKEN__END_OF_STATEMENT,      // КОНЕЦ ЗАЯВЛЕНИЯ

    TOKEN__LEFT_BRACE,            // ЛЕВАЯ ФИГУРНАЯ СКОБКА    ···· {
    TOKEN__RIGHT_BRACE,           // ПРАВАЯ ФИГУРНАЯ СКОБКА   ···· }

    TOKEN__LEFT_SQUARE_BRACKET,   // ЛЕВАЯ КВАДРАТНАЯ СКОБКА  ···· [
    TOKEN__RIGHT_SQUARE_BRACKET,  // ПРАВАЯ КВАДРАТНАЯ СКОБКА ···· ]

    TOKEN__LEFT_PARENTHESIS,      // ЛЕВАЯ КРУГЛАЯ СКОБКА     ···· (
    TOKEN__RIGHT_PARENTHESIS,     // ПРАВАЯ КРУГЛАЯ СКОБКА    ···· )

    // Ключевые слова
    TOKEN__KEYWORD_PRINT,   // КЛЮЧЕВОЕ_СЛОВО__ПЕЧАТЬ
    TOKEN__KEYWORD_IF,      // КЛЮЧЕВОЕ_СЛОВО__ЕСЛИ
    TOKEN__KEYWORD_WHILE,   // КЛЮЧЕВОЕ_СЛОВО__ПОКА
    TOKEN__KEYWORD_GOTO,    // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕЙТИ
    TOKEN__KEYWORD_SWITCH,  // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕКЛЮЧАТЕЛЬ
    TOKEN__KEYWORD_CASE,    // КЛЮЧЕВОЕ_СЛОВО__КЕЙС
    TOKEN__KEYWORD_DEFAULT, // КЛЮЧЕВОЕ_СЛОВО__ПО_УМОЛЧАНИЮ
    TOKEN__KEYWORD_RETURN,  // КЛЮЧЕВОЕ_СЛОВО__ВЕРНУТЬ
    //
    TOKEN__KEYWORD_ELSE,    // КЛЮЧЕВОЕ_СЛОВО__ИНАЧЕ (Опционально)
    //TOKEN__KEYWORD_ELSE_IF, // КЛЮЧЕВОЕ_СЛОВО__ИНАЧЕ_ЕСЛИ (Опционально)

    // Операторы
    TOKEN__ADDITION_OPERATOR,       // СЛОЖЕНИЕ             ·     +
    TOKEN__SUBTRACT_OPERATOR,       // ВЫЧИТАНИЕ            ·     -
    TOKEN__MULTIPLICATION_OPERATOR, // УМНОЖЕНИЕ            ·     *
    TOKEN__DIVISION_OPERATOR,       // ДЕЛЕНИЕ              ·     /
    TOKEN__INVERSION_OPERATOR,      // ИНВЕРСИЯ             ·     !
    TOKEN__EQUALITY_OPERATOR,       // РАВЕНСТВО            ·     ==
    TOKEN__INEQUALITY_OPERATOR,     // НЕРАВЕНСТВО          ·     !=
    TOKEN__BELOW_OPERATOR,          // МЕНЬШЕ               ·     <
    TOKEN__ABOVE_OPERATOR,          // БОЛЬШЕ               ·     >
    TOKEN__BELOW_EQUAL_OPERATOR,    // МЕНЬШЕ ИЛИ РАВНО     ·     <=
    TOKEN__ABOVE_EQUAL_OPERATOR,    // БОЛЬШЕ ИЛИ РАВНО     ·     >=

    TOKEN__UNKNOWN, // НЕИЗВЕСТНЫЙ
    TOKEN__EOF,     // КОНЕЦ
    TOKEN__ERROR    // ОШИБКА
};