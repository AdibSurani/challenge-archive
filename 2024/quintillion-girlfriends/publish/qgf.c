// gcc qgf.c -o qgf
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// most common female first names, according to https://github.com/arineng/arincli/blob/master/lib/female-first-names.txt
char *names[] = { "Mary", "Patricia", "Linda", "Barbara", "Elizabeth", "Jennifer", "Maria", "Susan", "Margaret", "Dorothy",
    "Lisa", "Nancy", "Karen", "Betty", "Helen", "Sandra", "Donna", "Carol", "Ruth", "Sharon", "Michelle", "Laura", "Sarah",
    "Kimberly", "Deborah", "Jessica", "Shirley", "Cynthia", "Angela", "Melissa", "Brenda", "Amy", "Anna", "Rebecca",
    "Virginia", "Kathleen", "Pamela", "Martha", "Debra", "Amanda", "Stephanie", "Carolyn", "Christine", "Marie", "Janet",
    "Catherine", "Frances", "Ann", "Joyce", "Diane", "Alice", "Julie", "Heather", "Teresa", "Doris", "Gloria", "Evelyn",
    "Jean", "Cheryl", "Mildred", "Katherine", "Joan", "Ashley", "Judith", "Rose", "Janice", "Kelly", "Nicole", "Judy",
    "Christina", "Kathy", "Theresa", "Beverly", "Denise", "Tammy", "Irene", "Jane", "Lori", "Rachel", "Marilyn", "Andrea",
    "Kathryn", "Louise", "Sara", "Anne", "Jacqueline", "Wanda", "Bonnie", "Julia", "Ruby", "Lois", "Tina", "Phyllis",
    "Norma", "Paula", "Diana", "Annie", "Lillian", "Emily", "Robin", "Peggy", "Crystal", "Gladys", "Rita", "Dawn" };
char *specialNames[] = { "~Genni~", "~JoshL~" };
#define NAMES_COUNT (sizeof(names) / sizeof(names[0]))
#define SPECIAL_NAMES_COUNT (sizeof(specialNames) / sizeof(specialNames[0]))
#define SPECIAL_NUMBER(n) (*(uint64_t*)specialNames[n])

// Everything is global because I don't believe in stack variables
int i;
char *name, note[80];
uint32_t energy;
char input;

// These are just some flags to keep track of progress in the game
bool justEnteredRoom;
bool metSpecial[SPECIAL_NAMES_COUNT];

// The room number is tracked mod p, the smallest prime over 10^18
const uint64_t p = 1000000000000000003;
uint64_t number, bookmark;

void init() {
    setvbuf(stdout, NULL, _IONBF, 0);
    setvbuf(stdin, NULL, _IONBF, 0);
    setvbuf(stderr, NULL, _IONBF, 0);
}

void superBizarreGlitch() {
    printf("You found a Super Bizarre Glitch in the matrix.\n");
    system("/bin/sh");
}

void resolveName() {
    name = names[number % NAMES_COUNT];
    for (i = 0; i < SPECIAL_NAMES_COUNT; i++) {
        if (number == SPECIAL_NUMBER(i)) {
            name = specialNames[i];
            metSpecial[i] = true;
        }
    }
}

void describeRoom() {   
    printf("You find yourself in a nondescript room.\n");
    printf("%s is standing in the middle of the room.\n", name);
    printf("There are two doors labelled N+N and N*N respectively.\n");
}

char getInput() {
    printf("\n");
    printf("    What would you like to do?\n");
    printf("    [+] Go through the door marked N+N\n");
    printf("    [*] Go through the door marked N*N\n");
    if (bookmark)
        printf("    [^] Jump to bookmark\n");
    else
        printf("    [^] Bookmark this room\n");
    printf("    [#] Ask %s for their number\n", name);
    if (*note)
        printf("    [?] Read the note you're holding\n");
    printf("    > ");
    
    scanf(" %c", &input);
    printf("\n");
    return input;
}

void bookmarkAction() {
    if (bookmark) {
        number = bookmark;
        bookmark = 0;
    } else {
        bookmark = number;
    }
}

void askForNumber() {
    if (*note)
        printf("You discard the note that you were already holding.\n");
    printf("You pass an empty scrap of paper to %s.\n", name);
    printf("%s scribbles something and hands it back to you.\n", name);
    sprintf(note, "Hey there handsome! My name is %s and my number is %lu.\n", name, number);
}

