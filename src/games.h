#ifndef GAMES_H
#define GAMES_H

#include <QWidget>

/**
 * @author Carsten Niehaus
 * @brief A token to play a game
 */
class Stone : public QObject
{
	Q_OBJECT

	public:
		/** 
		 * Constructor
		 */
		Stone();
		
		enum PLAYER
		{
			Black = 0,
			White
		};
		
		/**
		 * Set the kind of player to @p player
		 */
		Stone( PLAYER player )
		{
			m_player = player;
		}

		/**
		 * set the player to @p player
		 */
		void setPlayer( PLAYER player )
		{
			m_player = player;
		}

		/**
		 * @return the player of this stone
		 */
		PLAYER player() const
		{
			return m_player;
		}

		/**
		 * @return the position of the stone
		 */
		QPoint position() const
		{
			return m_position;
		}

		/**
		 * set the position to @p point
		 */
		void setPosition( const QPoint& point )
		{
			m_position = point;
			emit moved( m_position );
		}

	private:
		PLAYER m_player;

		QPoint m_position;

	signals:
		void removed();
		void moved(QPoint);
};

/**
 * @author Carsten Niehaus
 * @brief The playground of a game
 */
class Field
{
	public:
		/**
		 * Constructor
		 *
		 * @param x The x-size of the field
		 * @param y The y-size of the field
		 */
		Field(int x, int y)
		{
			m_size_x = x;
			m_size_y = y;
		}

		/**
		 * @return the x-size of the field
		 */
		int xSize() const
		{
			return m_size_x;
		}
		
		/**
		 * @return the y-size of the field
		 */
		int ySize() const
		{
			return m_size_y;
		}
		
		/**
		 * Destructor
		 */
		~Field();

		/**
		 * @return the Stone at the position @p pos
		 */
		Stone* stoneAtPosition( const QPoint& pos );

		/**
		 * moves Stone @p stone from the current position to the @p newPosition
		 */
		virtual void moveStoneTo( Stone* stone, const QPoint& newPosition );
		
		/**
		 * moves the Stone in @p currentPosition to the @p newPosition
		 */
		virtual void moveStoneTo( const QPoint& currentPosition, const QPoint& newPosition );
		
		/**
		 * @return the stones of the game
		 */
		virtual QList<Stone*> stones() const
		{
			return m_stones;
		}
		
	private:
		int m_size_x;
		
		int m_size_y;
		
		QList<Stone*> m_stones;
};


/**
 * @author Carsten Niehaus
 * @brief Baseclass for all games
 */
class Game
{
 	public:
		/**
		 * Destructor
		 */
		virtual ~Game();

		/**
		 * starts the game
		 */
		virtual void startGame();
		
		/**
		 * stops the game
		 */
		virtual void stopGame();

		/**
		 * set the field to @p field
		 */
		void setField( Field* field )
		{
			m_field = field;
		}

	private:
		Field* m_field;
		
	protected:
		/**
		 * The constructor
		 */
		Game();
};

/**
 * @author Carsten Niehaus
 */
class CrystalGame : public Game
{
	public:
		CrystalGame();
};
#endif // GAMES_H
