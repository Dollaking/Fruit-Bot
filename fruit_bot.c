// fruit_bot.c
// Assignment 3, COMP1511 18s1: Fruit Bot
//
// This program by Aven Au (z5208734) on 15/05/2018
//
// Version 1.0.0: Assignment released.

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>

#include "fruit_bot.h"

#define SELL	1
#define BUY		-1
#define NO_VALUE 30000

void print_player_name(void);
void print_move(struct bot *b);
void run_unit_tests(void);


// ADD PROTOTYPES FOR YOUR FUNCTIONS HERE
int nearest_distance(struct bot *b, char *produce, int price, int size);
int electricity_cost(struct bot *b, int size);
int map_size(struct bot *b);
int best_buy(struct bot *b, int size);
int best_sell(struct bot *b, char *produce, int size);
int best_sell_profit(struct bot *b, char *produce, struct location *l, int prior_distance, int quantity, int size);
int n_bots(struct location *l);
int best_sell_profit_quantity(struct bot *b, char *produce, struct location *l, int prior_distance, int quantity, int size);
int best_buy_quantity(struct bot *b, int size);
void best_buy_test(struct bot *b, int size);
void best_sell_test(struct bot *b, char *produce, int size);
int best_sell_distance(struct bot *b, char *produce, struct location *l, int prior_distance, int quantity, int size);
// YOU SHOULD NOT NEED TO CHANGE THIS MAIN FUNCTION

int main(int argc, char *argv[]) {

    if (argc > 2) {
        // supply any command-line argument to run unit tests
        run_unit_tests();
        return 0;
    }

    struct bot *me = fruit_bot_input(stdin);
    if (me == NULL) {
        print_player_name();
    } else {
        print_move(me);
    }

    return 0;
}

void print_player_name(void) {
    // CHANGE THIS PRINTF TO YOUR DESIRED PLAYER NAME
    printf("Tony did a shoey fix6");

}

// print_move - should print a single line indicating
//              the move your bot wishes to make
//
// This line should contain only the word Move, Sell or Buy
// followed by a single integer

void print_move(struct bot *b) {
    // REPLACE THEN WITH YOUR CODE
	struct location *start = b->location;
	struct location *current = b->location;
	int size = map_size(b);
	int nearest_electricity = nearest_distance(b, "Electricity", BUY, size);
	int elec_bill = electricity_cost(b, size);
	// This function allows me to see all the information
	//it has collected and see if they make the right decisions
	//////////////////////////////////////////////////////////////////////////
	//	//best_buy_test(b, size);											//
	//	//best_sell_test(b, b->fruit, size);								//
	//	//printf("%d\n", size);  //Check whether the electric cost is right //
	//////////////////////////////////////////////////////////////////////////

	//Best Sell, Best Buy Strat
	if (b->battery_level < 50 && strcmp(current->fruit, "Electricity") == 0 && 
			current->quantity > 0 && n_bots(current) < current->quantity && 
				nearest_distance(b, "Electricity", BUY, size) != NO_VALUE){
	        printf("Buy %d", b->battery_capacity); //Buys battery
    } else if (b->battery_level == 0) {
		//This makes sure that the bot doesnt buy anything when on zero battery
        printf("Sell %d", b->fruit_kg);
    } else if (b->fruit_kg == 0) { //Meaning that the bot has the incentive to buy
		//This makes sure that the bot doesnt buy anything because it usually takes
		// 4 moves to make a complete transaction
        if (b->turns_left < 4){
            printf("Move 0");
			//This means that the bot needs to be charged
		} else if (b->battery_level < 50 && b->turns_left > 30 &&
	               nearest_electricity != NO_VALUE &&
	                    nearest_electricity >= -1 * (b->maximum_move)
	                            && nearest_electricity <= (b->maximum_move)) {
	            printf("Move %d", nearest_electricity);
		//Best_buy function shows the best place to buy, if it is at zero
		//it means the best place to buy is at current position
        } else if (best_buy(b, size) == 0) {
			printf("Buy %d\n", best_buy_quantity(b, size));
		} else {
			printf("Move %d\n", best_buy(b, size));
		}
	} else if (b->fruit_kg > 0) { //Sell Mode
		if (best_sell(b, b->fruit, size) == 0) {
			printf("Sell %d\n", current->quantity);
		} else {
			printf("Move %d\n", best_sell(b, b->fruit, size));
		}
	}
}


// ADD A COMMENT HERE EXPLAINING YOUR OVERALL TESTING STRATEGY

