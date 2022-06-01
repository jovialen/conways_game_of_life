#include <algorithm>
#include <functional>
#include <vector>

#include <tex.hpp>

#define DEAD_CELL (tex::vec4<float>{ 0, 0, 0, 1 })

constexpr int TICKS_PER_SECOND = 10;

bool is_alive(tex::vec4<float> cell)
{
	return cell.r > 0 || cell.g > 0 || cell.b > 0;
}

tex::vec4<float> get_cell_color(tex::world &world, tex::vec2<int> position)
{
	tex::vec4<float> cell_color;
	cell_color.r = position.x / (float)tex::size(world).x;
	cell_color.g = position.y / (float)tex::size(world).y;
	cell_color.b = 1 - (cell_color.r / 2.0f + cell_color.g / 2.0f);
	return cell_color;
}

std::vector<bool> get_neighbours(tex::world &world, tex::vec2<int> position)
{
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
	return neighbours;
}

void tick(tex::world &world, tex::vec4<float> *back_buffer)
{
	tex::process(world, [&back_buffer](tex::world &world, tex::vec2<int> position) {
		bool cell_alive = is_alive(tex::get(world, position));

		// get cell neighbours
		auto neighbours = get_neighbours(world, position);
		int num_alive = std::count(neighbours.begin(), neighbours.end(), true);

		if (cell_alive)
		{
			// check if the cell has to many or to few neighbours to survive
			if (num_alive < 2 || num_alive > 3)
			{
				cell_alive = false;
			}
		}
		else
		{
			// check if the cell has enough neighbours to become alive
			if (num_alive == 3)
			{
				cell_alive = true;
			}
		}

		// update buffer
		back_buffer[tex::backend::get_linear_index(world, position)] = cell_alive ? get_cell_color(world, position) : DEAD_CELL;
		return tex::get(world, position);
	});
}

void process_input(tex::world &world, bool *running)
{
	tex::vec2<int> hovered_cell = tex::get_hovered_cell(world);
	if (tex::in_bounds(world, hovered_cell))
	{
		if (tex::get_mouse_down(world, tex::mouse_button::LEFT))
		{
			auto cell_color = get_cell_color(world, hovered_cell);
			tex::set(world, hovered_cell, cell_color);
		}
		else if (tex::get_mouse_down(world, tex::mouse_button::RIGHT))
		{
			tex::set(world, hovered_cell, DEAD_CELL);
		}
	}

	if (tex::get_key_down(world, tex::key::SPACE))
	{
		*running = false;
	}
	else if (tex::get_key_down(world, tex::key::ENTER))
	{
		*running = true;
	}
}

int main()
{
	tex::world world("Conway's Game of Life");

	// create back buffer to draw on while working on a new frame
	size_t capacity = tex::size(world).x * tex::size(world).y;
	tex::vec4<float> *back = new tex::vec4<float>[capacity];

	// clear front and back buffers
	std::fill(tex::begin(world), tex::end(world), DEAD_CELL);
	std::fill(back,              back + capacity, DEAD_CELL);

	double timer = 0;
	bool running = false;
	while (tex::update(world))
	{
		process_input(world, &running);

		timer += tex::get_delta_time(world);
		if (timer >= 1.0 / (double) TICKS_PER_SECOND && running)
		{
			timer = 0;

			// simulate a tick
			tick(world, back);

			// swap fron and back buffers
			//tex::vec4<float> **front = &world.m.data;
			std::swap(world.m.data, back);
		}
	}

	delete[] back;

	return 0;
}

#ifdef WINDOWS_BUILD
#include <Windows.h>

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    PSTR lpCmdLine, INT nCmdShow)
{
    return main();
}
#endif
