#include <iostream>
#include <cstdio>
#include <cmath>
#include <fstream>
#include <Windows.h>
#include "2D_graphics.h"
#include "timer.h"
#include "UDP_com.h"
#include "UDP_com6.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

#include "Objects.h"

using namespace std;

#define KEY(c) ( GetAsyncKeyState((int)(c)) & (SHORT)0x8000 )

void opening_screen(int initial_v[2][3],int &player); //Sets the initial values for the game number of players which player and which boat and how many islands

void client_update(int Sock, int Port, char *IP_local, char *IP_send, World &W, char *buff_in, char *buff_out); //gets information from the other player(client) computer and puts it into the buffer

void client_update6(int Sock, int Port, char* IP_local, char* IP_send, World& W, char* buff_in, char* buff_out); //gets information from the other player(client) computer and puts it into the buffer

int Check_intial_values(int Sock, int Port, char* IP_local, char* IP_send, char* buff_in, char* buff_out, int initial_v[2][3], int &player); //sets the player 2 values based on player 1 through the buffer if they both choose the same player value then the game is set to default values.

void set_default(int initial_v[2][3]); // sets the default parameters for the game

void set_default_player(char* IP_local, char* IP_send, int& player); // sets which player will be player 1 or player 2 if they both choose the same player in the opening screen. player muust be set for intial boat position.
  

