/* Tween, a lightweight easing library. boost licensed.
 * - rlyeh.

 * Based on code by Robert Penner, GapJumper, Terry Schubring, Jesus Gollonet,
 * Tomas Cepeda, John Resig, lQuery team. Thanks guys! :-)
 */

#pragma once
#include <cmath>

namespace tween
{
    // public API 

    // basics f(t) = t ; memoized
    double linear( double dt01 );

    // penner's f(t) = t^2 ; memoized
    double quadin( double dt01 );
    double quadout( double dt01 );
    double quadinout( double dt01 );

    // penner's f(t) = t^3 ; memoized
    double cubicin( double dt01 );
    double cubicout( double dt01 );
    double cubicinout( double dt01 );

    // penner's f(t) = t^4 ; memoized
    double quartin( double dt01 );
    double quartout( double dt01 );
    double quartinout( double dt01 );

    // penner's f(t) = t^5 ; memoized
    double quintin( double dt01 );
    double quintout( double dt01 );
    double quintinout( double dt01 );

    // penner's f(t) = sin(t) ; memoized
    double sinein( double dt01 );
    double sineout( double dt01 );
    double sineinout( double dt01 );

    // penner's f(t) = 2^t ; memoized
    double expoin( double dt01 );
    double expoout( double dt01 );
    double expoinout( double dt01 );

    // penner's f(t) = sqrt(1-t^2) ; memoized
    double circin( double dt01 );
    double circout( double dt01 );
    double circinout( double dt01 );

    // penner's exponentially decaying sine wave ; memoized
    double elasticin( double dt01 );
    double elasticout( double dt01 );
    double elasticinout( double dt01 );

    // penner's overshooting cubic easing f(t) = (s+1)*t^3 - s*t^2 ; memoized
    double backin( double dt01 );
    double backout( double dt01 );
    double backinout( double dt01 );

    // penner's exponentially decaying parabolic bounce ; memoized
    double bouncein( double dt01 );
    double bounceout( double dt01 );
    double bounceinout( double dt01 );

    // gapjumper's ; memoized
    double sinesquare( double dt01 );
    double exponential( double dt01 );

    // terry schubring's ; memoized
    double terrys1( double dt01 );
    double terrys2( double dt01 );
    double terrys3( double dt01 );

    // lQuery's and tomas cepeda's ; memoized
    double swing( double dt01 );
    double sinpi2( double dt01 );

    // modifiers
    double ping( double dt01 );
    double pong( double dt01 );
    double pingpong( double dt01 );
    double pongping( double dt01 );

    template<typename EASE> double in( double dt01, EASE &ease );
    template<typename EASE> double out( double dt01, EASE &ease );
    template<typename EASE> double inout( double dt01, EASE &ease );

    // generics
    double ease( int type, double dt01, bool memoized = false );
    const char *nameof( int type );

    enum TYPE
    {
        LINEAR,

        QUADIN,          // t^2
        QUADOUT,
        QUADINOUT,
        CUBICIN,         // t^3
        CUBICOUT,
        CUBICINOUT,
        QUARTIN,         // t^4
        QUARTOUT,
        QUARTINOUT,
        QUINTIN,         // t^5
        QUINTOUT,
        QUINTINOUT,
        SINEIN,          // sin(t)
        SINEOUT,
        SINEINOUT,
        EXPOIN,          // 2^t
        EXPOOUT,
        EXPOINOUT,
        CIRCIN,          // sqrt(1-t^2)
        CIRCOUT,
        CIRCINOUT,
        ELASTICIN,       // exponentially decaying sine wave
        ELASTICOUT,
        ELASTICINOUT,
        BACKIN,          // overshooting cubic easing: (s+1)*t^3 - s*t^2
        BACKOUT,
        BACKINOUT,
        BOUNCEIN,        // exponentially decaying parabolic bounce
        BOUNCEOUT,
        BOUNCEINOUT,

        SINESQUARE,      // gapjumper's
        EXPONENTIAL,     // gapjumper's
        SCHUBRING1,      // terry schubring's formula 1
        SCHUBRING2,      // terry schubring's formula 2
        SCHUBRING3,      // terry schubring's formula 3

