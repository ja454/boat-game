
#include <iostream>
#include <cstdio>
#include <cmath>
#include <Windows.h>
#include "2D_graphics.h"
#include "timer.h"
#include "ran.h"
#include "Objects.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

torpedo::torpedo(double xt, double yt, char* file_namet){  

	rho = 1000;
	s = 60;
	cf = 77.1;
	this->xt = xt;
	this->yt = yt;
	qt = 0.0;
	vt = 0.0;
	mass = 4.2e2;
	fx = 0.0;
	vmaxt = 235;
	scale_t = 0.04;
	create_sprite(file_namet, it);

}

void torpedo::vel(double dt) { /// works similar to function in the ship class, see notes for details

	if (vt > vmaxt) {
		vt = vmaxt;
	}
	double meq, ceq, c;
	double b;
	ceq = 0.5 * rho * (s*0.68) * cf;
	meq = mass / dt;
	b = meq / ceq;
	c = fx / ceq;
	vt = (-b + sqrt((b * b) + 4 * c)) / 2;

} 

void torpedo::sim_step(double dt) {
	
   if (vt > vmaxt) {
		vt = vmaxt;
	}
	xt = xt + vt * cos(qt) * dt;
	yt = yt + vt * sin(qt) * dt;

}

void torpedo::draw() {

	draw_sprite(it, xt, yt, qt, scale_t);

}


ship::ship(double x, double y, double q, char* file_name) {
	s_height = 800;
	s_width = 1500;
	scale = 0.12;
	this->x = x;
	this->y = y;
	this->q = q;
	vmax = 100;
	wmax = 1.0;
	vmin = 0.0;
	rho = 1000;
	s = 60;
	cf = 77.1;
	m = 9.86e6;
	fx = 0;
	v = 0;
	we = 0;
	d = 465 * scale;
	lenght = 930 * scale; // to find the lenght of the ship, need to multiply by scale. Same for width
	width= 121 * scale; //
	Ncircles = (int)(lenght / width); // Number of circles used for the collider
	create_sprite(file_name, id);
	xc = new double[Ncircles];  // For the collider
	yc = new double[Ncircles];
}

void ship::sim_step(double dt)
{

	if (v > vmax) {
		v = vmax;
	}
	if (v < vmin) {
		v = vmin;
	}
	x = x + v * cos(q) * dt;
	y = y + v * sin(q) * dt;
	

}

void ship::speed() {

	double xt, yt, sc, xs, ys;
	yt = 550;
	ys = 550;
	xt = 200;
	xs = 600;
	sc = 0.5;



	if (we > 8500) {
		we = 8500; //max rpm
	}
	if (we < 0) {
		we = 0;  //engine off
	}
	text("V (126 max)", s_width-1450, s_height-150, sc);
	text(v, s_width - 1200, s_height - 150, sc);
	text("Eng rpm (8500max)", s_width - 1450, s_height - 180, sc);
	text(we, s_width - 1175, s_height - 180, sc);
}

void ship::draw()
{
	draw_sprite(id, x, y, q, scale);
}

void ship::vel(double dt) {// this function determines the velocity of the ship, the derivation of the equation is explained in the report

	if (fx > 11.113e10) {
		fx = 11.113e10; // max forward force in newtons
	}
	if (fx < 0) {
		fx = 0;
	}
	double meq, ceq, c;
	double b;

	ceq = 0.5 * rho * s * cf;
	meq = m / dt;
	b = meq / ceq;
	c = fx / ceq;
	v = (-b + sqrt((b * b) + 4 * c)) / 2; //see notes for explanation
}