void run_unit_tests(void) {
    // PUT YOUR UNIT TESTS HERE
    // This is a difficult assignment to write unit tests for,
    // but make sure you describe your testing strategy above.

	/* My overall strategy is to add copy a function and add print functions
	in them. For example I copy and pasted the best_buy function and turned it
	into a void function and put it in the main function. This function shows me
	all the quantities, fruit, distance and the profit. This shows me what
	information that the bot has access to and see if they are making the right
	decisions. I use excel to tabulate the data from the map and compare the results
	with what is shown from the best_buy_print function. This functions is every
	useful and it shows me if other functions are bugged as my best_buy functions
	include best_Sell funcitons and electricity_cost functions. So if my
	"Sell quantity: " is wrong then there must be something wrong with the sells
	function, if the decisions made are wrong, then it must be the best_buy function's
	problem.

	I also test worlds from tournaments and use those worlds to test. I would
	tabulate the world on excel so i would know what move is the next best moves.
	If it does not move to that position I would use the print void FUNCTIONS
	that I mentioned before to debug to each function.*/
}


// ADD YOUR FUNCTIONS HERE

//This function allows me to find the nearest desired location to sell or buys
// a specific product
int nearest_distance(struct bot *b, char *produce, int price, int size) {
 	struct location *l = b->location;
	int spots[MAX_LOCATIONS] = {0}; //Spots with desired variables
	int position = 0; //Starting point
	char *starting_point = l->name;
	int counter = 0; //Serves as countdown of positions
	int i = 0; //Serves as a countdown to arrange arrays
	int difference = 0; //more like distance

	if (price > 0) { //This is when we are selling something
		while (position < size) {
			if (strcmp(l->fruit, produce) == 0 && l->quantity > 0 && n_bots(l)
					<= l->quantity  && price > 0) {
				spots[counter] = position;
				counter++;
			}
			position++;

			l = l->east;
		}
	} else if (price < 0) { //This is when we are buying something
		while (position < size) {
			if (strcmp(l->fruit, produce) == 0 && l->quantity > 0 && n_bots(l)
					<= l->quantity  && price < 0) {
				spots[counter] = position;
				counter++;
			}
			position++;

			l = l->east;
		}
	}
	int min_difference = spots[0];
    if (counter == 0) {
        return NO_VALUE;
    }
	while (i < counter) { //This is to find the lowest distances
		if (strcmp(b->location->fruit, produce) == 0 && l->quantity > 0 &&
				n_bots(l) <= l->quantity) {
			min_difference = 0;
			i = counter;
		} else if (spots[i] > (position / 2)) {
			difference = spots[i] - position; //This creates negative distances
		} else {
			difference = spots[i];
		}

		if (abs(difference) < abs(min_difference)) {
			min_difference = difference;
		}
		i++;
	}

    return min_difference;
}

