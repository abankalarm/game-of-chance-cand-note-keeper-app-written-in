

#include<stdio.h>
#include<string.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<time.h>
#include<stdlib.h>
#include"noteaddon.h"


#define DATAFILE "var/chance.data" //file to store user data
// custom user struct to store information about user

struct user{
    int uid;
    int credits;
    int highscore;
    char name[100];
    int (*current_game)();
};

//function prototypes

int get_player_data();
void register_new_player();
void update_player_data();
void showhighscore();
void jackpot();
void input_name();
void print_cards(char *, char *, int);
int take_wager(int , int);
void play_the_game();
int pick_a_number();
int dealer_no_match();
int find_the_ace();
void fatal(char *);

//global variables
struct user player; //player struct

int main() {
        int choice, last_game;

        srand(time(0)); //to seed the randomizer with current time

        if( get_player_data == -1)
            register_new_player();

    
        printf("----welcome to the world famous game of chance-----\n");
        printf("    )        (                   (                   \n  ");
        printf("        (          )     (             )\n");
        printf("            )    *           )        )  (\n");
        printf("        (                (        (      *\n");
        printf("            )               )        )\n");
        printf("                      [ ]            (\n");
        printf("               (  *   |-|       *     )    (\n");
        printf("        *       )     |_|        .          )\n");
        printf("               (      | |    .  \n");
        printf("        )            /   \\     .    ' .        *\n");
        printf("        (           |_____|  '  .    .  \n");
        printf("        )           | ___ |  \\~~~/  ' .   (\n");
        printf("                *   | \\ / |   \\_/  \\~~~/   )\n");
        printf("                    | _Y_ |    |    \\_/   (\n");
        printf("        *           |-----|  __|__   |      *\ns");
        printf("                    `-----`        __|__\n");
        printf("1 - play pick a number game\n");
        printf("2 - play the no match dealer game\n");
        printf("3 - play the find the ace game\n");
        printf("4 - show highscore\n");
        printf("5 - change your username\n");
        printf("6 - reset your account at 100 creds\n");
        printf("7 - quit\n");
        printf("[player : %s]", player.name);
        printf("[wallet : %u credits\n", player.credits);
        scanf("enter your choice %d ", &choice);

        if ((choice<1) || (choice>7))
            printf("\nplease choose the available options\n");
        else if (choice < 4)
        {
            if(choice != last_game) { // If the function ptr isn't set
                if(choice == 1)      // then point it at the selected game
                    player.current_game = pick_a_number;
                else if(choice == 2)
                    player.current_game = dealer_no_match;
                else
                    player.current_game = find_the_ace;
                last_game = choice; // and set last_game.
            }
            play_the_game();
        }
        else if (choice == 4)
            showhighscore();
        else if (choice == 5) {
            printf("\nChange player name\n");           
            printf("Enter your new name: ");
            input_name();
            printf("++++Your name has been changed.++++\n\n");
        }
        else if (choice == 6) {
            printf("\nYour account has been reset with 100 credits.\n\n");
            player.credits = 100;
        }
        
        printf("1");
        update_player_data();
        printf("\nThanks for playing! Bye.\n");    
}

//the program checks for user data against the current uid. return -1 if not able to find one

int get_player_data(){
    int fd, uid, read_bytes;
    struct user entry;

    uid = getuid(); //user id- linux user not really

    fd = open(DATAFILE, O_RDONLY);
    if(fd == -1) //couldnt open file
        return -1;
    read_bytes = read(fd ,&entry,sizeof(struct user)); 
    while (entry.uid != uid && read_bytes > 0){
        read_bytes = read(fd ,&entry ,sizeof(struct user)); //to loop and keep reading
    }
    close(fd);
    if (read_bytes< sizeof(struct user)) //means file end has been reached
        return -1;
    else 
        player = entry; //copy entry read into player struct
    return 1; //return success

}


//this is the registration for new user
//this will create a new player account and append it to the file

void register_new_player(){
    int fd;

    printf("===={ new player reg}====\n");
    printf("enter your name:");

    player.uid = getuid();
    player.highscore = player.credits=100;

    fd = open(DATAFILE, O_WRONLY|O_CREAT|O_APPEND, S_IRUSR|S_IWUSR);

    if(fd==-1)
        fatal("in register new player () while registering the player");
    write(fd, &player, sizeof(struct user));
    close(fd);

    printf("welcome to the game of chance\n");
    printf("you have been given %u credits", player.credits);
}


//this functionality writes the current player data to the file
//for updating credits after playing the game

void update_player_data(){
    int fd, i ,read_uid;
    char burned_byte;
    
    fd= open(DATAFILE, O_RDWR);
    if (fd==-1)
        fatal("there was an error while opening file");
    read(fd, &read_uid, 4); //to read the uid from the first struct
    while (read_uid != player.uid)
    {
        for(i=0; i<(sizeof(struct user)-4);i++)
            read(fd, &burned_byte, 1);
        read(fd, &read_uid, 4);
    }
    write(fd, &(player.credits), 4); //update credits
    write(fd, &(player.highscore), 4); //update highscore
    write(fd ,&(player.name), 4);
    close(fd);

}