void ship::collider() {// The collider, creates circles along the lenght of the ship of diameter equal to the width of the ship, this is what is used to check for collisions
	double xb, yb ; 
	double pi = 4*atan(1.0); 
	xb = (cos(q + pi)*d) + (x); //Position of the rear of the ship relative to the origin, used as a reference to draw the circles.
	yb = (sin(q + pi)*d)+(y);

	
	for (int i = 1; i <= Ncircles; i++) {   // As mentioned above, Ncircles determines the number of circles needed, ie lenght/width
		xc[i] = (cos(q) * (width * i)) + xb;
		yc[i] = (sin(q) * (width * i)) + yb;
	}
	return;
}

island::island(double xi, double yi, char* file_name) {

	this->xi = xi;
	this->yi = yi;
	scalei = 0.6;
	qi = 0.0;
	Ri = 65;       // radius of the island
	create_sprite(file_name, id);
	return;
}

void island::draw() {

	draw_sprite(id, xi, yi, qi, scalei);

	return;
}

Coin::Coin(double xc, double yc) {

	long int seed = -4;
	scalec = 0.05;
	
	this->xc = xc;
	this->yc = yc;
	cout << "\nxc =" << xc << endl;
	cout << "\nyc =" << yc << endl;
	

	create_sprite("coin.png", idc);

	return;
}

void Coin::draw() {

	draw_sprite(idc, xc, yc, 0, scalec);

	return;
}



