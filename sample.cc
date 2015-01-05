#include <cmath>
#include <limits>
#include <map>
#include <sstream>
#include <string>
#include <string>
#include <vector>
#include <vector>

std::string report( const std::vector<int> &easings, int W = 80, int H = 20, bool use_blur = true );

#include <iostream>
#include "tween.hpp"

int main( int argc, const char **argv ) {
    std::vector<int> type;
    if( argc <= 1 ) {
        type.push_back( tween::LINEAR );
    } else {
        for( int i = 1; i < argc; ++i ) {
            int t = std::strtol( argv[i], 0, 0 );
            while( t >=  tween::TOTAL ) t -= tween::TOTAL;
            while( t <= -tween::TOTAL ) t += tween::TOTAL;
            type.push_back( t );
        }        
    }
    std::cout << report( type ) << std::endl;
}




std::string report( const std::vector<int> &easings, int W, int H, bool use_blur ) {
    auto fn = [&]( float t01 ) -> double {
        float v = 1;
        for( auto &easing : easings ) {
            if( easing >= 0 ) {
                v *= tween::ease( easing, t01 ); //tween::inout( t01, tween::bouncein );
            }
            else
                v -= tween::ease( -easing, t01 );
        }
//      std::cout << v << ',';
        return v;
    };
    double max = 1; // fn(1);
    double min = 0; // fn(0);
    double avg = 0;
    double hits = 0;
    float lapse = 0;
    float current = 0;
    float now = 0;
    float then = 0;

    // init map

    const char blank = char( use_blur ? 1 : ' ' ), filler = char( use_blur ? 127 : '\xFE' );

    std::vector<std::string> l( H, std::string( W, blank ) );

    // draw map (downscaled)

    for( int x = 0; x < W; ++x )
    {
        // get Y
        double y = fn( x / double(W - 1) );

        // convert y to delta
        if( max - min > 0 )
        y = ( y - min ) / ( max - min );
        else
        y = 0;

        // rescale
        y *= (H - 1);

        if( unsigned(x) < W && unsigned(y) < H )
        l[ int(y) ][ x ] = filler;
    }

    // blur map

    if( use_blur )
    {
        std::vector<std::string> l_blur( H );

        l_blur = l;

        for( int y = 0; y < H; ++y )
        for( int x = 0; x < W; ++x )
        {
            double value = 0;
            int count = 0;

#           define blur( IX, IY ) if( x + (IX) >= 0 && x + (IX) < W && y + (IY) >= 0 && y + (IY) < H ) value += l[ y + (IY) ][ x + (IX) ], ++count;

            blur( -1, -1 );
            blur( -1,  0 );
            blur( -1, +1 );

            blur(  0, -1 );
            //blur(  0,  0 );
            blur(  0, +1 );

            blur( +1, -1 );
            blur( +1,  0 );
            blur( +1, +1 );

            value = ( value / count ) / ( 127.0 ); /* delta 0..1 */

#           undef blur

            // map blur into ascii art
            std::string brightness = " \xFA,+*$##";  // " \xFA"
            const double gamma = 2.25;               // 2.00

            double bright = std::pow( value, 1.0/gamma );

            bright = ( bright < 0 ? 0 : ( bright > 1 ? 1 : bright ));

            l_blur[ y ][ x ] = brightness.at( bright * brightness.size() );
        }

        l = l_blur;
    }

    // insert guides

    for( int x = 0; x < W; ++x )
    {
        if( l[0][x] == ' ' ) //blank )
            l[0][x] = '-';

        if( l[H - 1][x] == ' ' ) //blank )
            l[H - 1][x] = '-';
    }

    // print map (upside down)

    std::stringstream header, body, footer;

    for( auto easing : easings ) {
        if( easing >= 0 )
            header << "* " << tween::nameof( easing );
        else
            header << "- " << tween::nameof( -easing );
    }

    for( int y = H - 1; y >= 0; --y )
    {
        body << l[y] << " " << ( y == 0 /*|| y == H - 1*/ ? "(v) " : "    " ) << int( min + ( y / double(H - 1) ) * ( max - min )) << std::endl;
    }

    {
        char fmt[512];
        sprintf(fmt, "%s%*s (t)", std::to_string(then-then).c_str(), W - std::to_string(then-then).size(), std::to_string(now-then).c_str() );
        footer << fmt << std::endl;
    }

    footer << "Min: " << min << "; "
           << "Max: " << max << "; "
           << "Avg: " << avg << "; "
           << "Hit: " << hits << "; "
           << "Lapse: " << lapse << "s"  << "; "
           << "Avg Hits/sec: " << ( lapse > 0 ? hits / lapse : 0 ) << "; "
           << "Cur: " << current << std::endl;

    return (header.str().empty() ? header.str() : header.str().substr( 2 )) + "\n" + body.str() + footer.str();
}