//this function is to display current high score 

void showhighscore(){
    unsigned int top_score = 0;
    char top_name[100];
    struct user entry;

    int fd;
    printf("\n--------------------HIGH SCORE-------------------\n");
    fd = open(DATAFILE, O_RDONLY);
    if(fd==-1)
        fatal("there is something wrong while opening the file");
    while(read(fd, &entry, sizeof(struct user)) > 0) { // Loop until end of file.
        if(entry.highscore > top_score) { // If there is a higher score,
            top_score = entry.highscore; // set top_score to that score
            strcpy(top_name, entry.name); // and top_name to that username.
            }
}
    close(fd);
    if(top_score > player.highscore)
        printf("%s has the high score of %u\n", top_name, top_score);
    else
        printf("You currently have the high score of %u credits!\n", player.highscore);
    printf("======================================================\n\n");
}
// This function simply awards the jackpot for the Pick a Number game.
void jackpot() {
    printf("*+*+*+*+*+* JACKPOT *+*+*+*+*+*\n");
    printf("You have won the jackpot of 100 credits!\n");
    player.credits += 100;
}
// This function is used to input the player name, since
// scanf("%s", &whatever) will stop input at the first space.
void input_name() {
    char *name_ptr, input_char='\n';
    while(input_char == '\n')
    // Flush any leftover
        scanf("%c", &input_char); // newline chars.
    
    name_ptr = (char *) &(player.name); // name_ptr = player name's address
    while(input_char != '\n') { // Loop until newline.
        *name_ptr = input_char;
                                   // Put the input char into name field.
        scanf("%c", &input_char); // Get the next char.
        name_ptr++;              // Increment the name pointer.
    }
    *name_ptr = 0; // Terminate the string.
    }
// This function prints the 3 cards for the Findthe Ace game.
    
// It expects a message to display, a pointer tothe cards array,
    
// and the card the user has picked as input. Ifthe user_pick is
// -1, then the selection numbers are displayed.

void print_cards(char *message, char *cards, int user_pick) {
    int i;

    printf("\n\t*** %s ***\n", message);
    printf("\t._.\t._.\t._.\n");
    printf("Cards:\t|%c|\t|%c|\t|%c|\n\t", cards[0], cards[1], cards[2]);
    if(user_pick == -1)
        printf(" 1 \t 2 \t 3\n");
    else {
        for(i=0; i < user_pick; i++)
            printf("\t");
        printf(" ^-- your pick\n");    
    }
}

//this function is used to input wager for both thr no match dealer and find the ace game


int take_wager(int available_credits, int previous_wager){
    int wager, total_wager;

    printf("you have %d credits in your wallet, how much are you willing to wager", available_credits);
    scanf("%d",wager);

    if (wager<1){
        printf("nicetry but you must wager a positive number");
        return -1;
    }
    total_wager= wager + previous_wager;
    if(total_wager> available_credits){
        printf("this aimnt your town sheriff, you can only wager what you own");
        return -1;
    }


}

//this function is essentially a loop to allow the player to run game again and again

void play_the_game(){
    int play_again = 1;
    int (*game)();
    char selection;

    while (play_again){
        printf("\n[DEBUG] current_game pointer @\n", player.current_game);
        if(player.current_game() != -1) {// If the game plays without error and
            if(player.credits > player.highscore) // a new high score is set,
                player.highscore = player.credits; // update the highscore.
            printf("\nYou now have %u credits\n", player.credits);
            update_player_data();
            // Write the new credit total to file.
            printf("Would you like to play again? (y/n) ");
            selection = '\n';
            while(selection == '\n')// Flush any extra newlines.
                scanf("%c", &selection);
            if(selection == 'n')
                play_again = 0;
            }
        else
        // This means the game returned an error,
            play_again = 0; // so return to main menu.
    }
}