/*This function allows me to find out how much electricity would cost per turn*/
int electricity_cost(struct bot *b, int size) {
	struct location *l = b->location;
	struct location *start = b->location;
	char *starting_point = b->location->name;
	int flag = 0;
	int max_cost = 0;
	int n_stations = 0;
	int answer = 0;
	int i = 0;

	while (i < size) { //Finding the max electricity cost per distance moved
		if (strcmp(l->fruit, "Electricity") == 0) {
			if(abs(l->price) > max_cost){
				max_cost = abs(l->price);
			}
			n_stations++;
		}
		l = l->east;
		i++;
	}

	answer = max_cost;

	return answer;
}
/* This function determines how big the map is */
int map_size(struct bot *b) {
	char *start_point = b->location->name;
	int counter = 0;
	struct location *l = b->location;
	while (strcmp(l->east->name, start_point) != 0) {
		counter++;
		l = l->east;
	}
		counter++;

	return counter;
}
/*This tells me which location is the most profitable to buy from */
int best_buy(struct bot *b, int size) {
	struct location *l = b->location;
	struct location *head = b->location;
	int i = 0; //Counter for distance
	int distance = 0;
	int min_distance = 0; //Distance that is weighed the least as more profit
    int quantity = 0; // The amount that would be bought
    int max_profit = 0; //Max amount of profit per turn
    int profit = 0; //Current profit from total path
    int sell_quantity = 0; //The quantity that is able to be sold
	int cash_buy = 100000; // The quantity that could be bought with current cash
	int total_distance = 0; //Distance of total journey from start point to sell
	float profit_per_turn = 0; //Profits made per turn
	float n_turns = 0; // Number of turns
	while (i < size) {
		quantity = 0;
		if (l->price < 0){
			distance = i;
			if (distance > size / 2) { //Create negative distances
				distance = distance - size;
			}
	        sell_quantity = best_sell_profit_quantity(b, l->fruit, l, abs(distance), 0, size);
			if (l->price < 0) { //This function makes sures that cash_buy != x/0
				cash_buy = b->cash/abs(l->price);
			} else {
				cash_buy = 1000000;
			}

			//This whole if function determines which quantity is constant throughout
			// the path
	        if (l->quantity >= b->maximum_fruit_kg && sell_quantity >=
					b->maximum_fruit_kg && cash_buy >= b->maximum_fruit_kg) {
	            quantity = b->maximum_fruit_kg;
	        } else if(b->maximum_fruit_kg >= l->quantity && sell_quantity >=
						l->quantity && cash_buy >= l->quantity) {
	            quantity = l->quantity;
	        } else if (b->maximum_fruit_kg >= sell_quantity && l->quantity >=
							sell_quantity && cash_buy >= sell_quantity){
	            quantity = sell_quantity;
	        } else if (b->maximum_fruit_kg >= cash_buy && l->quantity >=
								cash_buy && sell_quantity >= cash_buy){
				quantity = cash_buy;
			}
			total_distance = abs(best_sell_distance(b, l->fruit, l, abs(distance),
			 					quantity, size)) + abs(distance);
			n_turns = ceil(total_distance / b->maximum_move);
			if (n_turns == 0){
				n_turns = 1;
			}
			profit = best_sell_profit(b, l->fruit, l, abs(distance), quantity, size) -
						(abs(l->price) * quantity + electricity_cost(b, size) * abs(distance)) ;
			profit_per_turn = ceil(profit/n_turns);

	        if (l->quantity > 0 && l->price < 0 && n_bots(l) <= l->quantity &&
					profit_per_turn > max_profit && strcmp(l->fruit, "Electricity") != 0
						&& n_turns < b->turns_left) {
	            min_distance = distance;
	            max_profit = profit_per_turn;
	        }
		}
		i++;
		l = l->east;
	}

	return min_distance;
}
/*This function tests my buy_test function, (Basically exactly the same as
	best_buy but with printfs) */