World::World(double X, double Y, double Q, char* background, char* island_pic, int initial_v[2][3], char* torp) {

	total_players = initial_v[0][0]; // An array that holds initial conditions
	this->X = X;
	this->Y = Y;
	this->Q = Q;
	int i;
	height=800;
	width =1500;


	N_fire = 5;    //Always 5 torpedos
	enemy_N_fire = 5;  // keep track of enemy torpedos
	nisland = initial_v[0][2];
	Player_2_health = 100;   // Initial health counter for both
	Player_health = 100;
	hit_points = 35;   // each torpedo hit cuases 35 damage
	coin_counter = 0;  // counter for the coins
	coins_number = 10;  
	player2_coin_counter = 0;////ADDED NEW 23/12


	create_sprite(background, iw);


	for (i = 0; i < total_players; i++) {                              // Initialises the ships

		if (initial_v[i][1] == 1) { ship_draw = "dest.gif"; }       // could change the players ships
		else if (initial_v[i][1] == 2) { ship_draw = "boat.png"; }
		else if (initial_v[i][1] == 3) { ship_draw = "sail.png"; }
		else if (initial_v[i][1] == 4) { ship_draw = "Spaceship.png"; }
		else if (initial_v[i][1] == 5) { ship_draw = "dest5.png"; }
		else if (initial_v[i][1] == 6) { ship_draw = "AC.png"; }
		

		S[i + 1] = new ship(X, Y, Q, ship_draw);

		if (S[i + 1] == NULL) {
			cout << "\nerror memory allocation world for boat";
		}
	}


	double xia, yia, xib, yib, xia2, yia2, d;     // Initialisation of the islands, number of islands used is a user input.

	double ri2 = 130;                   // Double the radius of the island, used to check for overlap

	for (int i = 1; i <= nisland; i++) {
		long int seed = -5 + i;
		xia = ran(seed) * width;                    //Using random values to initialise the islands
		yia = ran(seed) * height;
		I[i] = new island(xia, yia, island_pic);
	}

	for (int i = 1; i <= nisland; i++) {
		if (I[i] == NULL) {
			cout << "\nerror memory allocation world islands";
		}
	}


	for(int i = 1; i < nisland; i++){      // Checks to amke sure the islands do not over lap with each other, checks each island with the other
	for (int j = 1; j < nisland; j++) {
		if ((i + j) <= nisland) {
			xib = I[i]->xi;
			yib = I[i]->yi;
			xia2 = I[i + j]->xi;
			yia2 = I[i + j]->yi;
		
		d = sqrt(((xib - xia2) * (xib - xia2)) + ((yib - yia2) * (yib - yia2)));

		if (d <= ri2) { // 65 is the radius of the island, so 130 is the minimum distance betwwen two islands

			if (xib > xia2) {
				I[j]->xi += 30;                         //checking to make sure the islands do not overlap
				if (I[j]->xi > width) {
					I[j]->xi = 780;
				}
			}
			if (xib < xia2) {            
				I[j]->xi -= 30;
				if (I[j]->xi < 0) {
					I[j]->xi = 30;
				}
			}
			if (yib > yia2) {
				I[j]->yi += 30;
				if (I[j]->xi > height) {
					I[j]->xi = 580;
				}
			}
			if (yib < yia2) {
				I[j]->yi -= 30;
				if (I[j]->yi < 0) {
					I[j]->yi = 30;
				}
			}
		}
		}
	}
}	
double xss, yss, xss2, yss2,ds,d2s,xi1,yi1;    //avoid island on ships, similiar to checking for island overlap

	xss = S[1]->x;   
	yss = S[1]->y; 

	if (total_players == 2) {		//Will check against both ships in multiplayer
		xss2 = S[2]->x;
		yss2 = S[2]->y;
	}


	for (int k = 1; k <= nisland; k++) {
		xi1 = I[k]->xi;
		yi1 = I[k]->yi;

		ds = sqrt(((xi1 - xss) * (xi1 - xss)) + ((yi1 - yss) * (yi1 - yss)));

		if (ds <= 165) {
			if (xi1 < xss) {
				I[k]->xi -= 65;
			}
			if (xi1 > xss) {
				I[k]->xi += 65;
			}
			if (yi1 < yss) {
				I[k]->xi -= 65;
			}
			if (yi1 > yss) {
				I[k]->xi += 65;
			}
		}

		if (total_players == 2) {

			d2s = sqrt(((xi1 - xss2) * (xi1 - xss2)) + ((yi1 - yss2) * (yi1 - yss2)));


			if (d2s <= 165) {
				if (xi1 < xss2) {
					I[k]->xi -= 65;
				}
				if (xi1 > xss2) {
					I[k]->xi += 65;
				}
				if (yi1 < yss2) {
					I[k]->xi -= 65;
				}
				if (yi1 > yss2) {
					I[k]->xi += 65;
				}
			}

		}

	}


   double ric = 77.5; //Initialises the coins
	double dic;
	for (int i = 1; i <= coins_number; i++) {

		long int seed = (long int)-2+i;
		double xc, yc, xi,yi;


		for (int j = 1; j <= nisland; j++) {  //Checks to make sure coins dont appear on the islands
			
			xi = I[j]->xi;
			yi = I[j]->yi;

			xc = ran2(seed) * width; // random value with for loop as to not get the same values over and over
			yc = ran2(seed) * height; 

			C[i] = new Coin(xc, yc);

			if (C[i] == NULL) {
				cout << "\nerror memory allocation world for coin";
			}

			dic = sqrt(((xc - xi) * (xc - xi)) + ((yc - yi) * (yc - yi)));

			if (dic <= ric) {

				if (xc < xi) {
					C[i]->xc -= 80;        //checking for overlap
				}
				if (xc > xi) {
					C[i]->xc += 80;
				}
				if (yc < yi) {
					C[i]->yc -= 80;
				}
				if (yc > yi) {
					C[i]->yc += 80;
				}

			}

		}
	}

	
	for (int i = 0; i <= N_fire; i++) {      // initialises torpedo, first initialises to follow the ships

		T[i] = new torpedo(X, Y, torp);

		if (T[i] == NULL) {
			cout << "\nerror memory allocation world-torpedo";
		}
	}
	
	if (total_players == 2) {  // player two torpedos, similar process

		for (int i = 0; i <= N_fire; i++) {

			enemy_T[i] = new torpedo(S[2]->x, S[2]->y, torp);

			if (enemy_T[i] == NULL) {
				cout << "\nerror memory allocation world-enemy-torpedo";
				enemy_T[i]->qt = S[2]->q;
			}
		}
	}
	
	return;
}

