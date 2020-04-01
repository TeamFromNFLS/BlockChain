# Readme

1. ## Naming Rules

   1.**Common Rules**

      1. Name would only contain letters (A-Z, a-z) and numbers (0-9) or underline (_). Case sensitive.
      2. The name of files, functions and variables should be descriptive.
      3. Names of classes, user-defined types and variables should be nouns. Names of functions should be nouns or combinations of nouns and verbs.
      4. Never use abbreviations unless the meaning is absolutely clear.
      5. Often use camel case.

   2.**Name of Files**

      1. Start with lowercase letters. e.g. myFile.cpp
      2. Cpp files end with .cpp. Head files end with .h.
      3. The name of definition should correspond to its declaration. e.g. file.cpp and file.

   3.**Name of Types**

      1. Title case, no underline.

   4.**Name of Macros and enumerations**

      1. Try not to use Macros.
      2. All uppercase letters, space with underlines. e.g. MAX_VALUE

   5.**Name of Functions**

      1. Title case, no underline.
      2. Use combinations of nouns and verbs to claim the usage of this function. e.g. GetName()

   6.**Name of Variables**

      1. Camel case, start with a lowercase letter.
      2. Loop variables: i, j, k (u, v, ...)
      3. Temporary variable: tmp

   7.**Name of Constants**

      1. Title case. e.g. MaxValue

2. ## A really simple guide of gdb

   1. In your **makefile**, make sure that you add "-g3" into the compile order.
   2. Use command: gdb $(EXEDIR) to start debugging.
   3. In gdb, use command dir $(DIR) to locate to the file you want.
   4. Then use command break(**b** for short) fileName.cpp:n to make a break point at line #n.
   5. Use command run(**r** for short) to run the program. Then your program would stop at the break point.
   6. Use print(**p** for short) variableName to print the variable you want to.
   7. More commands, please search on the Internet.
Add anything new if you want!
Telling others what you've done through commits! *(NO need of date)*

