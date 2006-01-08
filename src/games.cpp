#include "games.h"

#include <QPoint>

Game::Game(){}
Game::~Game(){}

void Game::startGame()
{}

void Game::stopGame()
{}

//Field
void Field::moveStoneTo( Stone* stone, const QPoint& newPosition )
{
}

void Field::moveStoneTo( const QPoint& currentPosition, const QPoint& newPosition )
{
	Stone* s = stoneAtPosition( currentPosition );
	moveStoneTo( s, newPosition );
}

Stone* Field::stoneAtPosition( const QPoint& pos )
{
	Stone* stone;

	return stone;
}

//Crystalgame
CrystalGame::CrystalGame()
	: Game()
{
}