World::~World() { // Destructor for the World

	
	for (int i = 1; i <= nisland; i++) {
		if (I[i] == NULL) {
			cout << "\nerror deleting world islands";
		}
		else {
			delete I[i];
		}
	}

	
	for (int i = 1; i <= coins_number; i++) {
		if (C[i] == NULL) {
			cout << "\nerror deleting ~World coins";
		}
		else {
				delete C[i];
		}
	}
	
	if (N_fire != 0) {
		for (int i = 1; i <= N_fire; i++) {
			if (T[i] == NULL) {
				cout << "\nerror deleting error ~World torpedo ";
			}
			else {
				delete T[i];
			}
		}
	}
	
	if( total_players == 2 ){
		if (enemy_N_fire != 0) {
			for (int i = 1; i <= enemy_N_fire; i++) {
				if (enemy_T[i] == NULL) {
					cout << "\nerror deleting error ~World enemy torpedo ";
				}
				else {
					delete enemy_T[i];
				}
			}
		}
	}
	
		for (int i = 1; i <= total_players; i++) {

			if (S[i] == NULL) { 
				cout << "\nerror deleting error ~World ";
			}

			else {
				delete S[i];
			}
		}
}


void World::draw() { // Drwa function for the world

    double x, y, q, scalew;
	x = y = q = 0.0;
	scalew = -1.0;
	
	draw_sprite(iw, x, y, q, scalew);

	for (int i = 1; i <= total_players; i++) {
		S[i]->draw();

	}

	for (int i = 1; i <= nisland; i++) {
		I[i]->draw();
	
	}
	
	
	for (int i = 1; i <= coins_number; i++) {

		C[i]->draw();
	}
		

	if (N_fire != 0) {     //Only draws the torpedos if there are more than 0 torpedos, and if fx isnt 0, 
		if (T[N_fire]->fx != 0.0) {   // Non zero fx value only possible if the torpedo has been fired.
			T[N_fire]->draw();
		}
	}
	if (total_players == 2) {       //Only draws torpedo if in multiplayer, and if fx is non zero
		if (enemy_T[enemy_N_fire]->fx != 0.0) {
			enemy_T[enemy_N_fire]->draw();
		}
	}

	return;
}

void World::sim_step(double dt) { // Sim step function
	
	S[1]->sim_step(dt);       

		
	if (N_fire != 0) {      // Simulates the torpedo only if it has been fired
		T[N_fire]->sim_step(dt);
		
	}
	if (S[1]->x <= 0.0) {   // checks to see if the player goes out of the screen, and corrects
		S[1]->x = 0.0 + 10;
		S[1]->v *= 0.25;
	}
	if (S[1]->y <= 0.0) {
		S[1]->y = 0.0+10;
		S[1]->v *= 0.25;
	}
	if (S[1]->x >= width)
	{
		S[1]->x = width-10;
		S[1]->v *= 0.25;
	}
	if (S[1]->y >= height)
	{
		S[1]->y = height-10;
		S[1]->v *= 0.25;
	}


	if (total_players == 2) {    // Simulates the second player's torpedos, only if it has been fired and only if in multiplayer mode.
		if (enemy_N_fire != 0) {

			if (enemy_T[enemy_N_fire]->fx != 0.0) {
				enemy_T[enemy_N_fire]->vel(dt);
				enemy_T[enemy_N_fire]->sim_step(dt);
		
			}
			else {
				enemy_T[enemy_N_fire]->qt = S[2]->q;   // if not fired, the torpedo follows the enemy ship
				enemy_T[enemy_N_fire]->xt = S[2]->x;
				enemy_T[enemy_N_fire]->yt = S[2]->y;
			}
		}
	}

	S[1]->vel(dt);   // Caling the velocity function for ship and torpedo
	
	if (N_fire != 0) {
		T[N_fire]->vel(dt);
	}

}