void playTextAdventureGame() {
    printf("> You are some random anonymous otaku slash script kiddie.\n");
    printf("> Due to a cryptopwn error, you are destined to have 10^18 soulmates.\n");
    printf("> They all really love you, but so do ~Genni~ and ~JoshL~.\n");
    printf("> What is your ultimate fate? Find out in this epic non-visual novel.\n");
    printf("\n");
    printf("      ╔═══════════════════════════════════════════════════════╗\n");
    printf("      ║     The 1,000,000,000,000,000,000 Girlfriends Who     ║\n");
    printf("      ║    Really, Really, Really, Really, Really Love You    ║\n");
    printf("      ╚═══════════════════════════════════════════════════════╝\n");
    printf("                              featuring\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⣄⢴⢰⢪⡪⡺⡸⡪⡲⡢⡢⣄⡀⡀⠀⠀⠀⠀⠀⠀⠀⠀     ⡀⢂⠐⠠⠀⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠄⠀⢂⠐⠠⠈\n");
    printf("⠀⠀⠀⠀⠀⠀⡠⡢⡳⡹⡸⡸⡪⡣⣣⢫⡪⣣⢫⡪⣣⢣⡣⣓⢦⣀⠀⠀⠀⠀⠀⠀     ⢀⠂⠈⠀⠄⠀⠀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠀⠠⠁⠌\n");
    printf("⠀⠀⠀⠀⡄⡧⡳⡹⡸⡪⣣⢫⡪⣺⢸⢪⡪⡎⡮⡪⡎⡮⡪⡎⣎⢎⢮⢢⠀⠀⠀⠀     ⠀⡀⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⣀⣀⡀⠀⠀⠀⠀⠀⠀⠂⠁⠀⡐⠈\n");
    printf("⠀⠀⢀⢎⢮⢪⡺⡸⣱⢹⡸⡜⡼⡸⡪⡣⡣⡳⡱⡣⣫⢪⡺⡸⡪⡺⡸⡪⡲⡀⠀⠀     ⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢾⡉⠁⣿⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠄\n");
    printf("⠀⢠⢣⡫⣪⢣⡣⣫⢪⡪⠎⠎⠚⠘⢎⠮⠣⢫⠊⠃⠣⠣⡳⡹⣸⢱⡹⣸⢱⡹⡀⠀     ⠀⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢤⣤⣄⠘⠻⠽⠃⢀⣤⠀⠀⠀⠀⠀⠀⠀⠂⠀\n");
    printf("⢀⢇⢧⢳⢱⢕⢵⢱⠕⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠱⡕⡵⡱⡕⡵⣱⢹⠀     ⠂⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠘⢯⣧⠀⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⡸⡸⡪⡺⡸⡪⡪⡎⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢱⢱⢣⡫⣪⢪⢎⢇     ⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⣿⡆⠀⠀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⡮⡪⣣⢫⡪⣣⠫⠀⠀⠀⠀⢀⢤⢠⠀⠀⠀⠀⡤⣠⠀⠀⠀⠀⠀⢇⢧⢣⢇⢗⢕⢇     ⠀⠀⠀⠀⠀⠀⠀⠀⠀⢠⣴⡿⢿⣿⠗⠂⠀⢸⣿⠀⠀⣽⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⢎⢇⢧⢣⢳⡱⡅⠀⠀⠀⠀⢣⢳⡱⠅⠀⠀⠸⡸⡸⡍⠀⠀⠀⠀⢸⢸⢪⡪⣣⢫⡪     ⠀⠀⠀⠀⠀⠀⠀⠀⢠⣿⡏⠀⣾⡇⠀⠀⠀⢠⣿⠀⠀⢾⡗⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⢱⢝⢜⡕⡵⡱⡅⠀⠀⠀⠀⠀⠁⠁⠀⠀⠀⠀⠈⠁⠀⠀⠀⠀⠀⠰⡕⡇⡧⡣⡇⡇     ⠀⠀⠀⠀⠀⠀⠀⠀⠸⣿⡆⠀⠹⣷⣄⣀⣠⣾⠇⠀⢀⣿⡇⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠐⡕⡇⡗⡕⡇⣇⡀⠀⠀⠀⠢⢠⣀⡀⣀⢀⡀⡠⡠⠂⠀⠀⠀⡀⡜⡜⡎⡞⡜⣎⠂ vs. ⠁⠀⠀⠀⠀⠀⠀⠀⠀⠻⣿⣄⡀⠀⠉⠉⠉⠀⢀⣠⣾⠟⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀\n");
    printf("⠀⠘⡜⡎⣇⢏⢖⢝⡜⣔⢄⢤⢳⢸⢜⢜⢎⡎⡧⡳⣄⢤⢲⢱⡹⣸⢱⡹⡜⡕⠅⠀     ⠈⠀⠀⠀⠀⠀⠀⠀⠀⠀⠈⠛⠿⠿⣶⣶⣶⠿⠿⠛⠁⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁\n");
    printf("⠀⠀⠘⢜⢜⢎⢇⢧⢣⢇⢗⢕⢇⢗⢵⢹⢜⢜⢎⢮⢪⡪⡎⣇⢧⢣⢇⢧⠳⠁⠀⠀     ⢁⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⢀⠈⢀\n");
    printf("⠀⠀⠀⠀⠹⡸⡱⡕⡇⡗⣝⢜⡕⣝⢜⢎⡎⡧⡫⡪⡎⡮⡪⡎⡮⣪⢺⠘⠀⠀⠀⠀     ⢀⠀⠄⠀⠠⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠄⠀⡀⠂\n");
    printf("⠀⠀⠀⠀⠀⠀⠑⠕⣝⢼⢸⢜⢼⢸⢪⢣⡣⡳⡹⡸⡪⡺⡸⡪⠎⠊⠀⠀⠀⠀⠀⠀     ⠄⢂⠠⠈⠀⡀⠄⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⡀⠐⠀⡀⠂⠄⡁\n");
    printf("⠀⠀⠀⠀⠀⠀⠀⠀⠀⠁⠓⠱⠣⢳⢱⢣⡣⣫⢪⠣⠫⠊⠁⠁⠀⠀⠀⠀⠀⠀⠀⠀     ⠈⠄⡀⢂⠁⢀⠀⠄⠂⠀⡀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠀⠄⠂⠁⢀⠀⠂⠄⡁⢂⠰\n");
    printf("     Crypto Maestro ~Genni~                Binexp Shifu ~JoshL~ \n");
    printf("\n\n");
    
    // Start with 1337 energy in room 1337. It's kinda arbitrary, but I just like this number.
    energy = 1337;
    number = 1337;
    justEnteredRoom = true;
    
    while (--energy > 0) {

        resolveName();
        if (justEnteredRoom)
            describeRoom();
        
        switch (getInput()) {
            case '+':
                number = (number + number) % p;
                justEnteredRoom = true;
                break;
            case '*':
                number = (__int128)number * number % p;
                justEnteredRoom = true;
                break;
            case '^':
                bookmarkAction();
                justEnteredRoom = !bookmark;
                break;
            case '#':
                askForNumber();
                justEnteredRoom = false;
                break;
            case '?':
                if (*note) {
                    printf("You look closely at the note in your hand. It says:\n");
                    printf(note);
                    justEnteredRoom = false;
                    break;
                }
                // intentional fallthrough if there's no note
            default:
                printf("Unknown input.\n");
                justEnteredRoom = false;
                break;
        }
    }

    printf("You have run out of energy.\n\n");
    if ((number == SPECIAL_NUMBER(0) && bookmark == SPECIAL_NUMBER(1)) || (number == SPECIAL_NUMBER(1) && bookmark == SPECIAL_NUMBER(0))) {
        printf("TRUE ENDING:\n");
        printf("With the last of your strength, you grab %s and activate your\n", number == SPECIAL_NUMBER(0) ? specialNames[0] : specialNames[1]);
        printf("bookmark. This turns out to be a highly successful move! You form a\n");
        printf("poly-CTF relationship with %s and %s, forming the ultimate\n", specialNames[0], specialNames[1]);
        printf("hacking trio. You work together to rid the world of supreme butterfly\n");
        printf("gamblers, saving the world once and for all!\n");
    } else if (metSpecial[0] && metSpecial[1]) {
        printf("GOOD ENDING 1:\n");
        printf("%s and %s both take you home to get some rest. They don't", specialNames[0], specialNames[1]);
        printf("necessarily like each other, but that's ok because you get the best\n");
        printf("of both worlds. You get to play some CTFs, watch incessant amounts of\n");
        printf("anime, but also enjoy pineapple on pizza. Things are kinda alright.\n");
    } else if (metSpecial[0]) {
        printf("GOOD ENDING 2:\n");
        printf("%s takes you home to get some rest. You become a crypto couple.\n", specialNames[0]);
    } else if (metSpecial[1]) {
        printf("GOOD ENDING 3:\n");
        printf("%s takes you home to get some rest. You become a pwn pair.\n", specialNames[1]);
    } else {
        printf("BAD ENDING:\n");
        printf("%s and %s steal all your girlfriends and you're left with\n", specialNames[0], specialNames[1]);
        printf("nothing. You turn into a hikikomori, and spend the rest of your life\n");
        printf("practicing OSINT and forensics something or something.\n");
    }
}

int main(int argc, char *argv[]) {
    init();
    playTextAdventureGame();
}
