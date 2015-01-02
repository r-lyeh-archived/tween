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
        LINEAR_01,

        QUADIN_01,          // t^2
        QUADOUT_01,
        QUADINOUT_01,
        CUBICIN_01,         // t^3
        CUBICOUT_01,
        CUBICINOUT_01,
        QUARTIN_01,         // t^4
        QUARTOUT_01,
        QUARTINOUT_01,
        QUINTIN_01,         // t^5
        QUINTOUT_01,
        QUINTINOUT_01,
        SINEIN_01,          // sin(t)
        SINEOUT_01,
        SINEINOUT_01,
        EXPOIN_01,          // 2^t
        EXPOOUT_01,
        EXPOINOUT_01,
        CIRCIN_01,          // sqrt(1-t^2)
        CIRCOUT_01,
        CIRCINOUT_01,
        ELASTICIN_01,       // exponentially decaying sine wave
        ELASTICOUT_01,
        ELASTICINOUT_01,
        BACKIN_01,          // overshooting cubic easing: (s+1)*t^3 - s*t^2
        BACKOUT_01,
        BACKINOUT_01,
        BOUNCEIN_01,        // exponentially decaying parabolic bounce
        BOUNCEOUT_01,
        BOUNCEINOUT_01,

        SINESQUARE_01,      // gapjumper's
        EXPONENTIAL_01,     // gapjumper's
        SCHUBRING1_01,      // terry schubring's formula 1
        SCHUBRING2_01,      // terry schubring's formula 2
        SCHUBRING3_01,      // terry schubring's formula 3

        SINPI2_01,          // tomas cepeda's
        SWING_01,           // tomas cepeda's & lquery's

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
        case TYPE::LINEAR_01: {
                return t/d;
            }

        case TYPE::SWING_01: {
                return ((-cos(pi * t / d) / 2) + 0.5);            
            }

        case TYPE::SINPI2_01:
            {
                double fDelta = t/d;
                return sin(fDelta * 0.5f * pi);
            }

        case TYPE::BACKIN_01:
            {
                double s = 1.70158f;
                double postFix = t/=d;
                return postFix * t * ((s + 1) * t - s);
            }

        case TYPE::BACKOUT_01:
            {
                double s = 1.70158f;
                return 1.f * ((t = t/d-1)*t*((s+1)*t + s) + 1);
            }

        case TYPE::BACKINOUT_01:
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

        case TYPE::BOUNCEIN_01:
            {
                double v;
                t = d-t;
                $BOUNCE(v);
                return 1.f - v;
            }

        case TYPE::BOUNCEOUT_01:
            {
                double v;
                $BOUNCE(v);
                return v;
            }

        case TYPE::BOUNCEINOUT_01:
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

        case TYPE::CIRCIN_01:
            t /= d;
            return 1.f - sqrt(1 - t*t);

        case TYPE::CIRCOUT_01:
            t /= d;
            t--;
            return sqrt(1 - t*t);

        case TYPE::CIRCINOUT_01:
            t /= d/2;
            if(t < 1)
                return -1.f/2 * (sqrt(1 - t*t) - 1);

            t-=2;
            return 1.f/2 * (sqrt(1 - t*t) + 1);


        case TYPE::ELASTICIN_01:
            {
                t/=d;

                double p=d*.3f;
                double a=1.f;
                double s=p/4;
                double postFix =a*pow(2,10*(t-=1)); // this is a fix, again, with post-increment operators

                return -(postFix * sin((t*d-s)*(2*pi)/p ));
            }

        case TYPE::ELASTICOUT_01:
            {
                double p=d*.3f;
                double a=1.f;
                double s=p/4;

                return (a*pow(2,-10*t) * sin( (t*d-s)*(2*pi)/p ) + 1.f);
            }

        case TYPE::ELASTICINOUT_01:
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

        case TYPE::EXPOIN_01:
            return pow(2, 10 * (t/d - 1));

        case TYPE::EXPOOUT_01:
            return 1.f - ( t == d ? 0 : pow(2, -10.f * (t/d)));

        case TYPE::EXPOINOUT_01:
            t /= d/2;
            if (t < 1)
                return 1.f/2 * pow(2, 10 * (t - 1));

            t--;
            return 1.f/2 * (-pow(2, -10 * t) + 2);

        case TYPE::QUADIN_01:
            t /= d;
            return t*t;

        case TYPE::QUADOUT_01:
            t /= d;
            return (2.f - t) * t;

        case TYPE::QUADINOUT_01:
            t /= d/2;
            if(t < 1)
                return (1.f/2*t*t);
            t--;
            return -1.f/2 * (t*(t-2) - 1);

        case TYPE::CUBICIN_01:
            t /= d;
            return t*t*t;

        case TYPE::CUBICOUT_01:
            t /= d;
            t--;
            return (1.f + t*t*t);

        case TYPE::CUBICINOUT_01:
            t /= d/2;
            if (t < 1)
                return 1.f/2*t*t*t;
            t -= 2;
            return 1.f/2*(t*t*t + 2);

        case TYPE::QUARTIN_01:
            t /= d;
            return t*t*t*t;

        case TYPE::QUARTOUT_01:
            t /= d;
            t--;
            return (1.f - t*t*t*t);

        case TYPE::QUARTINOUT_01:
            t /= d/2;
            if(t < 1)
                return 1.f/2*t*t*t*t;
            t -= 2;
            return -1.f/2 * (t*t*t*t - 2);

        case TYPE::QUINTIN_01:
            t /= d;
            return t*t*t*t*t;

        case TYPE::QUINTOUT_01:
            t /= d;
            t--;
            return (1.f + t*t*t*t*t);

        case TYPE::QUINTINOUT_01:
            t /= d/2;
            if(t < 1)
                return 1.f/2*t*t*t*t*t;
            t -= 2;
            return 1.f/2*(t*t*t*t*t + 2);

        case TYPE::SINEIN_01:
            return 1.f - cos(t/d * (pi/2));

        case TYPE::SINEOUT_01:
            return sin(t/d * (pi/2));

        case TYPE::SINEINOUT_01:
            return -1.f/2 * (cos(pi*t/d) - 1);

        case TYPE::SINESQUARE_01: {
            double A = sin(0.5f*(t/d)*pi);
            return A*A;
        }

        case TYPE::EXPONENTIAL_01:
            return 1/(1+exp(6-12*(t/d)));

        case TYPE::SCHUBRING1_01:
            return t /= d, 2*(t+(0.5f-t)*abs(0.5f-t))-0.5f;

        case TYPE::SCHUBRING2_01:
            {
                t /= d;
                double p1pass= 2*(t+(0.5f-t)*abs(0.5f-t))-0.5f;
                double p2pass= 2*(p1pass+(0.5f-p1pass)*abs(0.5f-p1pass))-0.5f;
                double pAvg=(p1pass+p2pass)/2;
                return pAvg;
            }

        case TYPE::SCHUBRING3_01:
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
    $tween( linear, LINEAR_01 ) \
    $tween( quadin, QUADIN_01 ) \
    $tween( quadout, QUADOUT_01 ) \
    $tween( quadinout, QUADINOUT_01 ) \
    $tween( cubicin, CUBICIN_01 ) \
    $tween( cubicout, CUBICOUT_01 ) \
    $tween( cubicinout, CUBICINOUT_01 ) \
    $tween( quartin, QUARTIN_01 ) \
    $tween( quartout, QUARTOUT_01 ) \
    $tween( quartinout, QUARTINOUT_01 ) \
    $tween( quintin, QUINTIN_01 ) \
    $tween( quintout, QUINTOUT_01 ) \
    $tween( quintinout, QUINTINOUT_01 ) \
    $tween( sinein, SINEIN_01 ) \
    $tween( sineout, SINEOUT_01 ) \
    $tween( sineinout, SINEINOUT_01 ) \
    $tween( expoin, EXPOIN_01 ) \
    $tween( expoout, EXPOOUT_01 ) \
    $tween( expoinout, EXPOINOUT_01 ) \
    $tween( circin, CIRCIN_01 ) \
    $tween( circout, CIRCOUT_01 ) \
    $tween( circinout, CIRCINOUT_01 ) \
    $tween( elasticin, ELASTICIN_01 ) \
    $tween( elasticout, ELASTICOUT_01 ) \
    $tween( elasticinout, ELASTICINOUT_01 ) \
    $tween( backin, BACKIN_01 ) \
    $tween( backout, BACKOUT_01 ) \
    $tween( backinout, BACKINOUT_01 ) \
    $tween( bouncein, BOUNCEIN_01 ) \
    $tween( bounceout, BOUNCEOUT_01 ) \
    $tween( bounceinout, BOUNCEINOUT_01 ) \
    \
    $tween( sinesquare, SINESQUARE_01 ) \
    $tween( exponential, EXPONENTIAL_01 ) \
    \
    $tween( terrys1, SCHUBRING1_01 ) \
    $tween( terrys2, SCHUBRING2_01 ) \
    $tween( terrys3, SCHUBRING3_01 ) \
    \
    $tween( swing, SWING_01 ) \
    $tween( sinpi2, SINPI2_01 ) 

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
