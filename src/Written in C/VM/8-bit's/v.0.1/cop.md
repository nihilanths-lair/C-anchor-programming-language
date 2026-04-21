```
Виртуальная машина (Гибридная: Регистровая + Память-в-память), в духе x86

OPCODE (HEX) | OPCODE (DEC) | BASE SYNTAX ASSEMBLER | SEMANTICS | DESCRIPTION
```
```
                                                          Безусловный переход в ..-

01           | 001          | JMP >>           |        | .- предыдущую ячейку памяти
02           | 002          | JMP <<           |        | .- следующую ячейку памяти
03 XX        | 003 XXX      | JMP r8           | src:r8 | .- произвольную ячейку памяти через регистровую адресацию
04 XX        | 004 XXX      | JMP X            | src:i8 | .- произвольную ячейку памяти через непосредственную адресацию
05 XX        | 005 XXX      | JMP [X]          | src:m8 | .- произвольную ячейку памяти через прямую адресацию
06 XX        | 006 XXX      | JMP [[X]]        | src:p8 | .- произвольную ячейку памяти через косвенную адресацию

                                                          Инкремент ..-

07           | 007          | INC r8           | dst:r8 | .- регистра r8
08           | 008          | INC []           | dst:m8 | .- текущей ячейки памяти через прямую адресацию
09           | 009          | INC [[]]         | dst:p8 | .- текущей ячейки памяти через косвенную адресацию
0A XX        | 010 XXX      | INC [X]          | dst:m8 | .- произвольной ячейки памяти через прямую адресацию
0B XX        | 011 XXX      | INC [[X]]        | dst:p8 | .- произвольной ячейки памяти через косвенную адресацию

                                                          Декремент ..-

0C           | 012          | DEC r8           | dst:r8 | .- регистра r8
0D           | 013          | DEC []           | dst:m8 | .- текущей ячейки памяти через прямую адресацию
0E           | 014          | DEC [[]]         | dst:p8 | .- текущей ячейки памяти через косвенную адресацию
0F XX        | 015 XXX      | DEC [X]          | dst:m8 | .- произвольной ячейки памяти через прямую адресацию
10 XX        | 016 XXX      | DEC [[X]]        | dst:p8 | .- произвольной ячейки памяти через косвенную адресацию
```
```
                                                          Переместить из ..-

11 XX        | 017 XXX      | MOV r8, X        | dst:r8 << src:i8 | .- памяти прямой адресации в регистр r8
12 XX        | 018 XXX      | MOV r8, [X]      | dst:r8 << src:m8 | .- памяти прямой адресации в регистр r8
13 XX        | 019 XXX      | MOV r8, [[X]]    | dst:r8 << src:p8 | .- памяти косвенной адресации в регистр r8

14 XX        | 020 XXX      | MOV [X], r8      | dst:m8 << src:r8 | .- регистра r8 в память прямой адресации
15 XX        | 021 XXX      | MOV [[X]], r8    | dst:p8 << src:r8 | .- регистра r8 в память косвенной адресации

16 XX XX     | 022 XXX XXX  | MOV [X], X       | dst:m8 << src:i8 | .- памяти прямой адресации в память прямой адресации
17 XX        | 023 XXX      | MOV [X], [X]     | dst:m8 << src:m8 | .- памяти прямой адресации в память прямой адресации
18 XX        | 024 XXX      | MOV [X], [[X]]   | dst:m8 << src:p8 | .- памяти косвенной адресации в память прямой адресации

19 XX XX     | 025 XXX XXX  | MOV [[X]], X     | dst:p8 << src:i8 | .- памяти прямой адресации в память косвенной адресации
1A XX XX     | 026 XXX XXX  | MOV [[X]], [X]   | dst:p8 << src:m8 | .- памяти прямой адресации в память косвенной адресации
1B XX XX     | 027 XXX XXX  | MOV [[X]], [[X]] | dst:p8 << src:p8 | .- памяти косвенной адресации в память косвенной адресации
```
```
 Арифметико-логические операции
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
1C XX        | 028 XXX      | ADD r8, X        | dst:r8 << src:i8 |
1D XX        | 029 XXX      | ADD r8, [X]      | dst:r8 << src:m8 |
1E XX        | 030 XXX      | ADD r8, [[X]]    | dst:r8 << src:p8 |

1F XX        | 031 XXX      | ADD [X], r8      | dst:m8 << src:r8 |
20 XX        | 032 XXX      | ADD [[X]], r8    | dst:p8 << src:r8 |

21 XX XX     | 033 XXX XXX  | ADD [X], X       | dst:m8 << src:i8 |
22 XX        | 034 XXX      | ADD [X], [X]     | dst:m8 << src:m8 |
23 XX        | 035 XXX      | ADD [X], [[X]]   | dst:m8 << src:p8 |

24 XX XX     | 036 XXX XXX  | ADD [[X]], X     | dst:p8 << src:i8 |
25 XX XX     | 037 XXX XXX  | ADD [[X]], [X]   | dst:p8 << src:m8 |
26 XX XX     | 038 XXX XXX  | ADD [[X]], [[X]] | dst:p8 << src:p8 |
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
27 XX        | 039 XXX      | SUB r8, X        | dst:r8 << src:i8 |
28 XX        | 040 XXX      | SUB r8, [X]      | dst:r8 << src:m8 |
29 XX        | 041 XXX      | SUB r8, [[X]]    | dst:r8 << src:p8 |

2A XX        | 042 XXX      | SUB [X], r8      | dst:m8 << src:r8 |
2B XX        | 043 XXX      | SUB [[X]], r8    | dst:p8 << src:r8 |

2C XX XX     | 044 XXX XXX  | SUB [X], X       | dst:m8 << src:i8 |
2D XX        | 045 XXX      | SUB [X], [X]     | dst:m8 << src:m8 |
2E XX        | 046 XXX      | SUB [X], [[X]]   | dst:m8 << src:p8 |

2F XX XX     | 047 XXX XXX  | SUB [[X]], X     | dst:p8 << src:i8 |
30 XX XX     | 048 XXX XXX  | SUB [[X]], [X]   | dst:p8 << src:m8 |
31 XX XX     | 049 XXX XXX  | SUB [[X]], [[X]] | dst:p8 << src:p8 |
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
32 XX        | 050 XXX      | MUL r8, X        | dst:r8 << src:i8 |
33 XX        | 051 XXX      | MUL r8, [X]      | dst:r8 << src:m8 |
34 XX        | 052 XXX      | MUL r8, [[X]]    | dst:r8 << src:p8 |

35 XX        | 053 XXX      | MUL [X], r8      | dst:m8 << src:r8 |
36 XX        | 054 XXX      | MUL [[X]], r8    | dst:p8 << src:r8 |

37 XX XX     | 055 XXX XXX  | MUL [X], X       | dst:m8 << src:i8 |
38 XX        | 056 XXX      | MUL [X], [X]     | dst:m8 << src:m8 |
39 XX        | 057 XXX      | MUL [X], [[X]]   | dst:m8 << src:p8 |

3A XX XX     | 058 XXX XXX  | MUL [[X]], X     | dst:p8 << src:i8 |
3B XX XX     | 059 XXX XXX  | MUL [[X]], [X]   | dst:p8 << src:m8 |
3C XX XX     | 060 XXX XXX  | MUL [[X]], [[X]] | dst:p8 << src:p8 |
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
3D XX        | 061 XXX      | DIV r8, X        | dst:r8 << src:i8 |
3E XX        | 062 XXX      | DIV r8, [X]      | dst:r8 << src:m8 |
3F XX        | 063 XXX      | DIV r8, [[X]]    | dst:r8 << src:p8 |

40 XX        | 064 XXX      | DIV [X], r8      | dst:m8 << src:r8 |
41 XX        | 065 XXX      | DIV [[X]], r8    | dst:p8 << src:r8 |

42 XX XX     | 066 XXX XXX  | DIV [X], X       | dst:m8 << src:i8 |
43 XX        | 067 XXX      | DIV [X], [X]     | dst:m8 << src:m8 |
44 XX        | 068 XXX      | DIV [X], [[X]]   | dst:m8 << src:p8 |

45 XX XX     | 069 XXX XXX  | DIV [[X]], X     | dst:p8 << src:i8 |
46 XX XX     | 070 XXX XXX  | DIV [[X]], [X]   | dst:p8 << src:m8 |
47 XX XX     | 071 XXX XXX  | DIV [[X]], [[X]] | dst:p8 << src:p8 |
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
48 XX        | 072 XXX      | CALL X           | src:i8 |
49 XX        | 073 XXX      | CALL [X]         | src:m8 |
4A XX        | 074 XXX      | CALL [[X]]       | src:p8 |

4B           | 075          | RET              |        |
4C XX        | 076 XXX      | RET X            | src:i8 |
4D XX        | 077 XXX      | RET [X]          | dst:m8 |
4E XX        | 078 XXX      | RET [[X]]        | dst:p8 |

4F XX        | 079 XXX      | PUSH X           | src:i8 |
50 XX        | 080 XXX      | PUSH [X]         | src:m8 |
51 XX        | 081 XXX      | PUSH [[X]]       | src:p8 |

52 XX        | 082 XXX      | POP [X]          | dst:m8 |
53 XX        | 083 XXX      | POP [[X]]        | dst:p8 |
- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - -
```
    // Сравнение (8-bit's)
33 // <cmd=CMP> <arg-1=src:i8> <arg-2=src:i8>
34 // <cmd=CMP> <arg-1=src:m8> <arg-2=src:i8>
35 // <cmd=CMP> <arg-1=src:i8> <arg-2=src:m8>
36 // <cmd=CMP> <arg-1=src:m8> <arg-2=src:m8>

    // Условные переходы (8-bit's)
40 //      <cmd=JE> <arg-1=src:i8>  (Jump if Equal)
41 //     <cmd=JNE> <arg-1=src:i8>  (Jump if Not Equal)
42 //      <cmd=JB> <arg-1=src:i8>  (Jump if Below)
43 //      <cmd=JA> <arg-1=src:i8>  (Jump if Above)
44 // <cmd=JBE/JNA> <arg-1=src:i8>  (Jump if Below or Equal / Jump if Not Above)
45 // <cmd=JAE/JNB> <arg-1=src:i8>  (Jump if Above or Equal / Jump if Not Below)

46~255, // Неопределённые/неизвестные опкоды
256  // <cmd=HLT>