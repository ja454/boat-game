
#include <iostream>
#include <cstdio>
#include <cmath>
#include <Windows.h>
#include "2D_graphics.h"
#include "timer.h"
#include"ran.h"
#include "Objects.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

using namespace std;

void World::collision() { // Checks for collisions between ship and island

	double r2;
	double rship = (S[1]->width) / 2;
	int j;
	double d;

	S[1]->collider();  //Calling collider
	int Nc;
	Nc = S[1]->Ncircles;

	r2 = 65; // constant radius for all islands

	double xc, yc, xi, yi;

	for (int i = 1; i <= Nc; i++) {  // checks each circle in the collider against the island
		xc = S[1]->xc[i];
		yc = S[1]->yc[i];

		for (j = 1; j <= nisland; j++) {

			xi = I[j]->xi;
			yi = I[j]->yi;

			d = sqrt(((xc - xi) * (xc - xi)) + ((yc - yi) * (yc - yi)));

			if (d <= (rship + r2)) {
				S[1]->v *= 0.25;
				PlaySoundA("Land_Collision.wav", NULL, SND_ASYNC);
			}
		}

	}
}

void World::remove_torpedo(int i) {   // removes torpedo 

	if (T[i] == NULL) {
		cout << "\nerror delete collision: remove torpedo";
		return;
	}
	else {
		delete T[i];
	}
}

void World::remove_enemy_torpedo(int i) { //removes enemy torpedo

		if (enemy_T[i] == NULL) {
			cout << "\nerror delete collision: remove torpedo";
			return;
		}
		else {
			delete enemy_T[i];
		}
}


void World::remove_coins(int i) {    //Removes coins and calls the replensih function

	if (C[i] == NULL) {
		cout << "\nerror delete collision remove coins  ";
		return;
	}
	else {
		PlaySoundA("Coins_Collected.wav", NULL, SND_ASYNC);
		delete C[i];
		replenish_coins(i);
	}

}


void World::collision_torpedo() {  // Function to check if torpedo goes out of bounds

	if (N_fire != 0) {

		if ((T[N_fire]->xt <= 0 || T[N_fire]->xt >= width || T[N_fire]->yt <= 0 || T[N_fire]->yt >= height) && T[N_fire]->fx != 0) {
			remove_torpedo(N_fire);
			N_fire--;
		}
	}
}

void World::enemy_collision_torpedo(){   
   

	if (enemy_N_fire != 0) { //checking for enemy torpedo if out of bounds
		if ((enemy_T[enemy_N_fire]->xt <= 0 || enemy_T[enemy_N_fire]->xt >= width || enemy_T[enemy_N_fire]->yt <= 0 || enemy_T[enemy_N_fire]->yt >= height) && enemy_T[enemy_N_fire]->fx != 0) {
			remove_enemy_torpedo(enemy_N_fire);
			enemy_N_fire--;
		}
	}
		if (enemy_hit_marker == 1) {  //removes torpedo, updates enemy fire counter and resets hit marker
			remove_enemy_torpedo(enemy_N_fire);
			enemy_N_fire--;
			enemy_hit_marker = 0;
			Player_health -= hit_points;  // Decreases the player health
		}
}


void World::collision_coin() {// checks if coins where collected 
	double d12;
	int Nc;
	double xs, ys, xcc, ycc;
	
	S[1]->collider();

	if (total_players > 1) {

		S[2]->collider();
	}


	Nc = S[1]->Ncircles;
	double rship = (S[1]->width) / 2;
	double rcoin = 25 / 2;
	

	for (int k = 1; k <= total_players; k++) {  // Calls collider and checks each circle for collisions against the coins
		for (int j = 1; j <= coins_number; j++) {
			xcc = C[j]->xc;
			ycc = C[j]->yc;

			for (int i = 1; i <= Nc; i++) {
				xs = S[k]->xc[i];
				ys = S[k]->yc[i];

				d12 = sqrt(((xs - xcc) * (xs - xcc)) + ((ys - ycc) * (ys - ycc))); // distance between the collider circle and coin

				if (d12 <= rship + rcoin) {  // counter for player one
					remove_coins(j);
					if (k == 1) { 
					coin_counter++; 
					}
					if(k==2){
					player2_coin_counter++;  // Counter for the second player ADDED NEW 23/12
					}
				}
			}
		}
	}
		
}

void World::collision_torpedo_ship() { //Checks for collision  betwwen enemy ship and player torpedo
	
	S[2]->collider();
	double rtorpedo, d12;
	rtorpedo = 13.8; 
	int Nc = S[2]->Ncircles;
	double rship = (S[2]->width) / 2;

	
	double xt, yt, xc, yc;


		xt = T[N_fire]->xt;
		yt = T[N_fire]->yt;
		for (int j = 1; j <= Nc; j++) {
			xc = S[2]->xc[j];
			yc = S[2]->yc[j];

			d12 = sqrt(((xt - xc) * (xt - xc)) + ((yt - yc) * (yt - yc)));// distance between the collider circle and ctorpedo

		if(T[N_fire]->fx!=0){
			
			if (d12 <= rtorpedo + rship) {
				local_hit_marker = 1;  // if there is a hit, local marker is set to 1
				PlaySoundA("Boat_Projectile_Collision.wav",NULL,SND_ASYNC);
				
				remove_torpedo(N_fire);
				N_fire--;
				}
			}
			else{
				return;
			}

		}

	
}