        SINPI2,          // tomas cepeda's
        SWING,           // tomas cepeda's & lquery's

        TOTAL,
        UNDEFINED
    };

    // }

    // implementation

    static inline //constexpr
    double ease( int easetype, double t, bool memoized )
    {
        enum { LUT_SLOTS = 256 }; 
        static float lut[TOTAL][LUT_SLOTS], *init = 0; 
        if( !init ) {
            init = &lut[0][0];
            for( int j = 0; j < TOTAL; ++j ) {
                for( int i = 0; i < LUT_SLOTS; ++i ) {
                    lut[j][i] = ease( j, double(i) / (LUT_SLOTS-1), false );
                }
            }
        }

        using namespace std;

        const double is_looped = 0;     /* used to be a param long time ago */
        const double d = 1.f;           /* used to be a param long time ago */ /* (d)estination, final time */
        const double pi = 3.1415926535897932384626433832795;

        /* tiny optimizations { */

        if( is_looped ) {
            if( t < 0 ) {
                t = -t;
            }
            // todo: optimize me?
            while( t > d ) {
                t -= d;
            }            
        } else {
            // clamp
            if( t < 0.f ) {
                t = 0;
            } 
            else
            if( t > d ) {
                t = d;
            }
        }

        if( memoized ) {
            return lut[ easetype ][ int(t*(LUT_SLOTS-1)) ];
        }

        /* } */

        switch( easetype )
        {
        case TYPE::LINEAR: {
                return t/d;
            }

        case TYPE::SWING: {
                return ((-cos(pi * t / d) / 2) + 0.5);            
            }

        case TYPE::SINPI2:
            {
                double fDelta = t/d;
                return sin(fDelta * 0.5f * pi);
            }

        case TYPE::BACKIN:
            {
                double s = 1.70158f;
                double postFix = t/=d;
                return postFix * t * ((s + 1) * t - s);
            }

        case TYPE::BACKOUT:
            {
                double s = 1.70158f;
                return 1.f * ((t = t/d-1)*t*((s+1)*t + s) + 1);
            }

        case TYPE::BACKINOUT:
            {
                double s = 1.70158f;
                if ((t/=d/2) < 1)
                    return 1.f/2*(t*t*(((s*=(1.525f))+1)*t - s));

                double postFix = t-=2;
                return 1.f/2*((postFix)*t*(((s*=(1.525f))+1)*t + s) + 2);
            }

#       define $BOUNCE(v) \
        if ((t/=d) < (1/2.75f)) { \
            v = 1.f*(7.5625f*t*t); \
            } \
        else if (t < (2/2.75f)) { \
            double postFix = t-=(1.5f/2.75f); \
            v = 1.f*(7.5625f*(postFix)*t + .75f); \
            } \
        else if (t < (2.5/2.75)) { \
            double postFix = t-=(2.25f/2.75f); \
            v = 1.f*(7.5625f*(postFix)*t + .9375f); \
            } \
        else { \
            double postFix = t-=(2.625f/2.75f); \
            v = 1.f*(7.5625f*(postFix)*t + .984375f); \
            }

        case TYPE::BOUNCEIN:
            {
                double v;
                t = d-t;
                $BOUNCE(v);
                return 1.f - v;
            }

        case TYPE::BOUNCEOUT:
            {
                double v;
                $BOUNCE(v);
                return v;
            }

        case TYPE::BOUNCEINOUT:
            {
                double v;
                if (t < d/2) {
                    t = t*2;
                    t = d-t;
                    $BOUNCE(v);
                    return (1.f - v) * .5f;
                } else {
                    t = t*2 -d;
                    $BOUNCE(v);
                    return v * .5f + 1.f*.5f;
                }
            }

#       undef $BOUNCE

        case TYPE::CIRCIN:
            t /= d;
            return 1.f - sqrt(1 - t*t);

        case TYPE::CIRCOUT:
            t /= d;
            t--;
            return sqrt(1 - t*t);

        case TYPE::CIRCINOUT:
            t /= d/2;
            if(t < 1)
                return -1.f/2 * (sqrt(1 - t*t) - 1);

            t-=2;
            return 1.f/2 * (sqrt(1 - t*t) + 1);


        case TYPE::ELASTICIN:
            {
                t/=d;

                double p=d*.3f;
                double a=1.f;
                double s=p/4;
                double postFix =a*pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators

                return -(postFix * sin((t*d-s)*(2*pi)/p ));
            }

        case TYPE::ELASTICOUT:
            {
                double p=d*.3f;
                double a=1.f;
                double s=p/4;

                return (a*pow(2,-10*t) * sin( (t*d-s)*(2*pi)/p ) + 1.f);
            }

        case TYPE::ELASTICINOUT:
            {
                t/=d/2;

                double p=d*(.3f*1.5f);
                double a=1.f;
                double s=p/4;

                if (t < 1) {
                    double postFix =a*pow(2,10*(t-=1)); // postIncrement is evil
                    return -.5f*(postFix* sin( (t*d-s)*(2*pi)/p ));
                }

                double postFix =  a*pow(2,-10*(t-=1)); // postIncrement is evil
                return postFix * sin( (t*d-s)*(2*pi)/p )*.5f + 1.f;
            }

        case TYPE::EXPOIN:
            return pow(2, 10 * (t/d - 1));

        case TYPE::EXPOOUT:
            return 1.f - ( t == d ? 0 : pow(2, -10.f * (t/d)));

        case TYPE::EXPOINOUT:
            t /= d/2;
            if (t < 1)
                return 1.f/2 * pow(2, 10 * (t - 1));

            t--;
            return 1.f/2 * (-pow(2, -10 * t) + 2);

        case TYPE::QUADIN:
            t /= d;
            return t*t;

        case TYPE::QUADOUT:
            t /= d;
            return (2.f - t) * t;

        case TYPE::QUADINOUT:
            t /= d/2;
            if(t < 1)
                return (1.f/2*t*t);
            t--;
            return -1.f/2 * (t*(t-2) - 1);

        case TYPE::CUBICIN:
            t /= d;
            return t*t*t;

        case TYPE::CUBICOUT:
            t /= d;
            t--;
            return (1.f + t*t*t);

        case TYPE::CUBICINOUT:
            t /= d/2;
            if (t < 1)
                return 1.f/2*t*t*t;
            t -= 2;
            return 1.f/2*(t*t*t + 2);

        case TYPE::QUARTIN:
            t /= d;
            return t*t*t*t;

        case TYPE::QUARTOUT:
            t /= d;
            t--;
            return (1.f - t*t*t*t);

        case TYPE::QUARTINOUT:
            t /= d/2;
            if(t < 1)
                return 1.f/2*t*t*t*t;
            t -= 2;
            return -1.f/2 * (t*t*t*t - 2);

        case TYPE::QUINTIN:
            t /= d;
            return t*t*t*t*t;

        case TYPE::QUINTOUT:
            t /= d;
            t--;
            return (1.f + t*t*t*t*t);

        case TYPE::QUINTINOUT:
            t /= d/2;
            if(t < 1)
                return 1.f/2*t*t*t*t*t;
            t -= 2;
            return 1.f/2*(t*t*t*t*t + 2);

        case TYPE::SINEIN:
            return 1.f - cos(t/d * (pi/2));

        case TYPE::SINEOUT:
            return sin(t/d * (pi/2));

        case TYPE::SINEINOUT:
            return -1.f/2 * (cos(pi*t/d) - 1);

        case TYPE::SINESQUARE: {
            double A = sin(0.5f*(t/d)*pi);
            return A*A;
        }

        case TYPE::EXPONENTIAL:
            return 1/(1+exp(6-12*(t/d)));

        case TYPE::SCHUBRING1:
            return t /= d, 2*(t+(0.5f-t)*abs(0.5f-t))-0.5f;

        case TYPE::SCHUBRING2:
            {
                t /= d;
                double p1pass= 2*(t+(0.5f-t)*abs(0.5f-t))-0.5f;
                double p2pass= 2*(p1pass+(0.5f-p1pass)*abs(0.5f-p1pass))-0.5f;
                double pAvg=(p1pass+p2pass)/2;
                return pAvg;
            }

        case TYPE::SCHUBRING3:
            {
                t /= d;
                double p1pass= 2*(t+(0.5f-t)*abs(0.5f-t))-0.5f;
                double p2pass= 2*(p1pass+(0.5f-p1pass)*abs(0.5f-p1pass))-0.5f;
                return p2pass;
            }

        default:
            return 0;
        }
    }

#   define $tween_xmacro(...) \
    $tween( undefined, UNDEFINED ) \
    \
    $tween( linear, LINEAR ) \
    $tween( quadin, QUADIN ) \
    $tween( quadout, QUADOUT ) \
    $tween( quadinout, QUADINOUT ) \
    $tween( cubicin, CUBICIN ) \
    $tween( cubicout, CUBICOUT ) \
    $tween( cubicinout, CUBICINOUT ) \
    $tween( quartin, QUARTIN ) \
    $tween( quartout, QUARTOUT ) \
    $tween( quartinout, QUARTINOUT ) \
    $tween( quintin, QUINTIN ) \
    $tween( quintout, QUINTOUT ) \
    $tween( quintinout, QUINTINOUT ) \
    $tween( sinein, SINEIN ) \
    $tween( sineout, SINEOUT ) \
    $tween( sineinout, SINEINOUT ) \
    $tween( expoin, EXPOIN ) \
    $tween( expoout, EXPOOUT ) \
    $tween( expoinout, EXPOINOUT ) \
    $tween( circin, CIRCIN ) \
    $tween( circout, CIRCOUT ) \
    $tween( circinout, CIRCINOUT ) \
    $tween( elasticin, ELASTICIN ) \
    $tween( elasticout, ELASTICOUT ) \
    $tween( elasticinout, ELASTICINOUT ) \
    $tween( backin, BACKIN ) \
    $tween( backout, BACKOUT ) \
    $tween( backinout, BACKINOUT ) \
    $tween( bouncein, BOUNCEIN ) \
    $tween( bounceout, BOUNCEOUT ) \
    $tween( bounceinout, BOUNCEINOUT ) \
    \
    $tween( sinesquare, SINESQUARE ) \
    $tween( exponential, EXPONENTIAL ) \
    \
    $tween( terrys1, SCHUBRING1 ) \
    $tween( terrys2, SCHUBRING2 ) \
    $tween( terrys3, SCHUBRING3 ) \
    \
    $tween( swing, SWING ) \
    $tween( sinpi2, SINPI2 ) 

