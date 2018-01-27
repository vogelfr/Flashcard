# Flashcard
Small flashcard program

FlashCard v2.0.3
Jonathan Rosenthal, Frédéric Vogel, Eric Mink

## Compilation
The program has to be compiled using the C++11 standard.

`g++ -std=c++11 -Wall FlashCard.cpp -o FlashCard`

## Execution
The program is executed by calling ./FlashCard in the same directory as the program.
It can either be called with the name(s) of flashcard files as arguments or without. If called without
the program will ask for the name of the flashcard file, which must be in the same directory. If the same file is provided multiple times, it will be loaded multiple times

Additionally, `--version` may be passed as an argument with or without accompanying flashcard files. 

#### Card sorting options

*These are mutually exclusive*. The last specified will be used if you provide multiple.

`-r` for random cards (default)

`-R` for random cards that have not yet been shown. This option basically re-shuffles the deck when you're through all cards and then goes through it sequentially.

`-s`for sequential loading of cards.

#### Show progress

*Progress is not available for the default `-r` mode*. If you use it anyways, it will just do nothing.

`-p`to display progress (default)

`-P` to hide progresss

#### Continue where you left off

*Intended for use with `-s`.* If you use it with `-r` instead, you shouldn't notice a difference. If you use it with `-R`, you get weird behaviour, so ... don't do that. (What happens is that it does not shuffle the deck for the first run through)

`-c 12` sets the starting point to the 12th card (indexing starts by 1). You may use any number in the range`]0, number_of_cards]`.

## Flashcard File
The flashcard file contains n flashcards in the following manner:
- On the (2*i)-th line, delimited by '\n', the front of the i-th card.
- On the ((2*i)+1)-th line, delimited by '\n', the back of the i-th card.
  The following symbols have special meaning:
- `\n`: Delimits the front from the back from the front and so on
- `|`:  Delimits a new line *inside* of the card
- `$`:  Delimits a tab character *inside* of the card
- Any line starting with `//` will be ignored (since v 2.0.2)

The symbols '|', '$' and '\n' also break the flow of the program, requiring the user to type in enter to continue
the execution.

## Closing
Just press Ctrl+c