// This function is the Pick a Number game.
// It returns -1 if the player doesn't have enough credits.
int pick_a_number() {
    int pick, winning_number;

    printf("\n####### Pick a Number ######\n");
    printf("This game costs 10 credits to play. Simply pick a number\n");
    printf("between 1 and 20, and if you pick the winning number, you\n");
    printf("will win the jackpot of 100 credits!\n\n");
    winning_number = (rand() % 20) + 1; // Pick a number between 1 and 20.
    if(player.credits < 10) {
        printf("You only have %d credits. That's not enough to play!\n\n", player.credits);
        return -1; // Not enough credits to play
    }
    player.credits -= 10; // Deduct 10 credits.
    printf("10 credits have been deducted from your account.\n");
    printf("Pick a number between 1 and 20: ");
    
    scanf("%d", &pick);
    printf("The winning number is %d\n", winning_number);
    if(pick == winning_number)
        jackpot();
    else
        printf("Sorry, you didn't win.\n");
    return 0;
}
// This is the No Match Dealer game.
// It returns -1 if the player has 0 credits.
int dealer_no_match() {
    int i, j, numbers[16], wager = -1, match = -1;

    printf("\n::::::: No Match Dealer :::::::\n");
    printf("In this game, you can wager up to all of your credits.\n");
    printf("The dealer will deal out 16 random numbers between 0 and 99.\n");
    printf("If there are no matches among them, you double your money!\n\n");

    if(player.credits == 0) {
        printf("You don't have any credits to wager!\n\n");
        return -1;
    }
    while(wager == -1)
        wager = take_wager(player.credits, 0);
    
    printf("\t\t::: Dealing out 16 random numbers :::\n");
    for(i=0; i < 16; i++) {
        numbers[i] = rand() % 100; // Pick a number between 0 and 99.
        printf("%2d\t", numbers[i]);
    if(i%8 == 7)
    // Print a line break every 8 numbers.
        printf("\n");
    }
    for(i=0; i < 15; i++) {
    // Loop looking for matches.
        j = i + 1;
        while(j < 16) {
        if(numbers[i] == numbers[j])
        match = numbers[i];
        j++;
        }
    }
    if(match != -1) {
        printf("The dealer matched the number %d!\n", match);
        printf("You lose %d credits.\n", wager);
        player.credits -= wager;
    } else {
            printf("There were no matches! You win %d credits!\n", wager);
            player.credits += wager;
    }
    return 0;
}
// This is the Find the Ace game.
// It returns -1 if the player has 0 credits.
int find_the_ace() {
    int i, ace, total_wager;
    int invalid_choice, pick = -1, wager_one = -1, wager_two = -1;
    char choice_two, cards[3] = {'X', 'X', 'X'};
   
    ace = rand()%3; // Place the ace randomly.
   
    printf("******* Find the Ace *******\n");
    printf("In this game, you can wager up to all of your credits.\n");
    printf("Three cards will be dealt out, two queens and one ace.\n");
    printf("If you find the ace, you will win your wager.\n");
    printf("After choosing a card, one of the queens will be revealed.\n");
    printf("At this point, you may either select a different card or\n");
    printf("increase your wager.\n\n");
   
    if(player.credits == 0) {
        printf("You don't have any credits to wager!\n\n");
        return -1;
        }
   
    while(wager_one == -1) // Loop until valid wager is made.
         wager_one = take_wager(player.credits, 0);
    
    print_cards("Dealing cards", cards, -1);
    pick = -1;
    while((pick < 1) || (pick > 3)) { // Loop until valid pick is made.
        printf("Select a card: 1, 2, or 3 ");
        scanf("%d", &pick);
        }
    pick--; // Adjust the pick since card numbering starts at 0.
    i=0;
    while(i == ace || i == pick) // Keep looping until
        i++;
    // we find a valid queen to reveal.
    cards[i] = 'Q';
    print_cards("Revealing a queen", cards, pick);
    invalid_choice = 1;
    while(invalid_choice) {
    // Loop until valid choice is made.
        printf("Would you like to:\n[c]hange your pick\tor\t[i]ncrease your wager?\n");
        printf("Select c or i: ");
        choice_two = '\n';
        while(choice_two == '\n') // Flush extra newlines.
            scanf("%c", &choice_two);
        if(choice_two == 'i') {
            // Increase wager.
            invalid_choice=0;
        // This is a valid choice.
            while(wager_two == -1)
        // Loop until valid second wager is made.
                wager_two = take_wager(player.credits, wager_one);
        }
        if(choice_two == 'c') {
        // Change pick.
            i = invalid_choice = 0; // Valid choice
            while(i == pick || cards[i] == 'Q') // Loop until the other card
                i++;
        // is found,
            pick = i;
        // and then swap pick.
            printf("Your card pick has been changed to card %d\n", pick+1);
        }
    }
    for(i=0; i < 3; i++) { // Reveal all of the cards.
        if(ace == i)
            cards[i] = 'A';
        else
            cards[i] = 'Q';
        }
    print_cards("End result", cards, pick);
    
    if(pick == ace) { // Handle win.
        printf("You have won %d credits from your first wager\n", wager_one);
        player.credits += wager_one;
        if(wager_two != -1) {
            printf("and an additional %d credits from your second wager!\n", wager_two);
            player.credits += wager_two;
            }
    } else { // Handle loss.
        printf("You have lost %d credits from your first wager\n", wager_one);
        player.credits -= wager_one;
        if(wager_two != -1) {
            printf("and an additional %d credits from your second wager!\n", wager_two);
            player.credits -= wager_two;
            }
    }
    return 0;
}

















