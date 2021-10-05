
class ship {    // Class for the ship

public:
	double s_height, s_width;
	double x, y, q, v, vmax, wmax, vmin, d;//Ship is limited to a speed of vmax
	double rho, s, cf, m, fx; //density water, wetted surface area,frictional coeff, mass, forward force respectively
	double we; //engine rpm, only for display
	int id;
	double scale;
	double lenght;//
	double width; // Ship PNGs chosen haven similar dimensions
	int Ncircles; // 
	double* xc;
	double* yc;// centroids for the circle collider 
	ship(double x, double y, double q, char* file_name);
	void draw();
	void sim_step(double dt);
	void speed();
	void vel(double dt);
	void collider();

};

class torpedo{ /// Class for the torpedo, the rho, s,cf ect. values are similar to those used in the ship class
public:
	double xt, yt, vt, qt;
	int it;
	double scale_t;
	double mass,vmaxt;
	double rho, s, cf, fx;
	
	torpedo(double xt, double yt, char* file_namet);
	void draw();
	void sim_step(double dt);
	void vel(double dt);
};



class island {   /// Island class

public:
	double xi, yi, qi, scalei, Ri;
	int id;

	island(double xi, double yi, char *file_name);
	void draw();

};


class Coin {     //Coin class

public:
	int idc;
	double xc, yc, scalec;

	Coin(double xc, double yc);
	void draw();

};

class World {   // World class

public:
	double height, width;   // height and width of the screen
	int total_players;
	char* ship_draw;
	double X, Y, Q, fx;
	int local_hit_marker = 0;
	int enemy_hit_marker = 0;
	int enemy_N_fire;
	int N_fire;
	int iw, il;
	int coins_number;
	int coin_counter;
	int player2_coin_counter;
	int Player_health;
	int Player_2_health;
	int hit_points;
	int nisland;

	ship* S[2 + 1]; 
	island* I[4 + 1];
	torpedo* T[5 + 1];
	torpedo* enemy_T[5 + 1];
	Coin* C[10+1];
	
	


	World(double X, double Y, double Q, char *background, char *island_pic, int initial_v[2][3], char *torp);
	~World();
	void draw();
	void sim_step(double dt);
	void input();
	void ship_speed();
	void collision();

	void collision_torpedo();
	void enemy_collision_torpedo();
	void remove_torpedo(int i);
	void collision_coin();
	void remove_coins(int i);
	void collision_torpedo_ship();
	void remove_enemy_torpedo(int i);
	void enemy_torpedo_collision();
	void replenish_coins(int i);
};
