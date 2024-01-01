#pragma once

#include "Configuration.hpp"
#include "Particles.hpp"


class FixPointConstraints {
public:
	FixPointConstraints(const Configuration& config, Particles& particles) : config{ config }, particles{ particles } {}
	void Update(float dt) {}

private:
	const Configuration& config;
	Particles& particles;
};