    // interface for tweeners; includes memoization
#   define $tween(fn,type) \
    static inline double fn( double dt01 ) { \
        return tween::ease( tween::type, dt01 ); \
    }
    $tween_xmacro(expand functions)
#   undef $tween

    static inline
    const char *nameof( int type ) {
#   define $tween( unused, type ) case TYPE::type: return #type;
        switch(type) { 
            default:
            $tween_xmacro(expand cases)
        }
#   undef $tween
    }

    static inline
    double ping( double dt01 ) {
        return dt01;
    }
    static inline
    double pong( double dt01 ) {
        return 1 - dt01;
    }
    static inline
    double pingpong( double dt01 ) {
        return dt01 < 0.5f ? dt01 + dt01 : 2 - dt01 - dt01;
    }
    static inline
    double pongping( double dt01 ) {
        return 1 - (dt01 < 0.5f ? dt01 + dt01 : 2 - dt01 - dt01);
    }

    template<typename EASE>
    static inline double in( double dt01, EASE &ease ) { 
        return ease( dt01 );
    }
    template<typename EASE>
    static inline double out( double dt01, EASE &ease ) {
        return 1 - ease( 1 - dt01 );
    }
    template<typename EASE>
    static inline double inout( double dt01, EASE &ease ) { 
        return dt01 < 0.5f ? ease(dt01*2) * 0.5f : out(dt01*2-1, ease) * 0.5f + 0.5f;
    }
}