void best_buy_test(struct bot *b, int size) {

	struct location *l = b->location;
	struct location *head = b->location;
	int i = 0;
	int min = 9000000;
	int distance = i;
	int min_distance = 0;
    int quantity = 0;
    int max_profit = 0;
    int profit = 0;
    int sell_quantity = 0;
	int cash_buy = 100000;
    int max_distance = 1000000;
	int total_distance = 0;
	float profit_per_turn = 0;
	float n_turns = 0;
	while (i < size) {
		quantity = 0;
		if (l->price < 0){
			distance = i;
			if (distance > size / 2) {
				distance = distance - size;
			}
	        sell_quantity = best_sell_profit_quantity(b, l->fruit, l, abs(distance), 0, size);
			if (l->price < 0) {
				cash_buy = b->cash/abs(l->price);
			} else {
				cash_buy = 1000000;
			}
	        if (l->quantity >= b->maximum_fruit_kg && sell_quantity >=
					b->maximum_fruit_kg && cash_buy >= b->maximum_fruit_kg) {
	            quantity = b->maximum_fruit_kg;
	        } else if(b->maximum_fruit_kg >= l->quantity && sell_quantity >=
						l->quantity && cash_buy >= l->quantity) {
	            quantity = l->quantity;
	        } else if (b->maximum_fruit_kg >= sell_quantity && l->quantity >=
							sell_quantity && cash_buy >= sell_quantity){
	            quantity = sell_quantity;
	        } else if (b->maximum_fruit_kg >= cash_buy && l->quantity >=
							cash_buy && sell_quantity >= cash_buy){
				quantity = cash_buy;
			}
			total_distance = abs(best_sell_distance(b, l->fruit, l, abs(distance),
								quantity, size)) + abs(distance);
			n_turns = ceil(total_distance / b->maximum_move);
			if (n_turns == 0){
				n_turns = 1;
			}
			profit = best_sell_profit(b, l->fruit, l, abs(distance), quantity, size) -
						(abs(l->price) * quantity + electricity_cost(b, size) * abs(distance)) ;
			profit_per_turn = ceil(profit/n_turns);

	        if (l->quantity > 0 && l->price < 0 && n_bots(l) <= l->quantity &&
					profit_per_turn > max_profit && strcmp(l->fruit, "Electricity") != 0) {
	            min_distance = distance;
	            max_profit = profit_per_turn;
				//Gives me warnings when I shorten the print function
				printf("Cash_Buy: %d  Sell_quantity: %d Buy_Quantity: %d Max_store: %d Quantity: %d Profit: %f Distance: %d Fruit: %s  N: %f\n", cash_buy, sell_quantity, l->quantity, b->maximum_fruit_kg, quantity,profit_per_turn, total_distance, l->fruit, n_turns);
	        }
		}
		i++;
		l = l->east;
	}

}
/*This function finds the best location to sell */
int best_sell(struct bot *b, char *produce, int size) {

	struct location *l = b->location;
	struct location *head = b->location;
	int i = 0;
	int max = -10000;
	int distance = i;
	int max_distance = 0;
    int sell_profit = 0;
    int quantity = 0;
	float profit_per_turn = 0;
	float n_turns = 0;
	while (i < map_size(b)) {
		distance = i;
		if (distance > map_size(b) / 2) {
			distance = distance - map_size(b);
		}
		//Don't want to overestimate our profits so we need to find the smallest quantity
        if (l->quantity > b->fruit_kg) {
            quantity = b->fruit_kg;
        } else {
            quantity = l->quantity;
        }
		n_turns = ceil(abs(distance) / b->maximum_move);
		if (n_turns == 0){
			n_turns = 1;
		}
        sell_profit = l->price * quantity - abs(distance) * electricity_cost(b, size);
		profit_per_turn = ceil(sell_profit/n_turns);
		if ( (strcmp(produce, l->fruit) == 0)
            && l->price > 0 && l->quantity > 0 && profit_per_turn > max
                && distance <= b->battery_level && distance >= -1 *
						(b->battery_level) && n_bots(l) <= l->quantity && n_turns < b->turns_left){
			max_distance = distance;
			max = profit_per_turn;
		}

		i++;
		l = l->east;
	}
	// This function occurs when there is no where to sell, so last resort would
	// be to send it to Recyling
   if (max == -10000 && max_distance == 0) {
	   if (strcmp(produce, "Anything") == 0) {
		   return 0;
	   } else {
        return best_sell(b, "Anything", size);
		}
    }

	return max_distance;
}
/* Test for best_sell (Basically exactly the same as best_sell but with printf) */
void best_sell_test(struct bot *b, char *produce, int size) {

	struct location *l = b->location;
	struct location *head = b->location;
	int i = 0;
	int max = -10000;
	int distance = i;
	int max_distance = 0;
    int sell_profit = 0;
    int quantity = 0;
	while (i < size) {
		distance = i;
		if (distance > size / 2) {
			distance = distance - size;
		}
		//Don't want to overestimate our profits so we need to find the smallest quantity
        if (l->quantity > b->fruit_kg) {
            quantity = b->fruit_kg;
        } else {
            quantity = l->quantity;
        }

        sell_profit = l->price * quantity - abs(distance) * electricity_cost(b, size);
		if (l->fruit != NULL && produce != NULL){
			if ( (strcmp(produce, l->fruit) == 0)
	            && l->price > 0 && l->quantity > 0 && sell_profit > max
	                && distance <= b->battery_level && distance >= -1 *
							(b->battery_level) && n_bots(l) <= l->quantity){
				max_distance = distance;
				max = sell_profit;
				//Gives me warnings when I shorten the print function
				printf("i: %d, Distance: %d, Fruit: %s, Quantity: %d, Sell_profit: %d Max: %d Bots: %d Store: %d\n", i, distance, l->fruit, quantity, sell_profit, max, n_bots(l), l->quantity);
			}
		}

		i++;
		l = l->east;
	}



}
/*This function finds which sell location is most profitable */
int best_sell_profit(struct bot *b, char *produce, struct location *l,
							int prior_distance, int quantity, int size) {
	//Doesnt need to compare quantity
	int i = 0;
	int max = -10000;
	int distance = i;
	int max_distance = 0;
    int sell_profit = 0;
    int true_quantity = 0;
	while (i < size)  {
		if(l->price > 0){
			distance = i;
			if (distance > size / 2) {
				distance = distance - size;
			}

	        sell_profit = l->price * quantity;

			if ( (strcmp(produce, l->fruit) == 0)
	            && l->price > 0 && l->quantity > 0 && sell_profit > max
	                && distance <= b->battery_level - prior_distance && distance
							>= -1 * (b->battery_level - prior_distance) &&
									n_bots(l) <= l->quantity){
				max_distance = distance;
				max = sell_profit;
			}
		}
		i++;
		l = l->east;
	}

	return max;
}

