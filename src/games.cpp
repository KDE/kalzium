#include "games.h"

#include <QPoint>

#include <kdebug.h>

#include <math.h>

///Stone
Stone::Stone( PLAYER player, const QPoint& point )
{
	m_player = player;
	m_position = point;
}

void Stone::swap()
{
	if ( m_player == Stone::White )
		m_player = Stone::Black;
	else
		m_player = Stone::White;
}

///Field
Field::Field( int x, int y )
{}

Field::~Field()
{}

Game::Game(){}
Game::~Game(){}

void Game::startGame()
{}

void Game::stopGame()
{}

//RAField
RAGame::RAField::RAField( int x, int y )
	: Field( x, y )
{
	m_size_x = x;
	m_size_y = y;
}

void RAGame::RAField::moveStoneTo( Stone* stone, const QPoint& newPosition )
{
}

void RAGame::RAField::addStone( Stone* stone )
{
	m_stones.append( stone );
}

void RAGame::rollDices()
{
	//generating two random numbers
	//TODO make it a bit more random ;-)
	const int x = ( int ) ( random()*5+1 );
	const int y = ( int ) ( random()*5+1 );

	QPoint point( x, y );
	
	Stone* stone = m_field->stoneAtPosition( point );

	//increase if both dices have the same value. This
	//simulates the halflifetime
	if ( x == y )
		m_counter++;

	if ( !stone )
		return;

	//if there is a white stone make it black
	if ( stone->player() == Stone::White )
		stone->swap();
}

//RAgame
RAGame::RAGame()
	: Game()
{
	kdDebug() << "Game()" << endl;
	
	m_field = new RAField( 6,6 );

	m_counter = 0;

	//fill the field with 6x6 white stones
	for ( int x = 0 ; x < 6 ; ++x )
	{
		for ( int y = 0; y < 6 ; ++y )
		{
			m_field->addStone( new Stone( Stone::White, QPoint( x, y ) ) );
			kdDebug() << "Creating " << x << ", " << y << endl;
		}
	}
	
	//now lets play 100 times!
	while ( m_counter < 100 )
	{
		rollDices();
	}
	
	QString ds = QString();
	for ( int x = 0; x < 6 ; ++x )
	{
		ds = QString();
		for ( int y = 0; y < 6 ; ++y )
		{
			Stone* s = m_field->stoneAtPosition( QPoint( x,y ) );
			if ( s->player() == Stone::White )
				ds += "W";
			else
				ds += "B";
			
			kdDebug() << "testing " << x << ", " << y << endl;
		}
		kdDebug() << ds << endl;
	}
}

Stone* Field::stoneAtPosition( const QPoint& pos )
{
	Stone* stone;

	foreach( Stone* s, m_stones )
	{
		if ( s->position() == pos )
			return s;
	}

	return 0;
}

#include "games.moc"
