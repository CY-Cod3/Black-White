#include "../label.h"

void Label::create( const int posX , const int posY , const unsigned int Csize , const std::string& title , const sf::Font& font ) {

    Label::text.setCharacterSize( Csize );
    Label::text.setPosition( sf::Vector2f( posX , posY ) );
    Label::text.setColor( sf::Color::White );
    Label::text.setString( title );
    Label::text.setFont( font );

}

void Label::draw( sf::RenderTarget& target , sf::RenderStates states ) const {

    target.draw(Label::text , states);
}