int main()
{

		double xs, ys, qs, scale, dt;
		xs = 255; // initial x, y, q(anlge) values for the the ship if single player
		ys = 300;
		qs = 0.0;
		scale = 1;
		int i, j;
		int initial_value[2][3]; // the array of intial values passed into the world class and stores the game intial conditions
		/* initial_values arrray diagram
		
		row 1 local info { [number of players or game style], [Which boat you want to use], [number of islands] }
		row 2 enemy(client) info { [number of players or game style], [Which boat the enemy will use], [number of islands] }

		*/
		int Player = 1; // sets player initially to 1 as a default.
		char quit; // the value that will be set to quit the game
		double start_time = 0.0; // for the game timer
		double width = 1500, height = 800; // size of the direct x window

		for (i = 0; i < 2; i++) {
			for (j = 0;j < 3;j++) {

				initial_value[i][j] = 0; // initializes the intial values array to all zero.
			}
		}

		opening_screen(initial_value, Player); // openeing screen call. to get intial values and set player values

		initialize_graphics(); 

		if (initial_value[0][0] != 1) { // This is the parameter that holds the number of player so checks if single of multi

			dt = 1.0 / 60;
			char buffer_in[NMAX_UDP_BUFFER];
			char buffer_out[NMAX_UDP_BUFFER];
			int sock;
			int port;

			char IP_address_local[NMAX_ADDRESS] = "192.168.2.13"; // IPv4 Local IP


			char IP_address_send[NMAX_ADDRESS] = "192.168.2.41"; // IPv4 Client IP


			port = 37001; // IPv4
		//	port = 37000; // IPv6 Teredo

			activate_network();

			activate_socket(port, IP_address_local, sock); // IPv4
		//	activate_socket6(port,IP_address_local,sock); // IPv6


			while (Check_intial_values(sock, port, IP_address_local, IP_address_send, buffer_in, buffer_out, initial_value, Player) == 0) { // check_initial_values returns 0 or 1 depening on if the other cpmputer has sent a response if not it will wait for a response.
				cout << "\n waiting for the other player to be ready\n";
				Sleep(100); // give the other computer 0.1 sec to send a message before checking again
			}

			if (Player == 1) { // sets the starting positions for both players based on who is player 1 or player 2
				xs = 255;
				ys = 300;
				qs = 0.0;
			}
			else {
				xs = width-255;
				ys = height-300;
				qs = 3.145;
			}

			World W(xs, ys, qs, "openwater.png", "Island.png", initial_value, "torpedo3.png"); // sets a World object W.

			cout << "\nGame will start in:\n";

			for (int i = 5; i > 0; i--) {
				cout << endl << i << endl;
				Sleep(1000); // sleep for a second to count down for both player to start at same time and be able to switch from command prompt to the direct X window
			}

			cout << "\nGame is ready to play with both players\n";
			
			while (1) {
				start_time = high_resolution_time(); // timer begins counting up to 60
				cout << endl <<start_time << endl;											
				clear();
				W.draw();
				text((char)start_time, width - 755, height - 45, 0.5); // print the game time to the screen
				W.sim_step(dt);
				W.input();
				W.collision();
				W.ship_speed();
				W.collision_torpedo();
				W.collision_coin();
				W.collision_torpedo_ship();
				W.enemy_collision_torpedo();
				client_update(sock, port, IP_address_local, IP_address_send, W, buffer_in, buffer_out); // check the buffer for information about the other boat location, health and if torpedo has been shot

				if (start_time > 5) { // make sure the game is played for atleast five seconds.
					
					if (W.Player_health <= 0) {
						clear();
						text("You Lose", 500, 400, 1.5);
						update();
						break;
					}

					if (W.Player_2_health <= 0) {
						clear();
						text("You Win", 500, 400, 1.5);
						update();
						break;
					}
				}

				update();
				if (start_time > 60) { 

					if(W.coin_counter > W.player2_coin_counter){
						clear();
						text("You got more coins! You Win", 100, 400, 1.5);
						update();
						break;
					}
					else if(W.coin_counter < W.player2_coin_counter){
						clear();
						text("Sorry you got less coins you lose", 100, 400, 1.5);
						update();
						break;
					}
					else {

						clear();
						text("This Game was a Tie", 100, 400, 1.5);
						update();
						break;
					}
				} // When the game has been played for 60 sec the game ends and points will be compared to see who wins
			}

			Sleep(100);

			deactivate_socket(sock); // IPv4
			//deactivate_socket5(sock); //IPv6

			deactivate_network();
			
		}

		else { // this is the loop if the game is played in single player instead of multiplayer

			World W(xs, ys, qs, "openwater.png", "Island.png", initial_value, "torpedo3.png"); // sets a World object W.

			while(1) {

				cout << endl << start_time << endl; 
				start_time = high_resolution_time(); // timer begins counting up to 60
				clear();
				dt = 1.0 / 60;
				W.sim_step(dt);
				W.input();
				W.collision();
				W.draw();
				text((char)start_time, width - 755, height - 40, 0.5); // print the game time to the screen
				W.ship_speed();
				W.collision_torpedo();
				W.collision_coin();
				update();

				if (start_time > 60) { 

					clear();
					text("Congradulations you got: ", 100, 400, 1.5);
					text(W.coin_counter, 925, 400, 1.5);
					text("Coins", 1100, 400, 1.5);
					update();
					break; 
				} // Game ends after 60 sec and prints how many coins you got.
			}
		}

		cout << "\nGame Over.\n"; // prints game over to the command prompt

	return 0;
}