/*This function finds the quantity that would be used to sell at a particular location*/
int best_sell_profit_quantity(struct bot *b, char *produce, struct location *l,
								int prior_distance, int quantity, int size) {
    int i = 0;
	int max = -10000;
	int distance = i;
	int max_distance = 0;
    int sell_profit = 0;
    int true_quantity = 0;
	int min_quantity = 0;
	while (i < size)  {
		if(l->price > 0){
			distance = i;
			if (distance > size / 2) {
				distance = distance - size;
			}
	        true_quantity = l->quantity;

	        sell_profit = l->price * true_quantity - abs(distance) * electricity_cost(b, size);

			if ( (strcmp(produce, l->fruit) == 0)
	            && l->price > 0 && l->quantity > 0 && sell_profit > max
	                && distance <= b->battery_level - prior_distance && distance
							>= -1 * (b->battery_level - prior_distance) &&
									n_bots(l) <= l->quantity){
				max_distance = distance;
				max = sell_profit;
				min_quantity = true_quantity;
			}
		}

			i++;
			l = l->east;
	}

	return min_quantity;
}
/*Number of bots in a particular location */
int n_bots(struct location *l){
    struct bot_list *bot = l->bots;
    int i = 0;
	if (bot == NULL){
		return 0;
	}
    while (bot->next != NULL){
        i++;
        bot = bot->next;
    }
    i++;
    return i;
}
/*The quantity that would be bought (basically the same function as best_buy but
	returns a quantity variable) */
int best_buy_quantity(struct bot *b, int size) {

	struct location *l = b->location;
	struct location *head = b->location;
	int i = 0;
	int min = 9000000;
	int distance = i;
	int min_distance = 0;
    int quantity = 0;
    int max_profit = 0;
    int profit = 0;
    int sell_quantity = 0;
	int cash_buy = 100000;
    int max_distance = 1000000;
	int total_distance = 0;
	float profit_per_turn = 0;
	float n_turns = 0;
	int max_quantity = 0;
	while (i < size) {
		quantity = 0;
		if (l->price < 0){
			distance = i;
			if (distance > size / 2) {
				distance = distance - size;
			}
	        sell_quantity = best_sell_profit_quantity(b, l->fruit, l,
								abs(distance), 0, size);
			if (l->price < 0) {
				cash_buy = b->cash/abs(l->price);
			} else {
				cash_buy = 1000000;
			}
	        if (l->quantity >= b->maximum_fruit_kg && sell_quantity >=
						b->maximum_fruit_kg && cash_buy >= b->maximum_fruit_kg) {
	            quantity = b->maximum_fruit_kg;
	        } else if(b->maximum_fruit_kg >= l->quantity && sell_quantity >=
							l->quantity && cash_buy >= l->quantity) {
	            quantity = l->quantity;
	        } else if (b->maximum_fruit_kg >= sell_quantity && l->quantity >=
							sell_quantity && cash_buy >= sell_quantity){
	            quantity = sell_quantity;
	        } else if (b->maximum_fruit_kg >= cash_buy && l->quantity >=
							cash_buy && sell_quantity >= cash_buy){
				quantity = cash_buy;
			}
			total_distance = abs(best_sell_distance(b, l->fruit, l, abs(distance),
									quantity, size)) + abs(distance);
			n_turns = ceil(total_distance / b->maximum_move);
			if (n_turns == 0){
				n_turns = 1;
			}
			profit = best_sell_profit(b, l->fruit, l, abs(distance), quantity, size) - (
							abs(l->price) * quantity + electricity_cost(b, size) * abs(distance)) ;
			profit_per_turn = ceil(profit/n_turns);

	        if (l->quantity > 0 && l->price < 0 && n_bots(l) < l->quantity &&
					profit > max_profit && strcmp(l->fruit, "Electricity") != 0) {
	            min_distance = distance;
	            max_profit = profit;
				max_quantity = quantity;
	        }
		}
		i++;
		l = l->east;
	}

	return max_quantity;
}
/*Returns the distance it takes from buy site to sell site*/
int best_sell_distance(struct bot *b, char *produce, struct location *l, int prior_distance, int quantity, int size) {
	//Doesnt need to compare quantity
	int i = 0;
	int max = -10000;
	int distance = i;
	int max_distance = 0;
    int sell_profit = 0;
    int true_quantity = 0;
	while (i < size)  {
		if(l->price > 0){
			distance = i;
			if (distance > size / 2) {
				distance = distance - size;
			}

	        sell_profit = l->price * quantity;

			if ( (strcmp(produce, l->fruit) == 0)
	            && l->price > 0 && l->quantity > 0 && sell_profit > max
	                && distance <= b->battery_level - prior_distance && distance
							>= -1 * (b->battery_level - prior_distance) && n_bots(l) <= l->quantity){
				max_distance = distance;
				max = sell_profit;
			}
		}
		i++;
		l = l->east;
	}

	return max_distance;
}

