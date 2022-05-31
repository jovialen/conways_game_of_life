#include <functional>

#include <tex.hpp>

int main(int argc, char *argv[])
{
	tex::world world("Conway's Game of Life");

	std::fill(tex::begin(world), tex::end(world), (tex::vec4<float>) { 0, 0, 0, 1 });

	while (tex::update(world))
	{

	}

	return 0;
}
