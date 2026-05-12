char token__type_name[][64+1] =
{
    "         NUMERIC_LITERAL", // ЧИСЛОВОЙ ЛИТЕРАЛ
    "       CHARACTER_LITERAL", // СИМВОЛЬНЫЙ ЛИТЕРАЛ
    "          STRING_LITERAL", // СТРОКОВЫЙ ЛИТЕРАЛ
    "   LEFT_SIDED_ASSIGNMENT", // ЛЕВОСТОРОННЕЕ ПРИСВАИВАНИЕ
    "              IDENTIFIER", // ИДЕНТИФИКАТОР
    "        LABEL_IDENTIFIER", // ИДЕНТИФИКАТОР МЕТКИ
    "            END_OF_LABEL", // КОНЕЦ МЕТКИ
    "         SPACE_SEPARATOR", // РАЗДЕЛИТЕЛЬ ПРОСТРАНСТВА    ····     

    "                NEW_LINE", // НОВАЯ СТРОКА                ····    \n
    "        END_OF_STATEMENT", // КОНЕЦ ЗАЯВЛЕНИЯ             ····    ;

    "              LEFT_BRACE", // ЛЕВАЯ ФИГУРНАЯ СКОБКА    ···· {
    "             RIGHT_BRACE", // ПРАВАЯ ФИГУРНАЯ СКОБКА   ···· }

    "     LEFT_SQUARE_BRACKET", // ЛЕВАЯ КВАДРАТНАЯ СКОБКА  ···· [
    "    RIGHT_SQUARE_BRACKET", // ПРАВАЯ КВАДРАТНАЯ СКОБКА ···· ]

    "        LEFT_PARENTHESIS", // ЛЕВАЯ КРУГЛАЯ СКОБКА     ···· (
    "       RIGHT_PARENTHESIS", // ПРАВАЯ КРУГЛАЯ СКОБКА    ···· )

    // Ключевые слова
    "          KEYWORD__PRINT", // КЛЮЧЕВОЕ_СЛОВО__ПЕЧАТЬ
    "             KEYWORD__IF", // КЛЮЧЕВОЕ_СЛОВО__ЕСЛИ
    "          KEYWORD__WHILE", // КЛЮЧЕВОЕ_СЛОВО__ПОКА
    "           KEYWORD__GOTO", // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕЙТИ
    "         KEYWORD__SWITCH", // КЛЮЧЕВОЕ_СЛОВО__ПЕРЕКЛЮЧАТЕЛЬ
    "           KEYWORD__CASE", // КЛЮЧЕВОЕ_СЛОВО__КЕЙС
    "        KEYWORD__DEFAULT", // КЛЮЧЕВОЕ_СЛОВО__ПО_УМОЛЧАНИЮ
    "         KEYWORD__RETURN", // КЛЮЧЕВОЕ_СЛОВО__ВЕРНУТЬ
    //
    "           KEYWORD__ELSE", // КЛЮЧЕВОЕ_СЛОВО__ИНАЧЕ (Опционально)
    //"     KEYWORD__ELSE_IF", // КЛЮЧЕВОЕ_СЛОВО__ИНАЧЕ_ЕСЛИ (Опционально)

    // Операторы
    "      OPERATOR__ADDITION", // СЛОЖЕНИЕ             ·     +
    "      OPERATOR__SUBTRACT", // ВЫЧИТАНИЕ            ·     -
    "OPERATOR__MULTIPLICATION", // УМНОЖЕНИЕ            ·     *
    "      OPERATOR__DIVISION", // ДЕЛЕНИЕ              ·     /
    "     OPERATOR__INVERSION", // ИНВЕРСИЯ             ·     !
    "      OPERATOR__EQUALITY", // РАВЕНСТВО            ·     ==
    "    OPERATOR__INEQUALITY", // НЕРАВЕНСТВО          ·     !=
    "         OPERATOR__BELOW", // МЕНЬШЕ               ·     <
    "         OPERATOR__ABOVE", // БОЛЬШЕ               ·     >
    "   OPERATOR__BELOW_EQUAL", // МЕНЬШЕ ИЛИ РАВНО     ·     <=
    "   OPERATOR__ABOVE_EQUAL", // БОЛЬШЕ ИЛИ РАВНО     ·     >=

    "                 UNKNOWN", // НЕИЗВЕСТНЫЙ
    "                   FINAL", // ЗАВЕРШАЮЩИЙ
    "                   ERROR"  // ОШИБКА
};