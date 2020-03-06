#include "PlaneEnemy.hpp"
PlaneEnemy::PlaneEnemy(int x, int y) : Enemy("play/nerd2.png", x, y, 16, 50, 15, 15, (y-150 / 2)/150) {
	// Use bounding circle to detect collision is for simplicity, pixel-perfect collision can be implemented quite easily,
	// and efficiently if we use AABB collision detection first, and then pixel-perfect collision.
}
