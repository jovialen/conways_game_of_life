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

void tick(tex::world &world, tex::vec4<float> *back_buffer)
{
	tex::process(world, [&back_buffer](tex::world &world, tex::vec2<int> position) {
		bool cell_alive = is_alive(tex::get(world, position));

		// get cell neighbours
		std::vector<bool> neighbours;
		for (int x_off = -1; x_off <= 1; x_off++)
		{
			for (int y_off = -1; y_off <= 1; y_off++)
			{
				if (x_off == 0 && y_off == 0) continue;

				tex::vec2<int> neighbour_cell = position;
				neighbour_cell.x += x_off;
				neighbour_cell.y += y_off;

				if (tex::in_bounds(world, neighbour_cell))
				{
					neighbours.push_back(is_alive(tex::get(world, neighbour_cell)));
				}
			}
		}

		int num_alive = std::count(neighbours.begin(), neighbours.end(), true);

		if (cell_alive)
		{
			if (num_alive < 2 || num_alive > 3)
			{
				cell_alive = false;
			}
		}
		else
		{
			if (num_alive == 3)
			{
				cell_alive = true;
			}
		}

		back_buffer[tex::backend::get_linear_index(world, position)] = cell_alive ? LIVE_CELL : DEAD_CELL;
		return tex::get(world, position);
	});
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

	tex::set(world, (tex::vec2<int>) { 2, 1 }, LIVE_CELL);
	tex::set(world, (tex::vec2<int>) { 2, 2 }, LIVE_CELL);
	tex::set(world, (tex::vec2<int>) { 2, 3 }, LIVE_CELL);

	double timer = 0;
	while (tex::update(world))
	{
		timer += tex::get_delta_time(world);

		if (timer >= 1.0 / (double) TICKS_PER_SECOND)
		{
			timer = 0;

			// simulate a tick
			tick(world, back);

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
