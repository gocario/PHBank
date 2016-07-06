#pragma once
/**
 * @file view/savexiter.hpp
 * @author Gocario
 */

#include "view/scene.hpp"

class Savexiter : public Scene
{
	public:
		Savexiter(Scene* parent = NULL);
		Savexiter(SceneType sType, Scene* parent = NULL);
		~Savexiter();

		Result initialize() override;
		Result drawTopScreen() override;
		Result drawBotScreen() override;
		Result update() override;
};

 // SAVEXITER_HPP