void opening_screen(int initial_v[2][3], int &player){ //Sets the initial values for the game number of players which player and which boat and how many islands

	int i;

	for(;;) { //error checking to make sure they either put 1 or 2.

		cout << "\nSelect 1 for single player and 2 for multiplayer:\n";
		cin >> initial_v[0][0]; // set the game style single or multiplayer

		if (initial_v[0][0] == 1 || initial_v[0][0] == 2) { break; }
		else { cout << "\nError you did not input 1 or 2."; }
	}

	if (initial_v[0][0] == 2) { // checks if the game style if multiplayer runs this loop.
		for (;;) { //error checking to make sure they either put 1 or 2.

			cout << "\nSelect which player you will be? 1 or 2:\n";
			cin >> player;

			initial_v[1][0] = initial_v[0][0]; // intializes the game style to the same value 2 for both the first row values

			if (player == 1 || player == 2) { break; }
			else { cout << "\nError you did not input 1,2."; }
		}
	}
	else { player = 1; } // sets the player value to 1 if the game is set to single player

	if (initial_v[0][0] == 2) {// checks the game style if multiplayer runs this loop.
		if (player == 1) { //if youre player 1 then you will set the values for both users if youre player 2 you do not select inital values

			cout << "\nYou are player 1 your values will set the game for both players\n";

			for (i = 0; i < initial_v[0][0]; i++) {
				for (;;) { //error checking to make sure they either put 1,2, or 3.

					cout << "\nSelect Boat for player " << i+1 << " : 1,2,3,4,5,6 :\n";
					cin >> initial_v[i][1]; // sets the boat for player 1 then player 2

					if (initial_v[i][1] == 1 || initial_v[i][1] == 2 || initial_v[i][1] == 3 || initial_v[i][1] == 4 || initial_v[i][1] == 5 || initial_v[i][1] == 6) { break; }
					else { cout << "\nError you did not input 1,2,3,4,5,6."; }
				}
			}

			for (;;) { //error checking to make sure they either put 1,2,3, or 4.

				cout << "\nSelect number of islands 1,2,3,4:\n";
				cin >> initial_v[0][2];

				initial_v[1][2] = initial_v[0][2]; // sets the number of islands for local and client to the same in the inital values array

				if (initial_v[i][2] == 1 || initial_v[i][2] == 2 || initial_v[i][2] == 3 || initial_v[i][2] == 4) { break; }
				else { cout << "\nError you did not input 1,2,3."; }
			}
		}
		else { // if youre player 2 you will get initial values from the other players initial values so no need to set any

			cout << "\nInitial conditions will be taken from player 1.\n";
		}
	}
	else {
		for (;;) { //error checking to make sure they either put 1 or 2.

			cout << "\nSelect Boat for player" << initial_v[0][0] << "choose boat 1,2,3,4,5,6 :\n";
			cin >> initial_v[0][1];

			if (initial_v[0][1] == 1 || initial_v[0][1] == 2 || initial_v[0][1] == 3 || initial_v[0][1] == 4 || initial_v[0][1] == 5 || initial_v[0][1] == 6) { break; }
			else { cout << "\nError you did not input 1,2,3,4,5,6."; }
		}

		for (;;) { //error checking to make sure they either put 1 or 2.

			cout << "\nSelect number of islands 1,2,3,4:\n";
			cin >> initial_v[0][2];

			if (initial_v[0][2] == 1 || initial_v[0][2] == 2 || initial_v[0][2] == 3 || initial_v[0][2]==4) { break; }
			else { cout << "\nError you did not input 1,2,3."; }
		}
	}
	
	return;
}

