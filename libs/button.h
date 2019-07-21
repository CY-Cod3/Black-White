#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include "SFML/Graphics.hpp"
#include "label.h"

class Button : public sf::Drawable {

    public:

        void create( const int posX , const int posY , const int sizeX , const int sizeY );
        void update( bool hover );

    private:

        virtual void draw( sf::RenderTarget& target , sf::RenderStates states ) const;

        sf::RectangleShape shape;
};

#endif // BUTTON_H_INCLUDED
