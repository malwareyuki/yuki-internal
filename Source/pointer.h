class globals {
public:
	static inline void set_world_instance(uobject* world) {
		worldInstance = world;
	}

	static inline uobject* get_world_instance() {
		return worldInstance;
	}

	static inline void set_players(std::vector<actor*> players_obj) {
		players = players_obj;
	}

	static inline std::vector<actor*> get_players() {
		return players;
	}

	static inline void set_canvas(canvas* canvas_obj) {
		canvas = canvas_obj;
	}

	static inline canvas* get_canvas() {
		return canvas;
	}

	static inline void set_player_controller(controller* controller_obj) {
		controller = controller_obj;
	}

	static inline controller* get_player_controller() {
		return controller;
	}

	static inline void set_local_pawn(actor* local_pawn_obj) {
		local_pawn = local_pawn_obj;
	}

	static inline actor* get_local_pawn() {
		return local_pawn;
	}
private:
	static inline uobject* worldInstance;
	static inline std::vector<actor*> players;
	static inline canvas* canvas;
	static inline controller* controller;
	static inline actor* local_pawn;
};