void client_update(int Sock, int Port, char *IP_local, char *IP_send, World &W, char *buff_in, char *buff_out) //gets information from the other player(client) computer and puts it into the buffer
{

	int size_in;
	int size_out;
	int k, ans;
	
	char IP_address_recv[NMAX_ADDRESS]; // to get the ip address of the enemy(client) computer. to send information back too.


	char* p; // pointer to the buffer
	double* pd; // pointer to double values
	int* pi; // pointer to int values


		size_out = 0; // intiallizes to the amount of values being held by the buffer to 0 so that the size can be increased for each value added
		p = buff_out; // points the P value to the out buffer

		pd = (double*)p;  // casts p to a double to point at that location of pd.
		*pd = W.S[1]->x; //placess the local ships x coordinate into the buffer.
		p += sizeof(double); //increments p by the size of a double in bytes
		size_out += sizeof(double); // increases the amount of values the buffer holds by the size of a double in bytes

		pd = (double*)p;
		*pd = W.S[1]->y; //placess the local ships y coordinate into the buffer.
		p += sizeof(double);
		size_out += sizeof(double);

		pd = (double*)p;
		*pd = W.S[1]->q; //placess the local ships q(angle theta) coordinate into the buffer.
		p += sizeof(double);
		size_out += sizeof(double);

		pi = (int*)p; // casts p to an int to point at that location of pi.
		*pi = W.local_hit_marker; //set the hit_marker to either a 1 or 0 to signify that the local torpedo has hit the other ship.
		if (W.local_hit_marker == 1) { W.local_hit_marker = 0; } // resets the hitmarker back to 0 after the local hitmarker with value 1 has been sent to the buffer
		p += sizeof(int); //increments p by the size of an in in bytes
		size_out += sizeof(int); // increases the amount of values the buffer holds by the size of a int in bytes

		pi = (int*)p;
		*pi = W.Player_health; // places in the buffer the local players health information to be displayed and if they have been killed by torpedos. 
		p += sizeof(int);
		size_out += sizeof(int);

		pd = (double*)p;
		*pd = W.T[W.N_fire]->fx; //places in the buffer the fx value to signify a torpedo has been shown so that the local torpedo can be shown on the other players screen.
		p += sizeof(double);
		size_out += sizeof(double);

		send(buff_out, size_out, IP_send, Sock, Port); // sends the buffer to the othe rcomputer.

		ans = recv(buff_in, size_in, IP_address_recv, Sock); // receives the buffer from the other computer and plces it into buffer_in.


		if (ans > 0) { // checks if there was an answer from the other computer if ther was not then the function simplyreturns without updating the enemy player information until the function is called again and an answer is received.

			k = 1;

			p = buff_in; // points p to the newly received buffer.

			while (recv(buff_in, size_in, IP_address_recv, Sock) > 0) { // checks if there are multiple answers from the other computer and only reads the most latest answer to get the most up to date information.
				k++;
			}

			//p = buff_in; // points p to the newly received buffer.

			pd = (double*)p; // points pointer pd to the pointer p.
			W.S[2]->x = *pd; // sets the ship 2 in the world to the enemy(client) computer x value.
			p += sizeof(double); //moves the point p formward by the size of a double in bytes.

			pd = (double*)p;
			W.S[2]->y = *pd; // sets the ship 2 in the world to the enemy(client) computer y value.
			p += sizeof(double);

			pd = (double*)p;
			W.S[2]->q = *pd; // sets the ship 2 in the world to the enemy(client) computer q(angle) value.
			p += sizeof(double);

			pi = (int*)p; // points pointer pi to the pointer p.
			W.enemy_hit_marker = *pi; // sets the hit marker to inform the local player if their boat has been hit by an enemy torpedo.
			p += sizeof(int); //moves the point p formward by the size of an in in bytes.

			pi = (int*)p;
			W.Player_2_health = *pi; // sets the other players health to inform the game if it is over and to print to the screen.
			p += sizeof(int);

			pd = (double*)p;
			W.enemy_T[W.enemy_N_fire]->fx = *pd; // sets the fx value so that an enemy torpedo will be shown leaving ship to to hit the local ship.
			p += sizeof(double);

		}
		else { W.enemy_hit_marker = 0; } // if there is no response and the hitmarker was set to 1 before the enemy hit marker is set to zero so that additional hits are not counted

		//Sleep(100); //creates lag when there is sleeping since the function call is already delayed by running the rest of the functions in the while loop in the main function.

	return;
}

