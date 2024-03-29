#pragma once

#include <SFML/Graphics.hpp>

#include "Configuration.hpp"


class Camera
{
public:
    Camera(const Configuration& config, sf::RenderWindow& window) : config{ config }, window{ window }, zoom{ 1.0f }
    {
        Reset();
    }

    void Reset() {
        //view.setSize(static_cast<float>(window.getSize().x), static_cast<float>(window.getSize().y));
        view.setCenter(config.width / 2, config.height / 2);
        Update();
    }

    void Update() {
        window.setView(view);
    }

    void Resize(int width, int height) {
        view.setSize(width * zoom, height * zoom);
        //view.setCenter(view.getSize() * 0.5f);
        Update();
    }

    void Zoom(float factor) {
        zoom *= factor;
        view.zoom(factor);
        Update();
    }

    void SetZoom(float factor) {
        view.zoom(factor / zoom);
        zoom = factor;
        Update();
    }

    float GetZoom() {
        return zoom;
    }

    void Move(float offsetX, float offsetY) {
        view.move(offsetX, offsetY);
        Update();
    }

    void SetCenter(float x, float y) {
        view.setCenter(x, y);
        Update();
    }

    const sf::View& GetView() const {
        return view;
    }

    sf::Vector2f GetCenter() const {
        return view.getCenter();
    }

    sf::Vector2f GetSize() const {
        return view.getSize();
    }

private:
    const Configuration& config;
    sf::RenderWindow& window;
    sf::View view;

    float zoom;
};
