#pragma once

#include "Configuration.hpp"
#include "Particles.hpp"


class ElasticConstraints {
public:
	ElasticConstraints(const Configuration& config, Particles& particles) : config{ config }, particles{ particles } {}
	void Update(float dt) {}

private:
	const Configuration& config;
	Particles& particles;
};