void client_update6(int Sock, int Port, char* IP_local, char* IP_send, World& W, char* buff_in, char* buff_out) // Works the same as client_update just with the IPv6
{

	int size_in;
	int size_out;
	int k, ans;

	char IP_address_recv[NMAX_ADDRESS]; // to get the ip address of the enemy(client) computer. to send information back too.


	char* p; // pointer to the buffer
	double* pd; // pointer to double values
	int* pi; // pointer to int values


	size_out = 0; // intiallizes to the amount of values being held by the buffer to 0 so that the size can be increased for each value added
	p = buff_out; // points the P value to the out buffer

	pd = (double*)p;  // casts p to a double to point at that location of pd.
	*pd = W.S[1]->x; //placess the local ships x coordinate into the buffer.
	p += sizeof(double); //increments p by the size of a double in bytes
	size_out += sizeof(double); // increases the amount of values the buffer holds by the size of a double in bytes

	pd = (double*)p;
	*pd = W.S[1]->y; //placess the local ships y coordinate into the buffer.
	p += sizeof(double);
	size_out += sizeof(double);

	pd = (double*)p;
	*pd = W.S[1]->q; //placess the local ships q(angle theta) coordinate into the buffer.
	p += sizeof(double);
	size_out += sizeof(double);

	pi = (int*)p; // casts p to an int to point at that location of pi.
	*pi = W.local_hit_marker; //set the hit_marker to either a 1 or 0 to signify that the local torpedo has hit the other ship.
	if (W.local_hit_marker == 1) { W.local_hit_marker = 0; } // resets the hitmarker back to 0 after the local hitmarker with value 1 has been sent to the buffer
	p += sizeof(int); //increments p by the size of an in in bytes
	size_out += sizeof(int); // increases the amount of values the buffer holds by the size of a int in bytes

	pi = (int*)p;
	*pi = W.Player_health; // places in the buffer the local players health information to be displayed and if they have been killed by torpedos. 
	p += sizeof(int);
	size_out += sizeof(int);

	pd = (double*)p;
	*pd = W.T[W.N_fire]->fx; //places in the buffer the fx value to signify a torpedo has been shown so that the local torpedo can be shown on the other players screen.
	p += sizeof(double);
	size_out += sizeof(double);

	send6(buff_out, size_out, IP_send, Sock, Port); // sends the buffer to the othe rcomputer.

	ans = recv6(buff_in, size_in, IP_address_recv, Sock); // receives the buffer from the other computer and plces it into buffer_in.


	if (ans > 0) { // checks if there was an answer from the other computer if ther was not then the function simplyreturns without updating the enemy player information until the function is called again and an answer is received.

		k = 1;

		p = buff_in; // points p to the newly received buffer.

		while (recv6(buff_in, size_in, IP_address_recv, Sock) > 0) { // checks if there are multiple answers from the other computer and only reads the most latest answer to get the most up to date information.
			k++;
		}

		//p = buff_in; // points p to the newly received buffer.

		pd = (double*)p; // points pointer pd to the pointer p.
		W.S[2]->x = *pd; // sets the ship 2 in the world to the enemy(client) computer x value.
		p += sizeof(double); //moves the point p formward by the size of a double in bytes.

		pd = (double*)p;
		W.S[2]->y = *pd; // sets the ship 2 in the world to the enemy(client) computer y value.
		p += sizeof(double);

		pd = (double*)p;
		W.S[2]->q = *pd; // sets the ship 2 in the world to the enemy(client) computer q(angle) value.
		p += sizeof(double);

		pi = (int*)p; // points pointer pi to the pointer p.
		W.enemy_hit_marker = *pi; // sets the hit marker to inform the local player if their boat has been hit by an enemy torpedo.
		p += sizeof(int); //moves the point p formward by the size of an in in bytes.

		pi = (int*)p;
		W.Player_2_health = *pi; // sets the other players health to inform the game if it is over and to print to the screen.
		p += sizeof(int);

		pd = (double*)p;
		W.enemy_T[W.enemy_N_fire]->fx = *pd; // sets the fx value so that an enemy torpedo will be shown leaving ship to to hit the local ship.
		p += sizeof(double);

	}
	else { W.enemy_hit_marker = 0; } // if there is no response and the hitmarker was set to 1 before the enemy hit marker is set to zero so that additional hits are not counted

	//Sleep(100); //creates lag when there is sleeping since the function call is already delayed by running the rest of the functions in the while loop in the main function.

	return;
}

