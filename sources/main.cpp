#include <algorithm>
#include <functional>

#include <tex.hpp>

#define DEAD_CELL ((tex::vec4<float>) { 0, 0, 0, 1 })
#define LIVE_CELL ((tex::vec4<float>) { 1, 1, 1, 1 })

constexpr int TICKS_PER_SECOND = 2;

bool is_alive(tex::vec4<float> cell)
{
	return cell.r > 0 || cell.g > 0 || cell.b > 0;
}

int main(int argc, char *argv[])
{
	tex::world world("Conway's Game of Life");

	// create back buffer to draw on while working on a new frame
	size_t capacity = tex::size(world).x * tex::size(world).y;
	tex::vec4<float> *back = new tex::vec4<float>[capacity];

	// clear front and back buffers
	std::fill(tex::begin(world), tex::end(world), DEAD_CELL);
	std::fill(back,              back + capacity, DEAD_CELL);

	double timer = 0;
	while (tex::update(world))
	{
		timer += tex::get_delta_time(world);

		if (timer >= 1.0 / (double) TICKS_PER_SECOND)
		{
			timer = 0;

			// swap fron and back buffers
			tex::vec4<float> **front = &world.m.data;
			auto tmp = *front;
			*front = back;
			back = tmp;
		}
	}

	delete[] back;

	return 0;
}