void World::input() {

	double fx = 0.0 , Q = 0.0, dwe = 0.0;

	
	for (int i = 1; i <= N_fire; i++) {     // If not fired, torpedo follows the ship X,Y and Q
		if (T[i]->fx == 0.0) {
			T[i]->xt = S[1]->x;
			T[i]->yt = S[1]->y;
			T[i]->qt = S[1]->q;
		}
	}
	
	if (KEY('I')) {
		fx += 8.113e8; //throttle up
		dwe += 60;     //engine rpm up
	}
	if (KEY('K')) {
		fx -= 8.113e8; //throttle down
		dwe -= 60;     //engine rpm down
	}
	if (KEY('J')) {
		Q += 0.01;   // directly controls angle
	}
	if (KEY('L')) {
		Q -= 0.01;
	}


	if (KEY('T')) { //To fire the torpedo
		if (T[N_fire]->fx != 0.0 || T[N_fire] == NULL || N_fire == 0) {
			cout << "\ntorpedo empty";
			return;
		}
		else {
			T[N_fire]->fx = 3.713e10;
			PlaySoundA("Torpedo_Launching.wav", NULL, SND_ASYNC);
			cout << "\ntorpedo" << N_fire << "fired\n";
			return;
		}
	}
	
	S[1]->fx += fx;
	S[1]->we += dwe;
	S[1]->q += Q;


}

void World::ship_speed() {// Function displays stats for player and enemy
double yt, sc, xs;
	yt = 550;
	xs = 600;
	sc = 0.5;

	double nfire, enfire;
	nfire = (double)N_fire;
	enfire = (double)enemy_N_fire;
	double count;
	count = (double)coin_counter;
	double nh, ph;
	nh = (double)Player_2_health;
	ph = (double)Player_health;

	S[1]->speed();

	text("LOCAL", width-1490, height-10, sc);
	text("Torpedos left", width - 1490, height -40, sc);
	text(nfire, width - 1290, height - 40, sc);
	text("Coins collected", width - 1490, height - 70, sc);
	text(count, width - 1290, height - 70, sc);
	text("Health", width - 1490, height - 100, sc);
	text(ph, width - 1290, height - 100, sc);
	text("Timer",width-760,height-10,sc);
	
	if (total_players == 2) {
		text("ENEMY", width - 250, height - 10, sc);
		text("Torpedos left", width - 250, height - 40, sc);
		text(enfire, width - 50, height - 40, sc);
		text("Health", width - 250, height - 70, sc);
		text(nh, width - 50, height - 70, sc);
	}
	
}


void World::replenish_coins(int i) {// This function spawns a new coin if a coin has been collected by a player

	long int seed = -2 + i;
	double xc, yc, dic;
	double xi, yi;
	double ric = 77.5;  // Radius of the island + coins, 

	xc = ran3(seed) * width; // random value with for loop as to not get the same values over and over
	yc = ran3(seed) * height; 

	double xs, ys,d2;
	xs = S[1]->x;
	ys= S[1]->y;

	C[i] = new Coin(xc, yc);   // initialises the coins similar to how it was first initialises

	if (C[i] == NULL) {
		cout << "\nerror memory allocation world for coin";
	}

	for (int j = 1; j <= nisland; j++) {

		xi = I[j]->xi;       
		yi = I[j]->yi;

		dic = sqrt(((xc - xi) * (xc - xi)) + ((yc - yi) * (yc - yi)));

		d2 = sqrt(((xc - xs) * (xc - xs)) + ((yc - ys) * (yc - ys)));  // checks for distance to ship, makes sure coin doesnt spawn on ship.

		if (d2 <= 100) {
			
			C[i]->xc = ran3(seed) * width;
			C[i]->yc= ran3(seed) * height;
			
		}

		if (dic <= ric) {
			
			C[i]->xc = ran3(seed) * width;
			C[i]->yc = ran3(seed) * height;
			
		}
	}
}