int Check_intial_values(int Sock, int Port, char* IP_local, char* IP_send, char* buff_in, char* buff_out, int initial_v[2][3], int &player) { //sets the player 2 values based on player 1 through the buffer if they both choose the same player value then the game is set to default values.

	int size_in; // for the size of the buffer in
	int size_out; // for the size of the buffer out
	int i, j, k, ans;

	int client_player;

	char IP_address_recv[NMAX_ADDRESS]; // to get the ip address of the enemy(client) computer. to send information back too.

	char* p; // pointer to the buffer
	int* pi; // pointer to int values

	size_out = 0; // intiallizes to the amount of values being held by the buffer to 0 so that the size can be increased for each value added
	p = buff_out; // points the P value to the out buffer

	pi = (int*)p; // casts p to an int to point at that location of pi.
	*pi = player; // sets the pointer pi to the player value from the opening screen.
	p += sizeof(int); //increments p by the size of an in in bytes
	size_out += sizeof(int); // increases the amount of values the buffer holds by the size of a int in bytes

	for (i = 1; i >= 0; i--) { // so the client choices appear first in the array because the boat selection is the opposite for the other player.
		for (j = 0; j < 3 ; j++) {

			pi = (int*)p;
			*pi = initial_v[i][j]; // sets the buffer values to the intial values that were created.
			p += sizeof(int);
			size_out += sizeof(int);
		}
	}


	send(buff_out, size_out, IP_send, Sock, Port); // sends the buffer to the othe rcomputer.

	ans = recv(buff_in, size_in, IP_address_recv, Sock); // receives the buffer from the other computer and plces it into buffer_in.


	if (ans > 0) { // checks if there was an answer from the other computer if ther was not then the function end waiting for the other player is sent to the command prompt, sleeps of 0.1 sec and then client_initial_value is called again.

		k = 1;

		p = buff_in; // points p to the newly received buffer.

		while (recv(buff_in, size_in, IP_address_recv, Sock) > 0) { // checks if there are multiple answers from the other computer and only reads the most latest answer to get the most up to date information.
			k++;
		}

		//p = buff_in; // points p to the newly received buffer.

		pi = (int*)p;
		client_player = *pi;
		p += sizeof(int);

		if (client_player == player) { // checks if both players selected to be both player 1 or player 2 if this is the case then the player values will be set based on ip numbers and the games conditions will be set to default game conditions.
			cout << "\nBoth players entered the same player value. The game for both players will be set to default settings.\n";
			set_default(initial_v); // set the game to default settings
			set_default_player(IP_local, IP_send, player); // sets the player values based on which player has a higher 10th value in the ip address
			return 1;
		}

		if (player == 1) { return 1; } //if the local computer is player 1 then the function returns 1 because its values will be used to set the other player initial conditions 
		// return 1 to signify the while loop can be closed and the game countdown can begin
		else {
			for (i = 0; i < 2; i++) {
				for (j = 0; j < 3; j++) {

					pi = (int*)p; // points pointer pi to the pointer p.
					initial_v[i][j] = *pi; // sets the values of the intial values array based on the initial values with the rows flipped from the enemy(client) player 1 computer.
					p += sizeof(int); //moves the point p formward by the size of an in in bytes.
				}
			}
		}
			return 1; // return 1 to signify the while loop can be closed and the game countdown can begin
	}
	else { return 0; } // return 0 to signify the while loop must continue the other player is not ready yet to begin the game since a response hasnt been sent from the other computer.
}

void set_default(int initial_v[2][3]) { // sets the default parameters for the game

	int i, j;

	for (i = 0; i < 2; i++) { // sets all the intial values in the array to 1 so the boat selectionm adn islands will be set to 1 for both.
		for (j = 0; j < 3; j++) {

			initial_v[i][j] = 1;
		}
	}

	initial_v[0][0] = 2; // sets both the number of player values in the array to 2. since there needs to be 2 players to call this function
	initial_v[1][0] = 2;

	return;
}

void set_default_player(char* IP_local, char* IP_send, int& player) // sets which player will be player 1 or player 2 if they both choose the same player in the opening screen. player muust be set for intial boat position.
{
	if ((int)IP_local[10] < (int)IP_send[10]) { player = 1; } // checks the ip address of both players and the one with a higher 10th value will be set to player 1.
	else { player = 2; }

}